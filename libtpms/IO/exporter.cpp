#include "exporter.h"

Exporter::Exporter()
{

}

void Exporter::writeOBJ(const std::string &filename,
                        const std::vector<glm::vec3> &vertices,
                        const std::vector<glm::ivec3> &faces)
{
    std::cout << "Writing OBJ file" << std::endl;
    // check if we actually have an ISO surface
    if(vertices.size () == 0 || faces.size() == 0) {
        std::cout << "No ISO surface generated. Skipping OBJ generation." << std::endl;
        return;
    }

    // open output file
    std::ofstream file(filename);
    if(!file) {
        std::cout << "Error opening output file" << std::endl;
        return;
    }

    std::cout << "Generating OBJ mesh with " << vertices.size() << " vertices and "
              << faces.size() << " faces" << std::endl;

    // write vertices
    for(auto const & v : vertices) {
        file << "v " << v.x << ' ' << v.y << ' ' << v.z << '\n';
    }

    // write face indices
    for(auto const & f : faces) {
        file << "f " << (f.x + 1) << ' ' << (f.y + 1) << ' ' << (f.z + 1) << '\n';
    }

    file.close();
}

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
