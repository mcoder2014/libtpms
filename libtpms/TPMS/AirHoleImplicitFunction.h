#ifndef AIRHOLEIMPLICITFUNCTION_H
#define AIRHOLEIMPLICITFUNCTION_H

#include <Eigen/Dense>

#include "ImplicitFunction.h"

using Eigen::Vector2d;

namespace ImplicitFunciton {

/**
 * @brief The AirHoleImplicitFunction class
 * 单个透气孔的隐式函数
 */
class AirHoleImplicitFunction
        :public BaseFunction
{
public:
    AirHoleImplicitFunction();
    double operator() (double x, double y, double) override;

    // 半径
    double radius;

    // 位置
    Vector2d position;
};
}

#endif // AIRHOLEIMPLICITFUNCTION_H
