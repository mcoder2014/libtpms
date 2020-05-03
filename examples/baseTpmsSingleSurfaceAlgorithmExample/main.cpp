#include <iostream>
#include <memory>
#include <string>

using namespace std;

#include <TPMS/BoxTpmsSingeSurfaceAlgorithm.h>
#include <smoothtool.h>
#include <IO/exporter.h>

void createTPMS(string savePath, shared_ptr<BoxTpmsSingleSurfaceConfig> boxTpmsSingleSurfaceConfig);

int main(int argc, char *argv[])
{
    string saveFolder;
    if(argc < 2) {
        saveFolder = "boxTpmsSingeSurfaceAlgorithm";
        cout << "e.g. ./program <saveFolder>"
             << "\nDefault save Folder Path: " << saveFolder << endl;
    }

    for(TpmsType i = TpmsType::P; i <= TpmsType::I2_Y; i = (TpmsType)(i+1) ) {
        shared_ptr<BoxTpmsSingleSurfaceConfig> boxTpmsSingleSurfaceConfig = make_shared<BoxTpmsSingleSurfaceConfig>();
        boxTpmsSingleSurfaceConfig->setTpmsType(i);
        boxTpmsSingleSurfaceConfig->setReverse(false);
        boxTpmsSingleSurfaceConfig->setIsoLevel(0);

        Eigen::AlignedBox3d physical;
        physical.extend(Vector3d(1.0,1.0,1.0));
        physical.extend(Vector3d(.0,.0,.0));
        boxTpmsSingleSurfaceConfig->setBoundingBoxPhysial(physical);

        Eigen::AlignedBox3d logical;
        logical.extend(Vector3d(1.0,1.0,1.0));
        logical.extend(Vector3d(.0,.0,.0));

        boxTpmsSingleSurfaceConfig->setBoundingBoxLogical(logical);
        boxTpmsSingleSurfaceConfig->setVoxelDensity(Vector3i(64, 64, 64));

        string path = saveFolder + "/" + tpmsTypeToString(i) + ".ply";
        cout << "start " << path << endl;
        createTPMS(path, boxTpmsSingleSurfaceConfig);
        cout << "finished " << path << endl;
    }

    return 0;
}

void createTPMS(string savePath, shared_ptr<BoxTpmsSingleSurfaceConfig> boxTpmsSingleSurfaceConfig)
{
    BoxTpmsSingeSurfaceAlgorithm boxTpmsSingeSurfaceAlgorithm;
    boxTpmsSingeSurfaceAlgorithm.setConfig(boxTpmsSingleSurfaceConfig);
    Mesh mesh = boxTpmsSingeSurfaceAlgorithm.process();

    SmoothTool smoothTool;
    smoothTool.createMesh(&mesh);
    smoothTool.basicSmooth(10);

    Exporter expoter;
    expoter.writeOBJ(savePath, *smoothTool.getMesh());
}
