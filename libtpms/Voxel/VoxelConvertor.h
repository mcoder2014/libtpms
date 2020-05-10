#ifndef VOXELCONVERTOR_H
#define VOXELCONVERTOR_H

#include <vector>
#include <memory>

#include <Eigen/Dense>
#include <SurfaceMeshModel.h>
#include <Octree.h>

#include "VoxelNode.h"

using Eigen::Vector3d;
using Eigen::Vector3i;

/**
 * @brief The VoxelConvertor class
 * 将普通模型转换为一堆体素
 */
class VoxelConvertor
{
public:
    std::vector<std::shared_ptr<VoxelNode>> toVoxels(
            std::shared_ptr<SurfaceMesh::SurfaceMeshModel> surfaceMesh,
            double voxelSize);

private:
    std::vector<Vector3d> intersectPoints(
            Octree& octree,
            Eigen::Vector3d point);
};

#endif // VOXELCONVERTOR_H
