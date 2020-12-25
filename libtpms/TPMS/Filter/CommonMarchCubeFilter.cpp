#include "CommonMarchCubeFilter.h"

#include <numeric>

#include <TPMS/MarchBoxUtil.h>
#include <TPMS/MarchBoxTable.h>

CommonMarchCubeFilter::CommonMarchCubeFilter()
    :isoLevel(0), border(false)
{

}


/**
 * @brief CommonMarchCubeFilter::operator ()
 * March Cube 方法拟合
 * 没有加入任何的约束，直接根据采样点的结果拟合结果
 * @param sampleMatrix
 * @return
 */
Mesh CommonMarchCubeFilter::operator ()(vector<vector<vector<SamplePoint> > > &sampleMatrix)
{
    // 是否需要给模型加入边界
    if(border) {
        addBorder(sampleMatrix);
    }

    Vector3i indexBoundary;
    indexBoundary.x() = sampleMatrix.size() -1;
    indexBoundary.y() = sampleMatrix[0].size() -1;
    indexBoundary.z() = sampleMatrix[0][0].size() -1;

    Vector3i index(0,0,0);
    Mesh mesh;  // 预备生成的 mesh
    for(index.x() = 0; index.x() < indexBoundary.x(); index.x()++ ) {
        for(index.y() = 0; index.y() < indexBoundary.y(); index.y()++) {
            for(index.z() = 0; index.z() < indexBoundary.z(); index.z()++) {
                int cubeIndex = getMarchBoxCubeIndex(sampleMatrix, index, isoLevel);
                // 根据 cubeIndex 找到拟合情况
                int *faces = marchboxTriTable[cubeIndex];
                addFaces(faces, index, sampleMatrix, mesh);
            }
        }
    }

    // clean
    this->vertexIdMap.clear();

    return mesh;
}

/**
 * @brief CommonMarchCubeFilter::getVertexHandle
 * 获得将要加入模型的顶点 id
 * @param index
 * @param edgeIndex
 * @param mesh
 * @return
 */
Mesh::VertexHandle CommonMarchCubeFilter::getVertexHandle(Eigen::Vector3i index, int edgeIndex, vector<vector<vector<SamplePoint> > > &sampleMatrix, Mesh &mesh)
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
 * @brief CommonMarchCubeFilter::addFaces
 * 向模型添加面片
 * @param faces
 * @param index
 * @param mesh
 */
void CommonMarchCubeFilter::addFaces(int *faces, Eigen::Vector3i index, vector<vector<vector<SamplePoint> > > &sampleMatrix, Mesh &mesh)
{
    while(*faces != -1) {
        vector<int> edgeIndex(3);
        edgeIndex[0] = *(faces++);
        edgeIndex[1] = *(faces++);
        edgeIndex[2] = *(faces++);

        vector<Mesh::VertexHandle> face(3);
        for(int i = 0; i <3; i ++) {
            // 添加点，获得点 Id
            face[i] = getVertexHandle(index, edgeIndex[i], sampleMatrix, mesh);
        }

        // 添加面
        mesh.add_face(face);
    }
}

/**
 * @brief CommonMarchCubeFilter::addBorder
 * 给采样点加入一圈边界顶点
 * @param sampleMatrix
 */
void CommonMarchCubeFilter::addBorder(vector<vector<vector<SamplePoint> > > &sampleMatrix)
{
    // 提前 -1 约束边界
    Vector3i range;
    range.x() = sampleMatrix.size();
    range.y() = sampleMatrix[0].size();
    range.z() = sampleMatrix[0][0].size();

    Vector3i index(0, 0, 0);
    for(index.x() = 0; index.x() < range.x(); index.x()++) {
        for(index.y() = 0; index.y() < range.y(); index.y()++) {
            sampleMatrix[index.x()][index.y()][0].implicitValue = std::numeric_limits<float>::max();
            sampleMatrix[index.x()][index.y()][range.z()-1].implicitValue = std::numeric_limits<float>::max();
        }
    }

    for(index.x() = 0; index.x() < range.x(); index.x()++) {
        for(index.z() = 0; index.z() < range.z(); index.z()++) {
            sampleMatrix[index.x()][0][index.z()].implicitValue = std::numeric_limits<float>::max();
            sampleMatrix[index.x()][range.y()-1][index.z()].implicitValue = std::numeric_limits<float>::max();
        }
    }

    for(index.y() = 0; index.y() < range.y(); index.y()++) {
        for(index.z() = 0; index.z() < range.z(); index.z()++) {
            sampleMatrix[0][index.y()][index.z()].implicitValue = std::numeric_limits<float>::max();
            sampleMatrix[range.x()-1][index.y()][index.z()].implicitValue = std::numeric_limits<float>::max();
        }
    }
}
