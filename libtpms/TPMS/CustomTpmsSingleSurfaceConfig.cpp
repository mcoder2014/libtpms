#include "CustomTpmsSingleSurfaceConfig.h"

#include <Math/EigenUtil.h>

Eigen::Vector3i CustomTpmsSingleSurfaceConfig::getMatrixSize() const
{
    // TODO: 对于尺寸特别巨大模型要给 factor 约束，防止撑爆内存
    assert(customBoundary != nullptr);
    Eigen::AlignedBox3d boundingBoxPhysial = customBoundary->getOriginBoundingBox();
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
    return customBoundary->getOriginBoundingBox();
}

Eigen::AlignedBox3d CustomTpmsSingleSurfaceConfig::getBoundingBoxLogical() const
{
    assert(customBoundary != nullptr);
    Eigen::AlignedBox3d boundingBoxPhysial = customBoundary->getOriginBoundingBox();
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

Eigen::Vector3d CustomTpmsSingleSurfaceConfig::getPeriodCycleLength() const
{
    return periodCycleLength;
}

void CustomTpmsSingleSurfaceConfig::setPeriodCycleLength(const Eigen::Vector3d &value)
{
    periodCycleLength = value;
}

std::vector<std::shared_ptr<BaseSampleMatrixFilter> > CustomTpmsSingleSurfaceConfig::getSampleMatrixFilterVector() const
{
    return sampleMatrixFilterVector;
}

void CustomTpmsSingleSurfaceConfig::setSampleMatrixFilterVector(const std::vector<std::shared_ptr<BaseSampleMatrixFilter> > &value)
{
    sampleMatrixFilterVector = value;
}

/**
 * @brief CustomTpmsSingleSurfaceConfig::addSampleMatrixFilter
 * 增加滤波器，在算法执行前对采样点进行滤波
 * @param baseFilter
 */
void CustomTpmsSingleSurfaceConfig::addSampleMatrixFilter(std::shared_ptr<BaseSampleMatrixFilter> baseFilter)
{
    this->sampleMatrixFilterVector.push_back(baseFilter);
}

std::shared_ptr<VoxelModel> CustomTpmsSingleSurfaceConfig::getCustomBoundary() const
{
    return customBoundary;
}

void CustomTpmsSingleSurfaceConfig::setCustomBoundary(std::shared_ptr<VoxelModel> value)
{
    assert(value->getVoxelMatrixSize().x() > 0);
    customBoundary = value;
}
