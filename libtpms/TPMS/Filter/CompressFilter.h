#ifndef COMPRESSFILTER_H
#define COMPRESSFILTER_H

#include "BaseFilter.h"

#include "Voxel/VoxelModel.h"

class CompressFilter
        :public BaseSampleMatrixFilter, public BaseSamplePointFilter
{
public:
    CompressFilter();

    // Z 轴方向的压缩
    void process(vector<vector<vector<SamplePoint> > >& sampleMatrix) override;
    void operator ()(vector<vector<vector<SamplePoint> > > &sampleMatrix) override;

    std::shared_ptr<VoxelModel> getBoundary() const;
    void setBoundary(const std::shared_ptr<VoxelModel> &value);

private:
    // 滤波器的边界
    std::shared_ptr<VoxelModel> boundary;

    // 将 Z 轴方向的采样点均匀分布在两个边界点之间
    void uniformDistribute(vector<SamplePoint>& samplePoints, const vector<Vector3d>& boundary);

    void setInvalid(vector<SamplePoint>& samplePoints);
};

#endif // COMPRESSFILTER_H
