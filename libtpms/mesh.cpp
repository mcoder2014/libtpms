#include "mesh.h"

Mesh *to2Mesh(std::vector<glm::vec3> &vertices, std::vector<glm::ivec3> &faces)
{
    if(vertices.size() <= 3 || faces.size() < 1)
        return nullptr;

    Mesh * m_object = new Mesh();

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

    return m_object;
}
