#ifndef SAMPLEPOINT_H
#define SAMPLEPOINT_H

#include <Eigen/Dense>

using Eigen::Vector3d;
using Eigen::Vector3i;

class SamplePoint
{
public:
    SamplePoint():physical(0.0,0.0,0.0), tpms(0.0,0.0,0.0), implicitValue(0.0), valid(true){}
    Vector3d physical;
    Vector3d tpms;
    double implicitValue;
    bool valid;
};

#endif // SAMPLEPOINT_H
