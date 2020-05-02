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

protected:
    // TPMS 种类 enum
    TpmsType tpmsType;
    // 阈值面 isoLevel
    float isoLevel;
    // 反向 reverse
    bool reverse;
};

#endif // BOXTPMSSINGLESURFACECONFIG_H
