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
//    void setMesh(SurfaceMesh::SurfaceMeshModel& porousStructure,
//                 SurfaceMesh::SurfaceMeshModel& boundary);

//    SurfaceMesh::SurfaceMeshModel * m_porousStructure;
//    SurfaceMesh::SurfaceMeshModel * m_boundary;
};

#endif // GENERALPOROSITYCALCULATOR_H
