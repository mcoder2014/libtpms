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
 * @brief getImplicitNeededPoint
 * 用来给隐式函数传递合适的坐标
 * @param samplePoint
 * @param function
 * @return
 */
Vector3d& ImplicitFunciton::getImplicitNeededPoint(SamplePoint& samplePoint, ImplicitFunciton::ImplicitFunction &function)
{
    if(function.implicitCoorType == PHYSICAL) {
        return samplePoint.physical;
    } else if (function.implicitCoorType == TPMS) {
        return samplePoint.tpms;
    }

    return samplePoint.physical;
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
    Vector3d& impPos = getImplicitNeededPoint(samplePoint, function);
    Vector3d& weiPos = getWeightNeededPoint(samplePoint, function);

    samplePoint.implicitValue += function.weightFunction(weiPos.x(), weiPos.y(), weiPos.z())
              * function.implicitFunction(impPos.x(), impPos.y(), impPos.z());
}

/**
 * @brief ImplicitFunciton::andImplicitFunctionOperation
 * 求交操作，将 "> 0" 视为 "True < 0" 视为 False
 * samplevalue functionvalue result
 * 0 0 False min
 * 0 1 False min
 * 1 0 False min
 * 1 1 True  ---
 * 求交操作保留两个函数的最小值
 * @param samplePoint
 * @param function
 */
void ImplicitFunciton::andImplicitFunctionOperation(SamplePoint& samplePoint, ImplicitFunciton::ImplicitFunction &function)
{
    Vector3d& impPos = getImplicitNeededPoint(samplePoint, function);
    Vector3d& weiPos = getWeightNeededPoint(samplePoint, function);

    double oldValue = samplePoint.implicitValue;
    double newValue = function.weightFunction(weiPos.x(), weiPos.y(), weiPos.z())
            * function.implicitFunction(impPos.x(), impPos.y(), impPos.z());

    samplePoint.implicitValue = std::min(oldValue, newValue);

//    bool bOld = oldValue > 0;
//    bool bNew = newValue > 0;

//    if(bOld && bNew) {
//        samplePoint.implicitValue = std::numeric_limits<float>::max();
//    } else {
//        samplePoint.implicitValue = std::numeric_limits<float>::lowest();
//    }
}

/**
 * @brief ImplicitFunciton::orImplicitFunctionOperation
 * 求并操作，将 "> 0" 视为 "True < 0" 视为 False
 * samplevalue functionvalue result
 * 0 0 False ---
 * 0 1 True  max
 * 1 0 True  max
 * 1 1 True  max
 * 求并操作保留两个函数的最大值
 * @param samplePoint
 * @param function
 */
void ImplicitFunciton::orImplicitFunctionOperation(SamplePoint& samplePoint, ImplicitFunciton::ImplicitFunction &function)
{
    Vector3d& impPos = getImplicitNeededPoint(samplePoint, function);
    Vector3d& weiPos = getWeightNeededPoint(samplePoint, function);

    double oldValue = samplePoint.implicitValue;
    double newValue = function.weightFunction(weiPos.x(), weiPos.y(), weiPos.z())
            * function.implicitFunction(impPos.x(), impPos.y(), impPos.z());

    samplePoint.implicitValue = std::max(oldValue, newValue);

//    bool bOld = oldValue > 0;
//    bool bNew = newValue > 0;

//    if(bOld || bNew) {
//        samplePoint.implicitValue = std::numeric_limits<float>::max();
//    } else {
//        samplePoint.implicitValue = std::numeric_limits<float>::lowest();
//    }
}

/**
 * @brief ImplicitFunciton::notImplicitFunctionOperation
 * 非操作 认为是加上当前的隐函数的反向值
 * implicit function
 * 0 1
 * 1 0
 * @param samplePoint
 * @param function
 */
void ImplicitFunciton::notImplicitFunctionOperation(SamplePoint& samplePoint, ImplicitFunciton::ImplicitFunction &function)
{
    Vector3d& impPos = getImplicitNeededPoint(samplePoint, function);
    Vector3d& weiPos = getWeightNeededPoint(samplePoint, function);

    samplePoint.implicitValue -= function.weightFunction(weiPos.x(), weiPos.y(), weiPos.z())
              * function.implicitFunction(impPos.x(), impPos.y(), impPos.z());

//    bool bValue = (samplePoint.implicitValue) > 0;
//    if(!bValue) {
//        samplePoint.implicitValue = std::numeric_limits<float>::max();
//    } else {
//        samplePoint.implicitValue = std::numeric_limits<float>::lowest();
//    }
}

/**
 * @brief ImplicitFunciton::xorImplicitFunctionOperation
 * 异或操作操作，将 "> 0" 视为 "True < 0" 视为 False
 * samplevalue functionvalue result
 * 0 0 False 1 将最小值取反
 * 0 1 True  max
 * 1 0 True  max
 * 1 1 False 0 将最大值取反
 * 求并操作保留两个函数的最大值
 * @param samplePoint
 * @param function
 */
void ImplicitFunciton::xorImplicitFunctionOperation(SamplePoint& samplePoint, ImplicitFunciton::ImplicitFunction &function)
{
    Vector3d& impPos = getImplicitNeededPoint(samplePoint, function);
    Vector3d& weiPos = getWeightNeededPoint(samplePoint, function);

    double oldValue = samplePoint.implicitValue;
    double newValue = function.weightFunction(weiPos.x(), weiPos.y(), weiPos.z())
            * function.implicitFunction(impPos.x(), impPos.y(), impPos.z());

    if((oldValue < 0 && newValue > 0)
            || (oldValue > 0 && newValue < 0)) {
        samplePoint.implicitValue = std::max(oldValue, newValue);
    } else if(oldValue < 0 && newValue < 0) {
        samplePoint.implicitValue = - std::min(oldValue, newValue);
    } else {
        samplePoint.implicitValue = - std::max(oldValue, newValue);
    }

//    bool bOld = oldValue > 0;
//    bool bNew = newValue > 0;

//    if(bOld ^ bNew) {
//        samplePoint.implicitValue = std::numeric_limits<float>::max();
//    } else {
//        samplePoint.implicitValue = std::numeric_limits<float>::lowest();
//    }
}

/**
 * @brief ImplicitFunciton::getWeightNeededPoint
 * 用来给权重函数返回合适的输入坐标
 * @param samplePoint
 * @param function
 * @return
 */
Eigen::Vector3d &ImplicitFunciton::getWeightNeededPoint(SamplePoint &samplePoint, ImplicitFunciton::ImplicitFunction &function)
{
    if(function.weightCoorType == PHYSICAL) {
        return samplePoint.physical;
    } else if(function.weightCoorType == TPMS) {
        return samplePoint.tpms;
    }
     return samplePoint.physical;
}
