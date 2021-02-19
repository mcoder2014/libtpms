#ifndef MESHCUTTING_H
#define MESHCUTTING_H

#include <vector>
#include <string>
#include <iostream>

#include "mesh.h"

class MeshCutting
{
public:
    MeshCutting();

    // Set the plane to cut the mesh
    void setCuttingPlane(OpenMesh::Vec3f v0,
                         OpenMesh::Vec3f v1,
                         OpenMesh::Vec3f v2);
    void setCuttingPlane(float a0, float a1, float a2, float a3);
    std::vector<float> cuttingPlane();

    bool loadMesh(const std::string& filepath);
    Mesh *mesh(){return m_model;}

    void cutting();
    std::vector<std::vector<OpenMesh::Vec3f> > *getCuttingResult(){
        return &this->cuttingPoints;
    }

    std::vector<OpenMesh::Vec3f> faceIntersectionCuttingPlane(Mesh::FaceHandle faceHandle);
    std::vector<std::vector<OpenMesh::Vec2f> > *getCuttingPoints2d();
    void printCuttingResult();

private:
    // the param of y = a0x + a1y + a2z + a3;
    float m_a0, m_a1, m_a2, m_a3;

    float m_xmax, m_ymax, m_zmax, m_xmin, m_ymin, m_zmin;

    Mesh *m_model;              // the loaded model

    // The intersection points of cutting plane and the 3D model
    // Cause there may be more than one circle, so we use 2-d vector
    std::vector<std::vector<OpenMesh::Vec3f>> cuttingPoints;

    // The cutting points project on the cutting plane
    std::vector<std::vector<OpenMesh::Vec2f>> cuttingPoints2d;

    std::vector<float> cuttingVertex;

    // According to the function of cutting plane
    float functionCuttingPlane(OpenMesh::Vec3f& point);
    void adddIntersectionPoints(std::vector<OpenMesh::Vec3f>& points);
    OpenMesh::Vec3f lineCuttingPlane(OpenMesh::Vec3f& v0, OpenMesh::Vec3f& v1);

    void printCuttingPlane();   // output the function of cutting plane to the console.

   std::vector<unsigned int> debug_case;

};

#endif // MESHCUTTING_H
