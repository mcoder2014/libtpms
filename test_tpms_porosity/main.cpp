#include <iostream>
#include <string>
#include <vector>

#include <QFile>
#include <QDir>

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
    if(march_box.m_vertices.size() <= 10)
        return nullptr;

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
int test_cube_porosity(int argc, char* argv[])
{
    if(argc < 2)
    {
        std::cout << "test-tpms-porosity savefilepath"
                  << "\ne.g. test-tpms-porosity ./test_reult.txt"<< std::endl;
        return 0;
    }
    string fileName(argv[1]);
    // open file
    std::ofstream file;
    file.open(fileName,ios_base::out | ios_base::trunc);

    if(!file) {
        std::cout << "Error opening output file" << std::endl;
        return 0;
    }
    file << "Type,IsoLevel,Depth,Porosity" << std::endl;

    GeneralPorosityCalculator porosityCalc;

    //    std::vector<string> types{"p","d","g","l","tubular-p","i-wp","f-rd","tubular-g","i2-y"};
    std::vector<string> types{"p", "d", "g", "i-wp"};
    Eigen::Vector3i density{1,1,1};
    Eigen::Vector3i sampleSize{64,64,64};
    Eigen::Vector3d rangeMin{0,0,0};
    Eigen::Vector3d rangeMax{1,1,1};
    float isoLevel = 0;
    float isoLevel_start = -1;
    float isoLevel_end = 1;
    int isoLevelTimes = 50;
    float isoLevel_step = (isoLevel_end-isoLevel_start)/isoLevelTimes;

    float depth = 0.01;
    float depth_start = 0.01;
    float depth_end = 0.4;
    int depthTimes = 50;
    float depthStep = (depth_end-depth_start)/depthTimes;

    for(string type: types)
    {
        for(int iter_depth = 0; iter_depth <= depthTimes; iter_depth++)
        {
            depth = depth_start + iter_depth * depthStep;

            for(int iter_iso=0; iter_iso <= isoLevelTimes; iter_iso++)
            {

                isoLevel = isoLevel_start + iter_iso * isoLevel_step;

                std::cout << "Type: " << type << endl;
                Mesh* mesh = createCubeTPMS(type, sampleSize,density,
                                            rangeMin, rangeMax,
                                            isoLevel, depth);
                if(mesh == nullptr)
                    continue;

                Exporter exporter;
                QString folderPath;
                folderPath = QString("exp") + QDir::separator()
                        + QString::fromStdString(type) + QDir::separator()
                        + "depth_" + QString::number(depth) + QDir::separator();

                QDir dir(folderPath);
                if(!dir.exists())
                {
                    std::cout << "create path" << folderPath.toStdString() << std::endl;
                    QDir("").mkpath(folderPath);
                }

                QString filename = QString::fromStdString(type) + "_" +QString::number(isoLevel)
                        + "_" + QString::number(depth) +".ply";

                exporter.writeOBJ((folderPath + filename).toStdString(), *mesh);

                float porosity = porosityCalc.getPorosity(*mesh);

                file << type << " , "
                     << isoLevel << " , "
                     << depth << " , "
                     << porosity << std::endl;

                delete mesh;
            }
        }
    }

    file.close();
    return 0;
}

int test_part_tpms(int argc, char* argv[])
{

    GeneralPorosityCalculator porosityCalc;

    //    std::vector<string> types{"p","d","g","l","tubular-p","i-wp","f-rd","tubular-g","i2-y"};
    std::vector<string> types{"p", "d", "g", "i-wp"};
    Eigen::Vector3i density{1,1,1};
    Eigen::Vector3i sampleSize{64,64,64};
    Eigen::Vector3d rangeMin{0,0,0};
    Eigen::Vector3d rangeMax{1,1,1};
    float isoLevel = 0;
    float depth = 0.2;

    for(string type: types)
    {

        std::cout << "Type: " << type << endl;
        Mesh* mesh = createCubeTPMS(type, sampleSize,density,
                                    rangeMin, rangeMax,
                                    isoLevel, depth);
        if(mesh == nullptr)
            continue;

        Exporter exporter;
        string filename = "exp/" + type + "_" +std::to_string(isoLevel)
                + "_" + std::to_string(depth) +".ply";

        exporter.writeOBJ(filename, *mesh);

        float porosity = porosityCalc.getPorosity(*mesh);

        delete mesh;
    }
    return 0;
}


int main(int argc, char *argv[])
{

    // 测试 常数 c 厚度 d 对 孔隙率的影响
    return test_cube_porosity(argc, argv);

    // 测试 部分 tpms 与总体 tmps 的孔隙率是否一样
    //    return test_part_tpms(argc, argv);

    return 0;
}
