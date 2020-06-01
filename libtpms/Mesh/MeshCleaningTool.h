#ifndef MESHCLEANINGTOOL_H
#define MESHCLEANINGTOOL_H

#include <unordered_set>

#include "Mesh.h"

class MeshCleaningTool
{
public:
    MeshCleaningTool();

    // 对于模型中有一个主体和一些离散的冗余面片时，清除冗余面片
    void cleanRedundantTriangles(Mesh &mesh);

private:
    // bfs 算法遍历一个面可达的所有面片
    void bfs(Mesh& mesh, Mesh::FaceIter faceIter, std::unordered_set<int>& coveredFaceSet, std::unordered_set<int>& globalCoveredFaceSet);
    void removeFaceSet(Mesh& mesh, const std::unordered_set<int>& faceRemoveSet);
};

#endif // MESHCLEANINGTOOL_H
