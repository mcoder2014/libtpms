#ifndef AIRHOLESAMPLEPOINTGROUPFILTER_H
#define AIRHOLESAMPLEPOINTGROUPFILTER_H

#include <memory>
#include <map>
#include <string>

#include <Eigen/Dense>

#include "BaseFilter.h"
#include <Voxel/VoxelModel.h>

using std::shared_ptr;
using std::map;
using std::string;

using Eigen::Vector2d;
using Eigen::Vector2i;
using Eigen::AlignedBox3d;

/**
 * @brief The AirHoleSamplePointGroupFilter class
 * 将采样矩阵全部划分为 AirHole Group
 */
class AirHoleSamplePointGroupFilter
        :public BaseSamplePointGroupFilter
{
public:
    vector<ImplicitFunciton::SamplePointGroup> operator ()(vector<vector<vector<SamplePoint> > > &sampleMatrix) override;

    double getRadius() const {
        return radius;
    }
    void setRadius(double value) {
        radius = value;
    }

    Vector2d getOffset() const {
        return offset;
    }
    void setOffset(const Vector2d &value) {
        offset = value;
    }

    Vector2d getInterval() const {
        return interval;
    }
    void setInterval(const Vector2d &value) {
        interval = value;
    }

    shared_ptr<VoxelModel> getBoundary() const {
        return boundary;
    }
    void setBoundary(const shared_ptr<VoxelModel> &value) {
        boundary = value;
    }

    AlignedBox3d getBoundingBox() const{
        return boundingBox;
    }
    void setBoundingBox(const AlignedBox3d &value){
        boundingBox = value;
    }

private:

    /// 必要的数据
    // 小洞的半径
    double radius;

    // 第一个小洞相对包围盒的偏移
    Vector2d offset;

    // 包围盒的间距
    Vector2d interval;

    // 边界，用来之后使通气孔避开边界
    shared_ptr<VoxelModel> boundary;

    // 包围盒
    AlignedBox3d boundingBox;

    /// 中间临时数据
    // 用来找到对应的 map
    vector<ImplicitFunciton::SamplePointGroup> groups;
    map<string, int> groupMap;

    /// 函数
    // 根据 Id 找到 key
    string getKey(int i, int j){
        return std::to_string(i) + "x" + std::to_string(j);
    }

    // 新建一个组
    SamplePointGroup createGroup(const Eigen::Vector2d &pos);
};

#endif // AIRHOLESAMPLEPOINTGROUPFILTER_H
