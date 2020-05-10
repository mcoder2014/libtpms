#include "VoxelConvertor.h"

#include "TPMS/MarchBoxUtil.h"

/**
 * @brief VoxelConvertor::toVoxels
 * 将网格模型转换成一堆平坦的体素
 * @param surfaceMesh
 * @param voxelMatrixSize
 * @return
 */
std::vector<std::shared_ptr<VoxelNode> > VoxelConvertor::toVoxels(
        std::shared_ptr<SurfaceMesh::SurfaceMeshModel> surfaceMesh,
        double voxelSize)
{
    // 创建八叉树
    Octree octree(surfaceMesh.get());

    // 确定体素化网格的尺寸
    surfaceMesh->updateBoundingBox();
    Vector3d relativeSize = surfaceMesh->bbox().max() - surfaceMesh->bbox().min();

    Vector3d startPoint = surfaceMesh->bbox().center();
    startPoint.x() -= relativeSize.x()/2 + voxelSize/2;
    startPoint.y() -= relativeSize.y()/2 + voxelSize/2;
    startPoint.z() -= relativeSize.z()/2 + voxelSize/2;

    std::vector<std::shared_ptr<VoxelNode>> voxelList;
    Vector3i index;
    Vector3d tmpRange = relativeSize / voxelSize;
    Vector3i range;
    range.x() = lround(tmpRange.x());
    range.y() = lround(tmpRange.y());
    range.z() = lround(tmpRange.z());
    Vector3d iterPoint;

    // 迭代，将模型体素化
    for(index.x() = 0; index.x() < range.x(); index.x()++) {
        iterPoint.x() = startPoint.x() + voxelSize * index.x();

        for(index.y() = 0; index.y() < range.y(); index.y()++) {
            iterPoint.y() = startPoint.y() + voxelSize * index.y();
            // 获得交点 range
            std::vector<Eigen::Vector3d> intersects = intersectPoints(octree, iterPoint);

            for(index.z() = 0; index.z() < range.z(); index.z()++) {
                iterPoint.z() = startPoint.z() + voxelSize * index.z();
                if(iterPoint.z() < intersects[0].z()
                        || iterPoint.z() > intersects[1].z()) {
                    continue;
                }

                // 创建体素 加入 vector
                voxelList.push_back(std::make_shared<VoxelNode>(iterPoint, voxelSize));
            }
        }
    }

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
