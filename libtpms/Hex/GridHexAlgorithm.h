#ifndef GRIDHEXALGORITHM_H
#define GRIDHEXALGORITHM_H

#include <memory>
#include <vector>
#include <Octree.h>

#include "HexahedralMesh.h"

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

    shared_ptr<GridHexAlgorithmConfig> config;
};

class GridHexAlgorithmConfig
{
public:
    std::shared_ptr<Octree> octree;
};

#endif // GRIDHEXALGORITHM_H
