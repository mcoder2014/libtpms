#include "surfacemesh_load.h"

SurfaceMeshLoader::SurfaceMeshLoader()
{

}

bool SurfaceMeshLoader::load(SurfaceMesh::SurfaceMeshModel &model, std::string path)
{
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(
                path,aiProcess_Triangulate);
    if(scene == nullptr)
    {
        std::cout << "Model file " << path << " failed.\n";
        return false;
    }
    if(scene->mNumMeshes == 0)
    {
        std::cout << "Model counts < 0\n";
        return false;
    }

    // Read mesh
    aiMesh ** meshes = scene->mMeshes;      // meshes数组
    aiMesh * aimesh = meshes[0];            // Only load the first mesh;

    std::cout << "faces: " << aimesh->HasFaces() << "\t"
              << aimesh->mNumFaces << "\n"
              << "vertices: " << aimesh->mNumVertices << std::endl;

    // Add vertex and faces into the mesh
    size_t length_vertices = aimesh->mNumVertices;
    size_t length_faces = aimesh->mNumFaces;
    aiVector3D *aivertices = aimesh->mVertices;
    aiFace *aifaces = aimesh->mFaces;

    // For most common 3D model format
    clock_t start_time = clock();
    std::vector<SurfaceMesh::Vertex> face;  // Used to add each face

    // Add vertex
    for (size_t i=0; i < length_vertices; ++i)
    {

        Eigen::Vector3d vi(static_cast<double>(aivertices[i].x),
                           static_cast<double>(aivertices[i].y),
                           static_cast<double>(aivertices[i].z));
        model.add_vertex(vi);
    }

    // Add face
    for (size_t i=0; i < length_faces; ++i)
    {
        face.clear();
        for (size_t vec_indices=0; vec_indices < aifaces[i].mNumIndices; ++vec_indices)
        {
            face.push_back(
                        SurfaceMesh::Vertex(
                            static_cast<int>(aifaces[i].mIndices[vec_indices])));
        }

        model.add_face(face);
    }

    clock_t end_time = clock();
    std::cout << "Assimp loading approach costs "
              << 1.0 * (end_time-start_time)/CLOCKS_PER_SEC
              << "S" << std::endl;
    return true;
}
