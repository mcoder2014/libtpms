/*************************
 * 本文件目的：
 * 测试模型六面体
 ************************/

#include <iostream>
#include <string>
#include <memory>

#include <Mesh/Mesh>
#include <Hex/GridHexAlgorithm.h>
#include <Hex/HexahedralMeshDebugger.h>
#include <SurfaceMeshModel.h>
#include <IO/Importer.h>
#include <IO/Exporter.h>
#include <Octree.h>
#include <Voxel/VoxelModel.h>

int main()
{
    std::string originModelPath("origin.obj");

    // 准备素材
    Importer importer;
    std::shared_ptr<SurfaceMesh::SurfaceMeshModel> originMesh = importer.loadSurfaceMeshModel(originModelPath);
    Octree octree(originMesh.get());
    std::shared_ptr<VoxelModel> voxelModel = std::make_shared<VoxelModel>();
    voxelModel->setVoxelSize(1);
    voxelModel->build(octree);

    std::shared_ptr<GridHexAlgorithmConfig> config = std::make_shared<GridHexAlgorithmConfig>();
    config->voxelModel = voxelModel;
    config->initSize = 5;

    GridHexAlgorithm girdAlgorithm;
    girdAlgorithm.setConfig(config);

    std::shared_ptr<HexahedralMesh> hexMesh = girdAlgorithm.build();
    HexahedralMeshDebugger hexahedralMeshDebugger;

    Mesh resultMesh = hexahedralMeshDebugger.convertToMesh(hexMesh);
    Exporter exporter;
    exporter.writeOBJ("hexMesh.ply", resultMesh);

    return 0;
}
