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
    VoxelModel();

    void setVoxelSize(double value);
    void build(Octree& octree);
    bool contains(Vector3d& point);

    VoxelData getVoxelMatrix() const;
    void setVoxelMatrix(const VoxelData &value);

    double getVoxelSize() const;

    Vector3d getCenter() const;
    Vector3i getVoxelMatrixSize() const;
    Eigen::AlignedBox3d getBoundingBox() const;

private:
    Eigen::AlignedBox3d getBoundingBoxFromOctree(Octree& octree);
    Vector3i getMatrixIndex(Vector3d point);

    VoxelData voxelMatrix;
    Vector3i voxelMatrixSize;

    // 方形体素的边长
    double voxelSize;
    Vector3d center;
    Eigen::AlignedBox3d boundingBox;
};

#endif // VOXELMODEL_H
