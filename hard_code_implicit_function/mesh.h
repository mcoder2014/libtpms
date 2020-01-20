#ifndef MESH_H
#define MESH_H

#include <vector>

#include <glm/glm.hpp>

#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
//#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>

// OpenMesh Traits
struct Traits: public OpenMesh::DefaultTraits
{
public:
    VertexTraits
    {
        OpenMesh::Vec3f cog;    // center of gravity
    };

    // enable standard properties
    VertexAttributes   (OpenMesh::Attributes::Status|OpenMesh::Attributes::Normal|OpenMesh::Attributes::Color|OpenMesh::Attributes::TexCoord2D);
    HalfedgeAttributes (OpenMesh::Attributes::Status|OpenMesh::Attributes::PrevHalfedge);
    FaceAttributes     (OpenMesh::Attributes::Status|OpenMesh::Attributes::Normal|OpenMesh::Attributes::Color);
    EdgeAttributes     (OpenMesh::Attributes::Status|OpenMesh::Attributes::Color);
};

using Mesh = OpenMesh::TriMesh_ArrayKernelT<Traits>;

extern Mesh* to2Mesh(
        std::vector<glm::vec3> &vertices,
        std::vector<glm::ivec3> &faces);

#endif // MESH_H
