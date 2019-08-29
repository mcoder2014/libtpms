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
    int smooth_times = 5;

    // P
    implicit_surface.setType("p");
    march_box.marching_cubes(implicit_surface);
    march_box.writeOBJ("p_type.obj");

    smoothTool.createMesh(march_box.m_vertices,
                          march_box.m_faces);
    for(int i=0; i< smooth_times; i++)
        smoothTool.basicSmooth();
    smoothTool.writeOBJ("openMesh_p_type.obj",
                        "openMesh_smooth_p_type.obj");


    // D
    implicit_surface.setType("d");
    march_box.marching_cubes(implicit_surface);
    march_box.writeOBJ("d_type.obj");

    smoothTool.createMesh(march_box.m_vertices,
                          march_box.m_faces);
    for(int i=0; i< smooth_times; i++)
        smoothTool.basicSmooth();
    smoothTool.writeOBJ("openMesh_d_type.obj",
                        "openMesh_smooth_d_type.obj");

    //G
    implicit_surface.setType("g");
    march_box.marching_cubes(implicit_surface);
    march_box.writeOBJ("g_type.obj");

    smoothTool.createMesh(march_box.m_vertices,
                          march_box.m_faces);
    for(int i=0; i< smooth_times; i++)
        smoothTool.basicSmooth();
    smoothTool.writeOBJ("openMesh_g_type.obj",
                        "openMesh_smooth_g_type.obj");


    //i-wp
    implicit_surface.setType("i-wp");
    march_box.marching_cubes(implicit_surface);
    march_box.writeOBJ("i-wp_type.obj");

    smoothTool.createMesh(march_box.m_vertices,
                          march_box.m_faces);
    for(int i=0; i< smooth_times; i++)
        smoothTool.basicSmooth();
    smoothTool.writeOBJ("openMesh_i_wp_type.obj",
                        "openMesh_smooth_i_wp_type.obj");

    // f-rd
    implicit_surface.setType("f-rd");
    march_box.marching_cubes(implicit_surface);
    march_box.writeOBJ("f-rd_type.obj");

    smoothTool.createMesh(march_box.m_vertices,
                          march_box.m_faces);
    for(int i=0; i< smooth_times; i++)
        smoothTool.basicSmooth();
    smoothTool.writeOBJ("openMesh_f_rd_type.obj",
                        "openMesh_smooth_f_rd_type.obj");

    // l
    implicit_surface.setType("l");
    march_box.marching_cubes(implicit_surface);
    march_box.writeOBJ("l_type.obj");

    smoothTool.createMesh(march_box.m_vertices,
                          march_box.m_faces);
    for(int i=0; i< smooth_times; i++)
        smoothTool.basicSmooth();
    smoothTool.writeOBJ("openMesh_l_type.obj",
                        "openMesh_smooth_l_type.obj");

    // tubular-p
    implicit_surface.setType("tubular-p");
    march_box.marching_cubes(implicit_surface);
    march_box.writeOBJ("turbular-p_type.obj");

    smoothTool.createMesh(march_box.m_vertices,
                          march_box.m_faces);
    for(int i=0; i< smooth_times; i++)
        smoothTool.basicSmooth();
    smoothTool.writeOBJ("openMesh_tubular_p_type.obj",
                        "openMesh_smooth_tubular_p_type.obj");

    // tubular-g
    implicit_surface.setType("tubular-g");
    march_box.marching_cubes(implicit_surface);
    march_box.writeOBJ("tubular-g_type.obj");

    smoothTool.createMesh(march_box.m_vertices,
                          march_box.m_faces);
    for(int i=0; i< smooth_times; i++)
        smoothTool.basicSmooth();
    smoothTool.writeOBJ("openMesh_tubular_g_type.obj",
                        "openMesh_smooth_tubular_g_type.obj");

    // i2-y
    implicit_surface.setType("i2-y");
    march_box.marching_cubes(implicit_surface);
    march_box.writeOBJ("i2-y_type.obj");

    smoothTool.createMesh(march_box.m_vertices,
                          march_box.m_faces);
    for(int i=0; i< smooth_times; i++)
        smoothTool.basicSmooth();
    smoothTool.writeOBJ("openMesh_i2_y_type.obj",
                        "openMesh_smooth_i2_y_type.obj");

    return 0;
}
