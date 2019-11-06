#ifndef MARCHBOX_H
#define MARCHBOX_H

#include <vector>
#include <unordered_map>
#include <string>

#include <glm/glm.hpp>
#include <eigen3/Eigen/Dense>

#include "implicitsurface.h"


class MarchBox
{
public:

    static int edgeTable[256];      // edge table
    static int triTable[256][16];   // triangle table

    // dimension of the marching box
    int m_ncellsX;
    int m_ncellsY;
    int m_ncellsZ;

    // the range of the marching box
    float m_mcMinX;
    float m_mcMaxX;
    float m_mcMinY;
    float m_mcMaxY;
    float m_mcMinZ;
    float m_mcMaxZ;

    Eigen::AlignedBox3d m_boundingbox_physical;
    Eigen::AlignedBox3d m_boundingbox_logical;

    // The triangle mesh of the result of marching box algorithm
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::ivec3> m_faces;      // Index format(in vertices)

    MarchBox();

    void marching_cubes(ImplicitSurface& implicit_surface);
    void writeOBJ(std::string const & fileName);

private:
    std::unordered_map<int64_t, int> m_index_map;
    int getVertexIdx(int cx_id, int cy_id, int cz_id, int edge_idx,
                     std::vector<std::vector<std::vector<glm::vec3>>> &sample_points);
};

#endif // MARCHBOX_H
