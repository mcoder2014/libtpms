#ifndef POROSITYCALCULATOR_H
#define POROSITYCALCULATOR_H

#include "Mesh.h"

class PorosityCalculator
{
public:
    double getPorsity(Mesh& porousStructure, Mesh& boundary);
    double getVolume(Mesh& mesh);
};

#endif // POROSITYCALCULATOR_H
