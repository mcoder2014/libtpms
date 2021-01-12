#ifndef IMPLICITFUNCTIONUTIL_H
#define IMPLICITFUNCTIONUTIL_H

#include "SamplePointGroup.h"

namespace ImplicitFunciton {

// 计算 samplePointGroup 的隐函数值
void calculateSamplePointGroup(SamplePointGroup &group);

// 用来给隐式函数传递合适的坐标
Vector3d& getImplicitNeededPoint(SamplePoint& samplePoint, ImplicitFunciton::ImplicitFunction &function);

// 用来给权重函数传递合适的坐标
Vector3d &getWeightNeededPoint(SamplePoint& samplePoint, ImplicitFunciton::ImplicitFunction &function);

/// Operation
// DEFAULT
void defaultImplicitFunctionOperation(SamplePoint& samplePoint, ImplicitFunction &function);

// ADD
void addImplicitFunctionOperation(SamplePoint& samplePoint, ImplicitFunction &function);

// AND
void andImplicitFunctionOperation(SamplePoint& samplePoint, ImplicitFunction &function);

// OR
void orImplicitFunctionOperation(SamplePoint& samplePoint, ImplicitFunction &function);

// NOT
void notImplicitFunctionOperation(SamplePoint& samplePoint, ImplicitFunction &function);

// XOR
void xorImplicitFunctionOperation(SamplePoint& samplePoint, ImplicitFunction &function);

}


#endif // IMPLICITFUNCTIONUTIL_H
