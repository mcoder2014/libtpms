#include <iostream>
#include <string>
#include <vector>

#include <SurfaceMeshModel.h>

#include <marchbox.h>
#include <smoothtool.h>
#include <IO/importer.h>
#include <generalporositycalculator.h>

using namespace std;

///
/// 用来测试不同参数生成的 TPMS 立方结构的孔隙率
///

/// 生成指定参数的 TPMS 立方体
/// 指定类型、密度、尺寸
/// 指定厚度
///
SurfaceMesh::SurfaceMeshModel* createCubeTPMS(
        string type, Eigen::Vector3i sample, Eigen::Vector3i density,
        Eigen::Vector3d rangeMin, Eigen::Vector3d rangeMax)
{
    return nullptr;
}


/// 使用指定精度给定立方体 TPMS 的孔隙率

/// 测试一系列不同参数的 TPMS 立方体的孔隙率并将结果保存在文件中

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        std::cout << "test-tpms-porosity savefilepath"
                  << "\ne.g. test-tpms-porosity ./test_reult.txt"<< std::endl;
        return 0;
    }



    return 0;
}
