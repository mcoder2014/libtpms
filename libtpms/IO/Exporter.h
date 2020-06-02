#ifndef EXPORTER_H
#define EXPORTER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include "Mesh/Mesh.h"

class Exporter
{
public:
    void writeOBJ(const std::string & filename,
                  const Mesh& mesh);

};

#endif // EXPORTER_H
