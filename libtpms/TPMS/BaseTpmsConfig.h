#ifndef TPMSCONFIG_H
#define TPMSCONFIG_H

#include <eigen3/Eigen/Dense>

///
/// \brief The TPMSConfig class
/// 主要关于用什么方法生成TPMS的一些配置问题
///
class BaseTpmsConfig
{
public:
    virtual ~BaseTpmsConfig(){};

    Eigen::Vector3i getVoxelDensity() const;
    void setVoxelDensity(const Eigen::Vector3i &value);

    virtual Eigen::Vector3i getMatrixSize() const = 0;
protected:
    // 物理尺度上的采样体素密度 1 mm 对应的采样数量
    // [x][y][z] in one period
    Eigen::Vector3i voxelDensity;

};

#endif // TPMSCONFIG_H
