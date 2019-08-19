#include "smoothtool.h"

#include <iostream>

SmoothTool::SmoothTool()
{
    m_object = nullptr;
    m_result = nullptr;

}

bool SmoothTool::createMesh(
        std::vector<glm::vec3> &vertices,
        std::vector<glm::ivec3> &faces)
{
    if(m_object != nullptr)
    {
        delete m_object;
        m_object = nullptr;
    }

    if(vertices.size() <= 3 || faces.size() < 1)
        return false;

    m_object = new Mesh;    // Create new Mesh

    // Used for add face
    std::vector<Mesh::VertexHandle> vHandle(vertices.size());

    // Add vertices
    for (size_t i = 0; i < vertices.size(); i++)
    {

        vHandle[i] = m_object->add_vertex(
                    Mesh::Point(
                        vertices[i][0],
                        vertices[i][1],
                        vertices[i][2]));
    }

    // Add faces
    std::vector<Mesh::VertexHandle> face_vhandles;

    for (glm::vec3 face : faces)
    {
        face_vhandles.clear();

        face_vhandles.push_back(vHandle[face[0]]);
        face_vhandles.push_back(vHandle[face[1]]);
        face_vhandles.push_back(vHandle[face[2]]);

        m_object->add_face(face_vhandles);
    }

    return true;
}

void SmoothTool::basicSmooth()
{
    if(! m_object)
    {
        std::cerr << "m_object == nullptr\n"
                  << "Please check whether the mesh is created"
                  << std::endl;
        return;
    }

    Mesh &mesh = (*m_object);   // Get the reference
    m_result = new Mesh(mesh);  // New a result
    Mesh::VertexIter v_it, v_end(mesh.vertices_end());

    // To get the value into result object
    Mesh::VertexIter v_result_it(m_result->vertices_begin());

    Mesh::VertexVertexIter vv_it;
//    Mesh::VertexFaceIter vf_it;
    OpenMesh::Vec3f cog;        // To record the gravity of the point
    int valance = 0;            // Record the degree of the point

    int round = 0;

    for (v_it = mesh.vertices_begin(); v_it != v_end; v_it++, v_result_it++)
    {
        if(!mesh.is_boundary(*v_it))
        {
            // Don't change the position of boundary vertex

            cog = OpenMesh::Vec3f(0.0); // Init it into origin point
            valance = 0;
            for (vv_it = mesh.vv_iter(*v_it);
                 vv_it.is_valid();
                 vv_it++)
            {
                cog += mesh.point(*vv_it);
                valance++;
            }

            cog = cog / static_cast<float>(valance);
//            mesh.data(*v_it).cog = cog;     // Record the cog using kernel traits
            // Record the calculated value
            m_result->set_point(*v_result_it, cog);
            round ++;
        }
    }
    std::cout << "Valuable round : " << round << std::endl;
}

void SmoothTool::writeOBJ(const std::string &file_origin, const std::string &file_result)
{
    // export origin
    // write mesh to output.obj
    try
    {
        if(file_origin.length() > 0 && m_object)
        {
            if ( !OpenMesh::IO::write_mesh(*m_object, file_origin))
            {
                std::cerr << "Cannot write origin mesh to "
                          << file_origin
                          << std::endl;
            }
        }

        if(file_result.length() > 0 && m_result)
        {
            if(!OpenMesh::IO::write_mesh(*m_result, file_result))
            {
                std::cerr << "Cannot write result mesh to "
                          << file_result
                          << std::endl;
            }
        }

    }
    catch( std::exception& x )
    {
      std::cerr << x.what() << std::endl;
    }

    // export result
}
