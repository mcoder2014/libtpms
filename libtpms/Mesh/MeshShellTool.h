#ifndef MESHSHELLTOOL_H
#define MESHSHELLTOOL_H

#include "Mesh.h"
#include <unordered_map>

class MeshShellTool
{
public:
    void shell(Mesh& mesh, double depth);

private:
    void duplicate(Mesh &mesh);
    void fixHole(Mesh &mesh);
    void verticeNormarlDirectionMove(Mesh &mesh, double distance);

    // 备份复制的顶点的偏移位置
    std::unordered_map<int, int> vertexMap;
};

#endif // MESHSHELLTOOL_H
