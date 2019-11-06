#include <iostream>
#include <QString>
#include <time.h>

#include "implicitsurface.h"
#include "marchbox.h"
#include "smoothtool.h"

//#define Box_test
#define Cube_test

using namespace std;

int main()
{
    ImplicitSurface implicit_surface;
    MarchBox march_box;

#ifdef Cube_test
    int density = 128;

    float pos_min = 0;
    float pos_max = 3;

    march_box.m_mcMaxX = pos_max;
    march_box.m_mcMinX = pos_min;
    march_box.m_mcMaxY = pos_max;
    march_box.m_mcMinY = pos_min;
    march_box.m_mcMaxZ = pos_max;
    march_box.m_mcMinZ = pos_min;


    march_box.m_ncellsX = density * pos_max;
    march_box.m_ncellsY = density * pos_max;
    march_box.m_ncellsZ = density * pos_max;

#endif

#ifdef Box_test
    int density = 16;

    // 鞋垫尺寸
    int insole_width = 110;
    int insole_height = 25;
    int insole_length = 265;

    // 多少毫米长度一个周期
    int period_long = 5;

    float pos_min = 0;

    march_box.m_ncellsX = insole_width / period_long * density;
    march_box.m_ncellsY = insole_length / period_long * density;
    march_box.m_ncellsZ = insole_height / period_long * density;

    march_box.m_mcMinX = pos_min;
    march_box.m_mcMinY = pos_min;
    march_box.m_mcMinZ = pos_min;

    march_box.m_mcMaxX = 1.0 * insole_width / period_long;
    march_box.m_mcMaxY = 1.0 * insole_length / period_long;
    march_box.m_mcMaxZ = 1.0 * insole_height / period_long;

#endif
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

    return 0;
}
