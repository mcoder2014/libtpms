#include "MeshInfoChecker.h"

MeshInfoChecker::MeshInfoChecker()
    :meshinfo(std::make_shared<MeshInfo>())
{}

std::shared_ptr<MeshInfo> MeshInfoChecker::check(Mesh& mesh)
{
    checkSize(mesh);
    checkHole(mesh);
    return meshinfo;
}

/**
 * @brief MeshInfoChecker::checkHole
 * 查询模型是否有空洞
 * @param mesh
 * @return
 */
bool MeshInfoChecker::checkHole(Mesh &mesh)
{
    // 边界数量统计
    size_t boundaryVerticeNum = 0;
    size_t boundaryEdgeNum = 0;
    size_t boundaryFaceNum = 0;

    // Vertex
    for(Mesh::VertexIter v_it=mesh.vertices_begin(); v_it!=mesh.vertices_end(); v_it++){
        if(mesh.is_boundary(*v_it)) {
            boundaryVerticeNum++;
        }
    }

    // Edge
    for(Mesh::EdgeIter e_it = mesh.edges_begin(); e_it!=mesh.edges_end();e_it++){
        if(mesh.is_boundary(*e_it)) {
            boundaryEdgeNum++;
        }
    }

    // Face
    for(Mesh::FaceIter f_it=mesh.faces_begin();f_it!=mesh.faces_end();f_it++){
        if(mesh.is_boundary(*f_it)) {
            boundaryFaceNum++;
        }
    }

    // 统计空洞
    meshinfo->hasHole = (boundaryVerticeNum + boundaryEdgeNum + boundaryFaceNum) > 0 ;
    return meshinfo->hasHole;
}

/**
 * @brief MeshInfoChecker::checkSize
 * 检查 网格模型的尺寸、顶点数量
 * @param mesh
 * @return
 */
std::shared_ptr<MeshInfo> MeshInfoChecker::checkSize(Mesh &mesh)
{
    meshinfo->faceNum = mesh.n_faces();
    meshinfo->edgeNum = mesh.n_edges();
    meshinfo->verticesNum = mesh.n_vertices();

    meshinfo->boundingBox.setEmpty();

    // Vertex
    Eigen::Vector3d vertex;
    for(Mesh::VertexIter v_it=mesh.vertices_begin(); v_it!=mesh.vertices_end(); v_it++){
        Mesh::Point point = mesh.point(*v_it);
        vertex.x() = point[0];
        vertex.y() = point[1];
        vertex.z() = point[2];
        meshinfo->boundingBox.extend(vertex);
    }
    return meshinfo;
}

std::shared_ptr<MeshInfo> MeshInfoChecker::getMeshinfo() const
{
    return meshinfo;
}
