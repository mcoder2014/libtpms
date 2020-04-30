#include "BoxTpmsSingleSurfaceConfig.h"

TpmsType BoxTpmsSingleSurfaceConfig::getTpmsType() const
{
    return tpmsType;
}

void BoxTpmsSingleSurfaceConfig::setTpmsType(const TpmsType &value)
{
    tpmsType = value;
}

float BoxTpmsSingleSurfaceConfig::getIsoLevel() const
{
    return isoLevel;
}

void BoxTpmsSingleSurfaceConfig::setIsoLevel(float value)
{
    isoLevel = value;
}

bool BoxTpmsSingleSurfaceConfig::getReverse() const
{
    return reverse;
}

void BoxTpmsSingleSurfaceConfig::setReverse(bool value)
{
    reverse = value;
}
