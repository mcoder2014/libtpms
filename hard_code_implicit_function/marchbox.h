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
                  int sampleSize = -1,
                  int density = -1,
                  Eigen::Vector3d offset = Eigen::Vector3d(0,0,0));

    // The triangle mesh of the result of marching box algorithm
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::ivec3> m_faces;      // Index format(in vertices)

    void marching_cubes(ImplicitSurface& implicit_surface,
                        float isoLevel = 0);
    void marching_cubes_closed(ImplicitSurface& implicit_surface,
                               float isoLevel = 0);
    void marching_cubes_double_closed(ImplicitSurface& implicit_surface,
                                      float isoLevel_1,
                                      float isoLevel_2);

#ifdef USING_SURFACEMESH
    void marching_cubes_closed(ImplicitSurface& implicit_surface,
                        SurfaceMesh::SurfaceMeshModel& surface_mesh,
                        int sampleSize = -1,
                        int density = -1,
                        int isoLevel = 0);
#endif

    inline void setSampleSize(int sample){if(sample > 0) this->m_sampleSize = sample; this->updateRange();}
    inline void setDensity(int density){if(density > 0) this->m_density = density; this->updateRange();}
    inline void setOffset(Eigen::Vector3d offset){ this->m_offset = offset; this->updateRange();}
    inline void setReverse(bool reverse){m_reverse = reverse;}
    void updateRange();     // Update range according to the physical boundary and parameters.
    void writeOBJ(std::string const & fileName);

private:
    std::unordered_map<int64_t, int> m_index_map;
    int getVertexIdx(int cx_id, int cy_id, int cz_id, int edge_idx,
                     std::vector<std::vector<std::vector<glm::vec3>>> &sample_points);
    int m_sampleSize;           // sample size of single period
    int m_density;              // The ratio of the logical cycle size to the physical size
    Eigen::Vector3d m_offset;   // logical min point's position
    bool m_reverse;             // Select the opposite space to be the entity
};

#endif // MARCHBOX_H
