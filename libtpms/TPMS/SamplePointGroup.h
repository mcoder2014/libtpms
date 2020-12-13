#ifndef SAMPLEPOINTGROUP_H
#define SAMPLEPOINTGROUP_H

#include <set>

#include "SamplePoint.h"
#include "ImplicitFunction.h"

using std::set;

namespace ImplicitFunciton {

/**
 * @brief The SamplePointGroup class
 * Group 将采样点矩阵划分为多个不同的区域，
 * 不同区域拥有各自的隐函数
 */
class SamplePointGroup
        : public set<SamplePoint*>
{
public:
    // 该集合下使用的隐式函数曲线
    ImplicitFuncitonGroup implicitFunctionGroup;
};

}
#endif // SAMPLEPOINTGROUP_H
