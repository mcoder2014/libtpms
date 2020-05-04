#include "BoxTpmsSingleSurfaceConfig.h"

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
