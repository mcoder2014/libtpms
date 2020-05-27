#ifndef COMPRESSFILTER_H
#define COMPRESSFILTER_H

#include "BaseSampleMatrixFilter.h"

class CompressFilter
        :public BaseSampleMatrixFilter
{
public:
    CompressFilter();

    virtual void process(vector<vector<vector<SamplePoint> > >& sampleMatrix) override;

};

#endif // COMPRESSFILTER_H
