#ifndef VOXELTOOLS_H
#define VOXELTOOLS_H

#include <vector>

#include <Eigen/Dense>

using std::vector;
using Eigen::Vector3i;
using Eigen::Vector3d;
using Eigen::AlignedBox3d;
using Eigen::AlignedBox3i;

/**
 * @brief The VoxelFaceEnum enum
 * 涉及到体素的某个面的问题
 */
enum VoxelFaceEnum
{
    VOXEL_FRONT,
    VOXEL_BACK,
    VOXEL_LEFT,
    VOXEL_RIGHT,
    VOXEL_TOP,
    VOXEL_BOTTOM,
};

// 根据 x y z 的索引获得体素在实际数据中的索引
size_t getVoxelIndex(int xIndex, int yIndex, int zIndex, const Vector3i& voxelCount);

// 获得体素中指定索引元素的值
bool getVoxel(int xIdx, int yIdx, int zIdx, const Vector3i& voxelCount, const vector<bool> &voxelData);

/**
 * @brief getIndex
 * 估算顶点在体素中的索引
 * @param point
 * @param boundingBox
 * @param voxelSize
 * @return
 */
Vector3i getIndex(Vector3d point, const AlignedBox3d& boundingBox, double voxelSize);

// 将不规则形状的体素扩充为规则形状的空间
void expandVoxelToCube(AlignedBox3d& boundingBox, Vector3i& voxelCount,
              vector<bool>& voxelData, const double& voxelSize);

#endif // VOXELTOOLS_H
