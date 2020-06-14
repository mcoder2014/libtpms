#ifndef HEXAHEDRALMESH_H
#define HEXAHEDRALMESH_H

#include <vector>
#include <iostream>
#include <memory>

#include <Octree.h>

#include "Hexahedron.h"

class HexahedralMeshConfig;

using std::vector;
using std::shared_ptr;

class HexahedralMesh
{
public:
    HexahedralMesh();

    // 插入一个新的六面体
    void insertHexahedron(Hexahedron& hexahedron);
    void insertHexahedron(Hexahedron&& hexahedron);

    // 六面体
    vector<Hexahedron> hexahedrons;

};

#endif // HEXAHEDRALMESH_H
