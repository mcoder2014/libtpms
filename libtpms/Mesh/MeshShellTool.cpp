#include "MeshShellTool.h"

#include <iostream>

void MeshShellTool::shell(Mesh &mesh, double depth)
{

    /// 将曲面复制一份，追加到 mesh 中，面法相相反
    duplicate(mesh);

    /// 封闭曲面的缺口
    fixHole(mesh);

    /// 曲面中各定点向法方向移动 depth/2 位置
    verticeNormarlDirectionMove(mesh, depth/2);

}

void MeshShellTool::duplicate(Mesh &mesh)
{
    /// 将曲面复制一份，追加到 mesh 中，面法相相反
    int verticesSize = mesh.n_vertices();

    // 复制网格顶点
    Mesh::VertexIter vit, vend = mesh.vertices_end();
    for(vit = mesh.vertices_begin(); vit != vend; vit++)
    {
        OpenMesh::Vec3f point = mesh.point(*vit);
        mesh.add_vertex(point);
    }

    // 复制网格面片
    Mesh::FaceIter fit, fend = mesh.faces_end();
    std::vector<Mesh::VertexHandle> vface;
    for(fit = mesh.faces_begin(); fit != fend; fit++)
    {
        vface.clear();
        Mesh::FaceVertexIter fvit = mesh.fv_iter(*fit);
        for(;fvit.is_valid(); fvit++)
        {
            Mesh::VertexHandle vh = *fvit;
            vface.push_back(mesh.vertex_handle(vh.idx() + verticesSize));
        }

        // 颠倒顶点顺序，使面方向相反
        std::reverse(vface.begin(), vface.end());
        mesh.add_face(vface);
    }
}

void MeshShellTool::fixHole(Mesh &mesh)
{
    // 记录当前顶点数目
    int verticeSize = mesh.n_vertices() / 2;

    Mesh::HalfedgeIter halfEdgeIter, halfEdgeIterEnd(mesh.halfedges_end());
    for(halfEdgeIter = mesh.halfedges_begin(); halfEdgeIter != halfEdgeIterEnd; halfEdgeIter++)
    {
        // if boundary, add two face
        if(mesh.is_boundary(*halfEdgeIter))
        {
            Mesh::HalfedgeHandle heh = *halfEdgeIter;

            Mesh::VertexHandle vHandleFrom = mesh.from_vertex_handle(heh);
            Mesh::VertexHandle vHandleTo = mesh.to_vertex_handle(heh);

            int offset = verticeSize;
            if(vHandleFrom.idx() >= verticeSize)
                offset = -verticeSize;

            Mesh::VertexHandle cvHandleFrom = mesh.vertex_handle(vHandleFrom.idx() + offset);
            Mesh::VertexHandle cvHandleTo = mesh.vertex_handle(vHandleTo.idx() + offset);

            // add faces
            std::vector<Mesh::VertexHandle> vertexHandles;
            vertexHandles.push_back(vHandleFrom);
            vertexHandles.push_back(vHandleTo);
            vertexHandles.push_back(cvHandleFrom);
            mesh.add_face(vertexHandles);

            vertexHandles.clear();
            vertexHandles.push_back(cvHandleTo);
            vertexHandles.push_back(cvHandleFrom);
            vertexHandles.push_back(vHandleTo);
            mesh.add_face(vertexHandles);
        }
    }

}

void MeshShellTool::verticeNormarlDirectionMove(Mesh &mesh, double distance)
{
    // 构建法向量
    mesh.request_vertex_normals();
    mesh.update_normals();

    Mesh::VertexIter vit, vend;
    // init iterator
    vit = mesh.vertices_begin();
    vend = mesh.vertices_end();

    for( ; vit!=vend ; ++vit)
    {
        OpenMesh::Vec3f vpos = mesh.point(*vit);
        if(!mesh.is_boundary(*vit))
        {
            // 内部点向顶点法向量移动 distance 距离
            OpenMesh::Vec3f vnormal = mesh.normal(*vit);
            vpos += vnormal.normalized() * distance;
            mesh.set_point(*vit, vpos);
        }
        else
        {
            // 边界点向临界面法相平均值移动 distance 距离
            OpenMesh::Vec3f fnoraml(0.0, 0.0, 0.0);
            Mesh::VertexFaceIter vfit = mesh.vf_begin(*vit);
            for(; vfit.is_valid(); vfit++)
            {
                fnoraml += mesh.normal(*vfit);
            }
            vpos += fnoraml.normalized() * distance;
            mesh.set_point(*vit, vpos);
        }
    }
}
