#include "VoxelNode.h"

VoxelNode::VoxelNode(const Eigen::Vector3d &center, const double &edgeLength)
{
    this->center = center;
    this->edgeLength = edgeLength;
    updateBoundary();
}

Vector3d VoxelNode::getCenter() const
{
    return center;
}

void VoxelNode::setCenter(const Vector3d &value)
{
    center = value;
    updateBoundary();
}

double VoxelNode::getEdgeLength() const
{
    return edgeLength;
}

void VoxelNode::setEdgeLength(double value)
{
    edgeLength = value;
    updateBoundary();
}

double VoxelNode::operator [](const int value)
{
    return center.operator[](value);
}

bool VoxelNode::contains(const Eigen::Vector3d &point)
{
    return point.x() > min.x()
            && point.x() < max.x()
            && point.y() > min.y()
            && point.y() < max.y()
            && point.z() > min.z()
            && point.z() < max.z();
}

void VoxelNode::updateBoundary()
{
    double halfEdgeLength = edgeLength / 2.0;
    min.x() = center.x() - halfEdgeLength;
    min.y() = center.y() - halfEdgeLength;
    min.z() = center.z() - halfEdgeLength;

    max.x() = center.x() + halfEdgeLength;
    max.y() = center.y() + halfEdgeLength;
    max.z() = center.z() + halfEdgeLength;
}
