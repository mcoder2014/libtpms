#ifndef CUSTOMTPMSSINGLESURFACEALGORITHM_H
#define CUSTOMTPMSSINGLESURFACEALGORITHM_H

#include <memory>
#include <vector>

#include <Octree.h>

#include "BaseTpmsAlgorithm.h"
#include "CustomTpmsSingleSurfaceConfig.h"
#include "SamplePoint.h"
#include "Voxel/VoxelModel.h"

using std::vector;

class CustomTpmsSingleSurfaceAlgorithm
        :public BaseTpmsAlgorithm
{
public:
    CustomTpmsSingleSurfaceAlgorithm();

    virtual Mesh process() override;
    virtual void setConfig(std::shared_ptr<BaseTpmsConfig> config) override {
        this->config = std::dynamic_pointer_cast<CustomTpmsSingleSurfaceConfig>(config);
    }

    std::shared_ptr<CustomTpmsSingleSurfaceConfig> getConfig() const {
        return config;
    }
    void setConfig(const std::shared_ptr<CustomTpmsSingleSurfaceConfig> value) {
        config = value;
    }

protected:
    // 初始化算法的相关数据
    void init();
    // 清除算法临时数据
    void clear();

    void addFaces(int *faces, Vector3i index, Mesh& mesh);
    Mesh::VertexHandle getVertexHandle(Vector3i index, int edgeIndex, Mesh &mesh);

    Mesh marchMesh();

    // 算法 config
    std::shared_ptr<CustomTpmsSingleSurfaceConfig> config;

    vector<vector<vector<SamplePoint> > > sampleMatrix;
    std::unordered_map<std::string, int> vertexIdMap;

    VoxelModel voxelModel;
};

#endif // CUSTOMTPMSSINGLESURFACEALGORITHM_H
