#ifndef BLENDMARCHCUBEALGORITHM_H
#define BLENDMARCHCUBEALGORITHM_H

#include <vector>
#include <set>
#include <functional>

#include <Eigen/Dense>

#include "Mesh/Mesh"
#include "SamplePoint.h"
#include "SamplePointGroup.h"
#include "Filter/BaseFilter.h"

using std::vector;
using std::function;

using Eigen::Vector3i;
using Eigen::Vector3d;
using Eigen::AlignedBox3d;

using ImplicitFunciton::SamplePointGroup;

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
    BlendMarchCubeAlgorithm(){}

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
    inline vector<SamplePointFilterCallback>& getSamplePointFilters(){
        return samplePointFilters;
    }

    inline void setSamplePointFilters(const vector<SamplePointFilterCallback>& filters) {
        this->samplePointFilters = filters;
    }

    inline void pushBackSamplePointFilter(const SamplePointFilterCallback& filter) {
        this->samplePointFilters.push_back(filter);
    }

    // samplePointGroupFilter
    inline SamplePointGroupFilterCallback &getSamplePointGroupFilter(){
        return samplePointGroupFilter;
    }

    inline void setSamplePointGroupFilter(const SamplePointGroupFilterCallback &filters) {
        this->samplePointGroupFilter = filters;
    }

    // MarchCubeFilter
    inline MarchCubeFilterCallback &getMarchCubeFilter() {
        return marchCubeFilter;
    }

    inline void setMarchCubeFilter(const MarchCubeFilterCallback &filter) {
        this->marchCubeFilter = filter;
    }

    /// 中间变量
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
    vector<SamplePointFilterCallback> samplePointFilters;

    // 依赖翻转，用来初始化采样点矩阵
    SamplePointGroupFilterCallback samplePointGroupFilter;

    // 依赖翻转
    MarchCubeFilterCallback marchCubeFilter;

    // 作为网格位置的水平值
    double isoLevel;

    /// 算法运行中生成的中间数据

    // 算法的采样点
    vector<vector<vector<SamplePoint> > > sampleMatrix;

    // 将采样点划分为不同的组，每个组的采样点共用隐式函数组
    // NOTE:划分交由其他算法负责
    vector<SamplePointGroup> samplePointGroups;

};

#endif // BLENDMARCHCUBEALGORITHM_H
