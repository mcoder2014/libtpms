#include "VoxelOctree.h"

VoxelOctree::VoxelOctree()
{

}

void VoxelOctree::build()
{
    /// TODO:将体素的模型八叉树化 加快检索速度
}

/**
 * 查找 一空间点是否在模型内部
 * @brief VoxelOctree::contains
 * @param point
 * @return
 */
bool VoxelOctree::contains(const Eigen::Vector3d &point)
{
    /// TODO:迭代方法判断顶点是否在模型内部
    return false;
}

Eigen::AlignedBox3d VoxelOctree::getBoundingBox() const
{
    return boundingBox;
}

void VoxelOctree::setBoundingBox(const Eigen::AlignedBox3d &value)
{
    boundingBox = value;
}

std::shared_ptr<SurfaceMesh::SurfaceMeshModel> VoxelOctree::getModel() const
{
    return model;
}

void VoxelOctree::setModel(const std::shared_ptr<SurfaceMesh::SurfaceMeshModel> &value)
{
    model = value;
}
