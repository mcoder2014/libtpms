#ifndef GRIDHEXALGORITHM_H
#define GRIDHEXALGORITHM_H

#include <memory>
#include <vector>

#include "HexahedralMesh.h"
#include "Voxel/VoxelModel.h"

using std::vector;
using std::shared_ptr;

class GridHexAlgorithmConfig;

/**
 * @brief The GridHexAlgorithm class
 * 格栅法生成六面体网格
 * https://wenku.baidu.com/view/73bac60d52ea551810a687ad.html
 */
class GridHexAlgorithm
{
public:
    GridHexAlgorithm();

    // 构建六面体模型
    std::shared_ptr<HexahedralMesh> build();

    shared_ptr<GridHexAlgorithmConfig> getConfig() const;
    void setConfig(const shared_ptr<GridHexAlgorithmConfig> &value);

private:

    // 初始化一个略大于包围盒大小的分割立方体
    void initSampleMatrix();

    // 初始化 六面体列表
    void initHexahedronList();

    // 帮助函数，仅添加单个六面体
    void insertOneHexahedron(Vector3i index);

    // 轮廓线法收缩
    void scanLine();

    // 构建完成后，清除临时对象
    void clean();

    // 中间临时对象
    shared_ptr<HexahedralMesh> hexahedralMesh;

    // 六面体采样点
    vector<vector<vector<shared_ptr<HexSamplePoint>>>> hexSampleMatrix;

    // 算法的配置属性
    shared_ptr<GridHexAlgorithmConfig> config;
};

/**
 * @brief The GridHexAlgorithmConfig class
 * 算法的配置
 */
class GridHexAlgorithmConfig
{
public:
    GridHexAlgorithmConfig():voxelModel(nullptr),initSize(1){}

    // 八叉树
    std::shared_ptr<VoxelModel> voxelModel;

    // 初始的正六面体边长
    double initSize;
};

#endif // GRIDHEXALGORITHM_H
