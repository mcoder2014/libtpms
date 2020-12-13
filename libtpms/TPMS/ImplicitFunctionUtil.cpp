#include "ImplicitFunctionUtil.h"

#include <limits>
#include <functional>

using std::function;
using std::bind;
using namespace std::placeholders;

/**
 * @brief calculateSamplePointGroup
 * @param group
 */
void ImplicitFunciton::calculateSamplePointGroup(ImplicitFunciton::SamplePointGroup &group)
{
    /// Lambda 表达式来简化后续操作
    /// 固定隐函数，循环计算 group 内部所有采样点的数据，以此来减少对与 if else 的调用
    auto exec = [&group](
            std::function<void(SamplePoint&, ImplicitFunction &)> operation,
            ImplicitFunction& function) {
        for(SamplePoint* samplePoint : group) {
            operation(*samplePoint, function);
        }
    };

    /// 依次计算每个隐函数
    for(ImplicitFunction implicitFunction : group.implicitFunctionGroup) {
        if(implicitFunction.operation == DEFAULT){
            exec(bind(defaultImplicitFunctionOperation, _1, _2), implicitFunction);
        } else if(implicitFunction.operation == ADD) {
            exec(bind(addImplicitFunctionOperation, _1, _2), implicitFunction);
        } else if(implicitFunction.operation == AND) {
            exec(bind(andImplicitFunctionOperation, _1, _2), implicitFunction);
        } else if(implicitFunction.operation == OR) {
            exec(bind(orImplicitFunctionOperation, _1, _2), implicitFunction);
        } else if(implicitFunction.operation == NOT) {
            exec(bind(notImplicitFunctionOperation, _1, _2), implicitFunction);
        } else if(implicitFunction.operation == XOR) {
            exec(bind(xorImplicitFunctionOperation, _1, _2), implicitFunction);
        }
    }
}

/**
 * @brief ImplicitFunciton::defaultImplicitFunctionOperation
 * 默认隐函数操作
 * @param samplePoint
 * @param function
 */
void ImplicitFunciton::defaultImplicitFunctionOperation(SamplePoint& samplePoint, ImplicitFunciton::ImplicitFunction &function)
{
    addImplicitFunctionOperation(samplePoint, function);
}

/**
 * @brief ImplicitFunciton::addImplicitFunctionOperation
 * 权重相加操作
 * @param samplePoint
 * @param function
 */
void ImplicitFunciton::addImplicitFunctionOperation(SamplePoint& samplePoint, ImplicitFunciton::ImplicitFunction &function)
{
    Vector3d& pos = samplePoint.tpms;
    samplePoint.implicitValue = samplePoint.implicitValue
            + function.weightFunction(pos.x(), pos.y(), pos.z()) * function.implicitFunction(pos.x(), pos.z(), pos.z());
}

/**
 * @brief ImplicitFunciton::andImplicitFunctionOperation
 * 并操作
 * @param samplePoint
 * @param function
 */
void ImplicitFunciton::andImplicitFunctionOperation(SamplePoint& samplePoint, ImplicitFunciton::ImplicitFunction &function)
{
    Vector3d& pos = samplePoint.tpms;
    double oldValue = samplePoint.implicitValue;
    double newValue = function.weightFunction(pos.x(), pos.y(), pos.z()) * function.implicitFunction(pos.x(), pos.y(), pos.z());

    bool bOld = oldValue > 0;
    bool bNew = newValue > 0;

    if(bOld && bNew) {
        samplePoint.implicitValue = std::numeric_limits<float>::max();
    } else {
        samplePoint.implicitValue = std::numeric_limits<float>::lowest();
    }

}

/**
 * @brief ImplicitFunciton::orImplicitFunctionOperation
 * 或操作
 * @param samplePoint
 * @param function
 */
void ImplicitFunciton::orImplicitFunctionOperation(SamplePoint& samplePoint, ImplicitFunciton::ImplicitFunction &function)
{
    Vector3d& pos = samplePoint.tpms;
    double oldValue = samplePoint.implicitValue;
    double newValue = function.weightFunction(pos.x(), pos.y(), pos.z()) * function.implicitFunction(pos.x(), pos.y(), pos.z());

    bool bOld = oldValue > 0;
    bool bNew = newValue > 0;

    if(bOld || bNew) {
        samplePoint.implicitValue = std::numeric_limits<float>::max();
    } else {
        samplePoint.implicitValue = std::numeric_limits<float>::lowest();
    }
}

/**
 * @brief ImplicitFunciton::notImplicitFunctionOperation
 * 非操作 认为 是对采样点的数据取非
 * @param samplePoint
 * @param function
 */
void ImplicitFunciton::notImplicitFunctionOperation(SamplePoint& samplePoint, ImplicitFunciton::ImplicitFunction &function)
{
    bool bValue = (samplePoint.implicitValue) > 0;
    if(!bValue) {
        samplePoint.implicitValue = std::numeric_limits<float>::max();
    } else {
        samplePoint.implicitValue = std::numeric_limits<float>::lowest();
    }
}

/**
 * @brief ImplicitFunciton::xorImplicitFunctionOperation
 * @param samplePoint
 * @param function
 */
void ImplicitFunciton::xorImplicitFunctionOperation(SamplePoint& samplePoint, ImplicitFunciton::ImplicitFunction &function)
{
    Vector3d& pos = samplePoint.tpms;
    double oldValue = samplePoint.implicitValue;
    double newValue = function.weightFunction(pos.x(), pos.y(), pos.z()) * function.implicitFunction(pos.x(), pos.y(), pos.z());

    bool bOld = oldValue > 0;
    bool bNew = newValue > 0;

    if(bOld ^ bNew) {
        samplePoint.implicitValue = std::numeric_limits<float>::max();
    } else {
        samplePoint.implicitValue = std::numeric_limits<float>::lowest();
    }
}
