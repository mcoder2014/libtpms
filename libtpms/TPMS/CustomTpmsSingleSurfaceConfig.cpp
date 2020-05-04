#include "CustomTpmsSingleSurfaceConfig.h"

Eigen::Vector3i CustomTpmsSingleSurfaceConfig::getMatrixSize() const
{
    assert(customBoundary != nullptr);

    customBoundary->updateBoundingBox();
    Eigen::AlignedBox3d boundingBoxPhysial = customBoundary->bbox();
    Vector3d relativeSize = boundingBoxPhysial.max() - boundingBoxPhysial.min();
    Vector3i matrixSize;

    matrixSize.x() = relativeSize.x() * voxelDensity.x();
    matrixSize.y() = relativeSize.y() * voxelDensity.y();
    matrixSize.z() = relativeSize.z() * voxelDensity.z();

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
    maxPoint.x() /= periodCycleLength.x();
    maxPoint.y() /= periodCycleLength.y();
    maxPoint.z() /= periodCycleLength.z();

    minPoint.x() /= periodCycleLength.x();
    minPoint.y() /= periodCycleLength.y();
    minPoint.z() /= periodCycleLength.z();

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
