#ifndef TPMSCONFIG_H
#define TPMSCONFIG_H

#include <eigen3/Eigen/Dense>

#include "TpmsImplicitFunction.h"

///
/// \brief The TPMSConfig class
/// 主要关于用什么方法生成TPMS的一些配置问题
///
class BaseTpmsConfig
{
public:
    virtual ~BaseTpmsConfig()=0;

    Eigen::AlignedBox3d getBoundingBoxPhysial() const;
    void setBoundingBoxPhysial(const Eigen::AlignedBox3d &value);

    Eigen::AlignedBox3d getBoundingBoxLogical() const;
    void setBoundingBoxLogical(const Eigen::AlignedBox3d &value);

    Eigen::Vector3i getVoxelDensity() const;
    void setVoxelDensity(const Eigen::Vector3i &value);

    virtual Eigen::Vector3i getMatrixSize() const;

protected:
    // 生成模型的物理尺寸大小 physical size
    Eigen::AlignedBox3d boundingBoxPhysial;

    // 生成模型的逻辑周期大小（会缩放到物理尺寸） logical size
    Eigen::AlignedBox3d boundingBoxLogical;

    // 物理尺度上的采样体素密度 voxel density
    // [x][y][z] in one period
    Eigen::Vector3i voxelDensity;

};

#endif // TPMSCONFIG_H
