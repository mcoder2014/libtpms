#ifndef MESHINFOCHECKER_H
#define MESHINFOCHECKER_H

#include "Mesh.h"

#include <iostream>
#include <memory>

#include <Eigen/Dense>

class MeshInfo;

class MeshInfoChecker
{
public:
    MeshInfoChecker();

    // 对模型信息做全面检查
    std::shared_ptr<MeshInfo> check(Mesh& mesh);

    // 检查是否有空洞
    bool checkHole(Mesh& mesh);

    // 检查模型一些统计信息
    std::shared_ptr<MeshInfo> checkSize(Mesh& mesh);

    // 查询模型信息
    std::shared_ptr<MeshInfo> getMeshinfo() const;

private:
    std::shared_ptr<MeshInfo> meshinfo;
};

/**
 * @brief The MeshInfo class
 * 一个存储网格信息的数据结构
 */
class MeshInfo
{
public:
    size_t faceNum;
    size_t verticesNum;
    size_t edgeNum;
    bool hasHole;
    Eigen::AlignedBox3d boundingBox;

    MeshInfo()
        :faceNum(0), verticesNum(0), edgeNum(0),hasHole(false)
    {}

    /**
     * @brief print 打印网格模型信息
     */
    void print()
    {
        std::cout << "Mesh Info:\n"
                  << "Face Count:\t" << faceNum << "\n"
                  << "Edge Count:\t" << edgeNum << "\n"
                  << "Vertices Count:\t" << verticesNum << "\n"
                  << "Has Hole:\t" << (hasHole?"true":"false") << "\n"
                  << "BoundingBox: Min:[" << boundingBox.min().x() << ","
                  << boundingBox.min().y() << "," << boundingBox.min().z() << "]\n"
                  << "             Max:[" << boundingBox.max().x() << ","
                  << boundingBox.max().y() << "," << boundingBox.max().z() << "]\n";
    }
};

#endif // MESHINFOCHECKER_H
