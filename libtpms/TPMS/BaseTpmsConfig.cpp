#include "BaseTpmsConfig.h"


Eigen::AlignedBox3d BaseTpmsConfig::getBoundingBoxPhysial() const
{
    return boundingBoxPhysial;
}

void BaseTpmsConfig::setBoundingBoxPhysial(const Eigen::AlignedBox3d &value)
{
    boundingBoxPhysial = value;
}

Eigen::AlignedBox3d BaseTpmsConfig::getBoundingBoxLogical() const
{
    return boundingBoxLogical;
}

void BaseTpmsConfig::setBoundingBoxLogical(const Eigen::AlignedBox3d &value)
{
    boundingBoxLogical = value;
}

TpmsType BaseTpmsConfig::getTpmsType() const
{
    return tpmsType;
}

void BaseTpmsConfig::setTpmsType(const TpmsType &value)
{
    tpmsType = value;
}

Eigen::Vector3i BaseTpmsConfig::getVoxelDensity() const
{
    return voxelDensity;
}

void BaseTpmsConfig::setVoxelDensity(const Eigen::Vector3i &value)
{
    voxelDensity = value;
}
