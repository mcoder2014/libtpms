#include "CustomTpmsSingleSurfaceConfig.h"

Eigen::Vector3i CustomTpmsSingleSurfaceConfig::getMatrixSize() const
{
    customBoundary->updateBoundingBox();
    Eigen::AlignedBox3d boundaryBoxPhysical = customBoundary->bbox();

    return Eigen::Vector3i();
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
