#ifndef HEXAHEDRON_H
#define HEXAHEDRON_H

#include <iostream>
#include <vector>
#include <memory>

#include "HexSamplePoint.h"

using std::vector;
using std::shared_ptr;

class Hexahedron
{
public:
    Hexahedron();

    // 八个顶点（顺序固定）
    vector<shared_ptr<HexSamplePoint>> vertices;

    // 不重复的 id 标示每个六面体
    uint64_t id;
};

#endif // HEXAHEDRON_H
