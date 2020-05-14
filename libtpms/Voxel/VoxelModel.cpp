#include "VoxelModel.h"
#include "TPMS/MarchBoxUtil.h"
#include "Math/EigenUtil.h"
#include "SurfaceMesh/OctreeUtil.h"

VoxelModel::VoxelModel(double voxelSize)
{
    this->voxelSize = voxelSize;
}

/**
 * @brief VoxelModel::build
 * 构建体素模型
 * @param octree
 */
void VoxelModel::build(Octree& octree)
{
    // 从 octree 中获得包围盒
    boundingBox = getBoundingBoxFromOctree(octree);

    // 计算体素矩阵的大小
    voxelMatrixSize = createMatrixSize();

    // 初始化空体素模型
    voxelMatrix = create3DMatrix<bool>(voxelMatrixSize, false);

    // 构建 voxelModel
    generateVoxelModelFromOctree(voxelMatrix, octree);
}

/**
 * @brief VoxelModel::contains
 * 判断某个点是否在体素模型内部
 * @param point
 * @return
 */
bool VoxelModel::contains(Eigen::Vector3d &point)
{
    Vector3i index = getMatrixIndex(point);
    if(!validIndex(index)) {
        return false;
    }

    return voxelMatrix[index.x()][index.y()][index.z()];
}

/**
 * @brief VoxelModel::validIndex
 * 验证 index 是否合法有效
 * @param index
 * @return
 */
bool VoxelModel::validIndex(const Eigen::Vector3i &index) const
{
    if( index.x() < 0 || index.y() < 0 || index.z() < 0
        || index.x() > voxelMatrixSize.x()
        || index.y() > voxelMatrixSize.y()
        || index.y() > voxelMatrixSize.z() ) {
        return false;
    }

    return true;
}

void VoxelModel::clear()
{
    VoxelData emptyVoxelMatrix;
    voxelMatrix.swap(emptyVoxelMatrix);

    voxelMatrixSize = Vector3i(0,0,0);
    center = Vector3d(0.0, 0.0, 0.0);
    boundingBox.setEmpty();
}

VoxelData VoxelModel::getVoxelMatrix() const
{
    return voxelMatrix;
}

Vector3d VoxelModel::getCenter() const
{
    return center;
}

/**
 * @brief VoxelModel::getBoundingBoxFromOctree
 * 从八叉树中取得包围盒
 * 注：正确八叉树的包围盒应该是正方体
 * @param octree
 * @return
 */
Eigen::AlignedBox3d VoxelModel::getBoundingBoxFromOctree(Octree &octree)
{
    Eigen::AlignedBox3d boundingBox;
    boundingBox.extend(octree.boundingBox.vmax);
    boundingBox.extend(octree.boundingBox.vmin);
    return boundingBox;
}

/**
 * @brief VoxelModel::getMatrixIndex
 * 空间上一点，找到位于体素矩阵中哪个 Index
 * @param point
 * @return
 * 在模型外部，返回 (-1, -1, -1)
 */
Eigen::Vector3i VoxelModel::getMatrixIndex(Eigen::Vector3d point)
{
    Vector3d minPoint = boundingBox.min();
    point = point - minPoint;   // 根据情况
    point = point / voxelSize;

    // 判断 index
    if(point.x() > voxelMatrixSize.x() || point.x() < 0
       || point.y() > voxelMatrixSize.y() || point.y() < 0
       || point.z() > voxelMatrixSize.z() || point.z() < 0) {
        return Vector3i(-1,-1,-1);
    }

    // 转换浮点数为整型
    Vector3i index = vector3dToVector3i(point, [](double value)->int {
        return lround(value);});

    return index;
}

/**
 * @brief VoxelModel::createMatrixSize
 * 根据 boundingBox 和 voxelSize 计算矩阵大小
 * 为了取整，向 max 方向扩张 boundingBox
 * @return
 */
Eigen::Vector3i VoxelModel::createMatrixSize()
{
    Vector3d relative = boundingBox.max() - boundingBox.min();

    // 进一法
    Vector3i matrixSize;
    matrixSize.x() = (int)(relative.x() / voxelSize) + 1;
    matrixSize.y() = (int)(relative.y() / voxelSize) + 1;
    matrixSize.z() = (int)(relative.z() / voxelSize) + 1;

    // 更新包围盒
    Vector3d minPoint = boundingBox.min();
    Vector3d maxPoint;
    maxPoint.x() = minPoint.x() + matrixSize.x() * voxelSize;
    maxPoint.y() = minPoint.y() + matrixSize.y() * voxelSize;
    maxPoint.z() = minPoint.z() + matrixSize.z() * voxelSize;
    boundingBox.extend(maxPoint);

    return matrixSize;
}

/**
 * @brief VoxelModel::generateVoxelModelFromOctree
 * 利用射线，从八叉树中建立体素结构
 * @param voxelData
 * @param octree
 */
void VoxelModel::generateVoxelModelFromOctree(VoxelData &voxelData, Octree &octree)
{
    Vector3d minPoint = boundingBox.min();
    Vector3i index;
    Vector3d point;
    point.z() = minPoint.z();

    for(index.x() = 0; index.x() < voxelMatrixSize.x(); index.x()++) {
        point.x() = minPoint.x() + index.x() * voxelSize;
        for(index.y() = 0; index.y() < voxelMatrixSize.y(); index.y()++) {
            point.y() = minPoint.y() + index.y() * voxelSize;
            // 获得排序后的交点
            vector<Vector3d> intersectPoints =
                    getIntersectPointsDirectionZ(octree, point);
            // 根据交点修改对应位置的体素值为 true or false
            updateVoxelFromIntersects(voxelData[index.x()][index.y()], intersectPoints);
        }
    }
}

/**
 * @brief VoxelModel::getVoxelFromIntersects
 * 根据交点更新体素的值
 * @param voxels
 * @param intersects
 */
void VoxelModel::updateVoxelFromIntersects(
        vector<bool> &voxels,
        vector<Eigen::Vector3d> &intersects)
{
    double pos = boundingBox.min().z();
    int countZ = voxelMatrixSize.z();

    // 交点两两一组，一组中间的空间在体素内部
    if(intersects.size() < 2) {
        return;
    }

    Vector3d lowerBoundary;
    Vector3d upperBoundary;

    // 局部函数，更新边界
    auto updateBoundary = [&](){
        lowerBoundary = intersects[0];
        upperBoundary = intersects[1];
        intersects.erase(intersects.begin());
        intersects.erase(intersects.begin());
    };

    updateBoundary();

    // loop
    for(int i = 0; i < countZ; i++, pos += voxelSize) {
        if(pos > lowerBoundary.z() && pos < upperBoundary.z()) {
            voxels[i] = true;
        } else if(pos > upperBoundary.z()) {
            if(intersects.size() >= 2) {
                updateBoundary();
            } else {
                break;
            }
        }
    }
}

Eigen::AlignedBox3d VoxelModel::getBoundingBox() const
{
    return boundingBox;
}

Vector3i VoxelModel::getVoxelMatrixSize() const
{
    return voxelMatrixSize;
}

double VoxelModel::getVoxelSize() const
{
    return voxelSize;
}

void VoxelModel::setVoxelSize(double value)
{
    voxelSize = value;
}
