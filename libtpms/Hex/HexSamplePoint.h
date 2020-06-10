#ifndef HEXSAMPLEPOINT_H
#define HEXSAMPLEPOINT_H

#include <Eigen/Dense>

using Eigen::Vector3d;

class HexSamplePoint
{
public:

    HexSamplePoint()
        :physical(.0,.0,.0), inner(true){}

    HexSamplePoint(Vector3d physical, bool inner)
        :physical(physical), inner(inner){}

    // 采样点的物理坐标
    Vector3d physical;

    // 内部点、外部点标记
    bool inner;
};

#endif // HEXSAMPLEPOINT_H
