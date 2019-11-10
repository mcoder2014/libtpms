#include <iostream>
#include <QString>
#include <QDir>
#include <time.h>

#include "implicitsurface.h"
#include "marchbox.h"
#include "smoothtool.h"

//#define CUBE_TEST             // small cube test
//#define BOX_INSOLE_SIZE       // big cube at insole size
//#define CLOSED_SURFACE_1      // closed_surface_approach 1
//#define CLOSED_SURFACE_2        // closed_surface_approach 2

#ifdef USING_SURFACEMESH
#include "SurfaceMeshModel.h"
#include "surfacemesh_load.h"
//#define TEST_SURFACEMESH            // use surfacemesh as boundary
#define TEST_SURFACEMESH_PUSH
#define TEST_SURFACEMESH_PUSH_DOUBLE
#endif

using namespace std;

int main()
{
    ImplicitSurface implicit_surface;
    MarchBox march_box;

    SmoothTool  smoothTool;
    int smooth_times = 10;

    auto smooth_tool_function = [&](string openMeshObj,string smooth_obj)
    {
        std::cout << "Start Smooth\n";
        clock_t time_start = clock();

        smoothTool.createMesh(march_box.m_vertices, march_box.m_faces);
        smoothTool.basicSmooth(smooth_times);

        clock_t time_end = clock();
        std::cout << "Cost time " << 1.0 * (time_end-time_start)/CLOCKS_PER_SEC << " S\n\n";
        smoothTool.writeOBJ("", smooth_obj);
    };

    auto march_box_mesh_base = [&](QString dir, QString type, float isoLevel)
    {
        clock_t time_start = clock();
        QString savePath = dir + type + "_type.obj";

        implicit_surface.setType(type);
        march_box.marching_cubes(implicit_surface, isoLevel);

        clock_t time_end = clock();
        std::cout << "Cost time " << 1.0 * (time_end-time_start)/CLOCKS_PER_SEC << " S\n\n";
        march_box.writeOBJ(savePath.toStdString());

        // smooth
        QString smooth_path = dir + type + "_type_smooth_openmesh.obj";
        smooth_tool_function("", smooth_path.toStdString());
    };

    auto march_box_mesh_closed = [&](QString dir, QString type, float isoLevel)
    {
        clock_t time_start = clock();
        QString savePath = dir + type + "_type.obj";

        implicit_surface.setType(type);
        march_box.marching_cubes_closed(implicit_surface, isoLevel);

        clock_t time_end = clock();
        std::cout << "Cost time " << 1.0 * (time_end-time_start)/CLOCKS_PER_SEC << " S\n\n";
        march_box.writeOBJ(savePath.toStdString());

        // smooth
        QString smooth_path = dir + type + "_type_smooth_openmesh.obj";
        smooth_tool_function("", smooth_path.toStdString());

    };

#ifdef CUBE_TEST
    {
        auto cube_test = [&](int sample, int density, float isoLevel, Eigen::Vector3d offset, QString outer_dir)
        {
            // Create path
            QDir path("");
            if(!path.exists(outer_dir))
            {
                path.mkdir(outer_dir);
            }

            QString density_sample_path = "sample_" + QString::number(sample)
                    + "_density_" + QString::number(density)
                    + "_isoLevel_" + QString::number(isoLevel).remove('.');
            path.cd(outer_dir);
            if(!path.exists(density_sample_path))
            {
                path.mkdir(density_sample_path);
            }

            QString prefix = outer_dir + QDir::separator()
                    + density_sample_path + QDir::separator();

            march_box.setSampleSize(sample);
            march_box.setDensity(density);
            march_box.setOffset(offset);

            march_box_mesh_base(prefix, "P", isoLevel);
            march_box_mesh_base(prefix, "D", isoLevel);
            march_box_mesh_base(prefix, "G", isoLevel);
            march_box_mesh_base(prefix, "i-wp", isoLevel);
            march_box_mesh_base(prefix, "f-rd", isoLevel);
            march_box_mesh_base(prefix, "l", isoLevel);
            march_box_mesh_base(prefix, "tubular-p",isoLevel);
            march_box_mesh_base(prefix, "tubular-g", isoLevel);
            march_box_mesh_base(prefix, "i2-y", isoLevel);

        };

        march_box.setRange(Eigen::Vector3d(20.0, 20.0, 20.0),
                           Eigen::Vector3d(0.0, 0.0, 0.0));

        int sample = 32;
        QString Cube_test_floder = "Cube_test";
        march_box.setReverse(false);
        cube_test(sample, 4,-0.75, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);
        cube_test(sample, 4,-0.5, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);
        cube_test(sample, 4,-0.25, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);
        cube_test(sample, 4,0, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);
        cube_test(sample, 4,0.25, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);
        cube_test(sample, 4,0.5, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);
        cube_test(sample, 4,0.75, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);

        QString Cube_test_reverse = "Cube_test_reverse";
        march_box.setReverse(true);
        cube_test(sample, 4,-0.75, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_reverse);
        cube_test(sample, 4,-0.5, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_reverse);
        cube_test(sample, 4,-0.25, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_reverse);
        cube_test(sample, 4,0, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_reverse);
        cube_test(sample, 4,0.25, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_reverse);
        cube_test(sample, 4,0.5, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_reverse);
        cube_test(sample, 4,0.75, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_reverse);

    }
#endif

# ifdef CLOSED_SURFACE_1
    {
        auto cube_test = [&](int sample, int density, float isoLevel, Eigen::Vector3d offset, QString outer_dir)
        {
            // Create path
            QDir path("");
            if(!path.exists(outer_dir))
            {
                path.mkdir(outer_dir);
            }

            QString density_sample_path = "sample_" + QString::number(sample)
                    + "_density_" + QString::number(density)
                    + "_isoLevel_" + QString::number(isoLevel).remove('.');
            path.cd(outer_dir);
            if(!path.exists(density_sample_path))
            {
                path.mkdir(density_sample_path);
            }

            QString prefix = outer_dir + QDir::separator()
                    + density_sample_path + QDir::separator();

            march_box.setSampleSize(sample);
            march_box.setDensity(density);
            march_box.setOffset(offset);

            march_box_mesh_closed(prefix, "P", isoLevel);
            march_box_mesh_closed(prefix, "D", isoLevel);
            march_box_mesh_closed(prefix, "G", isoLevel);
            march_box_mesh_closed(prefix, "i-wp", isoLevel);
            march_box_mesh_closed(prefix, "f-rd", isoLevel);
            march_box_mesh_closed(prefix, "l", isoLevel);
            march_box_mesh_closed(prefix, "tubular-p",isoLevel);
            march_box_mesh_closed(prefix, "tubular-g", isoLevel);
            march_box_mesh_closed(prefix, "i2-y", isoLevel);

        };

        march_box.setRange(Eigen::Vector3d(20, 20, 20),
                           Eigen::Vector3d(0.0, 0.0, 0.0));

        int sample = 32;
        int density = 10;
        QString Cube_test_floder = "Cube_closed_test";
        march_box.setReverse(false);
//        cube_test(sample, density,-0.75, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);
//        cube_test(sample, density,-0.5, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);
//        cube_test(sample, density, -0.25, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);
        cube_test(sample, density,0, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);
//        cube_test(sample, density,0.25, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);
//        cube_test(sample, density,0.5, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);
//        cube_test(sample, density,0.75, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);

        QString Cube_test_reverse = "Cube_closed_test_reverse";
        march_box.setReverse(true);
//        cube_test(sample, density,-0.75, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_reverse);
//        cube_test(sample, density,-0.5, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_reverse);
//        cube_test(sample, density,-0.25, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_reverse);
        cube_test(sample, density,0, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_reverse);
//        cube_test(sample, density,0.25, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_reverse);
//        cube_test(sample, density,0.5, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_reverse);
//        cube_test(sample, density,0.75, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_reverse);

    }
#endif

# ifdef CLOSED_SURFACE_2
    {
        auto march_box_mesh_closed = [&](QString dir, QString type, float isoLevel_1, float isoLevel_2)
        {
            clock_t time_start = clock();
            QString savePath = dir + type + "_type.obj";

            implicit_surface.setType(type);
            march_box.marching_cubes_double_closed(implicit_surface, isoLevel_1,isoLevel_2);

            clock_t time_end = clock();
            std::cout << "Cost time " << 1.0 * (time_end-time_start)/CLOCKS_PER_SEC << " S\n\n";
            march_box.writeOBJ(savePath.toStdString());

            // smooth
            QString smooth_path = dir + type + "_type_smooth_openmesh.obj";
            smooth_tool_function("", smooth_path.toStdString());

        };
        auto cube_test = [&](int sample, int density, float isoLevel_1, float isoLevel_2, Eigen::Vector3d offset, QString outer_dir)
        {
            // Create path
            QDir path("");
            if(!path.exists(outer_dir))
            {
                path.mkdir(outer_dir);
            }

            QString density_sample_path = "sample_" + QString::number(sample)
                    + "_density_" + QString::number(density)
                    + "_isoLevel1_" + QString::number(isoLevel_1).remove('.')
                    + "_isoLevel2_" + QString::number(isoLevel_2).remove('.');
            path.cd(outer_dir);
            if(!path.exists(density_sample_path))
            {
                path.mkdir(density_sample_path);
            }

            QString prefix = outer_dir + QDir::separator()
                    + density_sample_path + QDir::separator();

            march_box.setSampleSize(sample);
            march_box.setDensity(density);
            march_box.setOffset(offset);

            march_box_mesh_closed(prefix, "P", isoLevel_1, isoLevel_2);
            march_box_mesh_closed(prefix, "D", isoLevel_1,isoLevel_2);
            march_box_mesh_closed(prefix, "G", isoLevel_1,isoLevel_2);
            march_box_mesh_closed(prefix, "i-wp", isoLevel_1,isoLevel_2);
            march_box_mesh_closed(prefix, "f-rd", isoLevel_1,isoLevel_2);
            march_box_mesh_closed(prefix, "l", isoLevel_1,isoLevel_2);
            march_box_mesh_closed(prefix, "tubular-p",isoLevel_1,isoLevel_2);
            march_box_mesh_closed(prefix, "tubular-g", isoLevel_1,isoLevel_2);
            march_box_mesh_closed(prefix, "i2-y", isoLevel_1,isoLevel_2);

        };

        march_box.setRange(Eigen::Vector3d(50.0, 50.0, 50.0),
                           Eigen::Vector3d(0.0, 0.0, 0.0));

        int sample = 128;
        int density = 25;
        QString Cube_test_floder = "Cube_double_closed_test";
        march_box.setReverse(false);
        cube_test(sample, density,0,0.5, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);
        cube_test(sample, density,0,0.25, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);
        cube_test(sample, density,0,0.1, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);

        QString Cube_test_reverse = "Cube_double_closed_test_reverse";
        march_box.setReverse(true);
        cube_test(sample, density,0,0.5, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);
        cube_test(sample, density,0,0.25, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);
        cube_test(sample, density,0,0.1, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_reverse);

    }
#endif

#ifdef TEST_SURFACEMESH
    auto march_box_mesh_boundary = [&](QString type, SurfaceMesh::SurfaceMeshModel& boundary)
    {
        clock_t time_start = clock();
        QString savePath = type + "_type.obj";

        implicit_surface.setType(type);
        march_box.marching_cubes_closed(implicit_surface, boundary);

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

#ifdef TEST_SURFACEMESH_PUSH
    {
        auto march_box_mesh_boundary = [&](QString dir, QString type,
                float isoLevel, SurfaceMesh::SurfaceMeshModel& boundary)
        {
            clock_t time_start = clock();
            QString savePath = dir + type + "_type.obj";

            implicit_surface.setType(type);
            march_box.marching_cube_push_closed(implicit_surface, boundary, -1, -1, isoLevel);

            clock_t time_end = clock();
            std::cout << "Cost time " << 1.0 * (time_end-time_start)/CLOCKS_PER_SEC << " S\n\n";
            march_box.writeOBJ(savePath.toStdString());

            // smooth
            QString smooth_path = dir + type + "_type_smooth_openmesh.obj";
            smooth_tool_function("", smooth_path.toStdString());

        };

        auto surfacemesh_test = [&](int sample, int density,
                float isoLevel,SurfaceMesh::SurfaceMeshModel& boundary,
                Eigen::Vector3d offset, QString outer_dir)
        {
            // Create path
            QDir path("");
            if(!path.exists(outer_dir))
            {
                path.mkdir(outer_dir);
            }

            QString density_sample_path = "sample_" + QString::number(sample)
                    + "_density_" + QString::number(density)
                    + "_isoLevel_" + QString::number(isoLevel).remove('.');
            path.cd(outer_dir);
            if(!path.exists(density_sample_path))
            {
                path.mkdir(density_sample_path);
            }

            QString prefix = outer_dir + QDir::separator()
                    + density_sample_path + QDir::separator();

            march_box.setSampleSize(sample);
            march_box.setDensity(density);
            march_box.setOffset(offset);

            march_box_mesh_boundary(prefix, "P", isoLevel, boundary);
            march_box_mesh_boundary(prefix, "D", isoLevel, boundary);
            march_box_mesh_boundary(prefix, "G", isoLevel, boundary);
            march_box_mesh_boundary(prefix, "i-wp", isoLevel, boundary);
            march_box_mesh_boundary(prefix, "f-rd", isoLevel, boundary);
            march_box_mesh_boundary(prefix, "l", isoLevel, boundary);
            march_box_mesh_boundary(prefix, "tubular-p",isoLevel, boundary);
            march_box_mesh_boundary(prefix, "tubular-g", isoLevel, boundary);
            march_box_mesh_boundary(prefix, "i2-y", isoLevel, boundary);

        };

        SurfaceMesh::SurfaceMeshModel boundary_model;
        SurfaceMeshLoader loader;
        loader.load(boundary_model, "insole_prototype.ply");

        int sample = 16;
        int density = 8;
        QString test_floder = "surfacemesh_push_closed_test";
        march_box.setReverse(false);
        surfacemesh_test(sample, density, 0.25, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder);
        surfacemesh_test(sample, density, 0, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder);
        surfacemesh_test(sample, density, -0.25, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder);

        QString test_reverse = "surfacemesh_push_closed_test_reverse";
        march_box.setReverse(true);
        surfacemesh_test(sample, density, 0.25, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder);
        surfacemesh_test(sample, density, 0, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder);
        surfacemesh_test(sample, density, -0.25, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder);
    }
#endif

    return 0;
}
