#include <iostream>
#include <memory>
#include <string>

using namespace std;

#include <TPMS/BoxTpmsSingeSurfaceAlgorithm.h>
#include <Mesh/Mesh>
#include <IO/Exporter.h>

void createTPMS(string savePath, shared_ptr<BoxTpmsSingleSurfaceConfig> boxTpmsSingleSurfaceConfig);
void createTpmsShell(string savePath, shared_ptr<BoxTpmsSingleSurfaceConfig> boxTpmsSingleSurfaceConfig);

int main(int argc, char *argv[])
{
    string saveFolder;
    if(argc < 2) {
        saveFolder = "boxTpmsSingeSurfaceAlgorithm";
        cout << "e.g. ./program <saveFolder>"
             << "\nDefault save Folder Path: " << saveFolder << endl;
    }

    vector<double> isoLevels{-1.0, -0.75, -0.5, -0.25, 0, 0.25, 0.5, 0.75, 1.0};

    for(TpmsType i = TpmsType::P; i <= TpmsType::I2_Y; i = (TpmsType)(i+1) ) {

        for(double isoLevel: isoLevels) {

            shared_ptr<BoxTpmsSingleSurfaceConfig> boxTpmsSingleSurfaceConfig = make_shared<BoxTpmsSingleSurfaceConfig>();
            boxTpmsSingleSurfaceConfig->setTpmsType(i);
            boxTpmsSingleSurfaceConfig->setReverse(false);
            boxTpmsSingleSurfaceConfig->setIsoLevel(isoLevel);

            Eigen::AlignedBox3d physical;
            physical.extend(Vector3d(2.0,2.0,2.0));
            physical.extend(Vector3d(.0,.0,.0));
            boxTpmsSingleSurfaceConfig->setBoundingBoxPhysial(physical);

            Eigen::AlignedBox3d logical;
            logical.extend(Vector3d(2.0,2.0,2.0));
            logical.extend(Vector3d(.0,.0,.0));

            boxTpmsSingleSurfaceConfig->setBoundingBoxLogical(logical);
            boxTpmsSingleSurfaceConfig->setVoxelDensity(Vector3i(64, 64, 64));

            string path = saveFolder + "/" + tpmsTypeToString(i) + "_" + std::to_string(isoLevel) + ".ply";
            cout << "start " << path << endl;
            createTPMS(path, boxTpmsSingleSurfaceConfig);
            cout << "finished " << path << endl;

            string shellPath = saveFolder + "/" + tpmsTypeToString(i) + "_" + std::to_string(isoLevel) + "_shell.ply";
            cout << "start shell" << shellPath << endl;
            createTpmsShell(shellPath, boxTpmsSingleSurfaceConfig);
            cout << "finished " << shellPath << endl;
        }
    }

    return 0;
}

void createTPMS(string savePath, shared_ptr<BoxTpmsSingleSurfaceConfig> boxTpmsSingleSurfaceConfig)
{
    BoxTpmsSingeSurfaceAlgorithm boxTpmsSingeSurfaceAlgorithm;
    boxTpmsSingeSurfaceAlgorithm.setConfig(boxTpmsSingleSurfaceConfig);
    Mesh mesh = boxTpmsSingeSurfaceAlgorithm.process();

    MeshSmoothTool smoothTool;
    smoothTool.basicSmooth(mesh, 10);

    Exporter expoter;
    expoter.writeOBJ(savePath, mesh);
}

void createTpmsShell(string savePath, shared_ptr<BoxTpmsSingleSurfaceConfig> boxTpmsSingleSurfaceConfig)
{
    BoxTpmsSingeSurfaceAlgorithm boxTpmsSingeSurfaceAlgorithm;
    boxTpmsSingeSurfaceAlgorithm.setConfig(boxTpmsSingleSurfaceConfig);
    Mesh mesh = boxTpmsSingeSurfaceAlgorithm.process();

    MeshSmoothTool smoothTool;
    smoothTool.basicSmooth(mesh, 10);

    MeshShellTool meshShellTool;
    meshShellTool.shell(mesh, 0.03);

    Exporter expoter;
    expoter.writeOBJ(savePath, mesh);
}
