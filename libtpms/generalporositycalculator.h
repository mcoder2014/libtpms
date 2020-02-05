#ifndef GENERALPOROSITYCALCULATOR_H
#define GENERALPOROSITYCALCULATOR_H

#include "mesh.h"

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
                      SurfaceMesh::SurfaceMeshModel& boundary);
    float getPorosity(SurfaceMesh::SurfaceMeshModel& porousStructure);

    float getPorosity(Mesh& porosusStructure,
                      Mesh& boundary);
    float getPorosity(Mesh& porosusStructure);

    double calcVolume(Mesh& mesh);  // 计算三维模型体积
    double calcVolume(SurfaceMesh::SurfaceMeshModel& model);
};

#endif // GENERALPOROSITYCALCULATOR_H
