#ifndef VOXELMODEL_H
#define VOXELMODEL_H

#include <vector>

#include <Eigen/Dense>

#include <Octree.h>

using std::vector;
using Eigen::Vector3d;
using Eigen::Vector3i;

// false -> not have a voxel
// true -> have a voxel
using VoxelData = vector<vector<vector<bool>>>;

/**
 * @brief The VoxelModel class
 * 基于方形体素的体素模型
 */
class VoxelModel
{
public:
    VoxelModel(double voxelSize = 1);

    void setVoxelSize(double value);
    void build(Octree& octree);
    bool contains(Vector3d& point);
    bool validIndex(const Vector3i &index) const;

    void clear();

    VoxelData getVoxelMatrix() const;
    double getVoxelSize() const;
    Vector3d getCenter() const;
    Vector3i getVoxelMatrixSize() const;
    Eigen::AlignedBox3d getBoundingBox() const;
    Eigen::AlignedBox3d getOriginBoundingBox() const;

private:
    Eigen::AlignedBox3d getBoundingBoxFromOctree(Octree& octree);
    Vector3i getMatrixIndex(Eigen::Vector3d point);
    Vector3i createMatrixSize();
    void generateVoxelModelFromOctree(VoxelData& voxelData, Octree& octree);
    void updateVoxelFromIntersects(vector<bool> &voxels, vector<Vector3d>& intersects);

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
