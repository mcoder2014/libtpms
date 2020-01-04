#ifndef PARAMETER_H
#define PARAMETER_H

#include <QRgb>

enum OBJECT_DRAW_TYPE
{
    MESH_TYPE = 0,
    WIRE_TYPE = 1,
    VERTICES_TYPE = 2,
    NONE_DRAW = 3
};

class QMeshRenderParameter
{
public:
    OBJECT_DRAW_TYPE drawType;
    QRgb color;
    bool bbox;

};

#endif // PARAMETER_H
