#ifndef BASESAMPLEMATRIXFILTER_H
#define BASESAMPLEMATRIXFILTER_H

#include <TPMS/MarchBoxUtil.h>

class BaseSampleMatrixFilter
{
public:
    /**
     * @brief 对采样矩阵进行一些处理，
     * 使采样矩阵可以根据需求调整部分属性。
     *
     * 需要自行实现本接口，并在 Custom 算s法中注册，
     * 当算法执行时，会调用本函数
     *
     * @param sampleMatrix
     */
    virtual void process(vector<vector<vector<SamplePoint> > >& sampleMatrix) = 0;
};

#endif // BASESAMPLEMATRIXFILTER_H
