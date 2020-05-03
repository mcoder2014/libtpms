#include "BaseTpmsSingleSurfaceConfig.h"

BaseTpmsSingleSurfaceConfig::BaseTpmsSingleSurfaceConfig()
{

}

TpmsType BaseTpmsSingleSurfaceConfig::getTpmsType() const
{
    return tpmsType;
}

void BaseTpmsSingleSurfaceConfig::setTpmsType(const TpmsType &value)
{
    tpmsType = value;
}

float BaseTpmsSingleSurfaceConfig::getIsoLevel() const
{
    return isoLevel;
}

void BaseTpmsSingleSurfaceConfig::setIsoLevel(float value)
{
    isoLevel = value;
}

bool BaseTpmsSingleSurfaceConfig::getReverse() const
{
    return reverse;
}

void BaseTpmsSingleSurfaceConfig::setReverse(bool value)
{
    reverse = value;
}
