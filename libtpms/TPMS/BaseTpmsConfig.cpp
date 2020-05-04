#include "BaseTpmsConfig.h"

Eigen::Vector3i BaseTpmsConfig::getVoxelDensity() const
{
    return voxelDensity;
}

void BaseTpmsConfig::setVoxelDensity(const Eigen::Vector3i &value)
{
    voxelDensity = value;
}



