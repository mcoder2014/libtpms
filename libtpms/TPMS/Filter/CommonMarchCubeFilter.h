#ifndef COMMONMARCHCUBEFILTER_H
#define COMMONMARCHCUBEFILTER_H

#include <map>

#include "BaseFilter.h"

using std::unordered_map;

/**
 * @brief The CommonMarchCubeFilter class
 * 最普通的 March Cube 重建方法，直接使用采样点矩阵重建
 */
class CommonMarchCubeFilter
        :public BaseMarchCubeFilter
{
public:
    CommonMarchCubeFilter();

    // 伪函数
    Mesh operator ()(vector<vector<vector<SamplePoint> > > &sampleMatrix) override;

    double getIsoLevel() const {
        return isoLevel;
    }
    void setIsoLevel(double value) {
        isoLevel = value;
    }

    bool getBorder() const {
        return border;
    }
    void setBorder(bool value) {
        border = value;
    }

private:

    /// 初始必要值
    // 水平值
    double isoLevel;

    // 是否需要给采样点加入一圈边界
    bool border;

    /// 中间临时数据
    // 记录当前已经加入网络的的顶点 Id
    std::unordered_map<std::string, int> vertexIdMap;

    /// 方法
    // 获得顶点 handle
    Mesh::VertexHandle getVertexHandle(Vector3i index, int edgeIndex, vector<vector<vector<SamplePoint> > > &sampleMatrix, Mesh &mesh);

    // 加入面片
    void addFaces(int *faces, Vector3i index, vector<vector<vector<SamplePoint> > > &sampleMatrix, Mesh& mesh);

    // 给采样点设置一圈边界点，边界为模型外部
    void addBorder(vector<vector<vector<SamplePoint> > > &sampleMatrix);
};

#endif // COMMONMARCHCUBEFILTER_H
