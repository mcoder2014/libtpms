#ifndef BASETPMSSINGLESURFACECONFIG_H
#define BASETPMSSINGLESURFACECONFIG_H

#include "BaseTpmsConfig.h"
#include "TpmsImplicitFunction.h"

class BaseTpmsSingleSurfaceConfig
        :public BaseTpmsConfig
{
public:
    BaseTpmsSingleSurfaceConfig();
    virtual ~BaseTpmsSingleSurfaceConfig(){};
    virtual Eigen::Vector3i getMatrixSize()  const override = 0;

    TpmsType getTpmsType() const;
    void setTpmsType(const TpmsType &value);

    float getIsoLevel() const;
    void setIsoLevel(float value);

    bool getReverse() const;
    void setReverse(bool value);

protected:
    // TPMS 种类 enum
    TpmsType tpmsType;
    // 阈值面 isoLevel
    float isoLevel;
    // 反向 reverse
    bool reverse;
};

#endif // BASETPMSSINGLESURFACECONFIG_H
