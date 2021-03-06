#include <iostream>
#include <memory>
#include <string>

using namespace std;

#include <Octree.h>

#include <TPMS/CustomTpmsSingleSurfaceAlgorithm.h>
#include <Mesh/Mesh>
#include <IO/Exporter.h>
#include <IO/Importer.h>
#include <Voxel/VoxelModel.h>

void createTPMS(string savePath, shared_ptr<CustomTpmsSingleSurfaceConfig> customTpmsSingleSurfaceConfig);
void createTpmsShell(string savePath, shared_ptr<CustomTpmsSingleSurfaceConfig> customTpmsSingleSurfaceConfig);

int main(int argc, char *argv[])
{
    string saveFolder;
    string boundaryMeshPath;
    if(argc < 3) {
        saveFolder = "customTpmsSingeSurfaceAlgorithm";
        boundaryMeshPath = "origin.obj";
        cout << "e.g. ./program <saveFolder> <boundaryModel>"
             << "\nDefault save Folder Path: " << saveFolder
             << "\nDefault boundaryMesh Model Path:" << boundaryMeshPath << endl;
    }

    Importer importer;
    std::shared_ptr<SurfaceMeshModel> boudaryMesh = importer.loadSurfaceMeshModel(boundaryMeshPath);
    Octree octree(boudaryMesh.get());
    std::shared_ptr<VoxelModel> voxelModel = std::make_shared<VoxelModel>();
    voxelModel->setVoxelSize(1);
    voxelModel->build(octree);

    std::cout << "Build voxelModel finished" << endl;

    for(TpmsType i = TpmsType::P; i <= TpmsType::I2_Y; i = (TpmsType)(i+1) ) {
        shared_ptr<CustomTpmsSingleSurfaceConfig> customTpmsSingleSurfaceConfig = make_shared<CustomTpmsSingleSurfaceConfig>();

        customTpmsSingleSurfaceConfig->setCustomBoundary(voxelModel);
        customTpmsSingleSurfaceConfig->setTpmsType(i);
        customTpmsSingleSurfaceConfig->setReverse(false);
        customTpmsSingleSurfaceConfig->setIsoLevel(0);

        customTpmsSingleSurfaceConfig->setPeriodCycleLength(
                    Eigen::Vector3d(20,20,10));
        customTpmsSingleSurfaceConfig->setVoxelDensity(
                    Vector3i(2,2,2));

        string path = saveFolder + "/" + tpmsTypeToString(i) + ".ply";
        cout << "start " << path << endl;
        createTPMS(path, customTpmsSingleSurfaceConfig);
        cout << "finished " << path << endl;

        string shellPath = saveFolder + "/" + tpmsTypeToString(i) + "_shell.ply";
        cout << "start shell" << shellPath << endl;
        createTpmsShell(shellPath, customTpmsSingleSurfaceConfig);
        cout << "finished " << shellPath << endl;
    }

    return 0;
}

void createTPMS(string savePath, shared_ptr<CustomTpmsSingleSurfaceConfig> customTpmsSingleSurfaceConfig)
{
    CustomTpmsSingleSurfaceAlgorithm customTpmsSingleSurfaceAlgorithm;
    customTpmsSingleSurfaceAlgorithm.setConfig(customTpmsSingleSurfaceConfig);
    Mesh mesh = customTpmsSingleSurfaceAlgorithm.process();

    MeshSmoothTool smoothTool;
    smoothTool.basicSmooth(mesh, 10);

    Exporter expoter;
    expoter.writeOBJ(savePath, mesh);
}

void createTpmsShell(string savePath, shared_ptr<CustomTpmsSingleSurfaceConfig> customTpmsSingleSurfaceConfig)
{
    CustomTpmsSingleSurfaceAlgorithm customTpmsSingleSurfaceAlgorithm;
    customTpmsSingleSurfaceAlgorithm.setConfig(customTpmsSingleSurfaceConfig);
    Mesh mesh = customTpmsSingleSurfaceAlgorithm.process();

    MeshSmoothTool smoothTool;
    smoothTool.basicSmooth(mesh, 10);

    MeshShellTool meshShellTool;
    meshShellTool.shell(mesh, 0.03);

    Exporter expoter;
    expoter.writeOBJ(savePath, mesh);
}
