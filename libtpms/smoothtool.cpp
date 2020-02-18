#include "smoothtool.h"

#include <iostream>

#include <OpenMesh/Tools/Smoother/JacobiLaplaceSmootherT.hh>

SmoothTool::SmoothTool()
{
    m_object = nullptr;
    m_result = nullptr;
    m_backup = nullptr;
}

SmoothTool::~SmoothTool()
{
    if(m_object != nullptr)
        delete m_object;
    if(m_result != nullptr)
        delete m_result;
    if(m_backup != nullptr)
        delete  m_backup;
}

Mesh* SmoothTool::createMesh(
        std::vector<glm::vec3> &vertices,
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

    // backup
    if(m_backup == nullptr)
        m_backup = new Mesh;
    m_backup->assign(*m_object);

    return m_object;
}

Mesh *SmoothTool::createMesh(std::string filepath)
{
    if(m_object != nullptr)
    {
        delete m_object;
        m_object = nullptr;
    }
    if(filepath.size() == 0)
        return nullptr;

    m_object = new Mesh();
    OpenMesh::IO::read_mesh(*m_object, filepath);

    // backup
    if(m_backup == nullptr)
        m_backup = new Mesh;
    m_backup->assign(*m_object);

    return nullptr;
}

Mesh *SmoothTool::createMesh(Mesh *mesh)
{
    if(m_object != nullptr)
    {
        delete m_object;
        m_object = nullptr;
    }
    if(mesh == nullptr)
        return nullptr;

    m_object = new Mesh();
    m_object->assign(*mesh);

    // backup
    if(m_backup == nullptr)
        m_backup = new Mesh;
    m_backup->assign(*m_object);

    return nullptr;
}

///
/// \brief SmoothTool::meshOffset
/// 将一个单层曲面向各顶点的法方向位移一定一定厚度，并封闭成体。
/// \param mesh
/// \param depth
/// \return
///
Mesh *SmoothTool::meshOffset(Mesh *mesh, float depth)
{
    // 记录当前顶点数目
    int nVectices = mesh->n_vertices();

    /// 将曲面复制一份，追加到 mesh 中，面法相相反
    // 复制网格顶点
    Mesh::VertexIter vit, vend = mesh->vertices_end();
    for(vit = mesh->vertices_begin(); vit != vend; vit++)
    {
        OpenMesh::Vec3f point = mesh->point(*vit);
        mesh->add_vertex(point);
    }

    // 复制网格面片
    Mesh::FaceIter fit, fend = mesh->faces_end();
    std::vector<Mesh::VertexHandle> vface;
    for(fit = mesh->faces_begin(); fit != fend; fit++)
    {
        vface.clear();
        Mesh::FaceVertexIter fvit = mesh->fv_iter(*fit);
        for(;fvit.is_valid(); fvit++)
        {
            Mesh::VertexHandle vh = *fvit;
            vface.push_back(mesh->vertex_handle(vh.idx() + nVectices));
        }

        std::reverse(vface.begin(), vface.end());
        mesh->add_face(vface);
    }

    /// 封闭曲面的缺口
    Mesh::HalfedgeIter heit, heend(mesh->halfedges_end());
    for(heit = mesh->halfedges_begin(); heit != heend; heit++)
    {
        // if boundary, add two face
        if(mesh->is_boundary(*heit))
        {
            Mesh::HalfedgeHandle heh = *heit;

            Mesh::VertexHandle vfrom = mesh->from_vertex_handle(heh);
            Mesh::VertexHandle vto = mesh->to_vertex_handle(heh);

            int offset = nVectices;
            if(vfrom.idx() >= nVectices)
                offset = -nVectices;

            Mesh::VertexHandle c_vfrom = mesh->vertex_handle(vfrom.idx() + offset);
            Mesh::VertexHandle c_vto = mesh->vertex_handle(vto.idx() + offset);

            // add face
            std::vector<Mesh::VertexHandle> face_vhandles;
            face_vhandles.push_back(vfrom);
            face_vhandles.push_back(vto);
            face_vhandles.push_back(c_vfrom);
            mesh->add_face(face_vhandles);

            face_vhandles.clear();
            face_vhandles.push_back(c_vto);
            face_vhandles.push_back(c_vfrom);
            face_vhandles.push_back(vto);
            mesh->add_face(face_vhandles);

        }
    }

    /// 曲面中各定点向法方向移动 depth/2 位置
    mesh->request_vertex_normals();
    mesh->update_normals();

    // init iterator
    vit = mesh->vertices_begin();
    vend = mesh->vertices_end();

    float mov_len = depth/2;    // Every point's mov length
    for( ; vit!=vend ; ++vit)
    {
        OpenMesh::Vec3f vpos = mesh->point(*vit);
        if(!mesh->is_boundary(*vit))
        {
            // 内部点向顶点法向量移动 depth/2 位置
            OpenMesh::Vec3f vnormal = mesh->normal(*vit);
            vpos += vnormal.normalized() * mov_len;
            mesh->set_point(*vit, vpos);
        }
        else
        {
            // 边界点向临界面法相平均值移动 depth/2 位置
            OpenMesh::Vec3f fnoraml(0.0, 0.0, 0.0);
            Mesh::VertexFaceIter vfit = mesh->vf_begin(vit);
            for(; vfit.is_valid(); vfit++)
            {
                fnoraml += mesh->normal(*vfit);
            }
            vpos += fnoraml.normalized() * mov_len;
            mesh->set_point(*vit, vpos);
        }
    }



    return mesh;
}

void SmoothTool::basicSmooth(int rounds)
{
    if(! m_object)
    {
        std::cerr << "m_object == nullptr\n"
                  << "Please check whether the mesh is created"
                  << std::endl;
        return;
    }

    if(m_result == nullptr)
        m_result = new Mesh;
    m_result->assign(*m_object);     // Deep copy a openmesh model

    for (int round = 0; round < rounds; round++)
    {
        // Every round, exchange the pointer of m_object and m_result
        // reduce the copy of mesh

        Mesh::VertexIter v_it, v_end(m_object->vertices_end());

        // To get the value into result object
        Mesh::VertexIter v_result_it(m_result->vertices_begin());

        Mesh::VertexVertexIter vv_it;
        Mesh::VertexFaceIter vf_it;
        OpenMesh::Vec3f cog;        // To record the gravity of the point
        int valance = 0;            // Record the degree of the point
        int points_count = 0;

        /// 计算每个顶点的目标坐标
        for (v_it = m_object->vertices_begin(); v_it != v_end; ++v_it, ++v_result_it)
        {
            // 决定边界的点要不要平滑一下
            if(!m_object->is_boundary(*v_it))
            {
                // Don't change the position of boundary vertex
                cog = OpenMesh::Vec3f(0.0); // Init it into origin point
                valance = 0;

                for (vf_it = m_object->vf_begin(*v_it);
                     vf_it.is_valid();vf_it++)
                {
                    OpenMesh::Vec3f gravity(0.0);
                    Mesh::FaceVertexIter fv_it;

                    for (fv_it = m_object->fv_begin(*vf_it);
                         fv_it.is_valid();fv_it++)
                    {
                        gravity += m_object->point(*fv_it);
                    }
                    gravity = gravity/3;
                    cog += gravity;
                    valance++;
                }
                points_count ++;
                cog = cog / static_cast<float>(valance);

                // Record the calculated value
                 m_result->set_point(*v_result_it, cog);


            } // if(!m_object->is_boundary(*v_it))
            else
            {
                // 对于边界点，只和相邻边界点进行平滑
                cog = OpenMesh::Vec3f(0.0); // Init it into origin point
                valance = 0;
                for(vv_it = m_object->vv_begin(*v_it); vv_it.is_valid(); ++vv_it)
                {
                    if(m_object->is_boundary(*vv_it))
                    {
                        valance++;
                        cog += m_object->point(*vv_it);
                    }
                }
                cog /= valance;
                cog = 0.5 * cog + 0.5 * m_object->point(*v_it);

                // Record the calculated value
                m_result->set_point(*v_it, cog);
            }
        }

        std::cout << "Valuable round: " << round
                  <<"\nPoints count: " << points_count << std::endl;

        // Exchange the pointer of m_object and m_result
        {
            Mesh *ptr = m_object;
            m_object = m_result;
            m_result = ptr;
        }
    }

    delete m_result;
    m_result = nullptr;
}

void SmoothTool::jacobiLaplaceSmooth(int rounds)
{
    if(rounds <= 0)
        return;

    // Initialize smoother with input mesh
    OpenMesh::Smoother::JacobiLaplaceSmootherT<Mesh> smoother(*m_object);
    smoother.initialize(  OpenMesh::Smoother::JacobiLaplaceSmootherT<Mesh>::Tangential_and_Normal,   //Smooth direction
                         OpenMesh::Smoother::JacobiLaplaceSmootherT<Mesh>::C0);                      //Continuity
    // Execute 3 smooth steps
    smoother.smooth(rounds);
}

void SmoothTool::writeOBJ(const std::string &file_origin, const std::string &file_result)
{
    // export origin
    // write mesh to output.obj
    try
    {

        OpenMesh::IO::Options option;
        option += OpenMesh::IO::Options::Binary;

        if(file_origin.length() > 0 && m_backup)
        {
            if ( !OpenMesh::IO::write_mesh(*m_backup, file_origin, OpenMesh::IO::Options::Binary))
            {
                std::cerr << "Cannot write origin mesh to "
                          << file_origin
                          << std::endl;
            }
            std::cout << "OpenMesh saving origin mesh into file:\t" << file_origin << std::endl;
        }

        if(file_result.length() > 0 && m_object)
        {
            if(!OpenMesh::IO::write_mesh(*m_object, file_result, OpenMesh::IO::Options::Binary))
            {
                std::cerr << "Cannot write result mesh to "
                          << file_result
                          << std::endl;
            }
            std::cout << "OpenMesh saving smoothed mesh into file:\t" << file_result << std::endl;
        }

    }
    catch( std::exception& x )
    {
      std::cerr << x.what() << std::endl;
    }

    // export result
}

Mesh *SmoothTool::getMeshDeep()
{
    Mesh *mesh = new Mesh;
    mesh->assign(*m_object);
    return mesh;
}
