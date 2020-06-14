#include "VoxelOctree.h"

VoxelOctree::VoxelOctree()
    :depth(8)
{

}

/**
 * @brief VoxelOctree::build
 * 构建八叉树结构
 */
void VoxelOctree::build(const VoxelModel &voxelModel)
{
    // 确保输入的 Voxel Model 是已经构建好的
    Vector3i voxelModelSize = voxelModel.getVoxelMatrixSize();
    assert(voxelModelSize.x() > 0);

    // TODO: 构建八叉树结构

}

/**
 * @brief VoxelOctree::getIntersects
 * 求射线与八叉树结构的相交点
 * @param startPoint
 * @param direction
 * @return
 */
vector<Eigen::Vector3d> VoxelOctree::getIntersects(const Eigen::Vector3d &startPoint, const Eigen::Vector3d &direction)
{
    startPoint.x();
    startPoint.x();
    direction.x();
    return vector<Eigen::Vector3d>();
}
