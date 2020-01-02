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


///
/// \brief GeneralPorosityCalculator::getPorosity
/// 通用方法
/// \param porousStructure
/// \param boundary
/// \param voxelSize
/// \return
///
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

    Eigen::Vector3d  relative_size = bbox.max()-bbox.min();
    Eigen::Vector3i sample_size(
        std::round(relative_size[0]/voxelSize),
        std::round(relative_size[1]/voxelSize),
        std::round(relative_size[2]/voxelSize));
    Eigen::Vector3d range_min = bbox.min();
    Eigen::Vector3d range_max = bbox.max();
    Eigen::Vector3d up_direction(0, 0, 1);

    /// 从一面发出射线和 boundary 模型相交，确定哪些采样点属于模型内部

    /// 再发出射线和 porous 模型相交，确定哪些段是内部和外部

    /// 最后通过计算两个点数，计算孔隙率

    /// 返回孔隙率的结果

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
