#ifndef MESH_H
#define MESH_H
#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>

// OpenMesh Traits
struct EigenTraits: public OpenMesh::DefaultTraits
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

using Mesh = OpenMesh::PolyMesh_ArrayKernelT<EigenTraits>;
#endif // MESH_H
