#ifndef QABSTRACTMESH_H
#define QABSTRACTMESH_H

#include <QString>

#include <Eigen/Dense>

#include "SurfaceMeshModel.h"
#include "Starlab.h"

using namespace SurfaceMesh;

class QAbstractMesh
{
public:
    QAbstractMesh();
    virtual ~QAbstractMesh();

    // Get type name
    virtual QString type() = 0;
    virtual QString name() = 0;
    virtual void draw() = 0;

    // Id
    virtual int getId(){return m_id;}

    // Position
    virtual void setPos(Eigen::Vector3d pos) = 0;
    virtual Eigen::Vector3d getPos() = 0;

    // Rotation
    virtual void setRotation(Eigen::Vector3d rotation) = 0;
    virtual Eigen::Vector3d getRotation() = 0;

    // Size
    virtual void setScale(Eigen::Vector3d scale) = 0;
    virtual Eigen::Vector3d getScale() = 0;

    // Select
    virtual void setSelected(bool select){this->m_selected = select;}
    virtual bool getSelected(){return this->m_selected;}


protected:
    SurfaceMeshModel * m_boundary;  // 边界模型
    SurfaceMeshModel * m_porous;    // 多孔模型
    bool m_selected;                // 是否被选中

    Eigen::Matrix4d m_transtion;
    int m_id;           // Used for differing the element of the handle
    static int m_count; // A count to auto plus the id of Surface Handle

};

#endif // QABSTRACTMESH_H
