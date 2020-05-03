#ifndef CONVERTOR_H
#define CONVERTOR_H

#include <memory>

#include <Eigen/Dense>

#include <SurfaceMeshModel.h>

#include "Mesh/Mesh.h"
using SurfaceMesh::SurfaceMeshModel;

class Convertor
{
public:
    std::shared_ptr<SurfaceMeshModel> mesh2SurfaceMeshModel(const Mesh& mesh);
    Mesh surfaceMeshModel2mesh(SurfaceMeshModel &model);
};

#endif // CONVERTOR_H
