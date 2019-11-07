#include <iostream>
#include <QString>
#include <time.h>

#include "implicitsurface.h"
#include "marchbox.h"
#include "smoothtool.h"

#define Box_test
#define Cube_test

#ifdef USING_SURFACEMESH
#include "SurfaceMeshModel.h"
#include "surfacemesh_load.h"
//#define TEST_SURFACEMESH
#endif

using namespace std;

int main()
{
    ImplicitSurface implicit_surface;
    MarchBox march_box;

    SmoothTool  smoothTool;
    int smooth_times = 20;

    auto march_box_function = [&](QString type, string savePath)
    {
        clock_t time_start = clock();

        implicit_surface.setType(type);
        march_box.marching_cubes(implicit_surface);

        clock_t time_end = clock();
        std::cout << "Cost time " << 1.0 * (time_end-time_start)/CLOCKS_PER_SEC << " S\n\n";
        march_box.writeOBJ(savePath);
    };
    auto smooth_tool_function = [&](string openMeshObj,string smooth_obj)
    {
        std::cout << "Start Smooth\n";
        clock_t time_start = clock();

        smoothTool.createMesh(march_box.m_vertices, march_box.m_faces);
        smoothTool.basicSmooth(smooth_times);

        clock_t time_end = clock();
        std::cout << "Cost time " << 1.0 * (time_end-time_start)/CLOCKS_PER_SEC << " S\n\n";

//        smoothTool.writeOBJ(openMeshObj,smooth_obj);
        smoothTool.writeOBJ("", smooth_obj);
    };
    auto smooth_tool_from_obj = [&](string inputfile, string openMeshObj, string smooth_obj)
    {
        smoothTool.createMesh(inputfile);
        for(int i = 0; i < smooth_times; i++)
            smoothTool.basicSmooth();
//        smoothTool.writeOBJ(openMeshObj, smooth_obj);
        smoothTool.writeOBJ("", smooth_obj);
    };

    auto march_box_mesh_closed = [&](QString type)
    {
        clock_t time_start = clock();
        QString savePath = type + "_type.obj";

        implicit_surface.setType(type);
        march_box.marching_cubes_closed(implicit_surface);

        clock_t time_end = clock();
        std::cout << "Cost time " << 1.0 * (time_end-time_start)/CLOCKS_PER_SEC << " S\n\n";
        march_box.writeOBJ(savePath.toStdString());

        // smooth
        QString smooth_path = type + "_type_smooth_openmesh.obj";
        smooth_tool_function("", smooth_path.toStdString());

    };

#ifdef TEST_SURFACEMESH
    auto march_box_mesh_boundary = [&](QString type, SurfaceMesh::SurfaceMeshModel& boundary)
    {
        clock_t time_start = clock();
        QString savePath = type + "_type.obj";

        implicit_surface.setType(type);
        march_box.marching_cubes(implicit_surface, boundary);

        clock_t time_end = clock();
        std::cout << "Cost time " << 1.0 * (time_end-time_start)/CLOCKS_PER_SEC << " S\n\n";
        march_box.writeOBJ(savePath.toStdString());

        // smooth
        QString smooth_path = type + "_type_smooth_openmesh.obj";
        smooth_tool_function("", smooth_path.toStdString());

    };

    march_box.setSampleSize(16);
    march_box.setDensity(8);

    SurfaceMesh::SurfaceMeshModel boundary_model;
    SurfaceMeshLoader loader;
    loader.load(boundary_model, "insole_prototype.ply");

    march_box_mesh_boundary("P", boundary_model);
    march_box_mesh_boundary("D", boundary_model);
    march_box_mesh_boundary("G", boundary_model);
    march_box_mesh_boundary("i-wp", boundary_model);
    march_box_mesh_boundary("f-rd", boundary_model);
    march_box_mesh_boundary("l", boundary_model);
    march_box_mesh_boundary("tubular-p", boundary_model);
    march_box_mesh_boundary("tubular-g", boundary_model);
    march_box_mesh_boundary("i2-y", boundary_model);
#endif



#ifdef Box_test
    int sampleSize = 16;

    // 鞋垫尺寸
    int insole_width = 110;
    int insole_height = 25;
    int insole_length = 265;

    // 多少毫米长度一个周期
    int density = 5;
    march_box.setRange(Eigen::Vector3d(insole_width, insole_length, insole_height),
                       Eigen::Vector3d(0.0, 0.0, 0.0),
                       sampleSize, density);

#ifdef Cube_test
    int sample = 32;
    density = 5;

    march_box.setRange(Eigen::Vector3d(100.0, 100.0, 100.0),
                       Eigen::Vector3d(0.0, 0.0, 0.0),
                       sample, density);
#endif

    march_box_mesh_closed("P");
    march_box_mesh_closed("D");
    march_box_mesh_closed("G");
    march_box_mesh_closed("i-wp");
    march_box_mesh_closed("f-rd");
    march_box_mesh_closed("l");
    march_box_mesh_closed("tubular-p");
    march_box_mesh_closed("tubular-g");
    march_box_mesh_closed("i2-y");

#endif
    return 0;
}
