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
        int id;
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

    std::vector<OpenMesh::Vec3f> face_intersection_cutting_plane(Mesh::FaceHandle faceHandle);
    std::vector<std::vector<OpenMesh::Vec2f> > *getCuttingPoints2d();
    void printCuttingResult();

private:
    // the param of y = a0x + a1y + a2z + a3;
    float m_a0, m_a1, m_a2, m_a3;

    float m_xmax, m_ymax, m_zmax, m_xmin, m_ymin, m_zmin;

    Mesh *m_model;              // the loaded model

    // The intersection points of cutting plane and the 3D model
    // Cause there may be more than one circle, so we use 2-d vector
    std::vector<std::vector<OpenMesh::Vec3f>> m_cuttingPoints;
    // The cutting points project on the cutting plane
    std::vector<std::vector<OpenMesh::Vec2f>> m_cuttingPoints_2d;
    std::vector<float> m_cutting_function_vertex;

    // According to the function of cutting plane
    float functionCuttingPlane(OpenMesh::Vec3f& point);
    void adddIntersectionPoints(std::vector<OpenMesh::Vec3f>& points);
    OpenMesh::Vec3f lineCuttingPlane(OpenMesh::Vec3f& v0, OpenMesh::Vec3f& v1);

    void printCuttingPlane();   // output the function of cutting plane to the console.

   std::vector<unsigned int> debug_case;

};

#endif // MESHCUTTING_H
