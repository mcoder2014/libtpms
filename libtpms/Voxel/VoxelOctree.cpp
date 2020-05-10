#include "VoxelOctree.h"

VoxelOctree::VoxelOctree(vector<std::shared_ptr<VoxelNode> > voxelList)
{
    this->voxelData = voxelList;
    initBuild();
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

Eigen::AlignedBox3d VoxelOctree::getBoundingBoxByVoxels()
{
    Eigen::AlignedBox3d boundingBox;

    // 寻找 center
    for(std::shared_ptr<VoxelNode> node:this->voxelData) {
        boundingBox.extend(node->getCenter());
    }

    // 适当扩大包围盒到 立方体
    Vector3d relativeSize = boundingBox.max() - boundingBox.min();
    double longestEdge = std::max(relativeSize.x(),
        std::max(relativeSize.y(), relativeSize.z()));
    longestEdge = longestEdge * 1.1;
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
