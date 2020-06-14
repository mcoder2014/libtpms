#include "MeshSmoothTool.h"

#include <iostream>
#include <Eigen/Dense>

void MeshSmoothTool::basicSmooth(Mesh &mesh, int rounds)
{
    // 深拷贝两份
    Mesh *tempMesh = new Mesh;
    Mesh *resultMesh = new Mesh;

    resultMesh->assign(mesh);
    tempMesh->assign(mesh);

    for (int round = 0; round < rounds; round++)
    {
        // Every round, exchange the pointer of m_object and m_result
        // reduce the copy of mesh

        Mesh::VertexIter vIt, vEnd(resultMesh->vertices_end());

        // To get the value into result object
        Mesh::VertexIter vResultIt(tempMesh->vertices_begin());

        Mesh::VertexVertexIter vvIt;
        Mesh::VertexFaceIter vfIt;
        Mesh::Point cog;        // To record the gravity of the point

        int valance = 0;            // Record the degree of the point
        int pointsCount = 0;

        /// 计算每个顶点的目标坐标
        for (vIt = resultMesh->vertices_begin(); vIt != vEnd; ++vIt, ++vResultIt)
        {
            // 决定边界的点要不要平滑一下
            if(!resultMesh->is_boundary(*vIt))
            {
                // Don't change the position of boundary vertex
                cog = Mesh::Point(0.0, 0.0, 0.0); // Init it into origin point
                valance = 0;

                for (vfIt = resultMesh->vf_begin(*vIt);
                     vfIt.is_valid();vfIt++)
                {
                    Mesh::Point gravity(0.0, 0.0, 0.0);
                    Mesh::FaceVertexIter fvIt;

                    for (fvIt = resultMesh->fv_begin(*vfIt);
                         fvIt.is_valid();fvIt++)
                    {
                        gravity += resultMesh->point(*fvIt);
                    }
                    gravity = gravity/3;
                    cog += gravity;
                    valance++;
                }
                pointsCount ++;
                cog = cog / static_cast<float>(valance);

                // Record the calculated value
                 tempMesh->set_point(*vResultIt, cog);


            } else {
                // 对于边界点，只和相邻边界点进行平滑
                cog = Mesh::Point(0.0, 0.0, 0.0); // Init it into origin point
                valance = 0;
                for(vvIt = resultMesh->vv_begin(*vIt); vvIt.is_valid(); ++vvIt) {
                    if(resultMesh->is_boundary(*vvIt)) {
                        valance++;
                        cog += resultMesh->point(*vvIt);
                    }
                }
                cog /= valance;
                cog = 0.5 * cog + 0.5 * resultMesh->point(*vIt);

                // Record the calculated value
                tempMesh->set_point(*vIt, cog);
            }
        }

        std::cout << "Valuable round: " << round
                  <<"\nPoints count: " << pointsCount << std::endl;

        // Exchange the pointer of m_object and m_result
        {
            Mesh *ptr = resultMesh;
            resultMesh = tempMesh;
            tempMesh = ptr;
        }
    }

    // 保存 Smooth 结果
    mesh.assign(*resultMesh);

    delete tempMesh;
    delete resultMesh;
}
