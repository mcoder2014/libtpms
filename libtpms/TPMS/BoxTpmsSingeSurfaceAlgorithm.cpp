#include "BoxTpmsSingeSurfaceAlgorithm.h"

#include "BoxTpmsSingleSurfaceConfig.h"
#include "MarchBoxUtil.h"

BoxTpmsSingeSurfaceAlgorithm::BoxTpmsSingeSurfaceAlgorithm()
{

}

Mesh BoxTpmsSingeSurfaceAlgorithm::process()
{
    // 1. 初始化 矩阵
    TpmsImplicitFunction tpmsFunction;
    tpmsFunction.setType(config->getTpmsType());

    Eigen::Vector3i sampleMatrixSize = this->config->getMatrixSize();
    vector<vector<vector<SamplePoint>>> sampleMatrix = create3DMatrix(
                sampleMatrixSize.x(),
                sampleMatrixSize.y(),
                sampleMatrixSize.z(),
                SamplePoint());

    initMatrix(sampleMatrix,
               config->getBoundingBoxPhysial(),
               config->getBoundingBoxLogical());

    // 2. 计算矩阵的值
    calcTpmsFunction(sampleMatrix, tpmsFunction);

    // 3. 拟合
}


