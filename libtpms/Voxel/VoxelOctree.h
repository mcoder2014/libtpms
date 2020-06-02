#ifndef VOXELOCTREE_H
#define VOXELOCTREE_H

#include <vector>
#include <Eigen/Dense>

#include "TPMS/MarchBoxUtil.h"
#include "VoxelModel.h"

// TODO: 实现体素八叉树，方便计算射线的交点
class VoxelOctree
{
public:
    VoxelOctree();
    void build(const VoxelModel& voxelModel);

    vector<Vector3d> getIntersects(const Vector3d& startPoint, const Vector3d& direction);

private:
    // sub nodes: 0 or 8
    std::vector<VoxelOctree> children;
    // boudingbox
    Eigen::AlignedBox3d boundingBox;
    // 八叉树的深度
    int depth;
};

#endif // VOXELOCTREE_H
