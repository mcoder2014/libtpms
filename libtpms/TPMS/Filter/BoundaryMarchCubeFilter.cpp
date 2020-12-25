#include "BoundaryMarchCubeFilter.h"

#include <TPMS/MarchBoxUtil.h>
#include <TPMS/MarchBoxTable.h>

BoundaryMarchCubeFilter::BoundaryMarchCubeFilter()
    :isoLevel(0)
{

}


Mesh BoundaryMarchCubeFilter::operator ()(vector<vector<vector<SamplePoint> > > &sampleMatrix)
{
    // 构建采样点为于模型内部缓存
    buildInBoundaryCache(sampleMatrix);

    Vector3i indexBoundary;
    indexBoundary.x() = sampleMatrix.size() -1;
    indexBoundary.y() = sampleMatrix[0].size() -1;
    indexBoundary.z() = sampleMatrix[0][0].size() -1;

    Vector3i index(0,0,0);
    Mesh mesh;  // 预备生成的 mesh
    for(index.x() = 0; index.x() < indexBoundary.x(); index.x()++ ) {
        for(index.y() = 0; index.y() < indexBoundary.y(); index.y()++) {
            for(index.z() = 0; index.z() < indexBoundary.z(); index.z()++) {
                if(!inBoundary(index, sampleMatrix)) {
                    // 跳过不在边界内部的体素
                    continue;
                }

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
 * @brief BoundaryMarchCubeFilter::inBoundary
 * 判断八个顶点是否均位于模型内部
 * @param index
 * @param sampleMatrix
 * @return
 */
bool BoundaryMarchCubeFilter::inBoundary(Eigen::Vector3i index, vector<vector<vector<SamplePoint> > > &sampleMatrix)
{
    return inBoundaryCache[index.x()][index.y()][index.z()]
            && inBoundaryCache[index.x()][index.y()][index.z()+1]
            && inBoundaryCache[index.x()][index.y()+1][index.z()]
            && inBoundaryCache[index.x()][index.y()+1][index.z()+1]
            && inBoundaryCache[index.x()+1][index.y()][index.z()]
            && inBoundaryCache[index.x()+1][index.y()][index.z()+1]
            && inBoundaryCache[index.x()+1][index.y()+1][index.z()]
            && inBoundaryCache[index.x()+1][index.y()+1][index.z()+1];
}

/**
 * @brief BoundaryMarchCubeFilter::buildInBoundaryCache
 * 构建位于模型内部缓存
 * @param sampleMatrix
 */
void BoundaryMarchCubeFilter::buildInBoundaryCache(vector<vector<vector<SamplePoint> > > &sampleMatrix)
{
    Vector3i indexBoundary;
    indexBoundary.x() = sampleMatrix.size();
    indexBoundary.y() = sampleMatrix[0].size();
    indexBoundary.z() = sampleMatrix[0][0].size();

    inBoundaryCache = vector<vector<vector<bool>>>(
        indexBoundary.x(),vector<vector<bool>>(
        indexBoundary.y(), vector<bool>(
        indexBoundary.z(), false)));

    Vector3i index(0, 0, 0);
    for(index.x() = 0; index.x() < indexBoundary.x(); index.x()++ ) {
        for(index.y() = 0; index.y() < indexBoundary.y(); index.y()++) {
            for(index.z() = 0; index.z() < indexBoundary.z(); index.z()++) {
                inBoundaryCache[index.x()][index.y()][index.z()] =
                    voxelModel->contains(sampleMatrix[index.x()][index.y()][index.z()].physical);
            }
        }
    }
}

Mesh::VertexHandle BoundaryMarchCubeFilter::getVertexHandle(Eigen::Vector3i index, int edgeIndex, vector<vector<vector<SamplePoint> > > &sampleMatrix, Mesh &mesh)
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

void BoundaryMarchCubeFilter::addFaces(int *faces, Eigen::Vector3i index, vector<vector<vector<SamplePoint> > > &sampleMatrix, Mesh &mesh)
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
