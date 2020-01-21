#include "importer.h"

Importer::Importer()
{

}

bool Importer::load(SurfaceMesh::SurfaceMeshModel &model, const std::string path)
{
    model.clear();  // remove all vertices and faces

    // Import new model using Assimp
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

SurfaceMesh::SurfaceMeshModel *Importer::loadSurfaceMeshModel(const std::string path)
{
    SurfaceMesh::SurfaceMeshModel *model = new SurfaceMesh::SurfaceMeshModel();
    if(load(*model, path))
    {
        return model;
    }
    else
    {
        delete model;
        return nullptr;
    }
}

Mesh *Importer::array2mesh(const std::vector<glm::vec3> &vertices, const std::vector<glm::ivec3> &faces)
{
    if(vertices.size() <= 3 || faces.size() < 1)
        return nullptr;

    Mesh *m_object = new Mesh;    // Create new Mesh

    // Used for add face
    std::vector<Mesh::VertexHandle> vHandle(vertices.size());

    // Add vertices
    for (size_t i = 0; i < vertices.size(); i++)
    {

        vHandle[i] = m_object->add_vertex(
                    Mesh::Point(
                        vertices[i][0],
                        vertices[i][1],
                        vertices[i][2]));
    }

    // Add faces
    std::vector<Mesh::VertexHandle> face_vhandles;

    for (glm::vec3 face : faces)
    {
        face_vhandles.clear();

        face_vhandles.push_back(vHandle[face[0]]);
        face_vhandles.push_back(vHandle[face[1]]);
        face_vhandles.push_back(vHandle[face[2]]);

        m_object->add_face(face_vhandles);
    }

    return m_object;
}

SurfaceMesh::SurfaceMeshModel *Importer::array2SurfaceMeshModel(
        const std::vector<glm::vec3> &vertices, const std::vector<glm::ivec3> &faces)
{
    SurfaceMesh::SurfaceMeshModel *model = new SurfaceMesh::SurfaceMeshModel();

    // vertex
    for(const glm::vec3& vec: vertices)
    {
        Eigen::Vector3d vi(static_cast<double>(vec[0]),
                           static_cast<double>(vec[1]),
                           static_cast<double>(vec[2]));
        model->add_vertex(vi);
    }

    // face
    std::vector<SurfaceMesh::Vertex> vface;  // Used to add each face
    for(const glm::ivec3& face:faces)
    {
        vface.clear();

        vface.push_back(SurfaceMesh::Vertex(face[0]));
        vface.push_back(SurfaceMesh::Vertex(face[1]));
        vface.push_back(SurfaceMesh::Vertex(face[2]));

        model->add_face(vface);
    }

    return model;
}

SurfaceMesh::SurfaceMeshModel *Importer::mesh2SurfaceMeshModel(const Mesh &mesh)
{
    // TODO
    return nullptr;
}

Mesh *Importer::surfaceMeshModel2mesh(const SurfaceMesh::SurfaceMeshModel &model)
{
    // TODO
    return nullptr;
}
