#ifndef HEXAHEDRALMESHDEBUGGER_H
#define HEXAHEDRALMESHDEBUGGER_H

#include <memory>

#include "HexahedralMesh.h"
#include "Mesh/Mesh"

class HexahedralMeshDebugger
{
public:
    HexahedralMeshDebugger();

    // 将六面体网格导出为 为 mesh 测试
    Mesh convertToMesh(std::shared_ptr<HexahedralMesh> hexahedralMesh);

private:
    // 辅助函数，添加单个六面体
    void insertOneHexahedron(Mesh& mesh, Hexahedron& hexahedron);
};

#endif // HEXAHEDRALMESHDEBUGGER_H
