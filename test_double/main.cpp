#include <iostream>
#include <QString>
#include <QDir>
#include <time.h>

#include "implicitsurface.h"
#include "marchbox.h"
#include "smoothtool.h"
#include "simplification.h"
#include "qualityinspection.h"

//#define CUBE_TEST             // small cube test
//#define BOX_INSOLE_SIZE       // big cube at insole size
//#define CLOSED_SURFACE_1      // closed_surface_approach 1
//#define CLOSED_SURFACE_2        // closed_surface_approach 2

#ifdef USING_SURFACEMESH
#include "SurfaceMeshModel.h"
#include "surfacemesh_load.h"
//#define TEST_SURFACEMESH            // use surfacemesh as boundary
//#define TEST_SURFACEMESH_PUSH
//#define TEST_SURFACEMESH_PUSH_DOUBLE
//#define TEST_MESH_PUSH_DIFF
//#define TEST_MESH_PUSH_D_DIFF
//#define TEST_MESH_PUSH_MUILT_DIFF
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

        // Check
        QualityInspection qualityInspection;
        qualityInspection.check(smoothTool.getMesh());
    };

    auto march_box_mesh_base = [&](QString dir, QString type, float isoLevel)
    {
        clock_t time_start = clock();
        QString savePath = dir + type + "_type.obj";

        implicit_surface.setType(type.toStdString());
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

        implicit_surface.setType(type.toStdString());
        march_box.marching_cubes_closed(implicit_surface, isoLevel);

        clock_t time_end = clock();
        std::cout << "Cost time " << 1.0 * (time_end-time_start)/CLOCKS_PER_SEC << " S\n\n";
        march_box.writeOBJ(savePath.toStdString());

        // smooth
        QString smooth_path = dir + type + "_type_smooth_openmesh.obj";
        smooth_tool_function("", smooth_path.toStdString());

    };

#define MC_CUBE_DOUBLE_SURFACE
#ifdef MC_CUBE_DOUBLE_SURFACE
    march_box.setRange(Eigen::Vector3d(20.0, 20.0, 20.0),
                       Eigen::Vector3d(0.0, 0.0, 0.0));

    int sample = 32;
    int density = 10;
    march_box.setSampleSize(sample);
    march_box.setDensity(density);
    march_box.setReverse(false);

    implicit_surface.setType("G");
    Mesh *mesh = march_box.mc_cube_double_surface(implicit_surface, 0, 0.6);
    OpenMesh::IO::write_mesh(*mesh,"test.obj");

#endif

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

        march_box.setRange(Eigen::Vector3d(50, 50, 50),
                           Eigen::Vector3d(0.0, 0.0, 0.0));

        int sample = 64;
        int density = 25;
        QString Cube_test_floder = "Cube_closed_test";
        march_box.setReverse(false);
//        cube_test(sample, density,-0.75, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);
        cube_test(sample, density,-0.5, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);
        cube_test(sample, density, -0.25, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);
        cube_test(sample, density,0, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);
        cube_test(sample, density,0.25, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);
        cube_test(sample, density,0.5, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);
//        cube_test(sample, density,0.75, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);

        QString Cube_test_reverse = "Cube_closed_test_reverse";
        march_box.setReverse(true);
//        cube_test(sample, density,-0.75, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_reverse);
        cube_test(sample, density,-0.5, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_reverse);
        cube_test(sample, density,-0.25, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_reverse);
        cube_test(sample, density,0, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_reverse);
        cube_test(sample, density,0.25, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_reverse);
        cube_test(sample, density,0.5, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_reverse);
//        cube_test(sample, density,0.75, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_reverse);

    }
#endif

# ifdef CLOSED_SURFACE_2
    {
        float pointsCount;
        auto march_box_mesh_closed = [&](QString dir, QString type,
                float isoLevel_1, float isoLevel_2)
        {
            clock_t time_start = clock();
            QString savePath = dir + type + "_type.obj";

            // Create implicit surface
            implicit_surface.setType(type);
            march_box.marching_cubes_double_closed(implicit_surface, isoLevel_1,isoLevel_2);

            clock_t time_end = clock();
            std::cout << "Create mesh Cost time "
                      << 1.0 * (time_end-time_start)/CLOCKS_PER_SEC << " S\n\n";
            std::cout << "Vertices: " << march_box.m_vertices.size()
                      << "\tFaces: " << march_box.m_faces.size()<< std::endl;
            march_box.writeOBJ(savePath.toStdString());

            // smooth
            QString smooth_path = dir + type + "_type_smooth_openmesh.obj";
            std::cout << "Start Smooth\n";
            time_start = clock();

            smoothTool.createMesh(march_box.m_vertices, march_box.m_faces);
            smoothTool.basicSmooth(smooth_times);
            time_end = clock();
            std::cout << "Smooth Cost time " << 1.0 * (time_end-time_start)/CLOCKS_PER_SEC << " S\n\n";
            smoothTool.writeOBJ("", smooth_path.toStdString());

//            // simplification
//            time_start = clock();
//            QString simplification_path = dir + type + "_type_smooth_simplication_openmesh.obj";
//            Simplification simplification;
//            simplification.createMesh(smoothTool.getMesh());
//            simplification.decimater_to(pointsCount);
//            time_end = clock();
//            std::cout << "Simplication Cost time "
//                      << 1.0 * (time_end-time_start)/CLOCKS_PER_SEC << " S\n\n";

//            Mesh * mesh = simplification.getMesh();

//            // smooth
//            time_start = clock();
//            smoothTool.createMesh(mesh);
//            smoothTool.basicSmooth(smooth_times);
//            time_end = clock();
//            std::cout << "Smooth Cost time " << 1.0 * (time_end-time_start)/CLOCKS_PER_SEC << " S\n\n";
//            mesh = smoothTool.getMesh();

//            try
//            {
//                if ( !OpenMesh::IO::write_mesh(*mesh, simplification_path.toStdString()))
//                {
//                    std::cerr << "Cannot write simplication mesh to "
//                              << simplification_path.toStdString()
//                              << std::endl;
//                }
//                std::cout << "OpenMesh saving simplication mesh into file:\t"
//                          << simplification_path.toStdString() << std::endl;
//            }
//            catch( std::exception& x )
//            {
//                std::cerr << x.what() << std::endl;
//            }


        };
        auto cube_test = [&](Eigen::Vector3i sample, Eigen::Vector3i density, float isoLevel_1, float isoLevel_2, Eigen::Vector3d offset, QString outer_dir)
        {
            // Create path
            QDir path("");
            if(!path.exists(outer_dir))
            {
                path.mkdir(outer_dir);
            }

            QString density_sample_path = "_s" +
                    QString::number(sample[0])+"-"+QString::number(sample[1])+"-"+QString::number(sample[2])
                    + "_d" + QString::number(density[0])+"-" +QString::number(density[1])+"-" +QString::number(density[2])
                    + "_i1_" + QString::number(isoLevel_1)
                    + "_i2_" + QString::number(isoLevel_2)
                    + "_pct" + QString::number(pointsCount*100.0,'g', 2);
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

        Eigen::Vector3i sample(64, 64, 64);
        Eigen::Vector3i density(25, 25, 25);
        QString Cube_test_floder = "Cube_double_closed_test";
        march_box.setReverse(false);

        pointsCount = 0.5;
        cube_test(sample, density,-0.2,0.2, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);

//        pointsCount = 0.7;
        cube_test(sample, density,-0.4,0.4, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);

        cube_test(sample, density,-0.5,0.5, Eigen::Vector3d(0.0,0.0,0.0), Cube_test_floder);

        // Cause the symmetry, No need to try the reverse
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

    march_box.setSampleSize(Eigen::Vector3i(64,64,64));
    march_box.setDensity(Eigen::Vector3i(20,20,25));

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

            implicit_surface.setType(type.toStdString());
            march_box.marching_cube_push_closed(implicit_surface, boundary, isoLevel);

            clock_t time_end = clock();
            std::cout << "Cost time " << 1.0 * (time_end-time_start)/CLOCKS_PER_SEC << " S\n\n";
//            march_box.writeOBJ(savePath.toStdString());

            // smooth
            QString smooth_path = dir + type + "_type_smooth_openmesh.ply";
            smooth_tool_function("", smooth_path.toStdString());

        };

        auto surfacemesh_test = [&](Eigen::Vector3i sample,
                Eigen::Vector3i density,
                float isoLevel,SurfaceMesh::SurfaceMeshModel& boundary,
                Eigen::Vector3d offset, QString outer_dir)
        {
            // Create path
            QDir path("");
            if(!path.exists(outer_dir))
            {
                path.mkdir(outer_dir);
            }

            QString density_sample_path =
                    "sample_" + QString::number(sample[0]) +"-"+ QString::number(sample[1])+ '-' + QString::number(sample[2])
                    + "_density_" + QString::number(density[0])+'-' + QString::number(density[1]) + '-' + QString::number(density[2])
                    + "_isoLevel_" + QString::number(isoLevel);
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
        loader.load(boundary_model, "final-2.ply");
//        loader.load(boundary_model, "insole_prototype.ply");

        Eigen::Vector3i sample(60,64,64);
        Eigen::Vector3i density(30, 30, 25);
        smooth_times = 10;
        QString test_floder = "surfacemesh_push_closed_test";
        march_box.setReverse(false);
//        surfacemesh_test(sample, density, 0, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder);
//        surfacemesh_test(sample, density, -0.25, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder);
//        surfacemesh_test(sample, density, 0.25, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder);

        test_floder = "surfacemesh_push_closed_test_reverse";
        march_box.setReverse(true);
//        surfacemesh_test(sample, density, 0, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder);
//        surfacemesh_test(sample, density, -0.25, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder);
//        surfacemesh_test(sample, density, 0.25, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder);

        density = Eigen::Vector3i(20, 20, 25);
        smooth_times = 10;
        test_floder = "surfacemesh_push_closed_test";
        march_box.setReverse(false);
        surfacemesh_test(sample, density, 0, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder);
//        surfacemesh_test(sample, density, -0.25, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder);
//        surfacemesh_test(sample, density, 0.25, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder);

        test_floder = "surfacemesh_push_closed_test_reverse";
        march_box.setReverse(true);
        surfacemesh_test(sample, density, 0, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder);
//        surfacemesh_test(sample, density, -0.25, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder);
//        surfacemesh_test(sample, density, 0.25, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder);

        density = Eigen::Vector3i(10, 10, 25);
        smooth_times = 10;
        test_floder = "surfacemesh_push_closed_test";
        march_box.setReverse(false);
        surfacemesh_test(sample, density, 0, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder);
//        surfacemesh_test(sample, density, -0.25, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder);
//        surfacemesh_test(sample, density, 0.25, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder);

        test_floder = "surfacemesh_push_closed_test_reverse";
        march_box.setReverse(true);
        surfacemesh_test(sample, density, 0, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder);
//        surfacemesh_test(sample, density, -0.25, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder);
//        surfacemesh_test(sample, density, 0.25, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder);


    }
#endif

#ifdef TEST_SURFACEMESH_PUSH_DOUBLE
    {
        auto march_box_mesh_boundary = [&](QString dir, QString type,
                float isoLevel1, float isoLevel2, SurfaceMesh::SurfaceMeshModel& boundary)
        {
            std::cout << "-----------------------------------------------------\n";
            clock_t time_start = clock();
            QString savePath = dir + type + "_type.obj";

            implicit_surface.setType(type.toStdString());
            march_box.marching_cube_push_double_closed(implicit_surface, boundary, isoLevel1, isoLevel2);

            clock_t time_end = clock();
            std::cout << "Cost time " << 1.0 * (time_end-time_start)/CLOCKS_PER_SEC << " S\n\n";
//            march_box.writeOBJ(savePath.toStdString());

            // smooth
            QString smooth_path = dir + type + "_type_smooth_openmesh.ply";
            smooth_tool_function("", smooth_path.toStdString());
            std::cout << "====================================================\n";
        };

        auto surfacemesh_test = [&](Eigen::Vector3i sample,
                Eigen::Vector3i density,
                float isoLevel1,float isoLevel2,SurfaceMesh::SurfaceMeshModel& boundary,
                Eigen::Vector3d offset, QString outer_dir,vector<QString> types)
        {
            // Create path
            QDir path("");
            if(!path.exists(outer_dir))
            {
                path.mkdir(outer_dir);
            }

            QString density_sample_path =
                    "sample_" + QString::number(sample[0]) +"-"+ QString::number(sample[1])+ '-' + QString::number(sample[2])
                    + "_density_" + QString::number(density[0])+'-' + QString::number(density[1]) + '-' + QString::number(density[2])
                    + "_isoLevel1_" + QString::number(isoLevel1)
                    + "_isoLevel2_" + QString::number(isoLevel2);
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

            for(QString type: types)
            {
                march_box_mesh_boundary(prefix, type, isoLevel1, isoLevel2,boundary);
            }
        };

//        clock_t time_start = clock();
        SurfaceMesh::SurfaceMeshModel boundary_model;
        SurfaceMeshLoader loader;
        loader.load(boundary_model, "final-2.ply");
//        loader.load(boundary_model,"insole_prototype.ply");

        Eigen::Vector3i sample(64,64,64);
        Eigen::Vector3i density(10, 10, 25);
        smooth_times = 10;
        QString test_floder = "surfacemesh_push_closed_double_test";
        march_box.setReverse(false);
//        surfacemesh_test(sample, density, -0.3,0.3, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder,
//            {"p","d","g","l","tubular-p"});
//        surfacemesh_test(sample, density, -0.6,0.6, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder,
//            {"i-wp","f-rd","tubular-g","i2-y"});

        density = Eigen::Vector3i(20, 20, 25);
        march_box.setReverse(false);
        surfacemesh_test(sample, density, -0.3,0.3, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder,
            {"p","d","g","l","tubular-p"});
        surfacemesh_test(sample, density, -0.6,0.6, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder,
            {"i-wp","f-rd","tubular-g","i2-y"});

//        density = Eigen::Vector3i(30, 30, 25);
//        march_box.setReverse(false);
//        surfacemesh_test(sample, density, -0.3,0.3, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder,
//            {"p","d","g","l","tubular-p"});
//        surfacemesh_test(sample, density, -0.6,0.6, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder,
//            {"i-wp","f-rd","tubular-g","i2-y"});

//        density = Eigen::Vector3i(40, 40, 25);
//        march_box.setReverse(false);
//        surfacemesh_test(sample, density, -0.3,0.3, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder,
//            {"p","d","g","l","tubular-p"});
//        surfacemesh_test(sample, density, -0.6,0.6, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder,
//            {"i-wp","f-rd","tubular-g","i2-y"});

    }
#endif

#ifdef TEST_MESH_PUSH_DIFF
    {
        auto march_box_mesh_boundary = [&](QString dir, QString type,
                float isoLevel_from, float isoLevel_to,
                SurfaceMesh::SurfaceMeshModel& boundary)
        {
            clock_t time_start = clock();
            QString savePath = dir + type + "_type.obj";

            implicit_surface.setType(type);
//            march_box.marching_cube_push_closed(implicit_surface, boundary, isoLevel);
            march_box.mb_pc_diff_test(implicit_surface, boundary, isoLevel_from, isoLevel_to);
            clock_t time_end = clock();
            std::cout << "Cost time " << 1.0 * (time_end-time_start)/CLOCKS_PER_SEC << " S\n\n";
//            march_box.writeOBJ(savePath.toStdString());

            // smooth
            QString smooth_path = dir + type + "_type_smooth_openmesh.ply";
            smooth_tool_function("", smooth_path.toStdString());

        };

        auto surfacemesh_test = [&](Eigen::Vector3i sample,
                Eigen::Vector3i density,
                float isoLevel_from, float isoLevel_to,
                SurfaceMesh::SurfaceMeshModel& boundary,
                Eigen::Vector3d offset, QString outer_dir,
                vector<QString> types)
        {
            // Create path
            QDir path("");
            if(!path.exists(outer_dir))
            {
                path.mkdir(outer_dir);
            }

            QString density_sample_path =
                    "sample_" + QString::number(sample[0]) +"-"+ QString::number(sample[1])+ '-' + QString::number(sample[2])
                    + "_density_" + QString::number(density[0])+'-' + QString::number(density[1]) + '-' + QString::number(density[2])
                    + "_iso_" + QString::number(isoLevel_from) + '-' + QString::number(isoLevel_to);
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

            for(QString type: types)
            {
                march_box_mesh_boundary(prefix, type, isoLevel_from, isoLevel_to, boundary);
            }
        };

        SurfaceMesh::SurfaceMeshModel boundary_model;
        SurfaceMeshLoader loader;
        loader.load(boundary_model, "origin.obj");

        Eigen::Vector3i sample(60,64,64);
        Eigen::Vector3i density(20, 20, 25);
        smooth_times = 10;
        QString test_floder = "mesh_push_c_diff";
        march_box.setReverse(false);
        surfacemesh_test(sample, density, -0.3, 1.0, boundary_model,
                         Eigen::Vector3d(0.0,0.0,0.0), test_floder,
        {"p","d","g","l","tubular-p","i-wp","f-rd","tubular-g","i2-y"});

        QString test_reverse_floder = "mesh_push_c_r_diff";
        march_box.setReverse(true);
        surfacemesh_test(sample, density, 0.3, 1.0, boundary_model,
                         Eigen::Vector3d(0.0,0.0,0.0), test_reverse_floder,
        {"p","d","g","l","tubular-p","i-wp","f-rd","tubular-g","i2-y"});
   }
#endif

#ifdef TEST_MESH_PUSH_D_DIFF
    {
        auto march_box_mesh_boundary = [&](QString dir, QString type,
                glm::vec2 isoLevel_from, glm::vec2 isoLevel_to,
                SurfaceMesh::SurfaceMeshModel& boundary)
        {
            std::cout << "-----------------------------------------------------\n";
            clock_t time_start = clock();
            QString savePath = dir + type + "_type.obj";

            implicit_surface.setType(type);
            march_box.mb_pd_diff_test(implicit_surface,boundary,isoLevel_to, isoLevel_from);

            clock_t time_end = clock();
            std::cout << "Cost time " << 1.0 * (time_end-time_start)/CLOCKS_PER_SEC << " S\n\n";
//            march_box.writeOBJ(savePath.toStdString());

            // smooth
            QString smooth_path = dir + type + "_type_smooth_openmesh.ply";
            smooth_tool_function("", smooth_path.toStdString());
            std::cout << "====================================================\n";
        };

        auto surfacemesh_test = [&](Eigen::Vector3i sample,
                Eigen::Vector3i density,
                glm::vec2 isoLevel_from,glm::vec2 isoLevel_to,SurfaceMesh::SurfaceMeshModel& boundary,
                Eigen::Vector3d offset, QString outer_dir,
                vector<QString> types)
        {
            // Create path
            QDir path("");
            if(!path.exists(outer_dir))
            {
                path.mkdir(outer_dir);
            }

            QString density_sample_path =
                    "sample_" + QString::number(sample[0]) + "-" + QString::number(sample[1])+ '-' + QString::number(sample[2])
                    + "_density_" + QString::number(density[0])+ '-' + QString::number(density[1]) + '-' + QString::number(density[2])
                    + "_isolf_" + QString::number(isoLevel_from[0]) + "-" + QString::number(isoLevel_from[1])
                    + "_isolto_" + QString::number(isoLevel_to[0]) + "-" + QString::number(isoLevel_to[1]);
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

            for(QString type: types)
            {
                march_box_mesh_boundary(prefix, type, isoLevel_from, isoLevel_to,boundary);
            }
        };

        SurfaceMesh::SurfaceMeshModel boundary_model;
        SurfaceMeshLoader loader;
        loader.load(boundary_model, "origin.obj");

        Eigen::Vector3i sample(64,64,64);
        Eigen::Vector3i density(20, 20, 25);
        smooth_times = 10;
        QString test_floder = "mesh_push_d_diff";
        march_box.setReverse(false);
        surfacemesh_test(sample, density, {-0.3,0.3}, {-0.6,0.6}, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder,
            {"p","d","g","l","tubular-p"});
        surfacemesh_test(sample, density, {-0.6,0.6}, {-1.0,1.0}, boundary_model, Eigen::Vector3d(0.0,0.0,0.0), test_floder,
            {"i-wp","f-rd","tubular-g","i2-y"});
    }
#endif

#ifdef TEST_MESH_PUSH_MUILT_DIFF
    {
        auto march_box_mesh_boundary = [&](
                QString dir, std::vector<std::string> types,
                float isoLevel_from, float isoLevel_to,
                SurfaceMesh::SurfaceMeshModel& boundary)
        {
            clock_t time_start = clock();
            QString savePath = dir;
            for(std::string s:types)
                savePath += QString::fromStdString(s);
            savePath += "_type.obj";

            march_box.mb_pc_mult_test(types, boundary,
                                      isoLevel_from, isoLevel_to);
            clock_t time_end = clock();
            std::cout << "Cost time " << 1.0 * (time_end-time_start)/CLOCKS_PER_SEC << " S\n\n";

            // smooth
            std::string smoothPath = dir.toStdString();
            for(std::string s:types)
            {
                smoothPath += s;
            }
                smoothPath += "_type_smooth_openmesh.ply";
            smooth_tool_function("", smoothPath);

        };

        // load model
        SurfaceMesh::SurfaceMeshModel boundary_model;
        SurfaceMeshLoader loader;
        loader.load(boundary_model, "origin.obj");

        /// Experience part

        QString outer_dir = "mesh_push_multi";
        std::vector<std::string> types{"g","l","d"};

        Eigen::Vector3i sample(64,64,64);
        Eigen::Vector3i density(20, 20, 25);
        Eigen::Vector3d offset(0.0,0.0,0.0);
        smooth_times = 10;

        float isoLevel_from = 0.0;
        float isoLevel_to = 0.5;
        // Create path
        QDir path("");
        if(!path.exists(outer_dir))
        {
            path.mkdir(outer_dir);
        }

        QString density_sample_path =
                "sample_" + QString::number(sample[0]) +"-"+ QString::number(sample[1])+ '-' + QString::number(sample[2])
                + "_density_" + QString::number(density[0])+'-' + QString::number(density[1]) + '-' + QString::number(density[2])
                + "_iso_" + QString::number(isoLevel_from) + '-' + QString::number(isoLevel_to);
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

        march_box_mesh_boundary(prefix, types, isoLevel_from, isoLevel_to,boundary_model);

   }
#endif

    return 0;
}
