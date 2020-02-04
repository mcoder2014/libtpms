#ifndef SMOOTHTOOL_H
#define SMOOTHTOOL_H

#include <vector>
#include <string>

#include <glm/glm.hpp>

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>

#include "mesh.h"

class SmoothTool
{
public:
    SmoothTool();
    ~SmoothTool();

    // Use the results from Marchbox algorithm to create
    // a openmesh object
    Mesh* createMesh(std::vector<glm::vec3>& vertices,
                    std::vector<glm::ivec3>& faces);
    Mesh* createMesh(std::string filepath);
    Mesh* createMesh(Mesh* mesh);

    Mesh* meshOffset(Mesh* mesh, float depth);  // 将单层曲面变成有厚度的立体

    // Basic smooth algorithm, store in m_result
    void basicSmooth(int rounds=1);
    void jacobiLaplaceSmooth(int rounds = 1);   // 效果太弱鸡了

    // Save the smoothed mesh into obj file.
    void writeOBJ(std::string const & file_origin,
                  std::string const & file_result);

    inline Mesh *getMesh(){return m_object;}
    inline Mesh *getMeshDeep();

private:
    Mesh *m_object;     // the origin openmesh object
    Mesh *m_backup;     // the backup of the smooth tool
    Mesh *m_result;     // the result of smooth
};

#endif // SMOOTHTOOL_H
