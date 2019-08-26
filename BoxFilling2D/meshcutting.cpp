#include "meshcutting.h"

#include <limits>

MeshCutting::MeshCutting()
    :debug_case(8,0)
{
    m_model = nullptr;
    m_a0 = m_a1 = m_a2 = m_a3 = 0.0f;
}

///
/// \brief MeshCutting::setCuttingPlane
/// \param v0
/// \param v1
/// \param v2
/// Get the plane function from three points.
/// They should not at the same point or same line!
///
void MeshCutting::setCuttingPlane(OpenMesh::Vec3f v0, OpenMesh::Vec3f v1, OpenMesh::Vec3f v2)
{
    OpenMesh::Vec3f v01 = v1 - v0;
    OpenMesh::Vec3f v02 = v2 - v0;

    OpenMesh::Vec3f normal = v01 % v02;
    normal.normalize();     // normallized to length 1

    m_a0 = normal[0];
    m_a1 = normal[1];
    m_a2 = normal[2];
    m_a3 = -(normal|v0);    // dot product

    printCuttingPlane();
}

///
/// \brief MeshCutting::setCuttingPlane
/// \param a0
/// \param a1
/// \param a2
/// \param a3
///
void MeshCutting::setCuttingPlane(float a0, float a1, float a2, float a3)
{
    m_a0 = a0;
    m_a1 = a1;
    m_a2 = a2;
    m_a3 = a3;

    printCuttingPlane();
}

///
/// \brief MeshCutting::cuttingPlane
/// \return
///
std::vector<float> MeshCutting::cuttingPlane()
{
    return std::vector<float>{m_a0, m_a1, m_a2, m_a3};
}

///
/// \brief MeshCutting::loadMesh
/// \param filepath
/// \return
/// load the insole model
///
bool MeshCutting::loadMesh(const std::string &filepath)
{
    if(m_model == nullptr)
        m_model = new Mesh;
    else {
        delete m_model;
        m_model = new Mesh;
    }

    if(!OpenMesh::IO::read_mesh(*m_model, filepath))
    {
        return false;
    }

    std::cout << "Vertices: " << m_model->n_vertices() << std::endl;
    std::cout << "Edges: " << m_model->n_edges() << std::endl;
    std::cout << "Faces: " << m_model->n_faces() << std::endl;

    // Calculate the bounding box
    m_xmax = m_ymax = m_zmax = std::numeric_limits<float>::min();
    m_xmin = m_ymin = m_zmin = std::numeric_limits<float>::max();

    Mesh::VertexIter    v_it, v_end(m_model->vertices_end());
    for (v_it=m_model->vertices_begin(); v_it!=v_end; ++v_it)
    {
        // for each point
        OpenMesh::Vec3f point = m_model->point(*v_it);

        if(point[0] > m_xmax) m_xmax = point[0];
        if(point[0] < m_xmin) m_xmin = point[0];

        if(point[1] > m_ymax) m_ymax = point[1];
        if(point[1] < m_ymin) m_ymin = point[1];

        if(point[2] > m_zmax) m_zmax = point[2];
        if(point[2] < m_zmin) m_zmin = point[2];
    }

    std::cout << "Bounding Box:\tMax\tMin\n" << "X:\t" << m_xmax << "\t" << m_xmin << "\n"
              << "Y:\t" << m_ymax << "\t" << m_ymin << "\n"
              << "Z:\t" << m_zmax << "\t" << m_zmin << std::endl;

    return true;
}

///
/// \brief MeshCutting::cutting
///     Start cutting circle
///     Save results in the 2-d vector
///
void MeshCutting::cutting()
{
    // Clear old results
    this->m_cuttingPoints.clear();
    this->m_cutting_function_vertex.clear();

    if(m_model == nullptr)
    {
        std::cerr << "Stop cutting! The point of the model is nullptr"
                  << std::endl;
        return;
    }

    // Rename as a regional variable
    Mesh& mesh = *m_model;

    {
        // Calculate f(vi), cause the same point need to be calculate multi-times
        int i =0;
        Mesh::VertexIter v_it, v_end(mesh.vertices_end());
        for (v_it=mesh.vertices_begin(); v_it != v_end; ++v_it, ++i)
        {
            mesh.data(*v_it).id = i;
            this->m_cutting_function_vertex.push_back(
                        functionCuttingPlane(mesh.point(*v_it)));
        }
    }

    // Processing all face to get the intersection result
    Mesh::FaceIter f_it, f_end(mesh.faces_end());
    for (f_it = mesh.faces_begin(); f_it != f_end; ++f_it)
    {
        if(!mesh.data(*f_it).checked)
        {
            // To prevent the repeated calculation
            // Check the face itself
            std::vector<OpenMesh::Vec3f> intersection = face_intersection_cutting_plane(*f_it);

            if(intersection.size() > 0)
            {
                // If we find a face intersection with the cutting plane,
                // check its neighbours.

                // Record the result. Add it into result list <Prevent the repeated points>
                // Add new vector of points to store the new circle of intersection points
                this->m_cuttingPoints.push_back(std::vector<OpenMesh::Vec3f>());
                this->adddIntersectionPoints(intersection); // add intersection points into vector

                // Record the current processing face
                Mesh::FaceHandle intersection_face = *f_it;

                while(intersection_face.is_valid())
                {
                    // check the neighbour faces
                    // if the neighbour is cutting, exchange the iter to it
                    Mesh::FaceFaceIter ff_it;
                    for (ff_it = mesh.ff_begin(intersection_face); ff_it.is_valid(); ++ff_it)
                    {
                        if(!mesh.data(*ff_it).checked)
                        {
                            std::vector<OpenMesh::Vec3f> neighbour_intersection =
                                    face_intersection_cutting_plane(*ff_it);
                            if(neighbour_intersection.size() > 0)
                            {
                                this->adddIntersectionPoints(neighbour_intersection);
                                // Take this face as next circle object
                                intersection_face = *ff_it;
                                break;
                            }
                        }
                    }

                    // if no neighbour is cutting, Reset the handle to invalid state
                    if(!ff_it.is_valid())
                        intersection_face.reset();
                }
            }    // if(intersection.size() > 0)
        }    // if(!mesh.data(*f_it).checked)
    }    // for (f_it = mesh.faces_begin(); f_it != f_end; ++f_it)

    // Get the intersections between model and cutting plane

}

///
/// \brief MeshCutting::face_intersection_cutting_plane
/// \param f_it
/// \return
/// Get the intersection points from one face and cutting plane
///
std::vector<OpenMesh::Vec3f> MeshCutting::face_intersection_cutting_plane(Mesh::FaceHandle faceHandle)
{
    std::vector<OpenMesh::Vec3f> results;
    Mesh &mesh = *m_model;
    mesh.data(faceHandle).checked = true;    // Mark as checked

    // Processing the three vertexes of the face
    Mesh::FaceVertexIter fv_it;
    std::vector<OpenMesh::Vec3f> triangle;
    std::vector<float> function_value;

    for(fv_it = mesh.fv_iter(faceHandle); fv_it.is_valid(); ++fv_it)
    {
        // Calculate the three vertexes of the face
        OpenMesh::Vec3f &point = mesh.point(*fv_it);
        // Not need calculate every time
        triangle.push_back(point);
        function_value.push_back(
                    m_cutting_function_vertex[
                        static_cast<unsigned long>(mesh.data(*fv_it).id)]);

    }

    ///
    /// Sort the three points, to reduce the size of possibilities
    ///
    {
        float tmp;
        OpenMesh::Vec3f tmpp;
        if(function_value[0] > function_value[1])
        {
            tmp = function_value[1];
            function_value[0] = function_value[1];
            function_value[1] = tmp;

            tmpp = triangle[0];
            triangle[0] = triangle[1];
            triangle[1] = tmpp;
        }
        if(function_value[1] > function_value[2])
        {
            tmp = function_value[1];
            function_value[1] = function_value[2];
            function_value[2] = tmp;

            tmpp = triangle[1];
            triangle[1] = triangle[2];
            triangle[2] = tmpp;
        }
        if(function_value[0] > function_value[1])
        {
            tmp = function_value[1];
            function_value[0] = function_value[1];
            function_value[1] = tmp;

            tmpp = triangle[0];
            triangle[0] = triangle[1];
            triangle[1] = tmpp;
        }
    }

    ///
    /// To Find the intersection points of the face and the cutting plane
    /// 6 case:
    /// 1. Not intersection
    /// 2. one vertex is in the cutting plane
    /// 3. one vertex is in the cutting plane and the opposite edge is intersected
    /// 4. one edge is in the cutting plane
    /// 5. all edges is in the cutting plane
    /// 6. two edges intersected
    ///

    /// Debug



    if(function_value[0] > 0 || function_value[2] < 0)
    {
        // They have the same positive and negtive sigh
        // Not intersection with the cutting face
        debug_case[0]++;
        return results;
    }
    else if(fabsf(function_value[0]) < 1e-6f
            && function_value[1] > 1e-6f)
    {
        // only one vertex in the face
        // and the vertex is the intersected point
        results.push_back(triangle[0]);
        debug_case[1]++;
        return results;
    }
    else if(function_value[0] < -1e-6f
            && fabsf(function_value[1]) < 1e-6f
            && function_value[2] > 1e-6f)
    {
        // one vertex in the face
        // the opposite edge intersects with the cutting face
        results.push_back(triangle[1]);
        results.push_back(lineCuttingPlane(triangle[0], triangle[2]));
        debug_case[2]++;
        return results;
    }
    else if ((fabsf(function_value[0]) < 1e-6f
              && fabsf(function_value[1]) < 1e-6f
              && function_value[2] > 1e-6f))
    {
        // Case 4
        results.push_back(triangle[0]);
        results.push_back(triangle[1]);
        debug_case[3]++;
        return results;
    }
    else if(fabsf(function_value[1]) < 1e-6f
            && fabsf(function_value[2]) < 1e-6f
            && function_value[0] < 1e-6f)
    {
        // Case 4
        results.push_back(triangle[1]);
        results.push_back(triangle[2]);
        debug_case[3]++;
        return results;
    }
    else if(fabsf(function_value[0]) < 1e-6f
            && fabsf(function_value[1]) < 1e-6f
            && fabsf(function_value[2]) < 1e-6f)
    {
        // Case 5
        // All edges and vertexes are in the cutting plane
        // This may cause error
        results.push_back(triangle[0]);
        results.push_back(triangle[1]);
        results.push_back(triangle[2]);
        debug_case[4]++;
        return results;
    }
    else if(function_value[0] < 1e-6f
            && function_value[1] > 1e-6f)
    {
        // Case 6
        results.push_back(lineCuttingPlane(triangle[0], triangle[1]));
        results.push_back(lineCuttingPlane(triangle[0], triangle[2]));
        debug_case[5]++;
        return results;
    }
    else if (function_value[1] < 1e-6f
             && function_value[2] > 1e-6f)
    {
        // Case 6
        results.push_back(lineCuttingPlane(triangle[2], triangle[0]));
        results.push_back(lineCuttingPlane(triangle[2], triangle[1]));
        debug_case[5]++;
        return results;
    }

    return results;
}

void MeshCutting::printCuttingResult()
{
    // Output the basic info
    std::cout << "Total circle: " << this->m_cuttingPoints.size() << std::endl;

    // Output the detail info
    for(unsigned long i=0; i < this->m_cuttingPoints.size(); i++)
    {
        std::cout << "Circle " << i
                  << "Points: " << this->m_cuttingPoints[i].size()
                  << std::endl;
        for (unsigned long j =0; j<this->m_cuttingPoints[i].size(); j++)
        {
            std::cout << "\tPoint " << j << "\t"
                      << this->m_cuttingPoints[i][j]
                      << std::endl;
        }
    }

    std::cout << "Cases count: \n";
    for (unsigned long i=0; i<6; i++)
    {
        std::cout << "Case " << i
                  << "\t" << debug_case[i] << "\n";
    }
}

///
/// \brief MeshCutting::functionCuttingPlane
/// \param point
/// \return
///
float MeshCutting::functionCuttingPlane(OpenMesh::Vec3f &point)
{
   float x = point[0], y = point[1], z = point[2];
   return m_a0 * x + m_a1 * y + m_a2 * z + m_a3;
}

///
/// \brief MeshCutting::adddIntersectionPoints
/// \param points
///     Add points into results, prevent repeated points
void MeshCutting::adddIntersectionPoints(std::vector<OpenMesh::Vec3f> &points)
{
    if(points.size() == 0)
        return;

    if(this->m_cuttingPoints.size() == 0)
        this->m_cuttingPoints.push_back(std::vector<OpenMesh::Vec3f>());

    std::vector<OpenMesh::Vec3f> &circle =
            this->m_cuttingPoints.at(this->m_cuttingPoints.size()-1);

    if(circle.size() == 0) {
        for(auto point : points)
            circle.push_back(point);
        return;
    }

    // If there were repeated points
    // Only remove one repeated point
    OpenMesh::Vec3f last_point = circle[circle.size()-1];
    for(std::vector<OpenMesh::Vec3f>::iterator iter = points.begin();
            iter != points.end(); iter++)
    {
        if(*iter == last_point)
        {
            iter = points.erase(iter);
        }
    }

    for(auto point : points)
        circle.push_back(point);
}

///
/// \brief MeshCutting::lineCuttingPlane
/// \param v0
/// \param v1
/// \return
///     The 2 points must be seperated by the plane
///
OpenMesh::Vec3f MeshCutting::lineCuttingPlane(OpenMesh::Vec3f &v0, OpenMesh::Vec3f &v1)
{
    float t = -(m_a3 + m_a0 * v0[0] + m_a1 * v0[1] + m_a2 * v0[2])
                / (m_a0 * v1[0] + m_a1 * v1[1] + m_a2 * v1[2]);

    OpenMesh::Vec3f intersectionPoint(v0[0] + v1[0] * t, v0[1] + v1[1] * t, v0[2] + v1[2] * t);
    return intersectionPoint;
}

void MeshCutting::printCuttingPlane()
{
    std::cout << "Cutting plane: "
              << "F(x,y,z) = " << m_a0 << " * x + "
              << m_a1 << "* y + "
              << m_a2 << "* z + "
              << m_a3 << std::endl;
}
