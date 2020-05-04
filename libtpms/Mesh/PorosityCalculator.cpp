#include "PorosityCalculator.h"

#include <iostream>

double PorosityCalculator::getPorsity(Mesh &porosusStructure, Mesh &boundary)
{
    float boundaryVolume = getVolume(boundary);
    float porosusVolume = getVolume(porosusStructure);

    float porosity = static_cast<float>(porosusVolume / boundaryVolume);

    std::cout<< "Volume of bondary structure: " << boundaryVolume
             << "\nVolume of porosus structure: " << porosusVolume
             << "\nPorosity: " << porosity
             << std::endl;
    return porosity;
}

/**
 * 计算三维模型的体积
 * https://stackoverflow.com/questions/5695322/calculate-volume-of-3d-mesh
 * @brief PorosityCalculator::getVolume
 * @param mesh
 * @return
 */
double PorosityCalculator::getVolume(Mesh &mesh)
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

    return fabs(volume);
}
