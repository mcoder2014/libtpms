#ifndef EXPORTER_H
#define EXPORTER_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#include <glm/glm.hpp>

#include "mesh.h"

class Exporter
{
public:
    Exporter();

    void writeOBJ(const std::string & filename,
                  const std::vector<glm::vec3>& vertices,
                  const std::vector<glm::ivec3>& faces);

    void writeOBJ(const std::string & filename,
                  const Mesh& mesh);

};

#endif // EXPORTER_H
