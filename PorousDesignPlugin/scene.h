#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <algorithm>
using namespace std;

#include "QMesh"

class Scene
{
public:
    Scene();

private:
    vector<QAbstractMesh *> m_objects;
};

#endif // SCENE_H
