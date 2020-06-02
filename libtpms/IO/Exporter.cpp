#include "Exporter.h"

///
/// \brief Exporter::writeOBJ
/// \param filename obj ply
/// \param mesh
///
void Exporter::writeOBJ(const std::string &filename, const Mesh &mesh)
{
    OpenMesh::IO::Options option;
    option = OpenMesh::IO::Options::Binary;
    if(filename.size() == 0)
    {
        std::cerr << "filename empty." << std::endl;
        return;
    }

    std::cout << "Mesh has vertices: " << mesh.n_vertices()
              << " faces: " << mesh.n_faces() << std::endl;

    if(!OpenMesh::IO::write_mesh(mesh, filename, option))
    {
        std::cerr << "Cannot write mesh into file "
                  << filename
                  << std::endl;
        return;
    }

    std::cout << "OpenMesh saving origin mesh into file:\t" << filename << std::endl;

}
