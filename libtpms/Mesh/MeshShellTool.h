#ifndef MESHSHELLTOOL_H
#define MESHSHELLTOOL_H

#include "Mesh.h"

class MeshShellTool
{
public:
    void offset(Mesh& mesh, double depth);

private:
    void duplicate(Mesh &mesh);
    void fixHole(Mesh &mesh);
    void verticeMoveNormalDirection(Mesh &mesh, double distance);
};

#endif // MESHSHELLTOOL_H
