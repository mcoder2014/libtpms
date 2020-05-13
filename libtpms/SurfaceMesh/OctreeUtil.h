#ifndef OCTREEUTIL_H
#define OCTREEUTIL_H

#include <vector>

#include <Eigen/Dense>

#include <Octree.h>


using std::vector;
using Eigen::Vector3d;

vector<Vector3d> getIntersectPointsDirectionZ(
        Octree &octree,
        Vector3d point);


#endif // OCTREEUTIL_H
