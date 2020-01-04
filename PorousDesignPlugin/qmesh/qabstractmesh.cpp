#include "qabstractmesh.h"

int QAbstractMesh::m_count = 0;

QAbstractMesh::QAbstractMesh()
    : m_boundary(nullptr), m_porous(nullptr), m_id(m_count++)
{

}

QAbstractMesh::~QAbstractMesh()
{

}
