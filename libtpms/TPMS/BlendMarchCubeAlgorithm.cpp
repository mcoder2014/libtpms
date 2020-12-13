#include "BlendMarchCubeAlgorithm.h"

#include <iostream>

#include "ImplicitFunctionUtil.h"
#include "MarchBoxTable.h"
#include "MarchBoxUtil.h"

/**
 * @brief BlendMarchCubeAlgorithm::initSampleMatrix
 * 初始化采样矩阵，将这部分工作交给 samplePointFilter 进行处理
 */
void BlendMarchCubeAlgorithm::initSampleMatrix()
{
    for(size_t index = 0; index < samplePointFilters.size(); index++) {
        std::cout << "Run " << index+1 << " / " << samplePointFilters.size() << "samplePointFilters" << std::endl;
        samplePointFilters[index](this->sampleMatrix);
    }
}

/**
 * @brief BlendMarchCubeAlgorithm::initSamplePointGroup
 * 初始化采样矩阵及部分使用隐函数，交给 SamplePointGroupFilter 进行处理
 */
void BlendMarchCubeAlgorithm::initSamplePointGroup()
{
    if(!samplePointGroupFilter){
        std::cerr << "Not Set SamplePointGroupFilter" << std::endl;
    }

    samplePointGroups = samplePointGroupFilter(sampleMatrix);
    std::cout << "SamplePointGroupFilter" << __FUNCTION__ << __LINE__ << std::endl;
}

/**
 * @brief BlendMarchCubeAlgorithm::caculateImplicitValue
 * 根据 SamplePointsGroup 计算隐函数值
 * - 每个 group
 *  - 每个 function
 *   - 处理每个 samplePoint
 */
void BlendMarchCubeAlgorithm::calculateImplicitValue()
{
    for(size_t index = 0; index < samplePointGroups.size(); index ++) {
        std::cout << __FUNCTION__ << " Group " << index+1 << " / " << samplePointGroups.size() << std::endl;
        ImplicitFunciton::calculateSamplePointGroup(samplePointGroups[index]);
    }
}

/**
 * @brief BlendMarchCubeAlgorithm::clear
 * 清空生成的中间数据
 * 1. sampleMatrix
 * 2. samplePointGroup
 * 3. map
 * 4. filters
 */
void BlendMarchCubeAlgorithm::clear()
{
    sampleMatrix.clear();
    samplePointGroups.clear();
    vertexIdMap.clear();

    samplePointFilters.clear();
    {
        SamplePointGroupFilter tmpFilter;
        samplePointGroupFilter.swap(tmpFilter);
    }
}

/**
 * @brief BlendMarchCubeAlgorithm::marchMesh
 * March Cube 方法拟合网格模型
 * @return
 */
Mesh BlendMarchCubeAlgorithm::marchMesh()
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

                    int cubeIndex = getMarchBoxCubeIndex(
                                sampleMatrix, index, isoLevel);

                    // 根据 cubeIndex 找到拟合情况
                    int *faces = marchboxTriTable[cubeIndex];
                    addFaces(faces, index, mesh);

            }
        }
    }

    return mesh;
}

/**
 * @brief BlendMarchCubeAlgorithm::addFaces
 * marchMesh 的中间函数，用来添加拟合的面片到模型中
 * @param faces
 * @param index
 * @param mesh
 */
void BlendMarchCubeAlgorithm::addFaces(int *faces, Eigen::Vector3i index, Mesh &mesh)
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
 * @brief BlendMarchCubeAlgorithm::getVertexHandle
 * marchMesh 的中间函数，用来添加顶点到网格模型中
 * @param index
 * @param edgeIndex
 * @param mesh
 * @return
 */
Mesh::VertexHandle BlendMarchCubeAlgorithm::getVertexHandle(Eigen::Vector3i index, int edgeIndex, Mesh &mesh)
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

/**
 * @brief BlendMarchCubeAlgorithm::process
 * 开始按照方法构建
 * 注意：执行此函数前，samplePoints samplepointGroup 都已经初始化完毕
 *
 * 1. 挨个 SamplePointsGroup 计算对应 samplePoint 的值
 * 2. 根据 samplePoints 的值 构造 Mesh
 * @return
 */
Mesh BlendMarchCubeAlgorithm::process()
{
    // 先初始化采样矩阵
    initSampleMatrix();

    // 再初始化采样点及其需要的 隐函数
    initSamplePointGroup();

    // 计算隐函数值
    calculateImplicitValue();

    // 拟合网格模型
    Mesh mesh = marchMesh();

    // 清理临时数据
    clear();

    // 返回拟合结果
    return mesh;
}
