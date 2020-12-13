#ifndef SAMPLEPOINT_H
#define SAMPLEPOINT_H

#include <Eigen/Dense>

using Eigen::Vector3d;
using Eigen::Vector3i;

/**
 * @brief The SamplePoint class
 * 采样点
 */
class SamplePoint
{
public:
    SamplePoint()
        :physical(0.0,0.0,0.0),
         tpms(0.0,0.0,0.0),
         implicitValue(0.0),
         valid(true){}

    // physical coordinate
    Vector3d physical;

    // tpms logical coordinate
    Vector3d tpms;

    // cache implicit function value
    // > 0 表示在曲面外部
    // < 0 表示在曲面内部
    double implicitValue;

    // valid to mark if the point contrbute to March Cube algorithm
    bool valid;
};

#endif // SAMPLEPOINT_H
