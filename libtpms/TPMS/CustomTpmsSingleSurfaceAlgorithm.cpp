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

    doFilters();

    calcTpmsFunction(sampleMatrix, TpmsImplicitFunction(config->getTpmsType()));

    Mesh mesh = marchMesh();

    clear();

    return mesh;
}

/**
 * @brief CustomTpmsSingleSurfaceAlgorithm::init
 * 初始化算法必要数据结构：
 * 1. 采样矩阵
 * 2. Map
 * 3. 体素模型
 */
void CustomTpmsSingleSurfaceAlgorithm::init()
{
    Vector3i sampleMatrixSize = config->getMatrixSize();
    sampleMatrix = create3DMatrix(sampleMatrixSize, SamplePoint());
    initMatrix(sampleMatrix,
               config->getBoundingBoxPhysial(),
               config->getBoundingBoxLogical());

    vertexIdMap.clear();

    // 引用体素模型
    voxelModel = config->getCustomBoundary();
}

/**
 * @brief CustomTpmsSingleSurfaceAlgorithm::doFilters
 * 如果有附加滤波函数，在此处执行
 */
void CustomTpmsSingleSurfaceAlgorithm::doFilters()
{
    for(std::shared_ptr<BaseSampleMatrixFilter> filter
            : config->getSampleMatrixFilterVector()) {
        filter->process(sampleMatrix);
    }
}

void CustomTpmsSingleSurfaceAlgorithm::clear()
{
    vector<vector<vector<SamplePoint>>> cleanSampleMatrix;
    sampleMatrix.swap(cleanSampleMatrix);

    std::unordered_map<std::string, int> cleanVertexIdMap;
    vertexIdMap.swap(cleanVertexIdMap);

    // 清空边界的引用
    voxelModel.reset();
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

/**
 * @brief CustomTpmsSingleSurfaceAlgorithm::getVertexHandle
 * 根据 体素点 Index、 边 index，添加顶点到 mesh
 * @param index
 * @param edgeIndex
 * @param mesh
 * @return 顶点的 handle
 */
Mesh::VertexHandle CustomTpmsSingleSurfaceAlgorithm::getVertexHandle(
        Eigen::Vector3i index, int edgeIndex, Mesh &mesh)
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
    for(index.x() = 0; index.x() < indexBoundary.x(); index.x()++ ) {
        for(index.y() = 0; index.y() < indexBoundary.y(); index.y()++) {
            for(index.z() = 0; index.z() < indexBoundary.z(); index.z()++) {
                if(voxelModel->contains(sampleMatrix[index.x()][index.y()][index.z()].physical)){
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
