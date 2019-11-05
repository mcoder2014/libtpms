#ifndef SMOOTHTOOL_H
#define SMOOTHTOOL_H

#include <vector>

#include <string>

#include <glm/glm.hpp>

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>

//#include "implicitsurface.h"



// OpenMesh Traits
struct Traits: public OpenMesh::DefaultTraits
{
public:
    VertexTraits
    {
        OpenMesh::Vec3f cog;    // center of gravity
    };
};

using Mesh = OpenMesh::PolyMesh_ArrayKernelT<Traits>;

class SmoothTool
{
public:
    SmoothTool();
    ~SmoothTool();


    // Use the results from Marchbox algorithm to create
    // a openmesh object
    bool createMesh(std::vector<glm::vec3>& vertices,
                    std::vector<glm::ivec3>& faces);
    bool createMesh(std::string filepath);

    // Basic smooth algorithm, store in m_result
    void basicSmooth();

    // Save the smoothed mesh into obj file.
    void writeOBJ(std::string const & file_origin,
                  std::string const & file_result);

private:
    Mesh *m_object;     // the origin openmesh object
    Mesh *m_backup;     // the backup of the smooth tool
    Mesh *m_result;     // the result of smooth
};

#endif // SMOOTHTOOL_H
