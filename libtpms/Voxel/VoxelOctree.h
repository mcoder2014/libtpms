#ifndef VOXELOCTREE_H
#define VOXELOCTREE_H

#include <vector>
#include <memory>

#include <Eigen/Dense>

#include "TPMS/MarchBoxUtil.h"
#include "VoxelModel.h"
#include "VoxelTools.h"

using std::shared_ptr;
using Eigen::Vector3d;
using Eigen::Vector3i;

using Eigen::AlignedBox3d;
using Eigen::AlignedBox3i;

class VoxelOctree
{
public:
    VoxelOctree(){}
    VoxelOctree(AlignedBox3d boundingBox, Vector3i voxelCount,
                vector<bool> voxelData, double voxelSize);

    // 构建八叉树结构
    void initRoot(
            AlignedBox3d boundingBox, Vector3i voxelCount,
            vector<bool> voxelData, double voxelSize,
            int maxDepth = 64, int voxelPerNode = 64);

    // 判断是否是叶子节点
    inline bool isLeaf() {return children.empty();}

    /// 布尔运算逻辑
    // TODO 射线求教，返回射线相交的外表面交点
    vector<Vector3d> intersectRay(Vector3d origin, Vector3d direction, bool allowBack = true);

    // 射线求交，返回射线经过的所有体素
    vector<Vector3i> intersectRayAllVoxel(Vector3d origin, Vector3d direction);

    // TODO 包含关系
    bool contains(const Vector3d& point);

public:
    /// 体素处理相关的工具函数
    // 获得体素的值
    inline bool getVoxel(int xIndex, int yIndex, int zIndex) {
        return voxelData->operator[](getVoxelIndex(xIndex, yIndex, zIndex, *voxelCount));
    }

    // 获得体素数据
    inline shared_ptr<vector<bool>> getVoxelData() {return voxelData;}

private:
    /// 节点的层级关系
    // 当前节点的父节点
    std::weak_ptr<VoxelOctree> parent;
    // 当前节点的子节点，有 8个 或 0个
    std::vector<shared_ptr<VoxelOctree>> children;

    /// 节点自身的数据
    // boudingbox
    Eigen::AlignedBox3d boundingBox;
    // 节点拥有体素的节点的范围, 左闭右开
    // [xFrom, xTo) * [yFrom, yTo) * [zFrom, zTo)
    Eigen::AlignedBox3i range;
    // 节点的深度
    int depth;

    /// 整颗八叉树共享的数据（这些数据在整棵树中应该是相同的
    // 体素数据 采用线性存储, 第 [x][y][z] 个体素的地址
    // index = x * voxelSize.y() * voxelSize.z() + y * voxelSize.z() + z
    shared_ptr<vector<bool>> voxelData;
    // 体素数量 X 方向数量 Y 方向数量 Z 方向数量
    shared_ptr<Vector3i> voxelCount;
    // 体素尺寸
    shared_ptr<double> voxelSize;
    // 指示叶子节点的层级
    shared_ptr<int> voxelPerNode;
    // 指示树的最大深度
    shared_ptr<int> maxDepth;

private:
    /// Functions
    // 判断是否是内部面，该体素需要是 1
    bool isInternalFace(int xIndex, int yIndex, int zIndex, VoxelFaceEnum faceEnum);

    // 中间节点 build 函数
    void build();

    // 构建子节点
    void helperNewNode(int axisX, int axisY, int axisZ, int depth);
};

#endif // VOXELOCTREE_H
