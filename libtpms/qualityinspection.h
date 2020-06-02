#ifndef QUALITYINSPECTION_H
#define QUALITYINSPECTION_H

#include "Mesh/Mesh.h"

class QualityInspection
{
public:
    QualityInspection();

    bool check(Mesh *mesh);
    bool checkHole(Mesh *mesh);

private:
    Mesh *m_mesh;
};

#endif // QUALITYINSPECTION_H
