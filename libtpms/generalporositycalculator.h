#ifndef GENERALPOROSITYCALCULATOR_H
#define GENERALPOROSITYCALCULATOR_H

#ifdef USING_SURFACEMESH
#include <SurfaceMeshModel.h>
#include <Octree.h>
#endif

class GeneralPorosityCalculator
{
public:
    GeneralPorosityCalculator();
    ~GeneralPorosityCalculator();

    float getPorosity(SurfaceMesh::SurfaceMeshModel& porousStructure,
                      SurfaceMesh::SurfaceMeshModel& boundary,
                      float voxelSize);
    float getPorosity(SurfaceMesh::SurfaceMeshModel& porousStructure,
                      float voxelSize);

private:
    uint64_t calcInner(std::vector<std::vector<Eigen::Vector3d>>& points_mat,
                  std::vector<float>& helper_z,
                  float voxelSize,
                  Octree& oct_model);
    uint64_t calcInner(std::vector<std::vector<Eigen::Vector3d>>& points_mat,
                  std::vector<float>& helper_z,
                  Eigen::AlignedBox3d boundary);
};

#endif // GENERALPOROSITYCALCULATOR_H
