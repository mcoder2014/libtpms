#ifndef HEXAHEDRON_H
#define HEXAHEDRON_H

#include <iostream>
#include <vector>
#include <memory>

#include <Eigen/Dense>

#include "HexSamplePoint.h"

using std::vector;
using std::shared_ptr;
using Eigen::Vector3i;

/**
 * @brief The Hexahedron class
 * 六面体，标识划分模型的一个六面体
 *
 *   z |
 *     |
 *     |
 *     |____________
 *    / O            y
 *   /
 *  / x
 *                 v4_______e4_____________v5
 *                  /|                    /|
 *                 / |                   / |
 *              e7/  |                e5/  |
 *               /___|______e6_________/   |
 *            v7|    |                 |v6 |e9
 *              |    |                 |   |
 *              |    |e8               |e10|
 *           e11|    |                 |   |
 *              |    |_________________|___|
 *              |   / v0      e0       |   /v1
 *              |  /                   |  /
 *              | /e3                  | /e1
 *              |/_____________________|/
 *              v3         e2          v2
 * # 顶点顺序
 * v0 (  x,  y,  z  )
 * v1 (  x,  y+1,z  )
 * v2 (  x+1,y+1,z  )
 * v3 (  x+1,y,  z  )
 * v4 (  x,  y,  z+1)
 * v5 (  x,  y+1,z+1)
 * v6 (  x+1,y+1,z+1)
 * v7 (  x+1,y,  z+1)
 *
 * # 六面体 Index
 * index = v0->index
 */
class Hexahedron
{
public:
    Hexahedron();

    // 构造函数
    Hexahedron(
            shared_ptr<HexSamplePoint> v0, shared_ptr<HexSamplePoint> v1,
            shared_ptr<HexSamplePoint> v2, shared_ptr<HexSamplePoint> v3,
            shared_ptr<HexSamplePoint> v4, shared_ptr<HexSamplePoint> v5,
            shared_ptr<HexSamplePoint> v6, shared_ptr<HexSamplePoint> v7){
        vector<shared_ptr<HexSamplePoint>> swapVertices({v0, v1, v2, v3, v4, v5, v6, v7});
        this->vertices.swap(swapVertices);
        this->index = v0->index;
    }

    // 构造函数
    explicit Hexahedron(vector<shared_ptr<HexSamplePoint>> vertices){
        assert(vertices.size() == 8);
        this->vertices = vertices;
        this->index = vertices[0]->index;
    }

    // 八个顶点（顺序固定）
    // 顶点顺序同 marchTable
    vector<shared_ptr<HexSamplePoint>> vertices;

    // 不重复的 index 标示每个六面体
    Vector3i index;
};

#endif // HEXAHEDRON_H
