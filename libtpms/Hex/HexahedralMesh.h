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
    vector<Hexahedron> hexahedrons;
};

#endif // HEXAHEDRALMESH_H
