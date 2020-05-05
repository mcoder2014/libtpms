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

    // 构建 八叉树
    boundary = Octree(config->getCustomBoundary().get());
}

void CustomTpmsSingleSurfaceAlgorithm::clear()
{
    vector<vector<vector<SamplePoint>>> cleanSampleMatrix;
    sampleMatrix.swap(cleanSampleMatrix);

    std::unordered_map<std::string, int> cleanVertexIdMap;
    vertexIdMap.swap(cleanVertexIdMap);

    boundary = Octree();
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
            vector<Vector3d> boundary = getBoundary(
                sampleMatrix[index.x()][index.y()][0].physical);
            for(index.z() = 0; index.z() < indexBoundary.z(); index.z()++) {
                if(sampleMatrix[index.x()][index.y()][index.z()].physical.z() < boundary[0].z()
                        || sampleMatrix[index.x()][index.y()][index.z()].physical.z() > boundary[1].z()) {
                    continue;
                }
                int cubeIndex = getMarchBoxCubeIndex(sampleMatrix, index, config->getIsoLevel());
                // 根据 cubeIndex 找到拟合情况
                int *faces = marchboxTriTable[cubeIndex];
                addFaces(faces, index, mesh);
            }
        }
    }

    return mesh;
}

vector<Eigen::Vector3d> CustomTpmsSingleSurfaceAlgorithm::getBoundary(Eigen::Vector3d point)
{
    // 任意一个射线方向
    Eigen::Vector3d direction(0.0,0.0,1.0);
    point.z() = -DBL_MAX;
    Ray ray(point, direction);
    QSet<int> intersectResult = boundary.intersectRay(ray, 0.00001, false);
    HitResult hitResult;
    vector<Vector3d> boundaryPoints(2);
    boundaryPoints[0].z() = DBL_MAX;    // 下边界
    boundaryPoints[1].z() = -DBL_MAX;    // 上边界

    int intersectCount = 0;
    for(int i : intersectResult) {
        boundary.rayTriangleIntersectionTest(SurfaceMesh::Model::Face(i), ray, hitResult);
        if(hitResult.hit) {
            intersectCount++;

            Eigen::Vector3d intersectPoint = ray.origin + (ray.direction * hitResult.distance);

            if(intersectPoint.z() < boundaryPoints[0].z()) {
                boundaryPoints[0] = intersectPoint;
            }
            if(intersectPoint.z() > boundaryPoints[1].z()) {
                boundaryPoints[1] = intersectPoint;
            }
        }
    }

    // 奇数次相交认为在模型内部
    return boundaryPoints;
}
