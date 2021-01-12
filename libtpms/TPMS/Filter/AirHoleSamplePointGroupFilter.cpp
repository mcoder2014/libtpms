#include "AirHoleSamplePointGroupFilter.h"

#include "TPMS/AirHoleImplicitFunction.h"
#include "TPMS/BoundaryImplicitFunction.h"

#include "TPMS/CommonImplicitFunction.h"

using ImplicitFunciton::AirHoleImplicitFunction;
using namespace std::placeholders;

/**
 * @brief AirHoleSamplePointGroupFilter::operator ()
 * 将整个鞋垫区域化为通气孔的区域
 * 1. 直接按照 interval 和 offset 开始均分成网格，每个格子为一个 group
 * @param sampleMatrix
 * @return
 */
vector<ImplicitFunciton::SamplePointGroup> AirHoleSamplePointGroupFilter::operator ()(vector<vector<vector<SamplePoint> > > &sampleMatrix)
{
    groups.clear();
    groupMap.clear();

    Vector3i range = getSampleMatrixSize(sampleMatrix);
    Vector3d startPoint = boundingBox.min();

    /// Lambda 根据 x y 的物理坐标计算通气孔的 idx
    auto getIdx = [&](Vector3d point)->Vector2i {
        point = point - startPoint;
        point.x() -= offset.x();
        point.y() -= offset.y();

        Vector2i idx;
        idx.x() = static_cast<int>(point.x() / interval.x());
        idx.y() = static_cast<int>(point.y() / interval.y());
        return idx;
    };

    /// Lambda 创建 idx 编号的通气孔的 group
    auto addGroup = [&](Vector2i idx, string key) {
        Vector2d position;
        position.x() = startPoint.x() + offset.x() + idx.x() * interval.x() + interval.x() / 2;
        position.y() = startPoint.y() + offset.y() + idx.y() * interval.y() + interval.y() / 2;
        SamplePointGroup sampleGroup = createGroup(position);
        // 添加记录
        groups.push_back(sampleGroup);
        groupMap[key] = groups.size()-1;
    };

    /// 循环处理每一列的通气孔的 group 逻辑关系
    for(int i = 0; i < range.x(); i++) {
        for(int j = 0; j < range.y(); j++) {
            Vector2i idx = getIdx(sampleMatrix[i][j][0].physical);
            string key = getKey(idx.x(), idx.y());

            if(groupMap.find(key) == groupMap.end()) {
                // 如果之前没有创建过这个 group，创建一个
                addGroup(idx, key);
            }

            // 将 Z 轴方向采样点全部加入 group
            SamplePointGroup& group = groups[groupMap[key]];
            for(int k = 0; k < range.z(); k++) {
                group.insert(&(sampleMatrix[i][j][k]));
            }
        }
    }

    return groups;
}

/**
 * @brief AirHoleSamplePointGroupFilter::createGroup
 * 增加一个 group
 * @param pos
 */
SamplePointGroup AirHoleSamplePointGroupFilter::createGroup(const Eigen::Vector2d& pos)
{
    /// 新建一个 group
    SamplePointGroup sampleGroup;


    /// 新建 Airhole implicitFunction
    AirHoleImplicitFunction airholeFunction;
    airholeFunction.radius = radius;
    airholeFunction.position = pos;

    // ImplicitFunction
    ImplicitFunciton::ImplicitFunction airholeImplicitFunction;
    airholeImplicitFunction.implicitFunction = airholeFunction;
    airholeImplicitFunction.implicitCoorType = ImplicitFunciton::PHYSICAL;
    airholeImplicitFunction.weightFunction = std::bind(ImplicitFunciton::ONE, _1, _2, _3);
    airholeImplicitFunction.operation = ImplicitFunciton::ADD;

    sampleGroup.implicitFunctionGroup.push_back(airholeImplicitFunction);

    /// 新建 boundary implicitFunction
    ImplicitFunciton::BoundaryImplicitFunction boundaryFunction;
    boundaryFunction.setBoundary(boundary);

    // implicitFunction
    ImplicitFunciton::ImplicitFunction boundaryImplicitFunction;
    boundaryImplicitFunction.implicitFunction = boundaryFunction;
    boundaryImplicitFunction.implicitCoorType = ImplicitFunciton::PHYSICAL;
    boundaryImplicitFunction.weightFunction = std::bind(ImplicitFunciton::ONE, _1, _2, _3);
    boundaryImplicitFunction.operation = ImplicitFunciton::OR;

    sampleGroup.implicitFunctionGroup.push_back(boundaryImplicitFunction);

    return sampleGroup;
}
