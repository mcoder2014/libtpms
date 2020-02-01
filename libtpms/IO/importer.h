#ifndef IMPORTER_H
#define IMPORTER_H

#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Eigen/Dense>

#include <SurfaceMeshModel.h>

#include "mesh.h"

class Importer
{
public:
    Importer();

    bool load(SurfaceMesh::SurfaceMeshModel& model,
              const std::string path);

    SurfaceMesh::SurfaceMeshModel *loadSurfaceMeshModel(const std::string path);
    Mesh *loadMesh(const std::string path);

    Mesh *array2mesh(const std::vector<glm::vec3>& vertices,
                     const std::vector<glm::ivec3>& faces);

    SurfaceMesh::SurfaceMeshModel *array2SurfaceMeshModel(
        const std::vector<glm::vec3>& vertices,
        const std::vector<glm::ivec3>& faces);

    SurfaceMesh::SurfaceMeshModel *mesh2SurfaceMeshModel(const Mesh& mesh);
    Mesh *surfaceMeshModel2mesh(SurfaceMesh::SurfaceMeshModel &model);
};

#endif // IMPORTER_H
