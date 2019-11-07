#ifndef SURFACEMESH_LOAD_H
#define SURFACEMESH_LOAD_H

#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <Eigen/Dense>

#include <SurfaceMeshModel.h>

class SurfaceMeshLoader
{
public:
    SurfaceMeshLoader();

    bool load(SurfaceMesh::SurfaceMeshModel& model,
              std::string path);
};

#endif // SURFACEMESH_LOAD_H
