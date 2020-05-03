#ifndef BOXTPMSSINGLESURFACECONFIG_H
#define BOXTPMSSINGLESURFACECONFIG_H

#include "BaseTpmsConfig.h"
#include "TpmsImplicitFunction.h"

class BoxTpmsSingleSurfaceConfig
        :public BaseTpmsConfig
{
public:
    virtual ~BoxTpmsSingleSurfaceConfig(){};

    TpmsType getTpmsType() const;
    void setTpmsType(const TpmsType &value);

    float getIsoLevel() const;
    void setIsoLevel(float value);

    bool getReverse() const;
    void setReverse(bool value);

    Eigen::AlignedBox3d getBoundingBoxPhysial() const;
    void setBoundingBoxPhysial(const Eigen::AlignedBox3d &value);

    Eigen::AlignedBox3d getBoundingBoxLogical() const;
    void setBoundingBoxLogical(const Eigen::AlignedBox3d &value);

    virtual Eigen::Vector3i getMatrixSize() const;

protected:
    // TPMS 种类 enum
    TpmsType tpmsType;
    // 生成模型的物理尺寸大小 physical size
    Eigen::AlignedBox3d boundingBoxPhysial;
    // 生成模型的逻辑周期大小（会缩放到物理尺寸） logical size
    Eigen::AlignedBox3d boundingBoxLogical;
    // 阈值面 isoLevel
    float isoLevel;
    // 反向 reverse
    bool reverse;
};

#endif // BOXTPMSSINGLESURFACECONFIG_H
