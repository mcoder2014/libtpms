#include "GridHexAlgorithm.h"

GridHexAlgorithm::GridHexAlgorithm()
{

}

std::shared_ptr<HexahedralMesh> GridHexAlgorithm::build()
{
    // 初始化
    initSampleMatrix();

    // 移除完全位于外部的六面体
    initHexahedronList();

    // 使用轮廓线、扫描线方法进行收缩
    scanLine();

    return this->hexahedralMesh;
}

shared_ptr<GridHexAlgorithmConfig> GridHexAlgorithm::getConfig() const
{
    return config;
}

void GridHexAlgorithm::setConfig(const shared_ptr<GridHexAlgorithmConfig> &value)
{
    config = value;
}

void GridHexAlgorithm::initSampleMatrix()
{
    shared_ptr<VoxelModel> voxelModel = config->voxelModel;
    double initSize = config->initSize;

    Eigen::AlignedBox3d boundingBox = config->voxelModel->getBoundingBox();
    int count = lround((boundingBox.max().x() - boundingBox.min().x()) / config->initSize) + 1;

    // 初始化 采样点矩阵
    vector<vector<vector<shared_ptr<HexSamplePoint>>>>
            initMatrix(
                count, vector<vector<shared_ptr<HexSamplePoint>>>(
                    count, vector<shared_ptr<HexSamplePoint>>(count)));

    this->hexSampleMatrix.swap(initMatrix);

    Eigen::Vector3i index(0,0,0);
    Eigen::Vector3d position(boundingBox.min());

    for(index.x() = 0, position.x() = boundingBox.min().x();
        index.x() < count;
        index.x()++,position.x() += initSize ) {

        for(index.y() = 0, position.y() = boundingBox.min().y();
            index.y() < count;
            index.y()++, position.y() += initSize) {

            for(index.z() = 0, position.z() = boundingBox.min().z();
                index.z() < count;
                index.z()++, position.z()+= initSize) {

                // 标记采样点坐标 以及是否位于 体素模型内部标示
                shared_ptr<HexSamplePoint> samplePoint = std::make_shared<HexSamplePoint>(position, index);
                samplePoint->inner = voxelModel->contains(samplePoint->physical);
                hexSampleMatrix[index.x()][index.y()][index.z()] = samplePoint;
            }
        }
    }
}

/**
 * @brief GridHexAlgorithm::initHexahedronList
 * 构建初始化的六面体列表
 */
void GridHexAlgorithm::initHexahedronList()
{
    // 新建新的六面体网格对象
    this->hexahedralMesh = std::make_shared<HexahedralMesh>();

    // 构建初始的 六面体模型
    Vector3i range;
    range.x() = hexSampleMatrix.size();
    range.y() = hexSampleMatrix[0].size();
    range.z() = hexSampleMatrix[0][0].size();

    // 检查是否需要构建立方体
    Vector3i index;
    for(index.x() = 0; index.x() < range.x()-1; index.x()++) {
        for(index.y() = 0; index.y() < range.y()-1; index.y() ++) {
            for(index.z() = 0; index.z() < range.z()-1; index.z() ++) {
                insertOneHexahedron(index);
            }
        }
    }
}

/**
 * @brief GridHexAlgorithm::insertOneHexahedron
 * 帮助函数：添加单个六面体到 mesh 中
 * @param index（注：范围小于 size()-1）
 */
void GridHexAlgorithm::insertOneHexahedron(Eigen::Vector3i index)
{
    if(!(hexSampleMatrix[index.x()  ][index.y()  ][index.z()  ]->inner
         || hexSampleMatrix[index.x()  ][index.y()  ][index.z()+1]->inner
         || hexSampleMatrix[index.x()  ][index.y()+1][index.z()  ]->inner
         || hexSampleMatrix[index.x()  ][index.y()+1][index.z()+1]->inner
         || hexSampleMatrix[index.x()+1][index.y()  ][index.z()  ]->inner
         || hexSampleMatrix[index.x()+1][index.y()  ][index.z()+1]->inner
         || hexSampleMatrix[index.x()+1][index.y()+1][index.z()  ]->inner
         || hexSampleMatrix[index.x()+1][index.y()+1][index.z()+1]->inner)) {
        // 如果八个顶点全部在模型外部，则取消
        return;
    }

    hexahedralMesh->insertHexahedron(
        Hexahedron(
            hexSampleMatrix[index.x()  ][index.y()  ][index.z()  ],
            hexSampleMatrix[index.x()  ][index.y()+1][index.z()  ],
            hexSampleMatrix[index.x()+1][index.y()+1][index.z()  ],
            hexSampleMatrix[index.x()+1][index.y()  ][index.z()  ],
            hexSampleMatrix[index.x()  ][index.y()  ][index.z()+1],
            hexSampleMatrix[index.x()  ][index.y()+1][index.z()+1],
            hexSampleMatrix[index.x()+1][index.y()+1][index.z()+1],
            hexSampleMatrix[index.x()+1][index.y()  ][index.z()+1]));
}

/**
 * @brief GridHexAlgorithm::scanLine
 * 扫面线方法来收缩六面体
 */
void GridHexAlgorithm::scanLine()
{

}

void GridHexAlgorithm::clean()
{

}
