#ifndef MARCHBOXUTIL_H
#define MARCHBOXUTIL_H
/***
 * 本文件放一些 March Box 算法相关的重复性函数
 */
#include <vector>

#include <Eigen/Dense>

#include "SamplePoint.h"
#include "TpmsImplicitFunction.h"
#include "mesh.h"

using std::vector;
using Eigen::Vector3d;

// 用某种初始值创建一个三维矩阵
template <class T>
vector<vector<vector<T>>> create3DMatrix(int X, int Y, int Z, T initValue);

// 初始化物理坐标三维矩阵
void initMatrix(
        vector<vector<vector<SamplePoint>>>& matrix,
        Eigen::AlignedBox3d boundingBoxPhysial,
        Eigen::AlignedBox3d boundingBoxLogical);

void calcTpmsFunction(
        vector<vector<vector<SamplePoint>>>& matrix,
        TpmsImplicitFunction& tpmsFunction);

Mesh marchMesh(vector<vector<vector<SamplePoint>>>& matrix, double isoLevel);

#endif // MARCHBOXUTIL_H
