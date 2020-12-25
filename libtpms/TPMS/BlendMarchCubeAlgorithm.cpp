#include "BlendMarchCubeAlgorithm.h"

#include <iostream>

#include "ImplicitFunctionUtil.h"
#include "MarchBoxTable.h"
#include "MarchBoxUtil.h"

/**
 * @brief BlendMarchCubeAlgorithm::initSampleMatrix
 * 初始化采样矩阵，将这部分工作交给 samplePointFilter 进行处理
 */
void BlendMarchCubeAlgorithm::initSampleMatrix()
{
    for(size_t index = 0; index < samplePointFilters.size(); index++) {
        std::cout << "Run " << index+1 << " / " << samplePointFilters.size() << "samplePointFilters" << std::endl;
        samplePointFilters[index](this->sampleMatrix);
    }
}

/**
 * @brief BlendMarchCubeAlgorithm::initSamplePointGroup
 * 初始化采样矩阵及部分使用隐函数，交给 SamplePointGroupFilter 进行处理
 */
void BlendMarchCubeAlgorithm::initSamplePointGroup()
{
    if(!samplePointGroupFilter){
        std::cerr << "Not Set SamplePointGroupFilter" << std::endl;
    }

    samplePointGroups = samplePointGroupFilter(sampleMatrix);
    std::cout << "SamplePointGroupFilter" << __FUNCTION__ << __LINE__ << std::endl;
}

/**
 * @brief BlendMarchCubeAlgorithm::caculateImplicitValue
 * 根据 SamplePointsGroup 计算隐函数值
 * - 每个 group
 *  - 每个 function
 *   - 处理每个 samplePoint
 */
void BlendMarchCubeAlgorithm::calculateImplicitValue()
{
    for(size_t index = 0; index < samplePointGroups.size(); index ++) {
        std::cout << __FUNCTION__ << " Group " << index+1 << " / " << samplePointGroups.size()
                  << "Group Size: " << samplePointGroups[index].size() << std::endl;
        ImplicitFunciton::calculateSamplePointGroup(samplePointGroups[index]);
    }

    /// Debug
    int countIn = 0;
    int countOut = 0;
    Vector3i matrixSize = getSampleMatrixSize(sampleMatrix);

    for(int i = 0; i < matrixSize.x(); i++) {
        for(int j = 0; j < matrixSize.y(); j++) {
            for(int k = 0; k < matrixSize.z(); k++) {
                if(sampleMatrix[i][j][k].implicitValue < isoLevel) {
                    countIn++;
                } else {
                    countOut++;
                }
            }
        }
    }

    std::cout << "Count In: " << countIn << "\t Count Out: " << countOut << std::endl;
}

/**
 * @brief BlendMarchCubeAlgorithm::clear
 * 清空生成的中间数据
 * 1. sampleMatrix
 * 2. samplePointGroup
 * 3. filters
 */
void BlendMarchCubeAlgorithm::clear()
{
    sampleMatrix.clear();
    samplePointGroups.clear();

    samplePointFilters.clear();
    {
        SamplePointGroupFilterCallback tmpSamplePorintGroupFilter;
        samplePointGroupFilter.swap(tmpSamplePorintGroupFilter);

        MarchCubeFilterCallback tmpMarchCubeFilter;
        marchCubeFilter.swap(tmpMarchCubeFilter);
    }
}

/**
 * @brief BlendMarchCubeAlgorithm::marchMesh
 * March Cube 方法拟合网格模型
 * @return
 */
Mesh BlendMarchCubeAlgorithm::marchMesh()
{
    if(!marchCubeFilter) {
        std::cerr << "Not Set marchCubeFilter" << std::endl;
        Mesh tmp;
        return tmp;
    }

    return marchCubeFilter(sampleMatrix);
}

/**
 * @brief BlendMarchCubeAlgorithm::process
 * 开始按照方法构建
 * 注意：执行此函数前，samplePoints samplepointGroup 都已经初始化完毕
 *
 * 1. 挨个 SamplePointsGroup 计算对应 samplePoint 的值
 * 2. 根据 samplePoints 的值 构造 Mesh
 * @return
 */
BlendMarchCubeAlgorithm::BlendMarchCubeAlgorithm()
{

}

Mesh BlendMarchCubeAlgorithm::process()
{
    // 先初始化采样矩阵
    initSampleMatrix();

    // 再初始化采样点及其需要的 隐函数
    initSamplePointGroup();

    // 计算隐函数值
    calculateImplicitValue();

    // 拟合网格模型
    Mesh mesh = marchMesh();

    // 清理临时数据
    clear();

    // 返回拟合结果
    return mesh;
}
