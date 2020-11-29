#include "VoxelTools.h"

/**
 * @brief getVoxelIndex
 * 根据 x y z 的索引获得体素在实际数据中的索引
 * @param xIndex
 * @param yIndex
 * @param zIndex
 * @param voxelCount
 * @return
 */
size_t getVoxelIndex(int xIndex, int yIndex, int zIndex, const Vector3i& voxelCount){
    return static_cast<size_t>(xIndex * voxelCount.y() * voxelCount.z() + yIndex * voxelCount.z() + zIndex);
}

/**
 * @brief getVoxel
 * @param xIdx
 * @param yIdx
 * @param zIdx
 * @param voxelCount
 * @param voxelData
 * @return
 */
bool getVoxel(int xIdx, int yIdx, int zIdx, const Eigen::Vector3i &voxelCount, const vector<bool> &voxelData)
{
    return voxelData[getVoxelIndex(xIdx, yIdx, zIdx, voxelCount)];
}

/**
 * @brief expandVoxelToCube
 * 体素如果不是规则的立方体，且边长不是 2 的幂次关系的话，比较麻烦
 * 将体素修复为边长统一的 2 的次幂关系
 * 1. 以最长边的最接近的 2 次幂为拓展后的体素的边长
 * 2. 检查，如果三边长度相等，则跳过扩展
 * 3. 将原 box 放在扩展的 box 的中心位置，在周围填充 0
 * @param boundingBox
 * @param voxelCount
 * @param voxelData
 * @param voxelSize
 */
void expandVoxelToCube(Eigen::AlignedBox3d &boundingBox, Eigen::Vector3i &voxelCount, vector<bool> &voxelData, const double &voxelSize)
{
    // 找到拓展后的体素边长
    int longestEdge = std::max(voxelCount.x(), std::max(voxelCount.y(), voxelCount.z()));
    int expandEdge = 1;
    while(expandEdge < longestEdge) {
        expandEdge *= 2;
    }
    Vector3i expandCount(expandEdge, expandEdge, expandEdge);

    // 如果不需要拓展
//    if(voxelCount.x() == expandEdge && voxelCount.y() == expandEdge&& voxelCount.z() == expandEdge) {
    if(expandCount == voxelCount){
        return;
    }

    // 将原模型放置在拓展的中心，左开右闭
    Vector3i startVoxel = (expandCount - voxelCount) / 2;
    Vector3i endVoxel = startVoxel + voxelCount;

    auto getIndex = [](Vector3i index, Vector3i voxelCount)->size_t{
            return static_cast<size_t>(
                        index.x() * voxelCount.y() * voxelCount.z()
                        + index.y() * voxelCount.z()
                        + index.z());
    };

    /// 复制 Data
    vector<bool> tmpVoxelData(static_cast<size_t>(expandEdge * expandEdge * expandEdge), false);
    for(Vector3i idx(startVoxel.x(), 0, 0); idx.x() < endVoxel.x(); idx.x()++) {
        for(idx.y() = startVoxel.y(); idx.y() < endVoxel.y(); idx.y()++) {
            for(idx.z() = startVoxel.z(); idx.z() < endVoxel.z(); idx.z()++) {
                // 复制数据
                tmpVoxelData[getIndex(idx, expandCount)]
                        = voxelData[getIndex(idx - startVoxel, voxelCount)];
            }
        }
    }

    voxelData.swap(tmpVoxelData);

    /// 修改包围盒
    Vector3d minPoint = boundingBox.min();
    minPoint.x() = minPoint.x() - startVoxel.x() * voxelSize;
    minPoint.y() = minPoint.y() - startVoxel.y() * voxelSize;
    minPoint.z() = minPoint.z() - startVoxel.z() * voxelSize;
    boundingBox.extend(minPoint);

    Vector3d maxPoint = boundingBox.max();
    maxPoint.x() = maxPoint.x() + (expandEdge - endVoxel.x()) * voxelSize;
    maxPoint.x() = maxPoint.y() + (expandEdge - endVoxel.y()) * voxelSize;
    maxPoint.x() = maxPoint.z() + (expandEdge - endVoxel.z()) * voxelSize;
    boundingBox.extend(maxPoint);

    /// 修改体素尺寸
    voxelCount = expandCount;
}

/**
 * @brief getIndex
 * 计算空间一点在体素中的索引；
 * - 索引从 (0,0,0) 开始
 * - 该点不在模型内部，返回索引(-1, -1, -1)
 * @param point
 * @param boundingBox
 * @param voxelSize
 * @return
 */
Eigen::Vector3i getIndex(Eigen::Vector3d point, const Eigen::AlignedBox3d &boundingBox, double voxelSize)
{
    Vector3i index(-1, -1, -1);
    if(!boundingBox.contains(point)) {
        return index;
    }
    point = point - boundingBox.min();
    point = point / voxelSize;

    index.x() = static_cast<int>(point.x());
    index.x() = static_cast<int>(point.x());
    index.x() = static_cast<int>(point.x());

    return index;
}
