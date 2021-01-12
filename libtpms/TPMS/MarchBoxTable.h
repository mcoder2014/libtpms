#ifndef MARCHBOXTABLE_H
#define MARCHBOXTABLE_H
/**
 * @author Chaoqun
 * 本文件只放了 March Box 的重要的表格数据
 */

#include <vector>

#include <Eigen/Dense>

using std::vector;
using Eigen::Vector3i;

// 保存的拟合表格
extern int marchboxTriTable[256][16];

// 由边的 id 找到对应的顶点的 index
vector<Vector3i> edgeToIndex(Vector3i index, int edgeIndex);

// 由顶点 id 找到对应的 Index
Vector3i vertexToIndex(const Eigen::Vector3i &index, int vertexId);

#endif // MARCHBOXTABLE_H
