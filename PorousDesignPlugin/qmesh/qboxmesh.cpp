#include "qboxmesh.h"

QBoxMesh::QBoxMesh()
    :QAbstractMesh ()
{

}

QBoxMesh::QBoxMesh(Eigen::AlignedBox3d bbox)
    :QAbstractMesh ()
{

}

QBoxMesh::~QBoxMesh()
{

}

void QBoxMesh::draw()
{

}

void QBoxMesh::setPos(Eigen::Vector3d pos)
{

}

Eigen::Vector3d QBoxMesh::getPos()
{
    return Eigen::Vector3d(0.0,0.0,0.0);
}

void QBoxMesh::setRotation(Eigen::Vector3d rotation)
{

}

Eigen::Vector3d QBoxMesh::getRotation()
{
    return Eigen::Vector3d(0.0,0.0,0.0);
}

void QBoxMesh::setScale(Eigen::Vector3d scale)
{

}

Eigen::Vector3d QBoxMesh::getScale()
{
    return Eigen::Vector3d(0.0,0.0,0.0);
}
