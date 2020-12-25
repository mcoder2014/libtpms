#include "BoundaryImplicitFunction.h"

#include <numeric>

ImplicitFunciton::BoundaryImplicitFunction::BoundaryImplicitFunction()
    :inValue(std::numeric_limits<float>::lowest()),
      outValue(std::numeric_limits<float>::max())
{
}

/**
 * @brief ImplicitFunciton::BoundaryImplicitFunction::operator ()
 * @param x
 * @param y
 * @param z
 * @return
 */
double ImplicitFunciton::BoundaryImplicitFunction::operator()(double x, double y, double z)
{
    if(boundary->contains(x, y, z)) {
        return inValue;
    }

    return outValue;
}
