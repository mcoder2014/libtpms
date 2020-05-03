#ifndef BOXTPMSSINGLESURFACECONFIG_H
#define BOXTPMSSINGLESURFACECONFIG_H

#include "BaseTpmsSingleSurfaceConfig.h"

class BoxTpmsSingleSurfaceConfig
        :public BaseTpmsSingleSurfaceConfig
{
public:
    virtual ~BoxTpmsSingleSurfaceConfig(){};

    Eigen::AlignedBox3d getBoundingBoxPhysial() const;
    void setBoundingBoxPhysial(const Eigen::AlignedBox3d &value);

    Eigen::AlignedBox3d getBoundingBoxLogical() const;
    void setBoundingBoxLogical(const Eigen::AlignedBox3d &value);

    virtual Eigen::Vector3i getMatrixSize() const;

protected:
    // 生成模型的物理尺寸大小 physical size
    Eigen::AlignedBox3d boundingBoxPhysial;
    // 生成模型的逻辑周期大小（会缩放到物理尺寸） logical size
    Eigen::AlignedBox3d boundingBoxLogical;
};

#endif // BOXTPMSSINGLESURFACECONFIG_H
