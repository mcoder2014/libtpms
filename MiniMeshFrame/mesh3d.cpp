#include "mesh3d.h"

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <limits>
#include <algorithm>
#include <cmath>


std::vector<HE_vert *> *Mesh3D::pvertices_list()
{
    return m_pvertices_list;
}



std::vector<HE_edge *> *Mesh3D::pedges_list()
{
    return m_pedges_list;
}


std::vector<HE_face *> *Mesh3D::pfaces_list()
{
    return m_pfaces_list;
}


int Mesh3D::num_of_vertex_list()
{
    if(m_pvertices_list == nullptr)
        return 0;
    else
        return static_cast<int>(m_pvertices_list->size());
}

int Mesh3D::num_of_half_edge_list()
{
    if(m_pedges_list == nullptr)
        return 0;
    else
        return static_cast<int>(m_pedges_list->size());
}

int Mesh3D::num_of_edge_list()
{
    return num_of_half_edge_list()/2;
}

int Mesh3D::num_of_face_list()
{
    if(m_pfaces_list == nullptr)
        return 0;
    else
        return static_cast<int>(m_pfaces_list->size());
}

HE_vert *Mesh3D::get_vertex(int id)
{
    return (id >= num_of_vertex_list() || id < 0 ) ? nullptr : (*m_pvertices_list)[id];
}

HE_edge *Mesh3D::get_half_edge(int id)
{
    return (id >= num_of_half_edge_list() || id < 0) ? nullptr : (*m_pedges_list)[id];
}

HE_face *Mesh3D::get_face(int id)
{
    return( id >= num_of_face_list() || id<0)? nullptr: (*m_pfaces_list)[id];
}

HE_edge *Mesh3D::get_edge(HE_vert *hv0, HE_vert *hv1)
{
    if(!hv0 || !hv1)
        return nullptr;
    HE_edge *edge = hv0->m_pedge;

    do
    {
        if(edge->m_pvert == hv1)
            return edge;

        edge = edge->m_ppair->m_pnext;

    }while (edge != hv0->m_pedge);

    return nullptr;
}

bool Mesh3D::isValid()
{
    if(num_of_vertex_list()==0 || num_of_face_list()==0)
        return false;

    return true;
}

///
/// \brief Mesh3D::InsertVertex
/// \param v
/// \return
///
HE_vert *Mesh3D::InsertVertex(const glm::vec3 &v)
{
    HE_vert *pvert = new HE_vert(v);

    if(m_pvertices_list == nullptr)
        m_pvertices_list = new std::vector<HE_vert*>();
    pvert->m_id= static_cast<int>(m_pvertices_list->size());
    m_pvertices_list->push_back(pvert);
    return pvert;
}

///
/// \brief Mesh3D::InsertEdge
/// \param vstart
/// \param vend
/// \return
///
HE_edge *Mesh3D::InsertEdge(HE_vert *vstart, HE_vert *vend)
{
    if(vstart == nullptr || vend == nullptr)
        return nullptr;
    if(m_pedges_list == nullptr)
        m_pedges_list = new std::vector<HE_edge *>();

    // If the half edge is already existing.
    if(m_edgemap[PAIR_VERTEX(vstart, vend)] != nullptr)
        return m_edgemap[PAIR_VERTEX(vstart, vend)];

    HE_edge* pedge = new HE_edge;
    pedge->m_pvert = vend;
    pedge->m_pvert->m_degree++;
    vstart->m_pedge = pedge;
    m_edgemap[PAIR_VERTEX(vstart, vend)] = pedge;

    pedge->m_id = static_cast<int>(m_pedges_list->size());
    m_pedges_list->push_back(pedge);

    return pedge;
}

///
/// \brief Mesh3D::InsertFace
/// \param vec_hv
/// \return
///
HE_face *Mesh3D::InsertFace(std::vector<HE_vert *> &vec_hv)
{
    int vsize = static_cast<int>(vec_hv.size());

    // First Check the face list
    if(m_pfaces_list == nullptr)
        m_pfaces_list = new std::vector<HE_face*>();

    // New a HE_face
    HE_face *pface = new HE_face();
    pface->m_valence = vsize;   // The number of vertexes the face have

    // Iter used to create the structure of the face
    VERTEX_ITER viter = vec_hv.begin();
    VERTEX_ITER nviter = vec_hv.begin();
    nviter ++;
    HE_edge *he1 = nullptr, *he2 = nullptr;

    std::vector<HE_edge* > vec_edges;   // edges

    int i=0;
    for(i=0; i<vsize-1;i++)
    {
        // Create a pair of Half edge
        he1 = InsertEdge(*viter, *nviter);
        he2 = InsertEdge(*nviter, *viter);

        // Leave a pointer in pface for handling
        if(pface->m_pedge == nullptr)
            pface->m_pedge = he1;

        // Create a pair
        he1->m_pface = pface;
        he1->m_ppair = he2;
        he2->m_ppair = he1;

        // Add the direct half_edge into vec_edges
        vec_edges.push_back(he1);

        viter++;
        nviter++;
    }

    // Now nviter is the first element of vec_hv
    // viter is end element of vec_hv
    // To create a full circle
    nviter = vec_hv.begin();

    he1 = InsertEdge(*viter, *nviter);
    he2 = InsertEdge(*nviter, *viter);

    he1->m_pface = pface;
    if(pface->m_pedge == nullptr)
        pface->m_pedge = he1;

    he1->m_ppair = he2;
    he2->m_ppair = he1;
    vec_edges.push_back(he1);

    // To get the half-edges end-to-end
    for (i=0;i<vsize-1;i++)
    {
        vec_edges[i]->m_pnext = vec_edges[i+1];
        vec_edges[i+1]->m_pprev = vec_edges[i];
    }
    vec_edges[i]->m_pnext = vec_edges[0];
    vec_edges[0]->m_pprev = vec_edges[i];

    // Insert the HE_face into the face list
    pface->m_id = static_cast<int>(m_pfaces_list->size());
    m_pfaces_list->push_back(pface);

    return pface;
}

///
/// \brief Mesh3D::LoadFromOBJFile
/// \param fins
/// \return
///
bool Mesh3D::LoadFromOBJFile(const char *fins)
{
    std::cout<<"Loding file: "<< fins << std::endl;
    FILE *pfile = fopen(fins, "r");

    char *tok;
    char temp[128];

    try
    {
        ClearData();

        // Read vertex
        fseek(pfile, 0, SEEK_SET);
        char pLine[512];

        while(fgets(pLine, 512, pfile))
        {
            if(pLine[0] == 'v' &&pLine[1]== ' ')
            {
                glm::vec3 nvv;

                tok = strtok(pLine, " ");
                for (int i = 0; i < 3; i++)
                {
                    tok = strtok(nullptr, " ");
                    strcpy(temp, tok);
                    temp[strcspn(temp, " ")] = 0;
                    nvv[i] = static_cast<float>(atof(temp));
                }
                InsertVertex(nvv);
            }
        }   // Read vertex

        // Read faces
        fseek(pfile, 0, SEEK_SET);
        while(fgets(pLine, 512, pfile))
        {
            char *pTmp = pLine;
            if(pTmp[0] == 'f')
            {
                std::vector<HE_vert* > s_faceid;

                tok = strtok(pLine, " ");
                while((tok = strtok(nullptr, " ")) != nullptr)
                {
                    strcpy(temp, tok);
                    temp[strcspn(temp, "/")] = 0;
                    int id = static_cast<int>(strtol(temp, nullptr, 10) -1);
                    HE_vert *hv = get_vertex(id);
                    bool findit = false;

                    for (int i = 0; i < static_cast<int>(s_faceid.size()); i++)
                    {
                        if(hv == s_faceid[i])
                        {
                            // remove redundant vertex id if it exists
                            findit = true;
                            break;
                        }
                    }

                    if(findit == false && hv != nullptr)
                    {
                        s_faceid.push_back(hv);
                    }
                }
                if(static_cast<int>(s_faceid.size()) >= 3)
                {
                    InsertFace(s_faceid);
                }
            }
        }   // Read faces

        // Read texture coords
        fseek(pfile, 0, SEEK_SET);
        std::vector<glm::vec3> texCoordsTemp;
        while(fscanf(pfile, "%s", pLine) != EOF)
        {
            if(pLine[0] == 'v' && pLine[1] == 't')
            {
                glm::vec3 texTemp(0.0f, 0.0f, 0.0f);
                fscanf(pfile, "%f %f", &texTemp[0], &texTemp[1]);
                texCoordsTemp.push_back(texTemp);
            }
        }

        // Read texture index
        if(texCoordsTemp.size() > 0)
        {
            fseek(pfile, 0, SEEK_SET);

            int faceIndex = 0;
            while(fscanf(pfile, "%s", pLine) != EOF)
            {
                if(pLine[0] == 'f')
                {
                    int v, t;
                    fscanf(pfile, "%s", pLine);
                    if(sscanf(pLine, "%d/%d", &v, &t) == 2)
                    {
                        std::map<int, int> v2tex;
                        v2tex[v - 1] = t - 1;

                        fscanf(pfile, "%s", pLine);
                        sscanf(pLine, "%d/%d", &v, &t);
                        v2tex[v-1] = t - 1;

                        fscanf(pfile, "%s", pLine);
                        sscanf(pLine, "%d/%d", &v, &t);
                        v2tex[v-1] = t - 1;

                        HE_edge* edgeTemp = m_pfaces_list->at(faceIndex)->m_pedge;
                        edgeTemp->m_texCoord = texCoordsTemp.at(v2tex[edgeTemp->m_pvert->m_id]);
                        edgeTemp->m_pvert->m_texCoord = edgeTemp->m_texCoord;

                        edgeTemp = edgeTemp->m_pnext;
                        edgeTemp->m_texCoord = texCoordsTemp.at(v2tex[edgeTemp->m_pvert->m_id]);
                        edgeTemp->m_pvert->m_texCoord = edgeTemp->m_texCoord;

                        edgeTemp = edgeTemp->m_pnext;
                        edgeTemp->m_texCoord = texCoordsTemp.at(v2tex[edgeTemp->m_pvert->m_id]);
                        edgeTemp->m_pvert->m_texCoord = edgeTemp->m_texCoord;

                        faceIndex++;
                    }
                }
            }
        }   // Read texture index

        UpdateMesh();
//        Unify(2.0f);

    }
    catch (...)
    {
        ClearData();
        fclose(pfile);
        return false;
    }
    fclose(pfile);
    return isValid();

}

void Mesh3D::WriteToOBJFile(const char *fouts)
{

}

void Mesh3D::UpdateMesh()
{

}

void Mesh3D::UpdateNormal()
{

}

///
/// \brief Mesh3D::ComputeBoundingBox
///
void Mesh3D::ComputeBoundingBox()
{
    // To few faces
    if(m_pfaces_list->size() < 3)
        return;

    // Get the range of foat
    const float MAX_FLOAT_VALUE = std::numeric_limits<float>::max();
    const float MIN_FLOAT_VALUE = std::numeric_limits<float>::min();

    m_xmax = m_ymax = m_zmax = MIN_FLOAT_VALUE;
    m_xmin = m_ymin = m_zmin = MAX_FLOAT_VALUE;

    VERTEX_ITER viter = m_pvertices_list->begin();
    for(; viter != m_pvertices_list->end(); viter++)
    {
        // Get the range of the model
        m_xmax = std::max<float>(m_xmax, (*viter)->m_position[0]);
        m_ymax = std::max<float>(m_ymax, (*viter)->m_position[1]);
        m_zmax = std::max<float>(m_zmax, (*viter)->m_position[2]);

        m_xmin = std::min<float>(m_xmin, (*viter)->m_position[0]);
        m_ymin = std::min<float>(m_ymin, (*viter)->m_position[1]);
        m_zmin = std::min<float>(m_zmin, (*viter)->m_position[2]);
    }

}

void Mesh3D::ClearData()
{
    ClearVertex();
    ClearEdges();
    ClearFaces();

    m_edgemap.clear();

    m_xmax = m_ymax = m_zmax = 1.0f;
    m_xmin = m_ymin = m_zmin = -1.0f;
}

void Mesh3D::ClearVertex()
{
    if(m_pvertices_list == nullptr)
        return;
    else
    {
        for(VERTEX_ITER viter = m_pvertices_list->begin();
            viter != m_pvertices_list->end(); viter++)
        {
            if(*viter != nullptr)
            {
                delete *viter;
                *viter = nullptr;
            }
            else {
                // Error
            }
        }
        delete m_pvertices_list;
        m_pvertices_list = nullptr;
    }

}

void Mesh3D::ClearEdges()
{
    if(m_pedges_list == nullptr)
        return;
    else
    {
        for (EDGE_ITER eiter = m_pedges_list->begin();
             eiter != m_pedges_list->end();eiter++)
        {
            if(*eiter != nullptr)
            {
                delete *eiter;
                *eiter = nullptr;
            }
            else
            {
                // Error
            }
        }
        delete m_pedges_list;
        m_pedges_list = nullptr;
    }

}

void Mesh3D::ClearFaces()
{
    if(m_pfaces_list == nullptr)
        return;
    else
    {
        for (FACE_ITER fiter = m_pfaces_list->begin();
             fiter != m_pfaces_list->end(); fiter++)
        {
            if(*fiter != nullptr)
            {
                delete *fiter;
                *fiter = nullptr;
            }
            else
            {
                // error
            }
        }
        delete m_pfaces_list;
        m_pfaces_list = nullptr;

    }
}

Mesh3D::Mesh3D()
{
    // initalization
    m_pvertices_list = nullptr;
    m_pfaces_list = nullptr;
    m_pedges_list = nullptr;

    m_xmax = m_ymax = m_zmax = 1.0f;
    m_xmin = m_ymin = m_zmin = - 1.0f;

    m_num_components = 0;
    m_average_edge_length = 1.0f;
}

Mesh3D::~Mesh3D()
{

}

void HE_face::face_verts(std::vector<HE_vert *> &verts)
{
    verts.resize(3);
    HE_edge *pedge = m_pedge;

    for (int i=0; i<3; i++)
    {
        verts[i] = pedge->m_pvert;
        pedge = pedge->m_pnext;
    }
}

glm::vec3 HE_face::center()
{
    glm::vec3 center(0);
    HE_edge *pedge = m_pedge;

    do
    {
        center = center + pedge->m_pvert->position();
        pedge = pedge->m_pnext;
    }
    while (pedge != m_pedge);
    center = center / glm::vec3(3.0);

    return center;
}
