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

Eigen::Vector3i BaseTpmsConfig::getVoxelDensity() const
{
    return voxelDensity;
}

void BaseTpmsConfig::setVoxelDensity(const Eigen::Vector3i &value)
{
    voxelDensity = value;
}

/**
 * 根据 config 的值计算采样矩阵的尺寸
 * 由 物理包围盒 和 体素密度决定
 * @brief BaseTpmsConfig::getMatrixSize
 * @return
 */
Eigen::Vector3i BaseTpmsConfig::getMatrixSize() const
{
    Eigen::Vector3d relativeSize = boundingBoxPhysial.max() - boundingBoxPhysial.min();
    Eigen::Vector3i matrixSize;

    matrixSize.x() = relativeSize.x() / voxelDensity.x();
    matrixSize.y() = relativeSize.y() / voxelDensity.y();
    matrixSize.z() = relativeSize.z() / voxelDensity.z();

    return matrixSize;
}
