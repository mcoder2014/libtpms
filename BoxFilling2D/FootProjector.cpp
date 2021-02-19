#include "FootProjector.h"

FootProjector::FootProjector()
    :model(nullptr),rangeTop(10), rangeBottom(-40)
{

}

vector<OpenMesh::Vec2f> FootProjector::getPoints2d()
{
    vector<OpenMesh::Vec3f> points3d = getPoints3d();
    vector<OpenMesh::Vec2f> points2d = points3d2points2d(points3d);
    return points2d;
}

vector<OpenMesh::Vec3f> FootProjector::getPoints3d()
{
    assert(model != nullptr);

    vector<OpenMesh::Vec3f> resultSet;

    Mesh & mesh = *model;
    Mesh::VertexIter v_it, v_end(mesh.vertices_end());
    for (v_it=mesh.vertices_begin(); v_it != v_end; ++v_it)
    {
        OpenMesh::Vec3f point = mesh.point(*v_it);
        if(point[2] > rangeBottom && point[2] < rangeTop) {
            resultSet.push_back(point);
        }
    }
    return resultSet;
}

vector<OpenMesh::Vec2f> FootProjector::points3d2points2d(vector<OpenMesh::Vec3f> &points3d)
{
    vector<OpenMesh::Vec2f> point2dSet;
    for(OpenMesh::Vec3f point3d : points3d) {
        point2dSet.push_back(OpenMesh::Vec2f(point3d[0], point3d[1]));
    }
    return point2dSet;
}

void FootProjector::setRange(double top, double bottom)
{
    this->rangeTop = top;
    this->rangeBottom = bottom;
}

Mesh *FootProjector::getModel() const
{
    return model;
}

void FootProjector::setModel(Mesh *value)
{
    model = value;
}
