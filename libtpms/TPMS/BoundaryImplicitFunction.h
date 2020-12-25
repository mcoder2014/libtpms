#ifndef BOUNDARYIMPLICITFUNCTION_H
#define BOUNDARYIMPLICITFUNCTION_H

#include <Eigen/Dense>
#include <memory>

#include "ImplicitFunction.h"
#include <Voxel/VoxelModel.h>

using std::shared_ptr;

namespace ImplicitFunciton {

/**
 * @brief The BoundaryImplicitFunction class
 * 用来做边界约束的隐式函数
 */
class BoundaryImplicitFunction
        :public BaseFunction
{
public:
    BoundaryImplicitFunction();

    double operator() (double x, double y, double z) override;

    shared_ptr<VoxelModel> getBoundary() const {
        return boundary;
    }
    void setBoundary(const shared_ptr<VoxelModel> &value) {
        boundary = value;
    }

    double getInValue() const {
        return inValue;
    }
    void setInValue(double value) {
        inValue = value;
    }

    double getOutValue() const {
        return outValue;
    }
    void setOutValue(double value) {
        outValue = value;
    }

private:
    // 体素模型
    shared_ptr<VoxelModel> boundary;

    // 顶点位于体素内返回值
    double inValue;

    // 顶点位于模型外部返回值
    double outValue;
};
}

#endif // BOUNDARYIMPLICITFUNCTION_H
