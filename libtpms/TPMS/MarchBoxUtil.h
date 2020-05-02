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

// 用某种初始值创建一个三维矩阵
template<class T>
vector<vector<vector<T> > >  create3DMatrix(Vector3i size, T initValue);

template<class T>
std::shared_ptr<vector<vector<vector<T>>>> create3DMatrixShared(Vector3i size, T initValue);

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
