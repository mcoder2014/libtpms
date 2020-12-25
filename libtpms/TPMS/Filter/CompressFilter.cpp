#include "CompressFilter.h"



CompressFilter::CompressFilter()
{

}

/**
 * @brief CompressFilter::process
 * 将采样矩阵压缩到体素模型边界内部
 * @param sampleMatrix
 */
void CompressFilter::process(vector<vector<vector<SamplePoint> > >& sampleMatrix)
{
    assert(boundary != nullptr);

    Vector3i index(0,0,0);
    Vector3i range(sampleMatrix.size(), sampleMatrix[0].size(), sampleMatrix[0][0].size());
    for(index.x() = 0; index.x() < range.x(); index.x() ++) {
        for(index.y() = 0; index.y() < range.y(); index.y() ++) {
            // 压缩每一个区域
            Vector3d point = sampleMatrix[index.x()][index.y()][0].physical;
            vector<Vector3d> outerBoundary = boundary->getOuterBoundaryZ(point);
            if(outerBoundary.size() < 2) {
                // 标记为在边界以外
                setInvalid(sampleMatrix[index.x()][index.y()]);
                continue;
            }

            // 调用函数均分
            uniformDistribute(sampleMatrix[index.x()][index.y()], outerBoundary);
        }
    }
}

std::shared_ptr<VoxelModel> CompressFilter::getBoundary() const
{
    return boundary;
}

void CompressFilter::setBoundary(const std::shared_ptr<VoxelModel> &value)
{
    boundary = value;
}

/**
 * @brief CompressFilter::uniformDistribute
 * 将Z轴方向的采样点均匀的分布在两个边界点之间
 * @param samplePoints
 * @param boundary
 */
void CompressFilter::uniformDistribute(vector<SamplePoint> &samplePoints, const vector<Eigen::Vector3d> &boundary)
{
    uint size = samplePoints.size();
    double bottom = boundary[0].z();
    double top = boundary[1].z();
    double step = (top - bottom) / (size - 1);
    double zCoordinate = bottom;
    for(SamplePoint& samplePoint:samplePoints) {
        samplePoint.physical.z() = zCoordinate;
        zCoordinate += step;
    }
}

/**
 * @brief CompressFilter::setInvalid
 * @param samplePoints
 */
void CompressFilter::setInvalid(vector<SamplePoint> &samplePoints)
{
    for(SamplePoint& samplePoint:samplePoints) {
        samplePoint.valid = false;
    }
}


void CompressFilter::operator ()(vector<vector<vector<SamplePoint> > > &sampleMatrix)
{
    process(sampleMatrix);
}
