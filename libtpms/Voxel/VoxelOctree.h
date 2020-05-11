#ifndef VOXELOCTREE_H
#define VOXELOCTREE_H

#include <vector>
#include <memory>

#include <Eigen/Dense>
#include <SurfaceMeshModel.h>

#include "VoxelNode.h"

using std::vector;

class VoxelOctree
{
public:
    VoxelOctree(vector<std::shared_ptr<VoxelNode>> voxelList);

    // 构造八叉树
    void initBuild();

    // 判断空间点 point 是否在模型内部
    bool contains(const Vector3d& point);

    Eigen::AlignedBox3d getBoundingBox() const;

    std::shared_ptr<SurfaceMesh::SurfaceMeshModel> getModel() const;
    void setModel(const std::shared_ptr<SurfaceMesh::SurfaceMeshModel> &value);

    int getMaxVoxelCounts() const;
    void setMaxVoxelCounts(int value);

private:

    void build();
    void newNode(double x, double y, double z);

    Eigen::AlignedBox3d getBoundingBoxByVoxels(double factor = 1.1);

    std::shared_ptr<SurfaceMesh::SurfaceMeshModel> model;
    // 包围盒
    Eigen::AlignedBox3d boundingBox;
    // 子区域
    vector<VoxelOctree> children;
    // 体素数据
    vector<std::shared_ptr<VoxelNode>> voxelData;

    int maxVoxelCounts = 128;   // 最小节点的最大体素数量
    VoxelOctree *parent;

};

#endif // VOXELOCTREE_H
