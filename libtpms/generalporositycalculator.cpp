#include "generalporositycalculator.h"

#include <eigen3/Eigen/Dense>

GeneralPorosityCalculator::GeneralPorosityCalculator()
{

}

GeneralPorosityCalculator::~GeneralPorosityCalculator()
{

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
        SurfaceMesh::SurfaceMeshModel &boundary)
{

    float boundaryVolume = calcVolume(boundary);
    float porosusVolume = calcVolume(porousStructure);

    float porosity = static_cast<float>(porosusVolume / boundaryVolume);

    std::cout<< "Volume of bondary structure: " << boundaryVolume
             << "\nVolume of porosus structure: " << porosusVolume
             << "\nPorosity: " << porosity
             << std::endl;
    return porosity;

    /// 返回孔隙率的结果
    return porosity;
}

float GeneralPorosityCalculator::getPorosity(SurfaceMesh::SurfaceMeshModel &porousStructure)
{
    // 计算包围盒的体积
    porousStructure.updateBoundingBox();
    Eigen::AlignedBox3d bbox = porousStructure.bbox();
    double boundingVolume = bbox.volume();

    // 计算体积
    double porosusVolume = calcVolume(porousStructure);
    float porosity = static_cast<float>(porosusVolume / boundingVolume);

    std::cout<< "Volume of bounding box: " << boundingVolume
             << "\nVolume of porosus structure: " << porosusVolume
             << "\nPorosity: " << porosity
             << std::endl;

    return porosity;
}

float GeneralPorosityCalculator::getPorosity(Mesh &porosusStructure, Mesh &boundary)
{
    float boundaryVolume = calcVolume(boundary);
    float porosusVolume = calcVolume(porosusStructure);

    float porosity = static_cast<float>(porosusVolume / boundaryVolume);

    std::cout<< "Volume of bondary structure: " << boundaryVolume
             << "\nVolume of porosus structure: " << porosusVolume
             << "\nPorosity: " << porosity
             << std::endl;
    return porosity;
}

float GeneralPorosityCalculator::getPorosity(Mesh &porosusStructure)
{
    // 计算包围盒的体积
    Eigen::AlignedBox3d bbox;
    Mesh::VertexIter vit, vend(porosusStructure.vertices_end());

    for(vit = porosusStructure.vertices_begin(); vit != vend; ++vit)
    {
        OpenMesh::Vec3f& point = porosusStructure.point(*vit);
        bbox.extend(Eigen::Vector3d(point[0], point[1], point[2]));
    }

    double boundingVolume = bbox.volume();

    // 计算体积
    double porosusVolume = calcVolume(porosusStructure);
    float porosity = static_cast<float>(porosusVolume / boundingVolume);

    std::cout<< "Volume of bounding box: " << boundingVolume
             << "\nVolume of porosus structure: " << porosusVolume
             << "\nPorosity: " << porosity
             << std::endl;

    return porosity;
}

///
/// \brief GeneralPorosityCalculator::calcVolume
/// 测试使用计算三维模型体积的方法计算孔隙率
/// \param mesh
/// \return
/// https://stackoverflow.com/questions/5695322/calculate-volume-of-3d-mesh
///
double GeneralPorosityCalculator::calcVolume(Mesh &mesh)
{
    // 思路是用每个面片和原点形成的有符号的 四面体，
    // 体积之和的绝对值是总体的体积

    auto signedVolumeOfTriangle = [](
            OpenMesh::Vec3f& p1,
            OpenMesh::Vec3f& p2,
            OpenMesh::Vec3f& p3) -> float
    {
        float v321 = p3[0] * p2[1] * p1[2];
        float v231 = p2[0] * p3[1] * p1[2];
        float v312 = p3[0] * p1[1] * p2[2];
        float v132 = p1[0] * p3[1] * p2[2];
        float v213 = p2[0] * p1[1] * p3[2];
        float v123 = p1[0] * p2[1] * p3[2];
        return (1.0/6) * (
                    -v321 + v231 + v312 -v132 -v213 + v123);
    };

    double volume = 0.0;    // 最终的体积

    Mesh::FaceIter fit, fend = mesh.faces_end();
    std::vector<OpenMesh::Vec3f> vface;
    for(fit = mesh.faces_begin(); fit != fend; fit++)
    {
        // 找到三个顶点
        vface.clear();
        Mesh::FaceVertexIter fvit;
        for(fvit = mesh.fv_begin(*fit); fvit.is_valid(); fvit++)
        {
            vface.push_back(mesh.point(*fvit));
        }
        if(vface.size() >= 3)
            volume += signedVolumeOfTriangle(vface[0], vface[1], vface[2]);
    }

    return abs(volume);
}

double GeneralPorosityCalculator::calcVolume(SurfaceMesh::SurfaceMeshModel &model)
{
    auto signedVolumeOfTriangle = [](
            Eigen::Vector3d& p1,
            Eigen::Vector3d& p2,
            Eigen::Vector3d& p3) -> float
    {
        float v321 = p3[0] * p2[1] * p1[2];
        float v231 = p2[0] * p3[1] * p1[2];
        float v312 = p3[0] * p1[1] * p2[2];
        float v132 = p1[0] * p3[1] * p2[2];
        float v213 = p2[0] * p1[1] * p3[2];
        float v123 = p1[0] * p2[1] * p3[2];
        return (1.0/6) * (
                    -v321 + v231 + v312 -v132 -v213 + v123);
    };

    double volume = 0.0;
    // Vertex
    SurfaceMesh::Vector3VertexProperty vecs = model.vertex_coordinates();

    //Faces
    std::vector<Eigen::Vector3d> vface;
    for(SurfaceMesh::Face f:model.faces())
    {
        vface.clear();

        SurfaceMesh::SurfaceMeshForEachVertexOnFaceHelper vit = model.vertices(f);
        vit = vit.begin();
        SurfaceMesh::SurfaceMeshForEachVertexOnFaceHelper vend = vit;
        for(;vit!=vend;++vit)
        {
            vface.push_back(vecs[vit]);
        }

        if(vface.size() >= 3)
            volume += signedVolumeOfTriangle(vface[0], vface[1], vface[2]);
    }

    return volume;
}
