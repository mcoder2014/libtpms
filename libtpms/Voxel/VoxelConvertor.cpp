#include "VoxelConvertor.h"

#include "TPMS/MarchBoxUtil.h"

/**
 * @brief VoxelConvertor::toVoxels
 * 将网格模型转换成一堆平坦的体素
 * @param surfaceMesh
 * @param voxelMatrixSize
 * @return
 */
std::vector<VoxelNode> VoxelConvertor::toVoxels(
        std::shared_ptr<SurfaceMesh::SurfaceMeshModel> surfaceMesh,
        double voxelSize)
{
    // 创建八叉树
    Octree octree(surfaceMesh.get());
    Vector3i index;
    Vector3d iterPoint;

    surfaceMesh->updateBoundingBox();
    Eigen::AlignedBox3d boundingBox = surfaceMesh->bbox();

    std::vector<VoxelNode> voxelList;

    /// TODO: 建立好迭代的过程，将鞋垫模型体素化

    return voxelList;
}

/**
 * @brief VoxelConvertor::intersectPoints
 * 求于point 点发射 (0,0,1) 射线与模型的交点
 * @param octree
 * @param point
 * @return
 */
std::vector<Eigen::Vector3d> VoxelConvertor::intersectPoints(
        Octree &octree,
        Eigen::Vector3d point)
{
    /// TODO: 后续更改为 更精细的操作

    // 任意一个射线方向
    Eigen::Vector3d direction(0.0,0.0,1.0);
    point.z() = -DBL_MAX;
    Ray ray(point, direction);
    QSet<int> intersectResult = octree.intersectRay(ray, 0.00001, false);
    HitResult hitResult;
    std::vector<Vector3d> boundaryPoints(2);
    boundaryPoints[0].z() = DBL_MAX;    // 下边界
    boundaryPoints[1].z() = -DBL_MAX;    // 上边界

    int intersectCount = 0;
    for(int i : intersectResult) {
        octree.rayTriangleIntersectionTest(SurfaceMesh::Model::Face(i), ray, hitResult);
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
