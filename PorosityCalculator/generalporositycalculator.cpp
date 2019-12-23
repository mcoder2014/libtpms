#include "generalporositycalculator.h"

#include <eigen3/Eigen/Dense>

GeneralPorosityCalculator::GeneralPorosityCalculator()
//    :m_porousStructure(nullptr),m_boundary(nullptr)
{

}

GeneralPorosityCalculator::~GeneralPorosityCalculator()
{
//    if(m_porousStructure != nullptr)
//        delete m_porousStructure;
//    if(m_boundary != nullptr)
//        delete m_boundary;
}

float GeneralPorosityCalculator::getPorosity(
        SurfaceMesh::SurfaceMeshModel &porousStructure,
        SurfaceMesh::SurfaceMeshModel &boundary,
        float voxelSize)
{
//    setMesh(porousStructure, boundary);

    Octree oct_boundary(&boundary);
    Octree oct_porous(&porousStructure);

    uint count_one = 0;
    uint count_zero = 0;

    Eigen::AlignedBox3d bbox = boundary.bbox();
    // 根据包围盒去设计

}

//void GeneralPorosityCalculator::setMesh(
//        SurfaceMesh::SurfaceMeshModel &porousStructure,
//        SurfaceMesh::SurfaceMeshModel &boundary)
//{
//    if(m_porousStructure != nullptr)
//        delete m_porousStructure;
//    m_porousStructure = porousStructure.clone();

//    if(m_boundary != nullptr)
//        delete m_boundary;
//    m_boundary = boundary.clone();
//}
