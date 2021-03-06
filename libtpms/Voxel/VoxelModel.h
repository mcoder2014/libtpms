#ifndef VOXELMODEL_H
#define VOXELMODEL_H

#include <vector>

#include <Eigen/Dense>

#include <Octree.h>

using Eigen::Vector3d;
using Eigen::Vector3i;
using std::vector;

// false -> not have a voxel
// true -> have a voxel
using VoxelData = vector<vector<vector<bool>>>;

/**
 * @brief The VoxelModel class
 * 基于方形体素的体素模型
 */
class VoxelModel {
public:
    VoxelModel(double voxelSize = 1);

    // 判断空间一点是否在体素模型内部
    bool contains(Vector3d &point);
    bool contains(double x, double y, double z);

    // 找到直线和模型相交的最两端的焦点(Z轴方向的直线)
    vector<Vector3d> getOuterBoundaryZ(const Eigen::Vector3d &point);

    // 设置体素的尺寸，体素过小会导致效率低、占内存，提速过大会速度缓慢
    void setVoxelSize(double value) { voxelSize = value; }

    // 构建体素模型
    void build(Octree &octree);

    // 使用多线程方式构建 VoxelModel
    void buildMT(Octree &octree, size_t threadCount = 8);

    // 验证输入的index 是否合法
    bool validIndex(const Vector3i &index) const;

    // 获得空间一点在 体素模型中的坐标
    std::shared_ptr<Vector3i> getMatrixIndex(Eigen::Vector3d point);

    // 清空 体素模型的内容，释放内存
    void clear();

    // 找到体素的某个坐标
    std::shared_ptr<Vector3d> getVoxelMinPoint(const Vector3i &index) const;
    std::shared_ptr<Vector3d> getVoxelMaxPoint(const Vector3i &index) const;

    VoxelData getVoxelMatrix() const { return voxelMatrix; }
    double getVoxelSize() const { return voxelSize; }
    Vector3d getCenter() const { return center; }
    Vector3i getVoxelMatrixSize() const { return voxelMatrixSize; }
    Eigen::AlignedBox3d getBoundingBox() const { return boundingBox; }
    Eigen::AlignedBox3d getOriginBoundingBox() const { return originBoundingBox; }

private:
    Eigen::AlignedBox3d getBoundingBoxFromOctree(Octree &octree);

    // 计算体素应该的尺寸
    Vector3i claculateMatrixSize();

    // 从 octree 中构建体素模型
    void generateVoxelModelFromOctree(VoxelData &voxelData, Octree &octree);

    // 多线程版本从 Octree 中构建体素模型
    void generateVoxelModelFromOctreeMT(VoxelData &voxelData, Octree &octree, size_t threadCount);

    // 根据交点，更新体素结果
    void updateFromIntersects(vector<bool> &voxels, vector<Vector3d> &intersects);

    /// 私有数据
    // 体素矩阵
    VoxelData voxelMatrix;

    // 体素矩阵的尺寸, 为了方便与体素八叉树转换，我们倾向于将体素模型设置为立方体
    Vector3i voxelMatrixSize;

    // 方形体素的边长
    double voxelSize;

    // 体素模型的中心点
    Vector3d center;

    // 体素模型的包围盒大小
    Eigen::AlignedBox3d boundingBox;

    // 原始模型的包围盒大小
    Eigen::AlignedBox3d originBoundingBox;
};

#endif // VOXELMODEL_H
