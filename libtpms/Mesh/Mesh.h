#ifndef MESH_H
#define MESH_H

#include <vector>

#include <glm/glm.hpp>

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>

/*
 #define VertexTraits \
   template <class Base, class Refs> struct VertexT : public Base

 #define HalfedgeTraits \
   template <class Base, class Refs> struct HalfedgeT : public Base

 #define EdgeTraits \
   template <class Base, class Refs> struct EdgeT : public Base

 #define FaceTraits \
   template <class Base, class Refs> struct FaceT : public Base
*/


// OpenMesh Traits
struct Traits: public OpenMesh::DefaultTraits
{
public:
    VertexTraits
    {
        OpenMesh::Vec3f cog;    // center of gravity
    };
    HalfedgeTraits  {};
    EdgeTraits      {};
    FaceTraits      {};

    // enable standard properties
    // 这里是初始化了四个 enum 变量，表示需要哪些标准属性
    VertexAttributes   (OpenMesh::Attributes::Status
                        | OpenMesh::Attributes::Normal);

    HalfedgeAttributes (OpenMesh::Attributes::Status
                        |OpenMesh::Attributes::PrevHalfedge);

    FaceAttributes     (OpenMesh::Attributes::Status
                        |OpenMesh::Attributes::Normal);

    EdgeAttributes     (OpenMesh::Attributes::Status);
};

using Mesh = OpenMesh::TriMesh_ArrayKernelT<Traits>;

#endif // MESH_H
