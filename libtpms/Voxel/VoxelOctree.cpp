#include "VoxelOctree.h"

VoxelOctree::VoxelOctree(vector<std::shared_ptr<VoxelNode> > voxelList)
{
    this->voxelData = voxelList;
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

std::shared_ptr<SurfaceMesh::SurfaceMeshModel> VoxelOctree::getModel() const
{
    return model;
}

void VoxelOctree::setModel(const std::shared_ptr<SurfaceMesh::SurfaceMeshModel> &value)
{
    model = value;
}

void VoxelOctree::initBuild()
{
    boundingBox = getBoundingBoxByVoxels();
    build();

    // 构建父指针

}

void VoxelOctree::build()
{
    if(static_cast<int>(voxelData.size()) > maxVoxelCounts) {
        // 细分
        newNode(-1, -1, -1);
        newNode(1, -1, -1);
        newNode(-1, 1, -1);
        newNode(1, 1, -1);
        newNode(1, -1, 1);
        newNode(1, -1, 1);
        newNode(-1, 1, 1);
        newNode(1, 1, 1);
    }
}


void VoxelOctree::newNode(double x, double y, double z)
{
    /// TODO: 往指定方向拓展节点
}

/**
 * @brief VoxelOctree::getBoundingBoxByVoxels
 * 从一堆体素中获得包围盒，做倍率放大，参考输入 1.1
 * @return
 */
Eigen::AlignedBox3d VoxelOctree::getBoundingBoxByVoxels(double factor)
{
    Eigen::AlignedBox3d boundingBox;

    // 构造包围盒(略小于真实情况
    for(std::shared_ptr<VoxelNode> node:this->voxelData) {
        boundingBox.extend(node->getCenter());
    }

    // 扩大包围盒尺寸：立方体、放大到 factor 倍率
    Vector3d relativeSize = boundingBox.max() - boundingBox.min();
    double longestEdge = std::max(relativeSize.x(),
        std::max(relativeSize.y(), relativeSize.z()));
    longestEdge = longestEdge * factor;
    Vector3d center = boundingBox.center();

    Vector3d minPoint;
    minPoint.x() = center.x() - longestEdge/2;
    minPoint.y() = center.y() - longestEdge/2;
    minPoint.z() = center.z() - longestEdge/2;

    Vector3d maxPoint;
    maxPoint.x() = minPoint.x() + longestEdge;
    maxPoint.y() = minPoint.y() + longestEdge;
    maxPoint.z() = minPoint.z() + longestEdge;

    // 重置包围盒到以原包围盒为中心的立方体包围盒
    boundingBox.setEmpty();
    boundingBox.extend(minPoint);
    boundingBox.extend(maxPoint);

    return boundingBox;
}

int VoxelOctree::getMaxVoxelCounts() const
{
    return maxVoxelCounts;
}

void VoxelOctree::setMaxVoxelCounts(int value)
{
    maxVoxelCounts = value;
}
