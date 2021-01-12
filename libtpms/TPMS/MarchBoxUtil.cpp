#include "MarchBoxUtil.h"

#include <string>
#include <unordered_map>

#include "MarchBoxTable.h"

/**
 * 初始化矩阵时，方便计算采样矩阵步长
 * @brief getStepSize
 * @param boundingBox
 * @param size
 * @return
 */
Vector3d getStepSize(Eigen::AlignedBox3d boundingBox, Vector3i size)
{
    Vector3d stepSize;
    Vector3d relativeSize = boundingBox.max() - boundingBox.min();

    stepSize.x() = relativeSize.x() / (std::max(size.x() - 1, 1));
    stepSize.y() = relativeSize.y() / (std::max(size.y() - 1, 1));
    stepSize.z() = relativeSize.z() / (std::max(size.z() - 1, 1));

    return stepSize;
}

/**
 * 初始化采样矩阵的物理坐标和 tpms 坐标
 * @brief initMatrix
 * @param matrix
 * @param boundingBoxPhysial
 * @param boundingBoxLogical
 */
void initMatrix(vector<vector<vector<SamplePoint> > > &matrix,
        Eigen::AlignedBox3d boundingBoxPhysial,
        Eigen::AlignedBox3d boundingBoxLogical)
{
    // Matrix Size
    Vector3i size = getSampleMatrixSize(matrix);

    // physical Boundary
    Vector3d physicalMin = boundingBoxPhysial.min();
    Vector3d physicalStep = getStepSize(boundingBoxPhysial, size);

    Vector3d logicalMin = boundingBoxLogical.min();
    Vector3d logicalStep = getStepSize(boundingBoxLogical, size);

    // 临时坐标
    Vector3d physicalCoordinate;
    Vector3d logicalCoordinate;
    for(int iterX = 0; iterX < size.x(); iterX++)
    {
        physicalCoordinate.x() = physicalMin.x() + physicalStep.x() * iterX;
        logicalCoordinate.x() = logicalMin.x() + logicalStep.x() * iterX;
        for (int iterY = 0; iterY < size.y(); iterY++)
        {
            physicalCoordinate.y() = physicalMin.y() + physicalStep.y() * iterY;
            logicalCoordinate.y() = logicalMin.y() + logicalStep.y() * iterY;
            for (int iterZ = 0; iterZ < size.z(); iterZ++)
            {
                physicalCoordinate.z() = physicalMin.z() + physicalStep.z() * iterZ;
                logicalCoordinate.z() = logicalMin.z() + logicalStep.z() * iterZ;

                matrix[iterX][iterY][iterZ].physical = physicalCoordinate;
                matrix[iterX][iterY][iterZ].tpms = logicalCoordinate;
            }
        }
    }
}

/**
 * 计算每个采样点的 Tpms 隐函数值
 * @brief calcTpmsFunction
 * @param matrix
 * @param tpmsFunction
 */
void calcTpmsFunction(vector<vector<vector<SamplePoint> > > &matrix,
        TpmsImplicitFunction tpmsFunction)
{
    for(vector<vector<SamplePoint> >& matrixYOZ:matrix) {
        for(vector<SamplePoint>& matrixOZ:matrixYOZ) {
            for(SamplePoint& samplePoint:matrixOZ) {
                samplePoint.implicitValue = tpmsFunction.f(
                            samplePoint.tpms.x(),
                            samplePoint.tpms.y(),
                            samplePoint.tpms.z());
            }
        }
    }
}

/**
 * March box 拟合网格时，用于独立标示每个顶点
 * @brief hashKey
 * @param xIdx
 * @param yIdx
 * @param zIdx
 * @param edgeIdx
 * @return
 */
std::string hashKey(Vector3i index, int edgeIdx)
{
    // 当 edgeIdx 为公共边时，将其变化为编号最小的 体素编号
    static std::unordered_map<int, int> edge_id_map_x{
        {3, 1},{7, 5},{8, 9},{11, 10}};
    static std::unordered_map<int, int> edge_id_map_y{
        {0, 4},{1, 5},{2, 6},{3, 7}};
    static std::unordered_map<int, int> edge_id_map_z{
        {0, 2},{4, 6},{8, 11},{9, 10}};

    if(index.x() > 0 && edge_id_map_x.find(edgeIdx) != edge_id_map_x.end()) {
        edgeIdx = edge_id_map_x[edgeIdx];
        index.x()--;
    }
    if(index.y() > 0 && edge_id_map_y.find(edgeIdx) != edge_id_map_y.end()) {
        edgeIdx = edge_id_map_y[edgeIdx];
        index.y()--;
    }
    if(index.z() > 0 && edge_id_map_z.find(edgeIdx) != edge_id_map_z.end()) {
        edgeIdx = edge_id_map_z[edgeIdx];
        index.z()--;
    }

    static const char SEP = ':';
    std::string key = std::to_string(index.x()) + SEP
        + std::to_string(index.y()) + SEP
        + std::to_string(index.z()) + SEP
        + std::to_string(edgeIdx);

    return key;
}

/**
 * 根据体素 8 个顶点的 tpms 隐函数值，拟合面片情况。
 * 不同的算法都需要
 * @brief getMarchBoxCubeIndex
 * @param matrix
 * @param index
 * @param isoLevel
 * @return
 */
int getMarchBoxCubeIndex(
        const vector<vector<vector<SamplePoint> > > &matrix,
        Vector3i& index,
        double isoLevel)
{
    int cubeIndex = 0;
    if(matrix[index.x()  ][index.y()  ][index.z()  ].implicitValue < isoLevel)
        cubeIndex |= 1;
    if(matrix[index.x()+1][index.y()  ][index.z()  ].implicitValue < isoLevel)
        cubeIndex |= 2;
    if(matrix[index.x()+1][index.y()  ][index.z()+1].implicitValue < isoLevel)
        cubeIndex |= 4;
    if(matrix[index.x()  ][index.y()  ][index.z()+1].implicitValue < isoLevel)
        cubeIndex |= 8;
    if(matrix[index.x()  ][index.y()+1][index.z()  ].implicitValue < isoLevel)
        cubeIndex |= 16;
    if(matrix[index.x()+1][index.y()+1][index.z()  ].implicitValue < isoLevel)
        cubeIndex |= 32;
    if(matrix[index.x()+1][index.y()+1][index.z()+1].implicitValue < isoLevel)
        cubeIndex |= 64;
    if(matrix[index.x()  ][index.y()+1][index.z()+1].implicitValue < isoLevel)
        cubeIndex |= 128;
    return cubeIndex;
}

/**
 * @brief getSampleMatrixSize
 * 从采样矩阵获得采样矩阵的 index 尺寸
 * @param matrix
 * @return
 */
Eigen::Vector3i getSampleMatrixSize(vector<vector<vector<SamplePoint> > > &matrix)
{
    Vector3i range(0, 0, 0);
    range.x() = matrix.size();
    range.y() = matrix[0].size();
    range.z() = matrix[0][0].size();
    return range;
}

/**
 * @brief getPhysicalStep
 * 获得采样点物理间距
 * @param matrix
 * @return
 */
Eigen::Vector3d getPhysicalStep(vector<vector<vector<SamplePoint> > > &matrix)
{
    /// 计算 offset 对应的起始坐标点
    Vector3d physicalStep(.0, .0, .0);
    physicalStep.x() = matrix[1][1][1].physical.x() - matrix[0][0][0].physical.x();
    physicalStep.y() = matrix[1][1][1].physical.y() - matrix[0][0][0].physical.y();
    physicalStep.z() = matrix[1][1][1].physical.z() - matrix[0][0][0].physical.z();
    return physicalStep;
}

Eigen::Vector3d getLogicalStep(vector<vector<vector<SamplePoint> > > &matrix)
{
    Vector3d tpmsStep(.0, .0, .0);
    tpmsStep.x() = matrix[1][1][1].tpms.x() - matrix[0][0][0].tpms.x();
    tpmsStep.y() = matrix[1][1][1].tpms.y() - matrix[0][0][0].tpms.y();
    tpmsStep.z() = matrix[1][1][1].tpms.z() - matrix[0][0][0].tpms.z();
    return tpmsStep;
}

/**
 * @brief calcMatrixSize
 * 计算矩阵尺寸
 * @param boundingBox
 * @param interval
 * @return
 */
Eigen::Vector3i calcMatrixSize(const Eigen::AlignedBox3d &boundingBox, const Eigen::Vector3d &interval)
{
    Vector3i matrixSize;
    Vector3d extent = boundingBox.max() - boundingBox.min();

    matrixSize.x() = static_cast<int>(round(extent.x() / interval.x()));
    matrixSize.y() = static_cast<int>(round(extent.y() / interval.y()));
    matrixSize.z() = static_cast<int>(round(extent.z() / interval.z()));

    return matrixSize;
}
