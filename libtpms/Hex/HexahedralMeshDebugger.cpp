#include "HexahedralMeshDebugger.h"

#include <vector>
using std::vector;

HexahedralMeshDebugger::HexahedralMeshDebugger()
{

}

Mesh HexahedralMeshDebugger::convertToMesh(std::shared_ptr<HexahedralMesh> hexahedralMesh)
{
    Mesh mesh;

    for(Hexahedron& hexahedron:hexahedralMesh->hexahedrons) {
        insertOneHexahedron(mesh, hexahedron);
    }

    return mesh;
}

/**
 * @brief HexahedralMeshDebugger::insertOneHexahedron
 * @param mesh
 * @param hexahedron
 */
void HexahedralMeshDebugger::insertOneHexahedron(Mesh &mesh, Hexahedron &hexahedron)
{
    // 先将八个顶点加入网格中
    vector<Mesh::VertexHandle> vertexHandles(8);
    vertexHandles[0] = mesh.add_vertex(hexahedron.vertices[0]->physical);
    vertexHandles[1] = mesh.add_vertex(hexahedron.vertices[1]->physical);
    vertexHandles[2] = mesh.add_vertex(hexahedron.vertices[2]->physical);
    vertexHandles[3] = mesh.add_vertex(hexahedron.vertices[3]->physical);
    vertexHandles[4] = mesh.add_vertex(hexahedron.vertices[4]->physical);
    vertexHandles[5] = mesh.add_vertex(hexahedron.vertices[5]->physical);
    vertexHandles[6] = mesh.add_vertex(hexahedron.vertices[6]->physical);
    vertexHandles[7] = mesh.add_vertex(hexahedron.vertices[7]->physical);

    // 临时 vector
    vector<Mesh::VertexHandle> face;

    auto addFace = [&](int v0, int v1, int v2) {
        face.clear();
        face.push_back(vertexHandles[v0]);
        face.push_back(vertexHandles[v1]);
        face.push_back(vertexHandles[v2]);
        mesh.add_face(face);
    };

    // 底面
    addFace(0,1,2);
    addFace(0,2,3);

    //左侧面
    addFace(0,3,7);
    addFace(0,7,4);

    // 正面
    addFace(2,6,3);
    addFace(3,6,7);

    // 右侧
    addFace(1,5,2);
    addFace(2,5,6);

    // 反面
    addFace(1,4,5);
    addFace(0,4,1);

    // 顶面
    addFace(4,7,5);
    addFace(5,7,6);
}
