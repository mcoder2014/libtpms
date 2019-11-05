#include <iostream>
#include <QString>

#include "implicitsurface.h"
#include "marchbox.h"
#include "smoothtool.h"

using namespace std;

int main()
{
    ImplicitSurface implicit_surface;
    MarchBox march_box;

    int size = 128;
    march_box.m_ncellsX = size;
    march_box.m_ncellsY = size;
    march_box.m_ncellsZ = size;

    float pos_min = 0;
    float pos_max = 1;
    march_box.m_mcMaxX = pos_max;
    march_box.m_mcMinX = pos_min;
    march_box.m_mcMaxY = pos_max;
    march_box.m_mcMinY = pos_min;
    march_box.m_mcMaxZ = pos_max;
    march_box.m_mcMinZ = pos_min;

    SmoothTool  smoothTool;
    int smooth_times = 20;

    auto march_box_function = [&](QString type, string savePath)
    {
        implicit_surface.setType(type);
        march_box.marching_cubes(implicit_surface);
        march_box.writeOBJ(savePath);
    };
    auto smooth_tool_function = [&](string openMeshObj,string smooth_obj)
    {
        smoothTool.createMesh(march_box.m_vertices,
                              march_box.m_faces);
        for(int i=0; i< smooth_times; i++)
            smoothTool.basicSmooth();
        smoothTool.writeOBJ(openMeshObj,smooth_obj);
    };
    auto smooth_tool_from_obj = [&](string inputfile, string openMeshObj, string smooth_obj)
    {
        smoothTool.createMesh(inputfile);
        for(int i = 0; i < smooth_times; i++)
            smoothTool.basicSmooth();
        smoothTool.writeOBJ(openMeshObj, smooth_obj);
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
