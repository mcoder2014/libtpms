#include "simplification.h"

#include <iostream>

#include <OpenMesh/Tools/Decimater/DecimaterT.hh>
#include <OpenMesh/Tools/Decimater/ModQuadricT.hh>

Simplification::Simplification()
    :m_object(nullptr), m_srcVecCount(0), m_dstVecCount(0)
{

}

Simplification::~Simplification()
{
    if(m_object != nullptr)
        delete m_object;
}

Mesh *Simplification::createMesh(std::vector<glm::vec3> &vertices,
                                 std::vector<glm::ivec3> &faces)
{
    if(m_object != nullptr)
    {
        delete m_object;
        m_object = nullptr;
    }

    if(vertices.size() <= 3 || faces.size() < 1)
        return nullptr;

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

    m_srcVecCount = m_object->n_vertices();
    return m_object;
}

Mesh *Simplification::createMesh(Mesh *mesh)
{
    if(m_object != nullptr)
    {
        delete m_object;
        m_object = nullptr;
    }

    if(mesh == nullptr)
        return nullptr;

    m_object = new Mesh;
    m_object->assign(*mesh); // copy
    m_srcVecCount = m_object->n_vertices();
    return m_object;
}

Mesh *Simplification::decimater(float factor)
{
    if(factor <0 || factor >1)
        return m_object;

    return decimater(static_cast<int>(m_srcVecCount * factor));
}

Mesh *Simplification::decimater_to(float factor)
{
    if(factor <0 || factor >1)
        return m_object;

    return decimater_to(static_cast<int>(m_srcVecCount * factor));
}

Mesh *Simplification::decimater(int n_collapses)
{
    if(m_object == nullptr)
        return nullptr;

    m_dstVecCount = m_srcVecCount - n_collapses;
    if(m_dstVecCount < 0)
        return nullptr;

    // Start
    OpenMesh::Decimater::DecimaterT<Mesh> decimater(*m_object);
    OpenMesh::Decimater::ModQuadricT<Mesh>::Handle hModQuadric;
    decimater.add(hModQuadric);
    std::cout << "Decimater module name: "
              << decimater.module(hModQuadric).name() << std::endl;
//    decimater.module(hModProgMesh).unset_max_err();
    decimater.initialize();
    size_t collapse = decimater.decimate(n_collapses);
    std::cout << "Collapse vertices: " << collapse << std::endl;
    m_object->garbage_collection();
    return m_object;
}

Mesh *Simplification::decimater_to(int n_target)
{
    if(m_object == nullptr || n_target <= 0)
        return nullptr;

    m_dstVecCount = n_target;

    // if no needs to reduce vectices
    if(m_srcVecCount < m_dstVecCount)
        return m_object;

    // Start
    OpenMesh::Decimater::DecimaterT<Mesh> decimater(*m_object);
    OpenMesh::Decimater::ModQuadricT<Mesh>::Handle hModQuadric;
    decimater.add(hModQuadric);
    std::cout << "Decimater module name: "
              << decimater.module(hModQuadric).name() << std::endl;
//    decimater.module(hModProgMesh).unset_max_err();
    decimater.initialize();
    size_t collapse =  decimater.decimate_to(m_dstVecCount);
    std::cout << "Collapse vertices: " << collapse << std::endl;
    m_object->garbage_collection();
    return m_object;
}
