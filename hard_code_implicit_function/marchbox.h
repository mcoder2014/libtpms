#ifndef MARCHBOX_H
#define MARCHBOX_H

#include <vector>
#include <unordered_map>
#include <string>

#include <glm/glm.hpp>
#include <eigen3/Eigen/Dense>

#ifdef USING_SURFACEMESH
#include "SurfaceMeshModel.h"
#include "Octree.h"
#endif

#include "implicitsurface.h"


class MarchBox
{
public:
    MarchBox();

    static int edgeTable[256];      // edge table
    static int triTable[256][16];   // triangle table

    // dimension of the marching box
    int m_ncellsX;
    int m_ncellsY;
    int m_ncellsZ;

    // the range of the marching box
    Eigen::AlignedBox3d m_boundingbox_physical;
    Eigen::AlignedBox3d m_boundingbox_logical;

    void setRange(Eigen::Vector3d physical_max,
                  Eigen::Vector3d physical_min,
                  int sampleSize,
                  int density);

    // The triangle mesh of the result of marching box algorithm
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::ivec3> m_faces;      // Index format(in vertices)

    void marching_cubes(ImplicitSurface& implicit_surface);

#ifdef USING_SURFACEMESH

    void marching_cubes(ImplicitSurface& implicit_surface,
                        SurfaceMesh::SurfaceMeshModel& surface_mesh,
                        int sampleSize = -1,
                        int density = -1);
#endif

    inline void setSampleSize(int sample){if(sample > 0) this->m_sampleSize = sample;}
    inline void setDensity(int density){if(density > 0) this->m_density = density;}
    void writeOBJ(std::string const & fileName);

private:
    std::unordered_map<int64_t, int> m_index_map;
    int getVertexIdx(int cx_id, int cy_id, int cz_id, int edge_idx,
                     std::vector<std::vector<std::vector<glm::vec3>>> &sample_points);
    int m_sampleSize;
    int m_density;
};

#endif // MARCHBOX_H
