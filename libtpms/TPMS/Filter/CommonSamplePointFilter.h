#ifndef COMMONSAMPLEPOINTFILTER_H
#define COMMONSAMPLEPOINTFILTER_H

#include "BaseFilter.h"

#include <Eigen/Dense>

using Eigen::AlignedBox3d;
using Eigen::Vector3d;
using Eigen::Vector3i;

/**
 * @brief The CommonSamplePointFilter class
 * 本 Filter 负责新建采样点矩阵
 */
class CommonSamplePointFilter
        :public BaseSamplePointFilter
{
public:
    CommonSamplePointFilter();

    // 初始化采样矩阵
    void operator ()(vector<vector<vector<SamplePoint> > > &sampleMatrix) override;

    AlignedBox3d getPhysicalBoundingBox() const {
        return physicalBoundingBox;
    }
    void setPhysicalBoundingBox(const AlignedBox3d &value) {
        physicalBoundingBox = value;
    }
    AlignedBox3d getLogicalBoundingBox() const {
        return logicalBoundingBox;
    }
    void setLogicalBoundingBox(const AlignedBox3d &value) {
        logicalBoundingBox = value;
    }
    Vector3i getSampleMatrixSize() const {
        return sampleMatrixSize;
    }
    void setSampleMatrixSize(const Vector3i &value) {
        sampleMatrixSize = value;
    }

private:
    /// 必要设置的变量
    // 物理包围盒范围
    AlignedBox3d physicalBoundingBox;

    // 逻辑包围盒范围
    AlignedBox3d logicalBoundingBox;

    // 采样点数量
    Vector3i sampleMatrixSize;
};

#endif // COMMONSAMPLEPOINTFILTER_H
