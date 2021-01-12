#include "VoxelOctree.h"

using std::make_shared;

VoxelOctree::VoxelOctree(
        Eigen::AlignedBox3d boundingBox,
        Eigen::Vector3i voxelCount,
        vector<bool> voxelData,
        double voxelSize)
{
    initRoot(boundingBox, voxelCount, voxelData, voxelSize);
}

/**
 * @brief VoxelOctree::initRoot
 * 构建整颗八叉树，根节点的构建入口
 * 1. 初始化必要数据
 * 2. 调用子节点构造函数，递归的构建子节点
 * @param boundingBox
 * @param voxelCount 限制尺寸 2^n * 2^n * 2^n
 * @param voxelData
 * @param voxelSize
 * @param maxDepth
 * @param voxelPerNode
 */
void VoxelOctree::initRoot(
        AlignedBox3d boundingBox,
        Vector3i voxelCount,
        vector<bool> voxelData,
        double voxelSize,
        int maxDepth,
        int voxelPerNode)
{
    /// 扩充体素，方便构建树
    expandVoxelToCube(boundingBox, voxelCount, voxelData, voxelSize);

    /// 整棵树的共享数据
    this->maxDepth = make_shared<int>(maxDepth);
    this->voxelPerNode = make_shared<int>(voxelPerNode);
    this->voxelSize = make_shared<double>(voxelSize);

    this->voxelData = std::make_shared<vector<bool>>();
    this->voxelData->assign(voxelData.begin(), voxelData.end());

    this->voxelCount = std::make_shared<Vector3i>();
    this->voxelCount->operator=(voxelCount);

    /// 根节点的数据内容
    // 包围盒
    this->boundingBox = boundingBox;

    // 节点范围
    this->range.extend(Vector3i(0, 0, 0));
    this->range.extend(voxelCount);

    // 当前深度
    this->depth = 0;

    /// 构建整棵树
    this->build();

}

/**
 * @brief VoxelOctree::intersectRay
 * 返回射线和体素模型相交的所有表面
 * @param origin
 * @param direction
 * @param allowBack
 * @return
 */
vector<Eigen::Vector3d> VoxelOctree::intersectRay(
        Eigen::Vector3d origin,
        Eigen::Vector3d direction,
        bool allowBack)
{
    // TODO
    return vector<Eigen::Vector3d>();
}

/**
 * @brief VoxelOctree::intersectRayAllVoxel
 * 返回射线经过的体素模型的体素的 Index
 * @param origin
 * @param direction
 * @return
 */
vector<Eigen::Vector3i> VoxelOctree::intersectRayAllVoxel(
        Eigen::Vector3d origin,
        Eigen::Vector3d direction)
{
    return vector<Eigen::Vector3i>();
}

/**
 * @brief VoxelOctree::contains
 * @param point
 * @return
 */
bool VoxelOctree::contains(const Eigen::Vector3d &point)
{
    Vector3i index = getIndex(point, boundingBox, *voxelSize);
    if(index.x() < 0) {
        // 小于 0 表示顶点位于体素模型外部
        return false;
    }

    return getVoxel(index.x(), index.y(), index.z());
}

/**
 * @brief VoxelOctree::isInternalFace
 * 判断是否是内部面
 * TODO: 未完成
 * @param xIndex
 * @param yIndex
 * @param zIndex
 * @param faceEnum
 * @return
 */
bool VoxelOctree::isInternalFace(int xIndex, int yIndex, int zIndex, VoxelFaceEnum faceEnum) {
    assert(getVoxel(xIndex, yIndex, zIndex));

    switch (faceEnum) {
    case VOXEL_FRONT:
        return !getVoxel(xIndex, yIndex, zIndex);
    case VOXEL_BACK:
        return !getVoxel(xIndex, yIndex, zIndex);
    case VOXEL_LEFT:
        return !getVoxel(xIndex, yIndex, zIndex);
    case VOXEL_RIGHT:
        return !getVoxel(xIndex, yIndex, zIndex);
    case VOXEL_TOP:
        return !getVoxel(xIndex, yIndex, zIndex);
    case VOXEL_BOTTOM:
        return !getVoxel(xIndex, yIndex, zIndex);
    }
}

/**
 * @brief VoxelOctree::build
 * 细分构建八叉树，每一个节点入口
 * 1. 判断当前节点是否需要继续细分：
 *     a. 将当前的节点分为8个子节点，构建子节点的数据
 */
void VoxelOctree::build()
{
    /// 检查递归的退出条件
    if(depth == *maxDepth) {
        // 达到最大设定深度
        return;
    }

    int counts = range.volume();
    if(counts < *voxelPerNode) {
        // 达到叶子节点最少体素数量
        return;
    }

    /// 构建八颗子树
    int childDepth = depth + 1;
    helperNewNode(-1, -1, -1, childDepth);
    helperNewNode(-1, -1,  1, childDepth);
    helperNewNode(-1,  1, -1, childDepth);
    helperNewNode(-1, -1,  1, childDepth);
    helperNewNode( 1, -1, -1, childDepth);
    helperNewNode( 1, -1,  1, childDepth);
    helperNewNode( 1,  1, -1, childDepth);
    helperNewNode( 1,  1,  1, childDepth);
}

/**
 * @brief VoxelOctree::helperNewNode
 * 构建子节点的帮助函数
 * 在父节点中调用
 * @param axisX
 * @param axisY
 * @param axisZ
 * @param depth
 */
void VoxelOctree::helperNewNode(int axisX, int axisY, int axisZ, int depth)
{
    shared_ptr<VoxelOctree> childOctree = make_shared<VoxelOctree>();
    this->children.push_back(childOctree);

    /// 构建共享数据
    childOctree->voxelData = voxelData;
    childOctree->voxelCount = voxelCount;
    childOctree->voxelSize = voxelSize;
    childOctree->maxDepth = maxDepth;
    childOctree->voxelPerNode = voxelPerNode;

    /// 构建节点私有数据
    // 包围盒
    childOctree->boundingBox.extend(boundingBox.center());
    Vector3d boundingHelperPoint = boundingBox.max() - boundingBox.min();
    boundingHelperPoint = boundingHelperPoint / 2;
    boundingHelperPoint.x() = boundingHelperPoint.x() * axisX;
    boundingHelperPoint.y() = boundingHelperPoint.y() * axisY;
    boundingHelperPoint.z() = boundingHelperPoint.z() * axisZ;
    childOctree->boundingBox.extend(boundingHelperPoint + boundingBox.center());

    // 深度
    childOctree->depth = depth;

    // 构建子节点负责的包围盒范围
    childOctree->range.extend(range.center());
    Vector3i rangeHelperPoint = range.max() - range.min();
    rangeHelperPoint = rangeHelperPoint / 2;
    rangeHelperPoint.x() = rangeHelperPoint.x() * axisX;
    rangeHelperPoint.y() = rangeHelperPoint.y() * axisY;
    rangeHelperPoint.z() = rangeHelperPoint.z() * axisZ;
    childOctree->range.extend(rangeHelperPoint + range.center());

    /// 将当前节点作为非叶子节点继续构建
    childOctree->build();
}
