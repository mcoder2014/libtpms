#ifndef SAMPLEPOINT_H
#define SAMPLEPOINT_H

#include <Eigen/Dense>

using Eigen::Vector3d;

class SamplePoint
{
public:
    SamplePoint():physical(0.0,0.0,0.0), tpms(0.0,0.0,0.0), implicitValue(0.0){}
    Vector3d physical;
    Vector3d tpms;
    double implicitValue;
};

#endif // SAMPLEPOINT_H
