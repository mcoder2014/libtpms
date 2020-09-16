#ifndef MESH_H
#define MESH_H

#include <vector>

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Geometry/EigenVectorT.hh>

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
struct EigenTraits: public OpenMesh::DefaultTraits
{
    using Point = Eigen::Vector3d;
    using Normal = Eigen::Vector3d;
    using TexCoord2D = Eigen::Vector2d;

public:
    VertexTraits
    {
        Point cog;    // center of gravity
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

using Mesh = OpenMesh::TriMesh_ArrayKernelT<EigenTraits>;

#endif // MESH_H
