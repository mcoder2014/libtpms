#ifndef VOXELNODE_H
#define VOXELNODE_H

#include <Eigen/Dense>

using Eigen::Vector3d;

class VoxelNode
{
public:
    VoxelNode(){}
    VoxelNode(const Vector3d& center, const double& edgeLength);

    Vector3d getCenter() const;
    void setCenter(const Vector3d &value);

    double getEdgeLength() const;
    void setEdgeLength(double value);

    double operator[] (const int value);
    bool contains(const Vector3d& point);

private:
    // 体素节点中心
    Vector3d center;

    // 体素节点边长
    double edgeLength;

    void updateBoundary();
    // 存下两个顶点帮助判断一个点是否在体素内
    Vector3d min;
    Vector3d max;

};

#endif // VOXELNODE_H
