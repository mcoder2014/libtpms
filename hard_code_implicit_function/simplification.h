#ifndef SIMPLIFICATION_H
#define SIMPLIFICATION_H

#include "smoothtool.h"

class Simplification
{
public:
    Simplification();
    ~Simplification();
    Mesh* createMesh(std::vector<glm::vec3>& vertices,
                    std::vector<glm::ivec3>& faces);
    Mesh* createMesh(Mesh * mesh);

    inline Mesh *getMesh(){return m_object;}

    Mesh* decimater(float factor);
    Mesh* decimater_to(float factor);
    Mesh* decimater(int n_collapses);
    Mesh* decimater_to(int n_target);
private:
    Mesh *m_object;
    int m_srcVecCount;  // the source vecCount of the mesh
    int m_dstVecCount;  // the destination
};

#endif // SIMPLIFICATION_H
