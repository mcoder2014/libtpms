#ifndef MESH3D_H
#define MESH3D_H

#include <vector>
#include <map>

#include <glm/glm.hpp>

// Forward declarations of mesh classes
class HE_vert;
class HE_edge;
class HE_face;

// an enum type
enum SelectTag
{
    UNSELECTED = 0,
    SELECTED,
    OTHER
};

enum BoundaryTag
{
    BOUNDARY,
    INNER,
    TO_SPLIT
};

///
/// \brief The HE_vert class
/// The basic vertex class for half-edge structure
///
class HE_vert
{
public:
    int m_id;               // id
    glm::vec3 m_position;   // vertex position
    glm::vec3 m_normal;     // vertex normal
    glm::vec3 m_texCoord;   // vertex coord
    glm::vec4 m_color;
    HE_edge * m_pedge;      // one of the half-edge list emanating from the vertex
    int m_degree;
    BoundaryTag m_boundary_tag; // boundary tag
    SelectTag m_selected;         // a tag: whether the vertex is selected

    std::vector<size_t> m_neighbourIdx;

public:
    HE_vert(const glm::vec3& v)
        :m_id(-1),m_position(v), m_pedge(nullptr), m_degree(0),
          m_boundary_tag(INNER), m_selected(UNSELECTED),
          m_color(255.0/255.0, 215.0/255.0, 0.0/255.0, 1.0),
          m_neighbourIdx()
    {}

    ~HE_vert(){m_neighbourIdx.clear();}

    bool    isOnBoundary(){return m_boundary_tag == BOUNDARY;}
    int     id(){return m_id;}
    glm::vec3& normal(){return m_normal;}
    glm::vec3& position(){return m_position;}
    glm::vec3& texCoordinate(){return m_texCoord;}
    glm::vec4& color(){return m_color;}
    int     degree(){return m_degree;}
    SelectTag selected(){return m_selected;}
    BoundaryTag boundary_tag(){return m_boundary_tag;}

    void set_normal(const glm::vec3& n){m_normal = n;}
    void set_position(const glm::vec3& pos){m_position = pos;}
    void set_id(int id){m_id = id;}
    void set_color(const glm::vec4& color){m_color = color;}
    void set_selected(SelectTag tag){m_selected = tag;}
    void set_boundary_tag(BoundaryTag tag){m_boundary_tag=tag;}
};


///
/// \brief The HE_edge class
/// The basic edge class for half-edge structure
///
class HE_edge
{
public:
    int m_id;
    HE_vert *m_pvert;   // vertex at the end of this half-edge
    HE_edge *m_ppair;   // oppositely oriented adjacent half-edge
    HE_face *m_pface;   // face the half-edge borders
    HE_edge *m_pnext;   // next half-edge around the face
    HE_edge *m_pprev;   // prev half-edge around the face
    glm::vec3 m_texCoord;   // texture coordinate of the end vertex
    BoundaryTag m_boundary_tag; // boundary tag

public:
    HE_edge()
        :m_id(-1), m_pvert(nullptr), m_ppair(nullptr),
          m_pface(nullptr), m_pnext(nullptr), m_pprev(nullptr),
          m_boundary_tag(INNER){}
    ~HE_edge(){}

    int id(){return m_id;}
    bool isBoundary(){return m_boundary_tag == BOUNDARY;}
    BoundaryTag boundaryTag(){return m_boundary_tag;}
    void set_boundary_tag(BoundaryTag tag){m_boundary_tag = tag;}
};

class HE_face
{
public:
    int m_id;
    HE_edge *m_pedge;           // one of the half-edge list bordering the face
    glm::vec3 m_normal;         // face normal
    int m_valence;              // the number of edge list
    SelectTag m_selected;       // a tag, whether the face is selected
    glm::vec4 m_color;          // the color of the face
    BoundaryTag m_boundary_tag; // this flag is used to split the mesh

public:
    HE_face()
        :m_id(-1), m_pedge(nullptr), m_valence(0), m_selected(UNSELECTED),
          m_boundary_tag(INNER) {}
    ~HE_face(){}

    int id(){return m_id;}
    int valence(){return m_valence;}
    glm::vec3& normal(){return m_normal;}
    SelectTag selected(void){return m_selected;}
    glm::vec4& color(){return m_color;}
    BoundaryTag boundaryTag(){return m_boundary_tag;}

    void set_selected(SelectTag tag){m_selected = tag;}
    void set_color(const glm::vec4& color){m_color = color;}
    void set_boundary_tag(BoundaryTag tag){m_boundary_tag = tag;}

    void face_verts(std::vector<HE_vert *> & verts);
    glm::vec3 center();

};


///
/// \brief The Mesh3D class
///
class Mesh3D
{
    using VERTEX_ITER = std::vector<HE_vert *>::iterator;
    using FACE_ITER = std::vector<HE_face *>::iterator;
    using EDGE_ITER = std::vector<HE_edge *>::iterator;

    using VERTEX_RITER = std::vector<HE_vert *>::reverse_iterator;
    using FACE_RITER = std::vector<HE_face *>::reverse_iterator;
    using EDGE_RITER = std::vector<HE_edge *>::reverse_iterator;

    using PAIR_VERTEX = std::pair<HE_vert *, HE_vert *>;

private:
    // mesh data
    std::vector<HE_vert *> * m_pvertices_list;  // store vertex
    std::vector<HE_edge *> * m_pedges_list;     // store edges
    std::vector<HE_face *> * m_pfaces_list;     // store faces

    // mesh info
    int m_num_components;   // number of components
    float m_average_edge_length;    // the average edge length

    // associate 2 end vertex with its edge: only using in creating mesh
    std::map<std::pair<HE_vert*, HE_vert*>, HE_edge *> m_edgemap;

    // value for bounding box
    float m_xmax, m_xmin, m_ymax, m_ymin, m_zmax, m_zmin;

public:
    Mesh3D();
    ~Mesh3D();

    std::vector<HE_vert *> *pvertices_list();
    std::vector<HE_edge *> *pedges_list();
    std::vector<HE_face *> *pfaces_list();

    int num_of_vertex_list();
    int num_of_half_edge_list();
    int num_of_edge_list();
    int num_of_face_list();
    int num_of_components(){return m_num_components;}
    float average_edge_length(){return m_average_edge_length;}

    HE_vert *get_vertex(int id);
    HE_edge *get_half_edge(int id);
    HE_face *get_face(int id);

    HE_edge *get_edge(HE_vert* hv0, HE_vert* hv1);

    bool isValid();

    // Insert a vertex
    HE_vert* InsertVertex(const glm::vec3& v);

    // Insert an edge
    HE_edge* InsertEdge(HE_vert* vstart, HE_vert* vend);

    // Insert a face
    HE_face* InsertFace(std::vector<HE_vert* >& vec_hv);

    // File IO
    bool LoadFromOBJFile(const char* fins);
    void WriteToOBJFile(const char* fouts);

    void UpdateMesh();      // update mesh
    void UpdateNormal();    // update normal

    void ComputeBoundingBox();  // compute the bounding box

public:
    // clear all the data
    void ClearData(void);
private:
    // clear vertex
    void ClearVertex(void);
    // clear edges
    void ClearEdges(void);
    // clear faces
    void ClearFaces(void);
};

#endif // MESH3D_H
