#ifndef IMPORTER_H
#define IMPORTER_H

#include <string>
#include <memory>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Eigen/Dense>

#include <SurfaceMeshModel.h>

#include "Mesh/Mesh.h"

using SurfaceMesh::SurfaceMeshModel;

class Importer
{
public:
    std::shared_ptr<SurfaceMeshModel> loadSurfaceMeshModel(const std::string& path);
    Mesh loadMesh(const std::string &path);
};

#endif // IMPORTER_H
