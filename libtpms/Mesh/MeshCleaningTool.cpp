#include "MeshCleaningTool.h"

#include <unordered_set>
#include <deque>
#include <iostream>

MeshCleaningTool::MeshCleaningTool()
{

}

/**
 * @brief MeshCleaningTool::cleanRedundantTriangles
 * 对于模型中有一个主体和一些离散的冗余面片时，清除冗余面片
 * @param mesh
 */
void MeshCleaningTool::cleanRedundantTriangles(Mesh &mesh)
{
    // 全局的覆盖的面集合
    std::unordered_set<int> globalCoveredFaceSet;
    Mesh::FaceIter faceIter, faceIterEnd = mesh.faces_end();

    size_t faceNumber = mesh.n_faces();

    std::unordered_set<int> faceRemoveSet;
    for(faceIter = mesh.faces_begin(); faceIter!=faceIterEnd; faceIter++) {
        if(globalCoveredFaceSet.find(faceIter->idx()) == globalCoveredFaceSet.end()) {

            // 单个面联通的覆盖面的集合
            std::unordered_set<int> coveredFaceSet;
            bfs(mesh, faceIter, coveredFaceSet, globalCoveredFaceSet);

            if(coveredFaceSet.size() < faceNumber / 2) {
                // 如果是离散面，则加入待删除面集合
                faceRemoveSet.insert(coveredFaceSet.begin(), coveredFaceSet.end());
            }
        }
    }

    // 删除零散的面
    removeFaceSet(mesh, faceRemoveSet);

    // 垃圾回收
    mesh.garbage_collection();
}

/**
 * @brief MeshCleaningTool::bfs
 * 使用 bfs 算法，遍历当前面片的所有可能联通面
 * @param mesh
 * @param startFaceIter
 * @param coveredFaceSet
 * @param globalCoveredFaceSet
 */
void MeshCleaningTool::bfs(Mesh &mesh, Mesh::FaceIter startFaceIter,
                           std::unordered_set<int> &coveredFaceSet, std::unordered_set<int> &globalCoveredFaceSet)
{
    std::deque<Mesh::FaceHandle> faceDeque;
    faceDeque.push_back(*startFaceIter);

    while(!faceDeque.empty()) {
        Mesh::FaceHandle faceHandle = faceDeque.front();
        faceDeque.pop_front();

        // 面的循环器
        Mesh::FaceFaceIter faceFaceIter;
        for(faceFaceIter = mesh.ff_begin(faceHandle); faceFaceIter.is_valid(); faceFaceIter++) {
            // 循环面的相邻面
            if(globalCoveredFaceSet.find(faceFaceIter->idx()) != globalCoveredFaceSet.end()) {
                continue;
            }

            faceDeque.push_back(*faceFaceIter);

            // 加入痕迹
            globalCoveredFaceSet.insert(faceFaceIter->idx());
            coveredFaceSet.insert(faceFaceIter->idx());
        }
    }
}

void MeshCleaningTool::removeFaceSet(Mesh &mesh, const std::unordered_set<int> &faceRemoveSet)
{
   printf("Remove %u redundant faces from mesh, mesh faces %u\n", (uint)faceRemoveSet.size(), (uint)mesh.n_faces());
    for(const int& idx:faceRemoveSet) {
        mesh.delete_face(Mesh::FaceHandle(idx));
    }
}


