#include "OctreeUtil.h"

/**
 * @brief getIntersectPoints
 * 找到射线模型与八叉树相交的所有点
 * @param octree
 * @param point
 * @return
 */
vector<Vector3d> getIntersectPointsDirectionZ(
        Octree &octree,
        Vector3d point)
{
    // 固定 Z 轴方向
    Eigen::Vector3d direction(0.0,0.0,1.0);
    Ray ray(point, direction);
    QSet<int> intersectResult = octree.intersectRay(ray, 0.00001, true);
    HitResult hitResult;
    std::vector<Vector3d> intersectPoints;

    for(int i : qAsConst(intersectResult)) {
        octree.rayTriangleIntersectionTest(SurfaceMesh::Model::Face(i), ray, hitResult);
        if(hitResult.hit) {
            Eigen::Vector3d intersectPoint = ray.origin + (ray.direction * hitResult.distance);
            intersectPoints.push_back(intersectPoint);
        }
    }

    // 按 Z 轴由小到大排序
    std::sort(intersectPoints.begin(), intersectPoints.end(),
        [](Vector3d& v1, Vector3d& v2) -> bool{
            return v1.z() < v2.z();
    });

    // 奇数次相交认为在模型内部
    return intersectPoints;
}
