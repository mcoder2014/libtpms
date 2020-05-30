#ifndef CUSTOMTPMSSINGLESURFACECONFIG_H
#define CUSTOMTPMSSINGLESURFACECONFIG_H

#include <SurfaceMeshModel.h>

#include "BaseTpmsSingleSurfaceConfig.h"
#include "Mesh/Mesh.h"
#include "Voxel/VoxelModel.h"

using Eigen::Vector3d;
using Eigen::Vector3i;

class CustomTpmsSingleSurfaceConfig
        :public BaseTpmsSingleSurfaceConfig
{
public:
    virtual ~CustomTpmsSingleSurfaceConfig(){}
    virtual Eigen::Vector3i getMatrixSize() const;
    Eigen::AlignedBox3d getBoundingBoxPhysial() const;
    Eigen::AlignedBox3d getBoundingBoxLogical() const;

    std::shared_ptr<VoxelModel> getCustomBoundary() const;
    void setCustomBoundary(std::shared_ptr<VoxelModel> value);

    Vector3d getPeriodCycleLength() const;
    void setPeriodCycleLength(const Vector3d &value);

protected:
    // Boundary
    std::shared_ptr<VoxelModel> customBoundary;

    // Tpms 的周期长度
    Vector3d periodCycleLength;
};

#endif // CUSTOMTPMSSINGLESURFACECONFIG_H
