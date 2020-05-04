#ifndef CUSTOMTPMSSINGLESURFACECONFIG_H
#define CUSTOMTPMSSINGLESURFACECONFIG_H

#include "BaseTpmsSingleSurfaceConfig.h"
#include "Mesh/Mesh.h"
#include <SurfaceMeshModel.h>

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

    std::shared_ptr<SurfaceMesh::SurfaceMeshModel> getCustomBoundary() const;
    void setCustomBoundary(const std::shared_ptr<SurfaceMesh::SurfaceMeshModel> &value);

    Vector3d getPeriodCycleLength() const;
    void setPeriodCycleLength(const Vector3d &value);

protected:
    // Boundary
    std::shared_ptr<SurfaceMesh::SurfaceMeshModel> customBoundary;

    // Tpms 的周期长度
    Vector3d periodCycleLength;
};

#endif // CUSTOMTPMSSINGLESURFACECONFIG_H
