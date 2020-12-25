#include "CommonSamplePointFilter.h"

#include <TPMS/MarchBoxUtil.h>

CommonSamplePointFilter::CommonSamplePointFilter()
{

}


void CommonSamplePointFilter::operator ()(vector<vector<vector<SamplePoint> > > &sampleMatrix)
{
    vector<vector<vector<SamplePoint> > > matrix(
                sampleMatrixSize.x(), vector<vector<SamplePoint>>(
                    sampleMatrixSize.y(),vector<SamplePoint>(
                        sampleMatrixSize.z(), SamplePoint())));
    sampleMatrix.swap(matrix);

    // 均匀的初始化矩阵
    initMatrix(sampleMatrix, physicalBoundingBox, logicalBoundingBox);
}
