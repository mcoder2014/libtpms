#ifndef VOXELNODE_H
#define VOXELNODE_H

#include <Eigen/Dense>

using Eigen::Vector3d;

class VoxelNode
{
public:
    VoxelNode();

    Vector3d getCenter() const;
    void setCenter(const Vector3d &value);

    double getEdgeLength() const;
    void setEdgeLength(double value);

private:
    // 体素节点中心
    Vector3d center;
    // 体素节点边长
    double edgeLength;

};

#endif // VOXELNODE_H
