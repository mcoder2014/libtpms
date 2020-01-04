#ifndef QBOXMESH_H
#define QBOXMESH_H

#include <Eigen/Dense>

#include "qabstractmesh.h"


class QBoxMesh : public QAbstractMesh
{
public:
    QBoxMesh();
    QBoxMesh(Eigen::AlignedBox3d bbox);

    ~QBoxMesh();

    virtual QString type(){return "QBoxMesh";}
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

#endif // QBOXMESH_H
