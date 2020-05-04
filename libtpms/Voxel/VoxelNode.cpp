#include "VoxelNode.h"

VoxelNode::VoxelNode()
{

}

Vector3d VoxelNode::getCenter() const
{
    return center;
}

void VoxelNode::setCenter(const Vector3d &value)
{
    center = value;
}

double VoxelNode::getEdgeLength() const
{
    return edgeLength;
}

void VoxelNode::setEdgeLength(double value)
{
    edgeLength = value;
}
