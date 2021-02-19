#ifndef FOOTPROJECTOR_H
#define FOOTPROJECTOR_H

#include <vector>

#include "mesh.h"

using std::vector;

class FootProjector
{
public:
    FootProjector();
    ~FootProjector(){}

    vector<OpenMesh::Vec2f> getPoints2d();
    vector<OpenMesh::Vec3f> getPoints3d();
    vector<OpenMesh::Vec2f> points3d2points2d(vector<OpenMesh::Vec3f>& points3d);

    void setRange(double top, double bottom);

    Mesh *getModel() const;
    void setModel(Mesh *value);

private:
    // foot mesh, weak pointer
    Mesh *model;
    double rangeTop;
    double rangeBottom;

};

#endif // FOOTPROJECTOR_H
