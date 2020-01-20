#ifndef QUALITYINSPECTION_H
#define QUALITYINSPECTION_H

#include "smoothtool.h"

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
