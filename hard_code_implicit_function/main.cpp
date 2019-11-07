#include <iostream>
#include <QString>
#include <time.h>

#include "implicitsurface.h"
#include "marchbox.h"
#include "smoothtool.h"

//#define Box_test
//#define Cube_test

#ifdef USING_SURFACEMESH
#include "SurfaceMeshModel.h"
#include "surfacemesh_load.h"
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

#ifdef USING_SURFACEMESH
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

    march_box.setSampleSize(8);
    march_box.setDensity(5);

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
    int sample = 128;
    int density = 5;

    march_box.setRange(Eigen::Vector3d(20.0, 20.0, 20.0),
                       Eigen::Vector3d(0.0, 0.0, 0.0),
                       sample, density);
#endif

    // P
    march_box_function("P","p_type.obj");
    smooth_tool_function("p_type_openmesh.obj", "p_type_smooth_openmesh.obj");

    // D
    march_box_function("D", "d_type.obj");
    smooth_tool_function("d_type_openmesh.obj", "d_type_smooth_openmesh.obj");

    //G
    march_box_function("G", "g_type.obj");
    smooth_tool_function("g_type_openmesh.obj", "g_type_smooth_openmesh.obj");

    //i-wp
    march_box_function("i-wp", "i-wp_type.obj");
    smooth_tool_function("i_wp_type_openmesh.obj",
                        "i_wp_type_smooth_openmesh.obj");

    // f-rd
    march_box_function("f-rd", "f-rd_type.obj");
    smooth_tool_function("f_rd_type_openmesh.obj",
                        "f_rd_type_smooth_openmesh.obj");

    // l
    march_box_function("l", "l_type.obj");
    smooth_tool_function("l_type_openmesh.obj",
                        "l_type_smooth_openmesh.obj");

    // tubular-p
    march_box_function("tubular-p", "turbular-p_type.obj");
    smooth_tool_function("tubular_p_type_openmesh.obj",
                        "tubular_p_type_smooth_openmesh.obj");

    // tubular-g
    march_box_function("tubular-g", "tubular-g_type.obj");
    smooth_tool_function("tubular_g_type_openmesh.obj",
                        "tubular_g_type_smooth_openmesh.obj");

    // i2-y
    march_box_function("i2-y", "i2-y_type.obj");
    smooth_tool_function("i2_y_type_openmesh.obj",
                        "i2_y_type_smooth_openmesh.obj");
#endif
    return 0;
}
