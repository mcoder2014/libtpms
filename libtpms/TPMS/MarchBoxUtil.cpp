#include "MarchBoxUtil.h"

Vector3d getStepSize(Eigen::AlignedBox3d boundingBox, Eigen::Vector3i size)
{
    Vector3d stepSize;
    Vector3d relativeSize = boundingBox.max() - boundingBox.min();

    stepSize.x() = relativeSize.x() / (std::max(size.x() - 1, 1));
    stepSize.y() = relativeSize.y() / (std::max(size.y() - 1, 1));
    stepSize.z() = relativeSize.z() / (std::max(size.z() - 1, 1));

    return stepSize;
}

template<class T>
vector<vector<vector<T> > > create3DMatrix(int X, int Y, int Z, T initValue)
{
    return vector<vector<vector<T>>>(X,
        vector<vector<T>>(Y,
            vector<T>(Z, initValue)));
}

void initMatrix(
        vector<vector<vector<SamplePoint> > > matrix,
        Eigen::AlignedBox3d boundingBoxPhysial,
        Eigen::AlignedBox3d boundingBoxLogical)
{
    // Matrix Size
    Eigen::Vector3i size;
    size.x() = matrix.size();
    size.y() = matrix[0].size();
    size.z() = matrix[0][0].size();

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
                matrix[iterX][iterY][iterZ].tmps = logicalCoordinate;
            }
        }
    }
}
