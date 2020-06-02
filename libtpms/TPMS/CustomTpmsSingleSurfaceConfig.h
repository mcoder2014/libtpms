#ifndef CUSTOMTPMSSINGLESURFACECONFIG_H
#define CUSTOMTPMSSINGLESURFACECONFIG_H

#include <SurfaceMeshModel.h>

#include "BaseTpmsSingleSurfaceConfig.h"
#include "Mesh/Mesh.h"
#include "Voxel/VoxelModel.h"
#include "Filter/BaseSampleMatrixFilter.h"

using Eigen::Vector3d;
using Eigen::Vector3i;

class CustomTpmsSingleSurfaceConfig
        :public BaseTpmsSingleSurfaceConfig
{
public:
    virtual ~CustomTpmsSingleSurfaceConfig(){}
    virtual Eigen::Vector3i getMatrixSize() const override;
    Eigen::AlignedBox3d getBoundingBoxPhysial() const;
    Eigen::AlignedBox3d getBoundingBoxLogical() const;

    std::shared_ptr<VoxelModel> getCustomBoundary() const;
    void setCustomBoundary(std::shared_ptr<VoxelModel> value);

    Vector3d getPeriodCycleLength() const;
    void setPeriodCycleLength(const Vector3d &value);

    std::vector<std::shared_ptr<BaseSampleMatrixFilter> > getSampleMatrixFilterVector() const;
    void setSampleMatrixFilterVector(const std::vector<std::shared_ptr<BaseSampleMatrixFilter> > &value);
    void addSampleMatrixFilter(std::shared_ptr<BaseSampleMatrixFilter> baseFilter);

protected:
    // Boundary
    std::shared_ptr<VoxelModel> customBoundary;

    // Tpms 的周期长度
    Vector3d periodCycleLength;

    std::vector<std::shared_ptr<BaseSampleMatrixFilter>> sampleMatrixFilterVector;
};

#endif // CUSTOMTPMSSINGLESURFACECONFIG_H
