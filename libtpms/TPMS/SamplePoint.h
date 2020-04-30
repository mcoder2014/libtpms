#ifndef SAMPLEPOINT_H
#define SAMPLEPOINT_H

#include <Eigen/Dense>

using Eigen::Vector3d;

class SamplePoint
{
public:
    Vector3d physical;
    Vector3d tmps;
    double implicitValue;
};

#endif // SAMPLEPOINT_H
