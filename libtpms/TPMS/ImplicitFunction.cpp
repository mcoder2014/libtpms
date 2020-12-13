#include "ImplicitFunction.h"

#include "CommonImplicitFunction.h"

using namespace std::placeholders;

/**
 * @brief ImplicitFunciton::ImplicitFunction::ImplicitFunction
 * 默认构造函数
 */
ImplicitFunciton::ImplicitFunction::ImplicitFunction()
    :implicitCoorType(PHYSICAL),weightCoorType(PHYSICAL),operation(DEFAULT)
{
    this->weightFunction = bind(ONE, _1, _2, _3);
    this->implicitFunction = bind(ZERO, _1, _2, _3);
}
