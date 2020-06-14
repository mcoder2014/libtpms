#ifndef HEXSAMPLEPOINT_H
#define HEXSAMPLEPOINT_H

#include <Eigen/Dense>

using Eigen::Vector3d;
using Eigen::Vector3i;

/**
 * @brief The HexSamplePoint class
 * 六面体的顶点
 */
class HexSamplePoint
{
public:

    HexSamplePoint()
        :physical(.0,.0,.0), inner(true){}

    HexSamplePoint(Vector3d physical, bool inner)
        :physical(physical), inner(inner){}

    HexSamplePoint(Vector3d physical, Vector3i index)
        :physical(physical), inner(false), index(index){}

    // 采样点的物理坐标
    Vector3d physical;

    // 内部点、外部点标记
    bool inner;

    // 标识顶点的 id、坐标
    Vector3i index;
};

#endif // HEXSAMPLEPOINT_H
