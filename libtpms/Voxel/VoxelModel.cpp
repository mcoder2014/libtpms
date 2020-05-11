#include "VoxelModel.h"
#include "TPMS/MarchBoxUtil.h"

VoxelModel::VoxelModel()
{

}

/**
 * @brief VoxelModel::build
 * 构建体素模型
 * @param octree
 */
void VoxelModel::build(Octree& octree)
{
    this->boundingBox = getBoundingBoxFromOctree(octree);

}

bool VoxelModel::contains(Eigen::Vector3d &point)
{
    return false;
}

VoxelData VoxelModel::getVoxelMatrix() const
{
    return voxelMatrix;
}

void VoxelModel::setVoxelMatrix(const VoxelData &value)
{
    voxelMatrix = value;
}

Vector3d VoxelModel::getCenter() const
{
    return center;
}

Eigen::AlignedBox3d VoxelModel::getBoundingBoxFromOctree(Octree &octree)
{
    Vector3d center = octree.boundingBox.center;
    Vector3d relative = octree.boundingBox.vmax - octree.boundingBox.vmin;
    double longestEdge = std::max(relative.x(), std::max(relative.y(), relative.z()));

    Vector3d minVertex = center;
    minVertex.x() -= longestEdge/2;
    minVertex.y() -= longestEdge/2;
    minVertex.z() -= longestEdge/2;

    Vector3d maxVertex = minVertex;
    maxVertex.x() += longestEdge;
    maxVertex.y() += longestEdge;
    maxVertex.z() += longestEdge;

    Eigen::AlignedBox3d boundingBox;
    boundingBox.extend(minVertex);
    boundingBox.extend(maxVertex);
    return boundingBox;
}

/**
 * @brief VoxelModel::getMatrixIndex
 * 空间上一点，找到位于体素矩阵中哪个 Index
 * @param point
 * @return
 */
Eigen::Vector3i VoxelModel::getMatrixIndex(Eigen::Vector3d point)
{

}

Eigen::AlignedBox3d VoxelModel::getBoundingBox() const
{
    return boundingBox;
}

Vector3i VoxelModel::getVoxelMatrixSize() const
{
    return voxelMatrixSize;
}

double VoxelModel::getVoxelSize() const
{
    return voxelSize;
}

void VoxelModel::setVoxelSize(double value)
{
    voxelSize = value;
}
