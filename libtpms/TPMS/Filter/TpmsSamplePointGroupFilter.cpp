#include "TpmsSamplePointGroupFilter.h"

#include "TPMS/BoundaryImplicitFunction.h"

#include "TPMS/CommonImplicitFunction.h"

using namespace std::placeholders;

/**
 * @brief TpmsSamplePointGroupFilter::operator ()
 * @param sampleMatrix
 * 1. 创建 group
 * 2. 将所有的采样点塞入同一个 group
 * @return
 */
vector<ImplicitFunciton::SamplePointGroup> TpmsSamplePointGroupFilter::operator ()(vector<vector<vector<SamplePoint> > > &sampleMatrix)
{
    // Groups
    vector<SamplePointGroup> samplePointGroups;

    // Group
    SamplePointGroup sampleGroup = createGroup();

    // 将所有的采样点塞入 group
    Vector3i range = getSampleMatrixSize(sampleMatrix);
    for(int i = 0; i < range.x(); i++) {
        for(int j = 0; j < range.y(); j++) {
            for(int k = 0; k < range.z(); k++) {
                sampleGroup.insert(&(sampleMatrix[i][j][k]));
            }
        }
    }

    samplePointGroups.push_back(sampleGroup);
    return samplePointGroups;
}

TpmsType TpmsSamplePointGroupFilter::getTpmsType() const
{
    return tpmsType;
}

void TpmsSamplePointGroupFilter::setTpmsType(const TpmsType &value)
{
    tpmsType = value;
}

shared_ptr<VoxelModel> TpmsSamplePointGroupFilter::getBoundary() const
{
    return boundary;
}

void TpmsSamplePointGroupFilter::setBoundary(const shared_ptr<VoxelModel> &value)
{
    boundary = value;
}

/**
 * @brief TpmsSamplePointGroupFilter::createGroup
 * 创建一个 Group 模板
 * @return
 */
ImplicitFunciton::SamplePointGroup TpmsSamplePointGroupFilter::createGroup()
{
    SamplePointGroup sampleGroup;

    // TpmsImplicitFunction
    ImplicitFunciton::ImplicitFunction tpmsImplictFunction;
    tpmsImplictFunction.implicitFunction = getTpmsFunction();
    tpmsImplictFunction.implicitCoorType = ImplicitFunciton::TPMS;
    tpmsImplictFunction.weightFunction = std::bind(ImplicitFunciton::ONE, _1, _2, _3);
    tpmsImplictFunction.operation = ImplicitFunciton::ADD;

    sampleGroup.implicitFunctionGroup.push_back(tpmsImplictFunction);

    /// 新建 boundary implicitFunction
    ImplicitFunciton::BoundaryImplicitFunction boundaryFunction;
    boundaryFunction.setBoundary(boundary);

    // Boundary ImplicitFunction
    ImplicitFunciton::ImplicitFunction boundaryImplicitFunction;
    boundaryImplicitFunction.implicitFunction = boundaryFunction;
    boundaryImplicitFunction.implicitCoorType = ImplicitFunciton::PHYSICAL;
    boundaryImplicitFunction.weightFunction = std::bind(ImplicitFunciton::ONE, _1, _2, _3);
    boundaryImplicitFunction.operation = ImplicitFunciton::OR;

    sampleGroup.implicitFunctionGroup.push_back(boundaryImplicitFunction);

    return sampleGroup;
}

/**
 * @brief TpmsSamplePointGroupFilter::getTpmsFunction
 * 获得 TPMS 函数
 * @return
 */
ImplicitFunciton::FunctionCallback TpmsSamplePointGroupFilter::getTpmsFunction()
{
    if (tpmsType == TpmsType::P) {
        return std::bind(ImplicitFunciton::P, _1, _2, _3);
    } else if (tpmsType == TpmsType::D) {
        return std::bind(ImplicitFunciton::D, _1, _2, _3);
    } else if (tpmsType == TpmsType::G) {
        return std::bind(ImplicitFunciton::G, _1, _2, _3);
    } else if (tpmsType == TpmsType::I_WP) {
        return std::bind(ImplicitFunciton::I_WP, _1, _2, _3);
    } else if (tpmsType == TpmsType::F_RD) {
        return std::bind(ImplicitFunciton::F_RD, _1, _2, _3);
    } else if (tpmsType == TpmsType::L) {
        return std::bind(ImplicitFunciton::L, _1, _2, _3);
    } else if (tpmsType == TpmsType::I2_Y) {
        return std::bind(ImplicitFunciton::I2_Y, _1, _2, _3);
    } else if (tpmsType == TpmsType::TUBULAR_G) {
        return std::bind(ImplicitFunciton::TUBULAR_G, _1, _2, _3);
    } else if (tpmsType == TpmsType::TUBULAR_P) {
        return std::bind(ImplicitFunciton::TUBULAR_P, _1, _2, _3);
    }
    return std::bind(ImplicitFunciton::D, _1, _2, _3);
}
