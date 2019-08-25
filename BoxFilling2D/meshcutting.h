#ifndef MESHCUTTING_H
#define MESHCUTTING_H

#include <vector>
#include <string>
#include <iostream>

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>

// OpenMesh Traits
struct Traits: public OpenMesh::DefaultTraits
{
public:
    VertexTraits
    {
//        OpenMesh::Vec3f cog;    // center of gravity
    };
    FaceTraits
    {
        bool checked = false;
    };
    EdgeTraits
    {
    };
};

using Mesh = OpenMesh::PolyMesh_ArrayKernelT<Traits>;

class MeshCutting
{
public:
    MeshCutting();

    // Set the plane to cut the mesh
    void setCuttingPlane(OpenMesh::Vec3f v0,
                         OpenMesh::Vec3f v1,
                         OpenMesh::Vec3f v2);
    void setCuttingPlane(float a0, float a1, float a2, float a3);
    std::vector<float> cuttingPlane();

    bool loadMesh(const std::string& filepath);
    Mesh *mesh(){return m_model;}

    void cutting();
    std::vector<std::vector<OpenMesh::Vec3f> > *cutting_result(){
        return &this->m_cuttingPoints;
    }

    std::vector<OpenMesh::Vec3f> face_intersection_cutting_plane(Mesh::FaceIter f_it);

private:
    // the param of y = a0x + a1y + a2z + a3;
    float m_a0, m_a1, m_a2, m_a3;

    float m_xmax, m_ymax, m_zmax, m_xmin, m_ymin, m_zmin;

    Mesh *m_model;              // the loaded model

    // The intersection points of cutting plane and the 3D model
    // Cause there may be more than one circle, so we use 2-d vector
    std::vector<std::vector<OpenMesh::Vec3f>> m_cuttingPoints;

    void printCuttingPlane();   // output the function of cutting plane to the console.

};

#endif // MESHCUTTING_H