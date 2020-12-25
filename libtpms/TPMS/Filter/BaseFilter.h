#ifndef BASESAMPLEMATRIXFILTER_H
#define BASESAMPLEMATRIXFILTER_H

#include <vector>

#include <Mesh/Mesh>

#include <TPMS/SamplePoint.h>
#include <TPMS/SamplePointGroup.h>
#include <TPMS/MarchBoxUtil.h>

using std::vector;
using ImplicitFunciton::SamplePointGroup;

// 重命名采样点处理回调函数为 SamplePointFilter
using SamplePointFilterCallback = function<void(vector<vector<vector<SamplePoint>>>&)>;

// 重命名采样点区域划分回调函数
using SamplePointGroupFilterCallback = function<vector<SamplePointGroup>(vector<vector<vector<SamplePoint>>>&)>;

// 重命名重建 Mesh 的回调函数
using MarchCubeFilterCallback = function<Mesh(vector<vector<vector<SamplePoint>>>&)>;

/**
 * @brief The BaseSampleMatrixFilter class
 * old 适配 CustomTpmsSingleSurfaceAlgorithm
 */
class BaseSampleMatrixFilter
{
public:
    ~BaseSampleMatrixFilter(){}
    /**
     * @brief 对采样矩阵进行一些处理，
     * 使采样矩阵可以根据需求调整部分属性。
     *
     * 需要自行实现本接口，并在 Custom 算s法中注册，
     * 当算法执行时，会调用本函数
     *
     * 如果想要让某个采样顶点失效，请置 valid = false
     *
     * @param sampleMatrix
     */
    virtual void process(vector<vector<vector<SamplePoint> > >& sampleMatrix) = 0;
};

/**
 * @brief The BaseSamplePointFilter class
 * 抽象类、仿函数，接口说明
 * 用途，对 Algorithm 中的 sampleMatrix 进行处理：
 * - 可以是初始化一个 sampleMatrix
 * - 也可以是对一个已有的 sampleMatrix 进行处理
 */
class BaseSamplePointFilter
{
public:
    virtual ~BaseSamplePointFilter() {}
    virtual void operator() (vector<vector<vector<SamplePoint>>>& sampleMatrix) = 0;
};

/**
 * @brief The BaseSamplePointGroupFilter class
 * 抽象类、仿函数，接口说明
 * 用途，对 Algorithm 中的 sampleMatrix 进行划分，分割为不同的 Group
 * - 分割 sampleMatrix 为多个 group
 * - 为每个 group 配置对应的隐式函数
 */
class BaseSamplePointGroupFilter
{
public:
    virtual ~BaseSamplePointGroupFilter(){}
    virtual vector<SamplePointGroup> operator() (vector<vector<vector<SamplePoint>>>& sampleMatrix) = 0;
};

/**
 * @brief The MarchCubeFilter class
 * 抽象类、仿函数
 * 用途，用来从采样矩阵中重建 Mesh
 */
class BaseMarchCubeFilter
{
public:
    virtual ~BaseMarchCubeFilter(){}
    virtual Mesh operator() (vector<vector<vector<SamplePoint>>>& sampleMatrix) = 0;
};

#endif // BASESAMPLEMATRIXFILTER_H
