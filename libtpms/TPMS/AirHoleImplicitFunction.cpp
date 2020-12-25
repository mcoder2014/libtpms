#include "AirHoleImplicitFunction.h"

ImplicitFunciton::AirHoleImplicitFunction::AirHoleImplicitFunction()
{

}

/**
 * @brief ImplicitFunciton::AirHoleImplicitFunction::operator ()
 *
 * @param x
 * @param y
 * @param
 * @return > 0 在圆柱内部 < 0 在圆柱外部
 */
double ImplicitFunciton::AirHoleImplicitFunction::operator()(double x, double y, double)
{
    // 求顶点到圆柱轴心的距离减去半径
    double deltaX = x - position.x();
    double deltaY = y - position.y();

    return radius * radius - deltaX * deltaX - deltaY * deltaY ;
}
