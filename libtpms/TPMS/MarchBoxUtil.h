#ifndef MARCHBOXUTIL_H
#define MARCHBOXUTIL_H
/***
 * 本文件放一些 March Box 算法相关的重复性函数
 */
#include <vector>
#include <memory>

#include <Eigen/Dense>

#include "SamplePoint.h"
#include "TpmsImplicitFunction.h"
#include "mesh.h"

using std::vector;
using Eigen::Vector3d;

/**
 * 创造一个指定大小的三维矩阵，用 initValue 初始化
 */
template<class T>
vector<vector<vector<T> > >  create3DMatrix(Vector3i size, T initValue)
{
    return vector<vector<vector<T>>>(size.x(),
        vector<vector<T>>(size.y(),
            vector<T>(size.z(), initValue)));
}

// 初始化物理坐标三维矩阵
void initMatrix(
        vector<vector<vector<SamplePoint>>>& matrix,
        Eigen::AlignedBox3d boundingBoxPhysial,
        Eigen::AlignedBox3d boundingBoxLogical);

void calcTpmsFunction(vector<vector<vector<SamplePoint>>>& matrix,
        TpmsImplicitFunction tpmsFunction);

std::string hashKey(Vector3i index, int edgeIdx);

int getMarchBoxCubeIndex(
        vector<vector<vector<SamplePoint> > > &matrix,
        Vector3i& index,
        double isoLevel);

#endif // MARCHBOXUTIL_H
