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
    VoxelOctree();

    // 构建 VoxelOctree
    void build();
    bool contains(const Vector3d& point);

    Eigen::AlignedBox3d getBoundingBox() const;
    void setBoundingBox(const Eigen::AlignedBox3d &value);

    std::shared_ptr<SurfaceMesh::SurfaceMeshModel> getModel() const;
    void setModel(const std::shared_ptr<SurfaceMesh::SurfaceMeshModel> &value);

private:
    std::shared_ptr<SurfaceMesh::SurfaceMeshModel> model;
    // 包围盒
    Eigen::AlignedBox3d boundingBox;
    // 子区域
    vector<VoxelOctree> children;
    // 体素数据
    vector<VoxelNode> voxelData;

    int maxVoxelCounts = 128;   // 最小节点的最大体素数量
    VoxelOctree *parent;

};

#endif // VOXELOCTREE_H
