#include "HexahedralMesh.h"

HexahedralMesh::HexahedralMesh()
{

}

/**
 * @brief HexahedralMesh::insertHexahedron
 * @param hexahedron
 */
void HexahedralMesh::insertHexahedron(Hexahedron &hexahedron)
{
    this->hexahedrons.push_back(hexahedron);
}

void HexahedralMesh::insertHexahedron(Hexahedron &&hexahedron) {
    this->hexahedrons.push_back(hexahedron);
}




