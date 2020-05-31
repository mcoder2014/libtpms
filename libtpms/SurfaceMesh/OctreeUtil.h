#ifndef OCTREEUTIL_H
#define OCTREEUTIL_H

#include <vector>
#include <memory>

#include <Eigen/Dense>

#include <Octree.h>


using std::vector;
using Eigen::Vector3d;

// 找到射线与八叉树模型的交点
vector<Vector3d> getIntersectPointsDirectionZ(
        Octree &octree,
        Vector3d point);


#endif // OCTREEUTIL_H
