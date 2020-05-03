#ifndef MESHSMOOTHTOOL_H
#define MESHSMOOTHTOOL_H

#include "Mesh.h"

class MeshSmoothTool
{
public:
    // Basic smooth algorithm, store in m_result
    void basicSmooth(Mesh &mesh, int rounds=1);
};

#endif // MESHSMOOTHTOOL_H
