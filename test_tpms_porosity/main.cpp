#include <iostream>
#include <string>
#include <vector>

#include <SurfaceMeshModel.h>

#include <marchbox.h>
#include <smoothtool.h>
#include <IO/importer.h>
#include <IO/exporter.h>
#include <generalporositycalculator.h>

using namespace std;

///
/// 用来测试不同参数生成的 TPMS 立方结构的孔隙率
///

/// 生成指定参数的 TPMS 立方体
/// 指定类型、密度、尺寸
/// 指定厚度
///
Mesh* createCubeTPMS(
        string type, Eigen::Vector3i sample, Eigen::Vector3i density,
        Eigen::Vector3d rangeMin, Eigen::Vector3d rangeMax,
        float isoLevel, float depth, int smooth_times = 10)
{
    ImplicitSurface implicit_surface;
    MarchBox march_box;
    SmoothTool  smoothTool;

    // Setting
    implicit_surface.setType(type);

    march_box.setRange(rangeMin, rangeMax);
    march_box.setDensity(density);
    march_box.setSampleSize(sample);
    march_box.setReverse(false);

    // 1. marchbox
    march_box.marching_cubes(implicit_surface, isoLevel);

    // 2. smooth
    smoothTool.createMesh(march_box.m_vertices, march_box.m_faces);
    smoothTool.basicSmooth(smooth_times);

    // 3. offset
    Mesh *result = new Mesh();
    result->assign(*(smoothTool.getMesh()));
    result = smoothTool.meshOffset(result, depth);

    return result;
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

    std::vector<string> types{"p","d","g","l","tubular-p","i-wp","f-rd","tubular-g","i2-y"};
    Eigen::Vector3i density{20,20,20};
    Eigen::Vector3i sampleSize{64,64,64};
    Eigen::Vector3d rangeMin{0,0,0};
    Eigen::Vector3d rangeMax{20,20,20};
    float isoLevel = 0;
    float depth = 0.2;

    for(string type: types)
    {
        std::cout << "Type: " << type << endl;

        Mesh* mesh = createCubeTPMS(type, sampleSize,density, rangeMin, rangeMax,
                                    isoLevel, depth);

        Exporter exporter;
        string filename = type + ".ply";
        exporter.writeOBJ(filename, *mesh);

        delete mesh;
    }


    return 0;
}
