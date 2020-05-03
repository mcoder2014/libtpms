#ifndef CUSTOMTPMSSINGLESURFACECONFIG_H
#define CUSTOMTPMSSINGLESURFACECONFIG_H

#include "BaseTpmsSingleSurfaceConfig.h"
#include "Mesh/Mesh.h"
#include <SurfaceMeshModel.h>

class CustomTpmsSingleSurfaceConfig
        :public BaseTpmsSingleSurfaceConfig
{
public:
    virtual ~CustomTpmsSingleSurfaceConfig(){}
    virtual Eigen::Vector3i getMatrixSize() const;

    std::shared_ptr<SurfaceMesh::SurfaceMeshModel> getCustomBoundary() const;
    void setCustomBoundary(const std::shared_ptr<SurfaceMesh::SurfaceMeshModel> &value);

    Eigen::Vector3d getPeriodCycleLength() const;
    void setPeriodCycleLength(const Eigen::Vector3d &value);

protected:
    // Boundary
    std::shared_ptr<SurfaceMesh::SurfaceMeshModel> customBoundary;

    // Tpms 的周期长度
    Eigen::Vector3d periodCycleLength;
};

#endif // CUSTOMTPMSSINGLESURFACECONFIG_H
