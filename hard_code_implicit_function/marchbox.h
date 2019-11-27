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
    ~MarchBox();

    static int edgeTable[256];      // edge table
    static int triTable[256][16];   // triangle table

    // dimension of the marching box
    int m_ncellsX;
    int m_ncellsY;
    int m_ncellsZ;

    // the range of the marching box
    Eigen::AlignedBox3d m_boundingbox_physical;
    Eigen::AlignedBox3d m_boundingbox_logical;

    // 清空元素，释放内存空间
    inline void clear(){this->m_vertices.clear();
                        std::vector<glm::vec3> tmp_v;
                        this->m_vertices.swap(tmp_v);
                        this->m_faces.clear();
                        std::vector<glm::ivec3> tmp_f;
                        this->m_faces.swap(tmp_f);}

    // The triangle mesh of the result of marching box algorithm
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::ivec3> m_faces;      // Index format(in vertices)

    void marching_cubes(ImplicitSurface& implicit_surface,
                        float isoLevel = 0);
    void marching_cubes_closed(ImplicitSurface& implicit_surface,
                               float isoLevel = 0);
    // IsoLevel1 < IsoLevel2
    void marching_cubes_double_closed(ImplicitSurface& implicit_surface,
                                      float isoLevel_low = -0.5,
                                      float isoLevel_high = 0.5);

#ifdef USING_SURFACEMESH
    void marching_cubes_closed(ImplicitSurface& implicit_surface,
                        SurfaceMesh::SurfaceMeshModel& surface_mesh,
                        int sampleSize = -1,
                        int density = -1,
                        float isoLevel = 0);

    void marching_cubes_double_closed(ImplicitSurface& implicit_surface,
                        SurfaceMesh::SurfaceMeshModel& surface_mesh,
                        int sampleSize = -1,
                        int density = -1,
                        float isoLevel_low = -0.5,
                        float isoLevel_high = 0.5);

    void marching_cube_push_closed(ImplicitSurface& implicit_surface,
                                   SurfaceMesh::SurfaceMeshModel& surface_mesh,
                                   int sampleSize = -1,
                                   int density = -1,
                                   float isoLevel = 0);

    void marching_cube_push_double_closed(ImplicitSurface& implicit_surface,
                                          SurfaceMesh::SurfaceMeshModel& surface_mesh,
                                          float isoLevel_low = -0.5,
                                          float isoLevel_high = 0.5);

#endif

    void setRange(Eigen::Vector3d physical_max,
                  Eigen::Vector3d physical_min,
                  int sampleSize = -1,
                  int density = -1);

    void setRange(Eigen::Vector3d physical_max,
                  Eigen::Vector3d physical_min,
                  Eigen::Vector3i sampleSize,
                  Eigen::Vector3i density);

    // 采样网格的尺寸（单个极小曲面周期内）
    inline void setSampleSize(int sample){
        if(sample > 0) {m_sampleSizeXYZ = Eigen::Vector3i(sample,sample,sample); this->updateRange();}}
    inline void setSampleSize(Eigen::Vector3i sample){
        if(sample[0] > 0 && sample [1]>0 && sample[2] > 0){m_sampleSizeXYZ = sample; this->updateRange();}}

    // 极小曲面的周期大小与物理大小的映射
    inline void setDensity(int density){
        if(density > 0) m_densityXYZ = Eigen::Vector3i(density,density,density); this->updateRange();}
    inline void setDensity(Eigen::Vector3i density){
        if(density[0]>0 && density[1]>0&&density[2] > 0){m_densityXYZ=density;this->updateRange();}}

    // 极小曲面起始的点坐标
    inline void setOffset(Eigen::Vector3d offset){ this->m_offset = offset; this->updateRange();}
    inline void setReverse(bool reverse){m_reverse = reverse;}
    void updateRange();     // Update range according to the physical boundary and parameters.
    void writeOBJ(std::string const & fileName);

# ifdef DEBUG_INFO
    void writeOBJ(std::vector<std::vector<std::vector<glm::vec3>>>& samplePoints,
                  std::string const & fileName);
#endif

private:
    // Create sample Points
    void initSampleMatrix(std::vector<std::vector<std::vector<glm::vec3>>> &sample_points,
                          int additions = 4);

    // Calculate IS_value
    void calculateIS_value(std::vector<std::vector<std::vector<glm::vec3>>> &sample_points,
                           std::vector<std::vector<std::vector<float>>>& IS_value, int additions);

    // 由 IS_value 和 sample_points 创建网格模型
    void createMesh(std::vector<std::vector<std::vector<glm::vec3>>> &sample_points,
                    std::vector<std::vector<std::vector<float>>>& IS_value,
                    float isoLevel);

    void createMesh(std::vector<std::vector<std::vector<glm::vec3>>> &sample_points,
                    std::vector<std::vector<std::vector<float>>>& IS_value,
                    float isoLevel_low, float isoLevel_high);

private:
    std::unordered_map<int64_t, int> m_index_map;
    int getVertexIdx(int cx_id, int cy_id, int cz_id, int edge_idx,
                     std::vector<std::vector<std::vector<glm::vec3>>> &sample_points);
    Eigen::Vector3i m_densityXYZ;       // 在 XYZ 三个方向上的逻辑密度
    Eigen::Vector3i m_sampleSizeXYZ;    // 在 XYZ 三个方向上的采样大小
    Eigen::Vector3d m_offset;   // logical min point's position
    bool m_reverse;             // Select the opposite space to be the entity
};

#endif // MARCHBOX_H
