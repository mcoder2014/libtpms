#ifndef QTRIANGLEMESH_H
#define QTRIANGLEMESH_H

#include "qabstractmesh.h"

class QTriangleMesh: public QAbstractMesh
{
public:
    QTriangleMesh();
    ~QTriangleMesh();

    virtual QString type(){return "QTriangleMesh";}
    virtual QString name(){return type();}
    virtual void draw();

    // Id
    virtual int getId(){return m_id;}

    // Position
    virtual void setPos(Eigen::Vector3d pos);
    virtual Eigen::Vector3d getPos();

    // Rotation
    virtual void setRotation(Eigen::Vector3d rotation);
    virtual Eigen::Vector3d getRotation();

    // Size
    virtual void setScale(Eigen::Vector3d scale);
    virtual Eigen::Vector3d getScale();

    // Select
    virtual void setSelected(bool select){this->m_selected = select;}
    virtual bool getSelected(){return this->m_selected;}
};

#endif // QTRIANGLEMESH_H
