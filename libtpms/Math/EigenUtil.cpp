#include "EigenUtil.h"

Eigen::Vector3i vector3dToVector3i(Eigen::Vector3d point, std::function<int (double)> callback)
{
    Vector3i result;
    result.x() = callback(point.x());
    result.y() = callback(point.y());
    result.z() = callback(point.z());
    return result;
}
