#ifndef TPMSSAMPLEPOINTGROUPFILTER_H
#define TPMSSAMPLEPOINTGROUPFILTER_H

#include <memory>
#include <map>
#include <string>

#include <Eigen/Dense>

#include "BaseFilter.h"
#include <Voxel/VoxelModel.h>
#include <TPMS/ImplicitFunction.h>

using std::shared_ptr;
using std::map;
using std::string;

using Eigen::Vector2d;
using Eigen::Vector2i;
using Eigen::AlignedBox3d;

/**
 * @brief The TpmsSamplePointGroupFilter class
 * 将所有采样点区域设置为 Tpms 结构
 */
class TpmsSamplePointGroupFilter
        :public BaseSamplePointGroupFilter
{
public:
    TpmsSamplePointGroupFilter() {}

    // Filter 执行函数
    vector<ImplicitFunciton::SamplePointGroup> operator ()(vector<vector<vector<SamplePoint> > > &sampleMatrix) override;

    TpmsType getTpmsType() const;
    void setTpmsType(const TpmsType &value);

    shared_ptr<VoxelModel> getBoundary() const;
    void setBoundary(const shared_ptr<VoxelModel> &value);

private:
    TpmsType tpmsType;

    // 边界
    shared_ptr<VoxelModel> boundary;

    // 创建一个 Group 模板
    SamplePointGroup createGroup();

    // 获得 tpms 函数
    ImplicitFunciton::FunctionCallback getTpmsFunction();

};

#endif // TPMSSAMPLEPOINTGROUPFILTER_H
