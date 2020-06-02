#include "BoxTpmsSingleSurfaceConfig.h"

#include "Math/EigenUtil.h"

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

    matrixSize.x() = (int)(relativeSize.x() * voxelDensity.x()) + 1;
    matrixSize.y() = (int)(relativeSize.y() * voxelDensity.y()) + 1;
    matrixSize.z() = (int)(relativeSize.z() * voxelDensity.z()) + 1;

    return matrixSize;
}
