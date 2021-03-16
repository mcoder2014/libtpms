#ifndef COMPRESSMARCHCUBEFILTER_H
#define COMPRESSMARCHCUBEFILTER_H

#include <map>
#include <memory>

#include "BaseFilter.h"
#include <Voxel/VoxelModel.h>

using std::unordered_map;
using std::shared_ptr;

class CompressMarchCubeFilter
        :public BaseMarchCubeFilter
{
public:
    CompressMarchCubeFilter();
    Mesh operator ()(vector<vector<vector<SamplePoint> > > &sampleMatrix) override;

    double getIsoLevel() const {
        return isoLevel;
    }
    void setIsoLevel(double value) {
        isoLevel = value;
    }

    std::shared_ptr<VoxelModel> getVoxelModel() const {
        return voxelModel;
    }
    void setVoxelModel(const std::shared_ptr<VoxelModel> &value) {
        voxelModel = value;
    }


    // 检查待拟合的体素是否在 boundary 内部
    bool inBoundary(Vector3i index, vector<vector<vector<SamplePoint> > > &sampleMatrix);

private:

    /// 初始必要值
    // 水平值
    double isoLevel;

    // 体素模型-边界值
    std::shared_ptr<VoxelModel> voxelModel;

    /// 中间临时数据
    // 记录当前已经加入网络的的顶点 Id
    std::unordered_map<std::string, int> vertexIdMap;

    vector<vector<vector<bool>>> inBoundaryCache;

    /// 方法
    // 缓存采样点是否位于 boundary 数据，减少非必要计算量
    void buildInBoundaryCache(vector<vector<vector<SamplePoint> > > &sampleMatrix);

    // 获得顶点 handle
    Mesh::VertexHandle getVertexHandle(Vector3i index, int edgeIndex, vector<vector<vector<SamplePoint> > > &sampleMatrix, Mesh &mesh);

    // 加入面片
    void addFaces(int *faces, Vector3i index, vector<vector<vector<SamplePoint> > > &sampleMatrix, Mesh& mesh);

};

#endif // COMPRESSMARCHCUBEFILTER_H
