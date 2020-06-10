#ifndef HEXAHEDRALMESHDEBUGGER_H
#define HEXAHEDRALMESHDEBUGGER_H

#include <memory>

#include "HexahedralMesh.h"
#include "Mesh/Mesh"

class HexahedralMeshDebugger
{
public:
    HexahedralMeshDebugger();
    Mesh convertToMesh(std::shared_ptr<HexahedralMesh> hexahedralMesh);
};

#endif // HEXAHEDRALMESHDEBUGGER_H
