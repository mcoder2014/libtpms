#include "MeshShellTool.h"

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

        std::reverse(vface.begin(), vface.end());
        mesh.add_face(vface);
    }
}

void MeshShellTool::fixHole(Mesh &mesh)
{
    // 记录当前顶点数目
    int verticeSize = mesh.n_vertices() / 2;
    Mesh::HalfedgeIter heit, heend(mesh.halfedges_end());
    for(heit = mesh.halfedges_begin(); heit != heend; heit++)
    {
        // if boundary, add two face
        if(mesh.is_boundary(*heit))
        {
            Mesh::HalfedgeHandle heh = *heit;

            Mesh::VertexHandle vfrom = mesh.from_vertex_handle(heh);
            Mesh::VertexHandle vto = mesh.to_vertex_handle(heh);

            int offset = verticeSize;
            if(vfrom.idx() >= verticeSize)
                offset = -verticeSize;

            Mesh::VertexHandle c_vfrom = mesh.vertex_handle(vfrom.idx() + offset);
            Mesh::VertexHandle c_vto = mesh.vertex_handle(vto.idx() + offset);

            // add face
            std::vector<Mesh::VertexHandle> face_vhandles;
            face_vhandles.push_back(vfrom);
            face_vhandles.push_back(vto);
            face_vhandles.push_back(c_vfrom);
            mesh.add_face(face_vhandles);

            face_vhandles.clear();
            face_vhandles.push_back(c_vto);
            face_vhandles.push_back(c_vfrom);
            face_vhandles.push_back(vto);
            mesh.add_face(face_vhandles);
        }
    }

}

void MeshShellTool::verticeNormarlDirectionMove(Mesh &mesh, double distance)
{
    // 构建法向量
    mesh.request_vertex_normals();
    mesh.update_normals();

    Mesh::VertexIter vit, vend = mesh.vertices_end();
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
