#include <iostream>
#include <QString>

#include "implicitsurface.h"
#include "marchbox.h"

using namespace std;

int main()
{
    ImplicitSurface implicit_surface;
    MarchBox march_box;

    int size = 128;
    march_box.m_ncellsX = size;
    march_box.m_ncellsY = size;
    march_box.m_ncellsZ = size;

    float length = 2;
    march_box.m_mcMaxX = length;
    march_box.m_mcMinX = - length;
    march_box.m_mcMaxY = length;
    march_box.m_mcMinY = - length;
    march_box.m_mcMaxZ = length;
    march_box.m_mcMinZ = - length;

    // P
    implicit_surface.setType("p");
    march_box.marching_cubes(implicit_surface);
    march_box.writeOBJ("p_type.obj");

    // D
    implicit_surface.setType("d");
    march_box.marching_cubes(implicit_surface);
    march_box.writeOBJ("d_type.obj");

    //G
    implicit_surface.setType("g");
    march_box.marching_cubes(implicit_surface);
    march_box.writeOBJ("g_type.obj");

    //i-wp
    implicit_surface.setType("i-wp");
    march_box.marching_cubes(implicit_surface);
    march_box.writeOBJ("i-wp_type.obj");

    // f-rd
    implicit_surface.setType("f-rd");
    march_box.marching_cubes(implicit_surface);
    march_box.writeOBJ("f-rd_type.obj");

    // l
    implicit_surface.setType("l");
    march_box.marching_cubes(implicit_surface);
    march_box.writeOBJ("l_type.obj");

    // tubular-p
    implicit_surface.setType("tubular-p");
    march_box.marching_cubes(implicit_surface);
    march_box.writeOBJ("turbular-p_type.obj");

    // tubular-g
    implicit_surface.setType("tubular-g");
    march_box.marching_cubes(implicit_surface);
    march_box.writeOBJ("tubular-g_type.obj");

    // i2-y
    implicit_surface.setType("i2-y");
    march_box.marching_cubes(implicit_surface);
    march_box.writeOBJ("i2-y_type.obj");

    return 0;
}
