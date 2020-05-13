#include "CustomTpmsSingleSurfaceConfig.h"

#include <Math/EigenUtil.h>

Eigen::Vector3i CustomTpmsSingleSurfaceConfig::getMatrixSize() const
{
    assert(customBoundary != nullptr);

    customBoundary->updateBoundingBox();
    Eigen::AlignedBox3d boundingBoxPhysial = customBoundary->bbox();
    Vector3d relativeSize = boundingBoxPhysial.max() - boundingBoxPhysial.min();
    Vector3i matrixSize;

    // 进一法
    matrixSize.x() = (int)(relativeSize.x() * voxelDensity.x()) + 1;
    matrixSize.y() = (int)(relativeSize.y() * voxelDensity.y()) + 1;
    matrixSize.z() = (int)(relativeSize.z() * voxelDensity.z()) + 1;

    return matrixSize;
}

Eigen::AlignedBox3d CustomTpmsSingleSurfaceConfig::getBoundingBoxPhysial() const
{
    assert(customBoundary != nullptr);
    customBoundary->updateBoundingBox();
    return customBoundary->bbox();
}

Eigen::AlignedBox3d CustomTpmsSingleSurfaceConfig::getBoundingBoxLogical() const
{
    assert(customBoundary != nullptr);
    customBoundary->updateBoundingBox();
    Eigen::AlignedBox3d boundingBoxPhysial = customBoundary->bbox();
    Vector3d minPoint = boundingBoxPhysial.min();
    Vector3d maxPoint = boundingBoxPhysial.max();

    // 缩放
    divide(maxPoint, periodCycleLength);
    divide(minPoint, periodCycleLength);

    Eigen::AlignedBox3d boundingBoxLogical;
    boundingBoxLogical.extend(minPoint);
    boundingBoxLogical.extend(maxPoint);
    return boundingBoxLogical;
}

std::shared_ptr<SurfaceMesh::SurfaceMeshModel> CustomTpmsSingleSurfaceConfig::getCustomBoundary() const
{
    return customBoundary;
}

void CustomTpmsSingleSurfaceConfig::setCustomBoundary(const std::shared_ptr<SurfaceMesh::SurfaceMeshModel> &value)
{
    customBoundary = value;
}

Eigen::Vector3d CustomTpmsSingleSurfaceConfig::getPeriodCycleLength() const
{
    return periodCycleLength;
}

void CustomTpmsSingleSurfaceConfig::setPeriodCycleLength(const Eigen::Vector3d &value)
{
    periodCycleLength = value;
}

double CustomTpmsSingleSurfaceConfig::getCustomBoundaryVoxelSize() const
{
    return customBoundaryVoxelSize;
}

void CustomTpmsSingleSurfaceConfig::setCustomBoundaryVoxelSize(double value)
{
    customBoundaryVoxelSize = value;
}
