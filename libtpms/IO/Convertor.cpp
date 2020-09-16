#include "Convertor.h"

std::shared_ptr<SurfaceMeshModel> Convertor::mesh2SurfaceMeshModel(const Mesh &mesh)
{
    std::shared_ptr<SurfaceMeshModel> model = std::make_shared<SurfaceMeshModel>();

    Mesh::VertexIter vit, vend = mesh.vertices_end();
    for(vit = mesh.vertices_begin(); vit != vend; vit++)
    {
        Mesh::Point point = mesh.point(*vit);
        model->add_vertex(
            Eigen::Vector3d(point[0], point[1], point[2]));
    }

    // faces
    Mesh::FaceIter fit;
    std::vector<SurfaceMesh::Vertex> vface;
    for(fit = mesh.faces_begin(); fit!=mesh.faces_end(); fit++)
    {
        vface.clear();

        Mesh::FaceVertexIter vfit = mesh.cfv_iter(*fit);
        for(;vfit.is_valid(); vfit++)
        {
            Mesh::VertexHandle vh = *vfit;
            vface.push_back(SurfaceMesh::Vertex(vh.idx()));
        }

        model->add_face(vface);
    }

    return model;
}

Mesh Convertor::surfaceMeshModel2mesh(SurfaceMesh::SurfaceMeshModel &model)
{
    Mesh mesh;;

    // Vertex
    SurfaceMesh::Vector3VertexProperty vecs = model.vertex_coordinates();

    for(SurfaceMesh::Vertex vit: model.vertices())
    {
        Mesh::Point point(vecs[vit][0],vecs[vit][1],vecs[vit][2]);
        mesh.add_vertex(point);
    }

    //Faces
    std::vector<OpenMesh::VertexHandle> face;
    for(SurfaceMesh::Face f:model.faces())
    {
        face.clear();

        SurfaceMesh::SurfaceMeshForEachVertexOnFaceHelper vit = model.vertices(f);
        vit = vit.begin();
        SurfaceMesh::SurfaceMeshForEachVertexOnFaceHelper vend = vit;
        for(;vit!=vend;++vit)
        {
            face.push_back(mesh.vertex_handle((*vit).idx()));
        }

        mesh.add_face(face);
    }
    return mesh;
}
