#include "qualityinspection.h"

#include <iostream>

QualityInspection::QualityInspection()
{

}

bool QualityInspection::check(Mesh *mesh)
{
    m_mesh = mesh;
    bool result = true;
    bool hasHole = checkHole(mesh);
    result &= hasHole;

    return result;
}

bool QualityInspection::checkHole(Mesh *mesh)
{
    // 查找模型中边界点、边界边、边界面的个数
    int boundaryVertices = 0;
    int boundaryEdges = 0;
    int boundaryFaces = 0;

    // Vertex
    for(Mesh::VertexIter v_it=mesh->vertices_begin(); v_it!=mesh->vertices_end(); v_it++)
    {
        if(mesh->is_boundary(*v_it))
            boundaryVertices++;
    }

    // Edge
    for(Mesh::EdgeIter e_it = mesh->edges_begin(); e_it!=mesh->edges_end();e_it++)
    {
        if(mesh->is_boundary(*e_it))
            boundaryEdges++;
    }

    // Face
    for(Mesh::FaceIter f_it=mesh->faces_begin();f_it!=mesh->faces_end();f_it++)
    {
        if(mesh->is_boundary(*f_it))
            boundaryFaces++;
    }

    std::cout << "\nCount of boundary vertices: " << boundaryVertices
              << "\nCount of boundary Edges: " << boundaryEdges
              << "\nCount of boundary Faces: " << boundaryFaces
              << std::endl;
    if(boundaryVertices+boundaryEdges+boundaryFaces > 0)
        return false;
    else
        return true;

}
