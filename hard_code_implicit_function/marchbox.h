#ifndef MARCHBOX_H
#define MARCHBOX_H

#include <vector>

#include <glm/glm.hpp>

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

    // The triangle mesh of the result of marching box algorithm
    std::vector<glm::vec3> vertices;
    std::vector<glm::ivec3> faces;      // Index format(in vertices)

    MarchBox();

    void marching_cubes(ImplicitSurface& implicit_surface);

};

#endif // MARCHBOX_H
