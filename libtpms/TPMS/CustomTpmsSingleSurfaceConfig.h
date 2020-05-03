#ifndef CUSTOMTPMSSINGLESURFACECONFIG_H
#define CUSTOMTPMSSINGLESURFACECONFIG_H

#include "BaseTpmsConfig.h"
#include "TpmsImplicitFunction.h"
#include "Mesh/Mesh.h"

class CustomTpmsSingleSurfaceConfig
        :public BaseTpmsConfig
{
public:
    virtual ~CustomTpmsSingleSurfaceConfig(){}

protected:
    // TPMS 种类 enum
    TpmsType tpmsType;
    // Boundary

    // 阈值面 isoLevel
    float isoLevel;
    // 反向 reverse
    bool reverse;
};

#endif // CUSTOMTPMSSINGLESURFACECONFIG_H
