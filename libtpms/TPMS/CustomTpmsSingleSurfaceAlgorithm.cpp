#include "CustomTpmsSingleSurfaceAlgorithm.h"

#include <climits>

#include "MarchBoxUtil.h"
#include "MarchBoxTable.h"

CustomTpmsSingleSurfaceAlgorithm::CustomTpmsSingleSurfaceAlgorithm()
{

}

Mesh CustomTpmsSingleSurfaceAlgorithm::process()
{
    init();

    calcTpmsFunction(sampleMatrix, TpmsImplicitFunction(config->getTpmsType()));

    Mesh mesh = marchMesh();

    clear();

    return mesh;
}

void CustomTpmsSingleSurfaceAlgorithm::init()
{
    Vector3i sampleMatrixSize = config->getMatrixSize();
    sampleMatrix = create3DMatrix(sampleMatrixSize, SamplePoint());
    initMatrix(sampleMatrix,
               config->getBoundingBoxPhysial(),
               config->getBoundingBoxLogical());

    vertexIdMap.clear();

    // 构建体素模型
    Octree octree(config->getCustomBoundary().get());
    voxelModel.setVoxelSize(config->getCustomBoundaryVoxelSize());
    voxelModel.build(octree);
}

void CustomTpmsSingleSurfaceAlgorithm::clear()
{
    vector<vector<vector<SamplePoint>>> cleanSampleMatrix;
    sampleMatrix.swap(cleanSampleMatrix);

    std::unordered_map<std::string, int> cleanVertexIdMap;
    vertexIdMap.swap(cleanVertexIdMap);

    voxelModel.clear();
}

void CustomTpmsSingleSurfaceAlgorithm::addFaces(
        int *faces, Eigen::Vector3i index, Mesh &mesh)
{
    while(*faces != -1) {
        vector<int> edgeIndex(3);
        edgeIndex[0] = *(faces++);
        edgeIndex[1] = *(faces++);
        edgeIndex[2] = *(faces++);

        vector<Mesh::VertexHandle> face(3);
        for(int i = 0; i <3; i ++) {
            // 添加点，获得点 Id
            face[i] = getVertexHandle(index, edgeIndex[i], mesh);
        }

        // 添加面
        mesh.add_face(face);
    }
}

Mesh::VertexHandle CustomTpmsSingleSurfaceAlgorithm::getVertexHandle(Eigen::Vector3i index, int edgeIndex, Mesh &mesh)
{
    std::string hash = hashKey(index, edgeIndex);
    if(vertexIdMap.find(hash) != vertexIdMap.end()) {
        return Mesh::VertexHandle(vertexIdMap[hash]);
    }

    // 添加点到 mesh
    vector<Vector3i> vectexList = edgeToIndex(index, edgeIndex);
    Vector3i first = vectexList[0];
    Vector3i second = vectexList[1];
    Vector3d midPoint = (sampleMatrix[first.x()][first.y()][first.z()].physical
            + sampleMatrix[second.x()][second.y()][second.z()].physical) / 2;
    Mesh::VertexHandle vertexHandle = mesh.add_vertex(Mesh::Point(midPoint.x(), midPoint.y(), midPoint.z()));
    vertexIdMap[hash] = vertexHandle.idx();
    return vertexHandle;
}

Mesh CustomTpmsSingleSurfaceAlgorithm::marchMesh()
{
    // 提前 -1 约束边界
    Vector3i indexBoundary;
    indexBoundary.x() = sampleMatrix.size() -1;
    indexBoundary.y() = sampleMatrix[0].size() -1;
    indexBoundary.z() = sampleMatrix[0][0].size() -1;

    Vector3i index(0,0,0);
    Mesh mesh;  // 预备生成的 mesh
    std::unordered_map<std::string, int> vertexIdMap;
    for(index.x() = 0; index.x() < indexBoundary.x(); index.x()++ ) {
        for(index.y() = 0; index.y() < indexBoundary.y(); index.y()++) {
            for(index.z() = 0; index.z() < indexBoundary.z(); index.z()++) {
                if(voxelModel.contains(sampleMatrix[index.x()][index.y()][index.z()].physical)){
                    int cubeIndex = getMarchBoxCubeIndex(sampleMatrix, index, config->getIsoLevel());
                    // 根据 cubeIndex 找到拟合情况
                    int *faces = marchboxTriTable[cubeIndex];
                    addFaces(faces, index, mesh);
                }
            }
        }
    }

    return mesh;
}
