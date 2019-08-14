#include "mesh3d.h"

#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <limits>
#include <algorithm>
#include <cmath>

#define SWAP(a,b,T) {T tmp=(a); (a)=(b); (b)=tmp;}

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
        Unify(2.0f);

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

///
/// \brief Mesh3D::get_face
/// \param vId0
/// \param vId1
/// \param vId2
/// \return If not exists will return false
///
HE_face *Mesh3D::get_face(int vId0, int vId1, int vId2)
{
    // Get 3 vertexes
    HE_vert *v0 = get_vertex(vId0);
    HE_vert *v1 = get_vertex(vId1);
    HE_vert *v2 = get_vertex(vId2);

    if(!v0 || !v1 ||!v2)
        return nullptr;

    HE_face *face = nullptr;

    // To Find the vertex not on the boundary
    if(v0->isOnBoundary())
    {
        if(!v1->isOnBoundary())
        {
            SWAP(v0, v1, HE_vert*);
        }
        else if(!v2->isOnBoundary())
        {
            SWAP(v0, v2, HE_vert*);
        }
        else
        {
            // v0 v1 v2 all on the boundary
            return nullptr;
        }
    }

    if(!v0->isOnBoundary())
    {
        HE_edge *edge = v0->m_pedge;

        bool inFace = true;
        do
        {
            bool b1 = isFaceContainVertex(edge->m_pface, v1);
            bool b2 = isFaceContainVertex(edge->m_pface, v2);
            if(!b1 && !b2)
            {
                edge = edge->m_ppair->m_pnext;
            }
            else if (b1 && b2)
            {
                face = edge->m_pface;
                break;
            }
            else
            {
                inFace = false;
                break;
            }
        }while(edge != v0->m_pedge && edge != nullptr);
    }

    return face;
}


HE_face *Mesh3D::get_face(const std::vector<unsigned int> &ids)
{
    if( ids.size() < 3)
    {
        std::cout << "Too few search parameter for function get_face:"
                  << ids.size()
                  << std::endl;
        return nullptr;
    }

    // First, find a non-boundary vertex
    HE_vert* v = nullptr;
    for (unsigned int i = 0; i < ids.size(); i++)
    {
        if(!get_vertex(ids[i])->isOnBoundary())
        {
            v = get_vertex(ids[i]);
            break;
        }
    }

    if(!v)
    {
        // All points are on the boundary
        // Not to process
        return nullptr;
    }

    HE_edge *edge = v->m_pedge;
    HE_face *face = nullptr;

    do
    {
        face = edge->m_pface;
        edge = edge->m_ppair->m_pnext;

        bool bInFace = isFaceContainVertex(face, get_vertex(ids[0]));
        if(!bInFace)
            continue;
        for (unsigned int i = 1; i < ids.size(); i++)
        {
            bool b = isFaceContainVertex(face, get_vertex(ids[i]));
            if( b != bInFace )
            {
                bInFace = false;
                break;
            }
        }
        if (bInFace)
        {
            return face;
        }
    }while( edge != v->m_pedge && edge != nullptr);

    return nullptr;
}

int Mesh3D::GetFaceId(HE_face *face)
{
    return !face ? -1 : face->id();
}

void Mesh3D::ResetVertexSelectedTags(SelectTag tag)
{
    for (int i = 0; i < num_of_vertex_list(); i++)
    {
        get_vertex(i)->set_selected(tag);
    }
}

void Mesh3D::ResetFaceSelectedTags(SelectTag tag)
{
    for (int i = 0; i < num_of_face_list(); i++)
    {
        get_face(i)->set_selected(tag);
    }
}

bool Mesh3D::isNeighbours(HE_vert *v0, HE_vert *v1)
{
    if(!v0 || !v1)
        return false;
    HE_edge * edge = v0->m_pedge;
    do
    {
        if(edge->m_pvert == v1)
            return true;
        edge = edge->m_ppair->m_pnext;
    }while(edge != v0->m_pedge && edge);

    return false;
}

int Mesh3D::GetSelectedVrtId()
{
    if (!isValid())
    {
        return -1;
    }
    for (int i=0; i<num_of_vertex_list(); i++)
    {
        if (get_vertex(i)->selected()==SELECTED)
        {
            return i;
        }
    }
    return -1;
}

///
/// \brief Mesh3D::CreateMesh
/// \param verts
/// \param triIdx
///
void Mesh3D::CreateMesh(const std::vector<glm::vec3> &verts, const std::vector<int> &triIdx)
{
    ClearData();
    for (unsigned int i = 0; i < verts.size(); i++)
    {
        InsertVertex(verts[i]);
    }

    for(unsigned int i = 0; i < triIdx.size(); i = i+3)
    {
        std::vector<HE_vert* > tri;
        HE_vert *v0 = get_vertex(triIdx[i]);
        HE_vert *v1 = get_vertex(triIdx[i+1]);
        HE_vert *v2 = get_vertex(triIdx[i+2]);

        if(!v0 || !v1 || !v2)
            continue;
        tri.push_back(v0);
        tri.push_back(v1);
        tri.push_back(v2);
        InsertFace(tri);
    }

    UpdateMesh();
}

///
/// \brief Mesh3D::CreateMesh
/// \param verts
/// \param triIdx
///
void Mesh3D::CreateMesh(const std::vector<double> &verts, const std::vector<unsigned> &triIdx)
{
    ClearData();
    for (unsigned int i = 0; i < verts.size(); i = i+3)
    {
        InsertVertex(glm::vec3(verts[i], verts[i+1], verts[i+2]));
    }

    for (unsigned int i = 0; i < triIdx.size(); i = i+3)
    {
        std::vector<HE_vert *> tri;
        HE_vert *v0 = get_vertex(triIdx[i]);
        HE_vert *v1 = get_vertex(triIdx[i+1]);
        HE_vert *v2 = get_vertex(triIdx[i+2]);

        if(!v0 || !v1 || !v2)
            continue;

        tri.push_back(v0);
        tri.push_back(v1);
        tri.push_back(v2);

        InsertFace(tri);
    }

    UpdateMesh();
}

int Mesh3D::GetBoundaryVrtSize()
{
    int count = 0;
    for (int i = 0; i < num_of_vertex_list(); i++)
    {
        if(get_vertex(i)->isOnBoundary())
            count ++;
    }

    return count;
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

void Mesh3D::LinearTex()
{
    for (size_t i=0; i != num_of_vertex_list(); i++)
    {
        m_pvertices_list->at(i)->m_texCoord = m_pvertices_list->at(i)->m_position;
    }
}

void Mesh3D::SphereTex()
{
    static const float pi = 3.1415926;
    for (size_t i=0; i != num_of_vertex_list(); i++)
    {
        HE_vert * vert = m_pvertices_list->at(i);
        glm::vec3 p = glm::vec3(vert->m_position.length()) * vert->m_normal;

        float r = sqrt(p[1] * p[1] + p[0] * p[0]);

        vert->m_texCoord[0] = asin(p[2])/pi + 0.5;
        vert->m_texCoord[1] = acos(p[0])/(2*pi);
    }
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

///
/// \brief Mesh3D::ComputeFaceslistNormal
///
void Mesh3D::ComputeFaceslistNormal()
{
    for (FACE_ITER fiter = m_pfaces_list->begin(); fiter != m_pfaces_list->end(); fiter ++)
    {
        ComputePerFaceNormal(*fiter);
    }
}

///
/// \brief Mesh3D::ComputePerFaceNormal
/// \param hf
///
void Mesh3D::ComputePerFaceNormal(HE_face *hf)
{
    HE_edge *pedge = hf->m_pedge;
    HE_edge *nedge = hf->m_pedge->m_pnext;

    HE_vert *p = pedge->m_pvert;
    HE_vert *c = pedge->m_pnext->m_pvert;
    HE_vert *n = nedge->m_pnext->m_pvert;

    glm::vec3 pc, nc;
    pc = p->m_position - c->m_position;
    nc = n->m_position - c->m_position;

    hf->m_normal = glm::cross(nc, pc);
    glm::normalize(hf->m_normal);
}

///
/// \brief Mesh3D::ComputeVertexlistNormal
///
void Mesh3D::ComputeVertexlistNormal()
{
    for (VERTEX_ITER viter = m_pvertices_list->begin(); viter!=m_pvertices_list->end(); viter++)
    {
        ComputePerVertexNormal(*viter);
    }
}

void Mesh3D::ComputePerVertexNormal(HE_vert *hv)
{
    if (hv->m_degree < 2)
    {
        // Error: the degree of vertex is less than 2, can't calculate the degree.
        hv->m_normal = glm::vec3(1.0f, 0.0f, 0.0f);
        return;
    }

    HE_edge *edge = hv->m_pedge;
    if(edge == nullptr)
    {
        // Error: the edge attached to the vertex is nullptr
        hv->m_normal = glm::vec3(1.0f, 0.0f, 0.0f);
        return;
    }

    // init
    hv->m_normal = glm::vec3(0.0f, 0.0f, 0.0f);
    if(hv->m_boundary_tag == INNER)
    {
        // INNER vertices
        int iterNum = 0;
        do
        {
            iterNum++;
            if(iterNum > hv->degree())
            {
                std::cout << "iterNum > hv.degree: " << hv->id() << std::endl;
                break;
            }

            glm::vec3 p = edge->m_pvert->position();
            glm::vec3 q = edge->m_pnext->m_pvert->position();
            glm::vec3 r = edge->m_pprev->m_pvert->position();

            glm::vec3 n = glm::cross((q-p),(r-p));
            hv->m_normal = hv->m_normal + n;
            edge = edge->m_ppair->m_pnext;

        }while (edge != hv->m_pedge && edge != nullptr);
    }
    else
    {
        // Boundary vertices
        hv->m_normal = glm::vec3(1.0f, 0.0f, 0.0f);

    }

    glm::normalize(hv->m_normal);
}

void Mesh3D::ComputeNumComponents()
{
    // ???
}

void Mesh3D::ComputeAvarageEdgeLength()
{
    if(!isValid())
    {
        m_average_edge_length = 0.0f;
        return;
    }
    float aveEdgeLength = 0.f;

    for (int i = 0; i < num_of_half_edge_list(); i++)
    {
        HE_edge* edge = m_pedges_list->at(i);
        HE_vert* v0 = edge->m_pvert;
        HE_vert* v1 = edge->m_ppair->m_pvert;

        aveEdgeLength += (v0->m_position - v1->m_position).length();
    }
    m_average_edge_length = aveEdgeLength / num_of_half_edge_list();
}

void Mesh3D::SetBoundaryFlag()
{
    for (EDGE_ITER eiter = m_pedges_list->begin(); eiter != m_pedges_list->end(); eiter++)
    {
        if((*eiter)->m_pface == nullptr)
        {
            // This half edge doesn't refer any face
            (*eiter)->set_boundary_tag(BOUNDARY);           // this half edge is boundary
            (*eiter)->m_ppair->set_boundary_tag(BOUNDARY);  // the pair edge is boundary
            (*eiter)->m_pvert->set_boundary_tag(BOUNDARY);  // thie half edge refer vertex is boundary
            (*eiter)->m_ppair->m_pvert->set_boundary_tag(BOUNDARY); // the pair's vertex is boundary
            (*eiter)->m_ppair->m_pface->set_boundary_tag(BOUNDARY); // the pair's face is boundary
        }
    }
}


void Mesh3D::BoundaryCheck()
{
    for (VERTEX_ITER viter = m_pvertices_list->begin(); viter != m_pvertices_list->end(); viter++)
    {
        if((*viter)->isOnBoundary())
        {
            HE_edge* edge = (*viter)->m_pedge;
            int deg = 0;
            while (edge->m_pface != nullptr && deg < (*viter)->degree())
            {
                edge = edge->m_pprev->m_ppair;
                deg ++;
            }

            (*viter)->m_pedge = edge;
        }
    }
}


void Mesh3D::Unify(float size)
{
    float scaleX = m_xmax - m_xmin;
    float scaleY = m_ymax - m_ymin;
    float scaleZ = m_zmax - m_zmin;
    float scaleMax;

    if(scaleX < scaleY)
        scaleMax = scaleY;
    else
        scaleMax = scaleX;

    if(scaleMax < scaleZ)
        scaleMax = scaleZ;

    float scaleV = size / scaleMax;
    glm::vec3 centerPos((m_xmin + m_xmax) / 2.0f, (m_ymin + m_ymax) / 2.0f, (m_zmin + m_zmax) / 2.0f);

    for (size_t i = 0; i != m_pvertices_list->size(); i++)
    {
        m_pvertices_list->at(i)->m_position =
                (m_pvertices_list->at(i)->m_position - centerPos) * glm::vec3(scaleV);
    }

}

///
/// \brief Mesh3D::isFaceContainVertex
/// \param face
/// \param vert
/// \return
///
bool Mesh3D::isFaceContainVertex(HE_face *face, HE_vert *vert)
{
    HE_edge * edge = face->m_pedge;

    do
    {
        if(edge->m_pvert == vert)
            return true;
        edge = edge->m_pnext;
    }while(edge != face->m_pedge && edge != nullptr);

    return false;
}

void Mesh3D::get_neighbourID(const size_t &vertid, std::vector<size_t> &neighbours)
{
    HE_edge* edge = get_vertex(vertid)->m_pedge;
    neighbours.clear();
    do
    {
        neighbours.push_back(edge->m_pvert->id());
        edge = edge->m_ppair->m_pnext;
    }while(edge != get_vertex(vertid)->m_pedge && edge != nullptr);

    std::vector<size_t> tmp = neighbours;
    for(size_t i = 0; i != tmp.size(); i++)
    {
        neighbours[i] = tmp[tmp.size() - i - 1];
    }
}

void Mesh3D::SetNeighbours()
{
    for(size_t i = 0; i != num_of_vertex_list(); i++)
    {
        get_neighbourID(i, m_pvertices_list->at(i)->m_neighbourIdx);
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
