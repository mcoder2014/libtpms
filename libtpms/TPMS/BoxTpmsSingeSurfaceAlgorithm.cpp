#include "BoxTpmsSingeSurfaceAlgorithm.h"

#include "BoxTpmsSingleSurfaceConfig.h"
#include "MarchBoxUtil.h"
#include "MarchBoxTable.h"

BoxTpmsSingeSurfaceAlgorithm::BoxTpmsSingeSurfaceAlgorithm()
{
}

Mesh BoxTpmsSingeSurfaceAlgorithm::process()
{
    // 1. 初始化 矩阵
    init();

    // 2. 计算矩阵的值
    calcTpmsFunction(sampleMatrix, TpmsImplicitFunction(config->getTpmsType()));

    // 3. 拟合
    Mesh mesh = marchMesh(sampleMatrix);

    // 4. 清理临时数据
    clear();

    return mesh;
}

/**
 * 初始化算法需要的采样矩阵等数据
 * @brief BoxTpmsSingeSurfaceAlgorithm::init
 */
void BoxTpmsSingeSurfaceAlgorithm::init()
{
    Vector3i sampleMatrixSize = this->config->getMatrixSize();
    this->sampleMatrix = create3DMatrix( sampleMatrixSize, SamplePoint());
    initMatrix(sampleMatrix,
               config->getBoundingBoxPhysial(),
               config->getBoundingBoxLogical());
    this->vertexIdMap.clear();
}

/**
 * 清除临时数据的内存占用
 * @brief BoxTpmsSingeSurfaceAlgorithm::clear
 */
void BoxTpmsSingeSurfaceAlgorithm::clear()
{
    vector<vector<vector<SamplePoint> > > cleanSampleMatrix;
    sampleMatrix.swap(cleanSampleMatrix);

    std::unordered_map<std::string, int> cleanVertexIdMap;
    vertexIdMap.swap(cleanVertexIdMap);
}

/**
 * 根据计算采样矩阵 Tpms 函数值，拟合网格模型
 * @brief BoxTpmsSingeSurfaceAlgorithm::marchMesh
 * @return
 */
Mesh BoxTpmsSingeSurfaceAlgorithm::marchMesh( vector<vector<vector<SamplePoint>>> &sampleMatrix)
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
                int cubeIndex = getMarchBoxCubeIndex(sampleMatrix, index, config->getIsoLevel());
                // 根据 cubeIndex 找到拟合情况
                int *faces = marchboxTriTable[cubeIndex];
                addFaces(faces, index, mesh);
            }
        }
    }

    return mesh;
}

/**
 * 从 MarchBoxTable 条目中增加拟面片
 * @brief BoxTpmsSingeSurfaceAlgorithm::addFaces
 * @param faces
 * @param index
 * @param mesh
 */
void BoxTpmsSingeSurfaceAlgorithm::addFaces(
        int *faces, Vector3i index, Mesh &mesh)
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
 * 获得所需要顶点的 VIdx，不会重复添加
 * @brief BoxTpmsSingeSurfaceAlgorithm::getVertexHandle
 * @param index
 * @param edgeIndex
 * @param mesh
 * @return
 */
Mesh::VertexHandle BoxTpmsSingeSurfaceAlgorithm::getVertexHandle(
        Vector3i index, int edgeIndex, Mesh &mesh)
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



