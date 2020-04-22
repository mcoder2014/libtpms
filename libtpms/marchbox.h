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
#include "mesh.h"

class MarchBox
{
public:
    MarchBox();
    ~MarchBox();

    // 清空元素，释放内存空间
    inline void clear(){
        this->m_vertices.clear();
        std::vector<glm::vec3> tmp_v;
        this->m_vertices.swap(tmp_v);
        this->m_faces.clear();
        std::vector<glm::ivec3> tmp_f;
        this->m_faces.swap(tmp_f);
    }

    // 方形、只采样一层曲面
    void marching_cubes(
            ImplicitSurface& implicit_surface,
            float isoLevel = 0);

    // 方形、封闭孔洞的采样方法
    void marching_cubes_closed(
            ImplicitSurface& implicit_surface,
            float isoLevel = 0);

    // 方形、有厚度曲面的封闭方式 IsoLevel1 < IsoLevel2
    void marching_cubes_double_closed(
            ImplicitSurface& implicit_surface,
            float isoLevel_low = -0.5,
            float isoLevel_high = 0.5);

    Mesh* mc_cube_double_surface(
            ImplicitSurface& implicit_surface,
            float isoLevel = 0,
            float thickness = 0.2);

    /// 当使用 Surfacemesh 作为生成八叉树的数据结构时有效
#ifdef USING_SURFACEMESH
    // 不做压缩、直接截取鞋垫形状、封闭孔洞的方式
    void marching_cubes_closed(
            ImplicitSurface& implicit_surface,
            SurfaceMesh::SurfaceMeshModel& surface_mesh,
            float isoLevel = 0);

    // 不做压缩、直接截取鞋垫形状、有厚度曲面的方式
    void marching_cubes_double_closed(
            ImplicitSurface& implicit_surface,
            SurfaceMesh::SurfaceMeshModel& surface_mesh,
            float isoLevel_low = -0.5,
            float isoLevel_high = 0.5);

    // 对鞋垫区域做Z轴方向压缩、封闭孔洞方式
    void marching_cube_push_closed(
            ImplicitSurface& implicit_surface,
            SurfaceMesh::SurfaceMeshModel& surface_mesh,
            float isoLevel = 0);

    // 对鞋垫区域做Z轴方向压缩、有厚度曲面的方式
    void marching_cube_push_double_closed(
            ImplicitSurface& implicit_surface,
            SurfaceMesh::SurfaceMeshModel& surface_mesh,
            float isoLevel_low = -0.5,
            float isoLevel_high = 0.5);

    // 测试生成压缩的单层 曲面 Z轴方向压缩的单层网络，不封闭
    void mb_push(
            ImplicitSurface& implicit_surface,
            SurfaceMesh::SurfaceMeshModel& boundary);

    // 测试非均匀的拟合效果
    void mb_pc_diff_test(
            ImplicitSurface& implicit_surface,
            SurfaceMesh::SurfaceMeshModel& surface_mesh,
            float isoLevel_from = 0,
            float isoLevel_to = 0);

    void mb_pd_diff_test(
            ImplicitSurface& implicit_surface,
            SurfaceMesh::SurfaceMeshModel& surface_mesh,
            glm::vec2 isoLevel_low,
            glm::vec2 isoLevel_high);

    // 测试Y轴多种晶格过渡
    void mb_pc_mult_test(
            std::vector<std::string> surfaces,
            SurfaceMesh::SurfaceMeshModel& surface_mesh,
            float isoLevel_from = 0,
            float isoLevel_to = 0);

    // 利用 surfacemesh 模型设置物理模型范围
    void setRange(SurfaceMesh::SurfaceMeshModel& surface_mesh);

    // 初始化采样矩阵，对鞋垫范围做Z轴方向压缩
    void initSampleMatrix_compress_z(
            Octree& octree,
            std::vector<std::vector<std::vector<glm::vec3>>> &sample_points,
            int additions = 4);
    void initSampleMatrix_compress_z_and_cal_IS_value(
            Octree& octree,
            ImplicitSurface& implicit_surface,
            std::vector<std::vector<std::vector<glm::vec3>>> &sample_points,
            std::vector<std::vector<std::vector<float>>>& IS_value,
            int additions = 4);
    // 初始化采样矩阵，对鞋垫下表面以上的点做Z轴方向压缩
    void initSampleMatrix_compress_z_without_lower(
            Octree& octree,
            std::vector<std::vector<std::vector<glm::vec3>>> &sample_points,
            int additions = 4);

    // Calculate IS_value
    void calculateIS_value(
            ImplicitSurface& implicit_surface,
            Octree& octree,
            std::vector<std::vector<std::vector<glm::vec3>>> &sample_points,
            std::vector<std::vector<std::vector<float>>>& IS_value,
            int additions);

    /// Y axis direction
    void calculateIS_value(
            std::vector<std::vector<std::pair<std::string, float>>>& implicit_surfaces,
            Octree& octree,
            std::vector<std::vector<std::vector<glm::vec3>>> &sample_points,
            std::vector<std::vector<std::vector<float>>>& IS_value,
            int additions);

    // 拟合根据足底压力的非均匀极小曲面
    void createMesh(
            std::vector<std::vector<std::vector<glm::vec3>>> &sample_points,
            std::vector<std::vector<std::vector<float>>>& IS_value,
            std::vector<std::vector<float>>& isoLevel);

    void createMesh(
            std::vector<std::vector<std::vector<glm::vec3>>> &sample_points,
            std::vector<std::vector<std::vector<float>>>& IS_value,
            std::vector<std::vector<glm::vec2>>& isoLevel);

#endif

    void setRange(
            Eigen::Vector3d physical_max,
            Eigen::Vector3d physical_min,
            int sampleSize = -1,
            int density = -1);
    void setRange(Eigen::AlignedBox3d bbox);

    void setRange(
            Eigen::Vector3d physical_max,
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
        if(density > 0) { m_densityXYZ = Eigen::Vector3i(density,density,density); } this->updateRange();}
    inline void setDensity(Eigen::Vector3i density){
        if(density[0]>0 && density[1]>0&&density[2] > 0){m_densityXYZ=density; }this->updateRange();}

    // 极小曲面起始的点坐标
    inline void setOffset(Eigen::Vector3d offset){ this->m_offset = offset; this->updateRange();}
    inline void setReverse(bool reverse){m_reverse = reverse;}
    void updateRange();     // Update range according to the physical boundary and parameters.
    void writeOBJ(std::string const & fileName);

# ifdef DEBUG_INFO
    void writeOBJ(std::vector<std::vector<std::vector<glm::vec3>>>& samplePoints,
                  std::string const & fileName);
#endif

    // Create sample Points
    void initSampleMatrix(
            std::vector<std::vector<std::vector<glm::vec3>>> &sample_points,
            int additions = 4);

    // Calculate IS_value
    void calculateIS_value(
            ImplicitSurface& implicit_surface,
            std::vector<std::vector<std::vector<float>>>& IS_value,
            int additions);

    // 由 IS_value 和 sample_points 创建网格模型
    void createMesh(
            std::vector<std::vector<std::vector<glm::vec3>>> &sample_points,
            std::vector<std::vector<std::vector<float>>>& IS_value,
            float isoLevel);

    // 提取有厚度的曲面
    void createMesh(
            std::vector<std::vector<std::vector<glm::vec3>>> &sample_points,
            std::vector<std::vector<std::vector<float>>>& IS_value,
            float isoLevel_low, float isoLevel_high);

    void createMesh(
            std::vector<std::vector<std::vector<glm::vec3>>> &sample_points,
            std::vector<std::vector<std::vector<float>>>& IS_value,
            glm::vec2 isoLevel);

    // 获得指定采样方格边的中点的 index
    int getVertexIdx(
            int cx_id, int cy_id, int cz_id, int edge_idx,
            std::vector<std::vector<std::vector<glm::vec3>>> &sample_points);

private:
    // 数据区域
    static int edgeTable[256];      // edge table
    static int triTable[256][16];   // triangle table

    // dimension of the marching box
    int m_ncellsX;
    int m_ncellsY;
    int m_ncellsZ;

    // the range of the marching box
    Eigen::AlignedBox3d m_boundingbox_physical;
    Eigen::AlignedBox3d m_boundingbox_logical;

    std::unordered_map<int64_t, int> m_index_map;   // 用来避免重复添加同一个顶点

    Eigen::Vector3i m_densityXYZ;       // 在 XYZ 三个方向上的逻辑密度
    Eigen::Vector3i m_sampleSizeXYZ;    // 在 XYZ 三个方向上的采样大小
    Eigen::Vector3d m_offset;   // logical min point's position
    bool m_reverse;             // Select the opposite space to be the entity

public:
    // The triangle mesh of the result of marching box algorithm
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::ivec3> m_faces;      // Index format(in vertices)
};

#endif // MARCHBOX_H
