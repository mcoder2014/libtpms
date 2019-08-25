#include "meshcutting.h"

#include <limits>

MeshCutting::MeshCutting()
{
    m_model = nullptr;
    m_a0 = m_a1 = m_a2 = m_a3 = 0.0f;
}

///
/// \brief MeshCutting::setCuttingPlane
/// \param v0
/// \param v1
/// \param v2
/// Get the plane function from three points.
/// They should not at the same point or same line!
///
void MeshCutting::setCuttingPlane(OpenMesh::Vec3f v0, OpenMesh::Vec3f v1, OpenMesh::Vec3f v2)
{
    OpenMesh::Vec3f v01 = v1 - v0;
    OpenMesh::Vec3f v02 = v2 - v0;

    OpenMesh::Vec3f normal = v01 % v02;
    normal.normalize();     // normallized to length 1

    m_a0 = normal[0];
    m_a1 = normal[1];
    m_a2 = normal[2];
    m_a3 = -(normal|v0);    // dot product

    printCuttingPlane();
}

///
/// \brief MeshCutting::setCuttingPlane
/// \param a0
/// \param a1
/// \param a2
/// \param a3
///
void MeshCutting::setCuttingPlane(float a0, float a1, float a2, float a3)
{
    m_a0 = a0;
    m_a1 = a1;
    m_a2 = a2;
    m_a3 = a3;

    printCuttingPlane();
}

///
/// \brief MeshCutting::cuttingPlane
/// \return
///
std::vector<float> MeshCutting::cuttingPlane()
{
    return std::vector<float>{m_a0, m_a1, m_a2, m_a3};
}

///
/// \brief MeshCutting::loadMesh
/// \param filepath
/// \return
/// load the insole model
///
bool MeshCutting::loadMesh(const std::string &filepath)
{
    if(m_model == nullptr)
        m_model = new Mesh;
    else {
        delete m_model;
        m_model = new Mesh;
    }

    if(!OpenMesh::IO::read_mesh(*m_model, filepath))
    {
        return false;
    }

    std::cout << "Vertices: " << m_model->n_vertices() << std::endl;
    std::cout << "Edges: " << m_model->n_edges() << std::endl;
    std::cout << "Faces: " << m_model->n_faces() << std::endl;

    // Calculate the bounding box
    m_xmax = m_ymax = m_zmax = std::numeric_limits<float>::min();
    m_xmin = m_ymin = m_zmin = std::numeric_limits<float>::max();

    Mesh::VertexIter    v_it, v_end(m_model->vertices_end());
    for (v_it=m_model->vertices_begin(); v_it!=v_end; ++v_it)
    {
        // for each point
        OpenMesh::Vec3f point = m_model->point(*v_it);

        if(point[0] > m_xmax) m_xmax = point[0];
        if(point[0] < m_xmin) m_xmin = point[0];

        if(point[1] > m_ymax) m_ymax = point[1];
        if(point[1] < m_ymin) m_ymin = point[1];

        if(point[2] > m_zmax) m_zmax = point[2];
        if(point[2] < m_zmin) m_zmin = point[2];
    }

    std::cout << "Bounding Box:\tMax\tMin\n" << "X:\t" << m_xmax << "\t" << m_xmin << "\n"
              << "Y:\t" << m_ymax << "\t" << m_ymin << "\n"
              << "Z:\t" << m_zmax << "\t" << m_zmin << std::endl;

    return true;
}

///
/// \brief MeshCutting::cutting
///     Start cutting circle
///     Save results in the 2-d vector
///
void MeshCutting::cutting()
{
    if(m_model == nullptr)
    {
        std::cerr << "Stop cutting! The point of the model is nullptr"
                  << std::endl;
        return;
    }

    // Rename as a regional variable
    Mesh& mesh = *m_model;

    Mesh::FaceIter f_it, f_end(mesh.faces_end());
    for (f_it = mesh.faces_begin(); f_it != f_end; f_it++)
    {
        if(!mesh.data(*f_it).checked)
        {
            // To prevent the repeated calculation
            // Check the face itself
            std::vector<OpenMesh::Vec3f> intersection = face_intersection_cutting_plane(f_it);

            // check its neighbour
        }

    }
}

///
/// \brief MeshCutting::face_intersection_cutting_plane
/// \param f_it
/// \return
/// Get the intersection points from one face and cutting plane
///
std::vector<OpenMesh::Vec3f> MeshCutting::face_intersection_cutting_plane(Mesh::FaceIter f_it)
{
    std::vector<OpenMesh::Vec3f> results;
    Mesh &mesh = *m_model;

    auto face = mesh.face(*f_it);

}

void MeshCutting::printCuttingPlane()
{
    std::cout << "Cutting plane: "
              << "F(x,y,z) = " << m_a0 << " * x + "
              << m_a1 << "* y + "
              << m_a2 << "* z + "
              << m_a3 << std::endl;
}
