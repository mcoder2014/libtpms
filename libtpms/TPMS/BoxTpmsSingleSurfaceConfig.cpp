#include "BoxTpmsSingleSurfaceConfig.h"

TpmsType BoxTpmsSingleSurfaceConfig::getTpmsType() const
{
    return tpmsType;
}

void BoxTpmsSingleSurfaceConfig::setTpmsType(const TpmsType &value)
{
    tpmsType = value;
}

float BoxTpmsSingleSurfaceConfig::getIsoLevel() const
{
    return isoLevel;
}

void BoxTpmsSingleSurfaceConfig::setIsoLevel(float value)
{
    isoLevel = value;
}

bool BoxTpmsSingleSurfaceConfig::getReverse() const
{
    return reverse;
}

void BoxTpmsSingleSurfaceConfig::setReverse(bool value)
{
    reverse = value;
}

Eigen::AlignedBox3d BoxTpmsSingleSurfaceConfig::getBoundingBoxPhysial() const
{
    return boundingBoxPhysial;
}

void BoxTpmsSingleSurfaceConfig::setBoundingBoxPhysial(const Eigen::AlignedBox3d &value)
{
    boundingBoxPhysial = value;
}

Eigen::AlignedBox3d BoxTpmsSingleSurfaceConfig::getBoundingBoxLogical() const
{
    return boundingBoxLogical;
}

void BoxTpmsSingleSurfaceConfig::setBoundingBoxLogical(const Eigen::AlignedBox3d &value)
{
    boundingBoxLogical = value;
}

Eigen::Vector3i BoxTpmsSingleSurfaceConfig::getMatrixSize() const
{
    Eigen::Vector3d relativeSize = boundingBoxPhysial.max() - boundingBoxPhysial.min();
    Eigen::Vector3i matrixSize;

    matrixSize.x() = relativeSize.x() * voxelDensity.x();
    matrixSize.y() = relativeSize.y() * voxelDensity.y();
    matrixSize.z() = relativeSize.z() * voxelDensity.z();

    return matrixSize;
}
