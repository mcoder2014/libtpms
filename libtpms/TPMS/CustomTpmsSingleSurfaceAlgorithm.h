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
    // 算法 config
    std::shared_ptr<CustomTpmsSingleSurfaceConfig> config;

    // 采样矩阵
    vector<vector<vector<SamplePoint> > > sampleMatrix;

    // 顶点表，防止重复添加顶点
    std::unordered_map<std::string, int> vertexIdMap;

    // 体素模型-边界值
    std::shared_ptr<VoxelModel> voxelModel;

    // 初始化算法的相关数据
    void init();

    // 执行滤波函数
    void doFilters();

    // 清除算法临时数据
    void clear();

    // 添加一个体素的拟合方案
    void addFaces(int *faces, Vector3i index, Mesh& mesh);

    // 添加、或查找一个顶点的handle
    Mesh::VertexHandle getVertexHandle(Vector3i index, int edgeIndex, Mesh &mesh);

    // march box 算法
    Mesh marchMesh();

    // 是否拟合该体素的面片
    bool validVoxel(Vector3i index);

};

#endif // CUSTOMTPMSSINGLESURFACEALGORITHM_H
