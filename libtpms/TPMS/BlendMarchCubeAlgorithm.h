#ifndef BLENDMARCHCUBEALGORITHM_H
#define BLENDMARCHCUBEALGORITHM_H

#include <vector>
#include <set>
#include <functional>

#include <Eigen/Dense>

#include "Mesh/Mesh"
#include "SamplePoint.h"
#include "SamplePointGroup.h"

using std::vector;
using std::function;

using Eigen::Vector3i;
using Eigen::Vector3d;
using Eigen::AlignedBox3d;

using ImplicitFunciton::SamplePointGroup;

// 重命名采样点处理回调函数为 SamplePointFilter
using SamplePointFilter = function<void(vector<vector<vector<SamplePoint>>>&)>;

// 重命名采样点区域划分回调函数
using SamplePointGroupFilter = function<vector<SamplePointGroup>(vector<vector<vector<SamplePoint>>>&)>;

// 重命名重建 Mesh 的回调函数
using MarchCubeFilter = function<Mesh(vector<vector<vector<SamplePoint>>>&)>;

/**
 * @brief The BlendMarchCubeAlgorithm class
 * 混合多种 Implicit Function 的 Marching Cube 的方法
 * 需要解决问题：
 * 1. 采样点构建
 * 2. 混合公式
 */
class BlendMarchCubeAlgorithm
{
public:

    // 执行处理算法
    Mesh process();

    // 初始化 SampleMatrix 交给 SamplePointFilter 进行处理
    void initSampleMatrix();

    // 初始化 SamplePointGroups 交给 SamplePointGroupFilter 进行处理
    void initSamplePointGroup();

    // 计算隐函数值 自行处理
    void calculateImplicitValue();

    // 清除临时数据
    void clear();

    // March Cube 方法拟合网格模型
    Mesh marchMesh();

    // March Mesh 的中间函数，用来添加拟合面片
    void addFaces(int *faces, Vector3i index, Mesh& mesh);

    // March Mesh 的中间函数，用来添加拟合顶点
    Mesh::VertexHandle getVertexHandle(Vector3i index, int edgeIndex, Mesh &mesh);

public:
    /// getter setter
    // physicalBoundingBox
    inline AlignedBox3d getPhysicalBoundingBox(){return physicalBoundingBox;}

    inline void setPhysicalBoundingBox(AlignedBox3d physicalBoundingBox) {
        this->physicalBoundingBox = physicalBoundingBox;
    }

    // logicalBoundingBox
    inline AlignedBox3d getLogicalBoundingBox(){return logicalBoundingBox;}

    inline void setLogicalBoundingBox(AlignedBox3d logicalBoundingBox) {
        this->logicalBoundingBox = logicalBoundingBox;
    }

    // samplePointFilters
    inline vector<SamplePointFilter>& getSamplePointFilters(){
        return samplePointFilters;
    }

    inline void setSamplePointFilters(const vector<SamplePointFilter>& filters) {
        this->samplePointFilters = filters;
    }

    inline void pushBackSamplePointFilter(const SamplePointFilter& filter) {
        this->samplePointFilters.push_back(filter);
    }

    // samplePointGroupFilter
    inline SamplePointGroupFilter &getSamplePointGroupFilter(){
        return samplePointGroupFilter;
    }

    inline void setSamplePointGroupFilter(const SamplePointGroupFilter &filters) {
        this->samplePointGroupFilter = filters;
    }

    /// 中间变量
    inline Vector3i getSampleMatrixSize() {
        return sampleMatrixSize;
    }

    inline vector<vector<vector<SamplePoint> > >& getSampleMatrix() {
        return sampleMatrix;
    }

    inline vector<SamplePointGroup> & getSamplePointGroups() {
        return samplePointGroups;
    }

private:

    /// 算法最基本数据
    // 物理包围盒用来初始化采样点的物理范围
    AlignedBox3d physicalBoundingBox;

    // 逻辑包围盒用来初始采样点的逻辑范围
    AlignedBox3d logicalBoundingBox;

    // 依赖翻转，调整 SamplePoints 物理坐标或者逻辑坐标
    vector<SamplePointFilter> samplePointFilters;

    // 依赖翻转，用来初始化采样点矩阵
    SamplePointGroupFilter samplePointGroupFilter;

    // 依赖翻转
    MarchCubeFilter marchCubeFilter;

    // 作为网格位置的水平值
    double isoLevel;

    /// 算法运行中生成的中间数据
    // 算法的采样点矩阵大小
    Vector3i sampleMatrixSize;

    // 算法的采样点
    vector<vector<vector<SamplePoint> > > sampleMatrix;

    // 将采样点划分为不同的组，每个组的采样点共用隐式函数组
    // NOTE:划分交由其他算法负责
    vector<SamplePointGroup> samplePointGroups;

    // March Cube 方法所需要的顶点去重用途
    std::unordered_map<std::string, int> vertexIdMap;

};

#endif // BLENDMARCHCUBEALGORITHM_H
