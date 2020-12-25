TEMPLATE = lib
TARGET = tpms

# Version number of the library
VERSION = 1.1

CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

DESTDIR = ../bin/

SOURCES += \
    IO/Convertor.cpp \
    IO/Exporter.cpp \
    IO/Importer.cpp \
    Math/EigenUtil.cpp \
    Mesh/Mesh.cpp \
    Mesh/MeshCleaningTool.cpp \
    Mesh/MeshInfoChecker.cpp \
    Mesh/MeshShellTool.cpp \
    Mesh/MeshSmoothTool.cpp \
    Mesh/PorosityCalculator.cpp \
    SurfaceMesh/OctreeUtil.cpp \
    TPMS/AirHoleImplicitFunction.cpp \
    TPMS/BaseTpmsConfig.cpp \
    TPMS/BaseTpmsSingleSurfaceConfig.cpp \
    TPMS/BlendMarchCubeAlgorithm.cpp \
    TPMS/BoundaryImplicitFunction.cpp \
    TPMS/BoxTpmsSingeSurfaceAlgorithm.cpp \
    TPMS/BoxTpmsSingleSurfaceConfig.cpp \
    TPMS/CommonImplicitFunction.cpp \
    TPMS/CustomTpmsSingleSurfaceAlgorithm.cpp \
    TPMS/CustomTpmsSingleSurfaceConfig.cpp \
    TPMS/Filter/AirHoleSamplePointGroupFilter.cpp \
    TPMS/Filter/BoundaryMarchCubeFilter.cpp \
    TPMS/Filter/CommonMarchCubeFilter.cpp \
    TPMS/Filter/CommonSamplePointFilter.cpp \
    TPMS/Filter/CompressFilter.cpp \
    TPMS/ImplicitFunction.cpp \
    TPMS/ImplicitFunctionUtil.cpp \
    TPMS/MarchBoxTable.cpp \
    TPMS/MarchBoxUtil.cpp \
    TPMS/SamplePointGroup.cpp \
    TPMS/TpmsImplicitFunction.cpp \
    Voxel/VoxelModel.cpp \
    Voxel/VoxelOctree.cpp \
    Voxel/VoxelTools.cpp

HEADERS += \
    IO/Convertor.h \
    IO/Exporter.h \
    IO/Importer.h \
    Math/EigenUtil.h \
    Mesh/Mesh \
    Mesh/Mesh.h \
    Mesh/MeshCleaningTool.h \
    Mesh/MeshInfoChecker.h \
    Mesh/MeshShellTool.h \
    Mesh/MeshSmoothTool.h \
    Mesh/PorosityCalculator.h \
    SurfaceMesh/OctreeUtil.h \
    TPMS/AirHoleImplicitFunction.h \
    TPMS/BaseTpmsAlgorithm.h \
    TPMS/BaseTpmsConfig.h \
    TPMS/BaseTpmsSingleSurfaceConfig.h \
    TPMS/BlendMarchCubeAlgorithm.h \
    TPMS/BoundaryImplicitFunction.h \
    TPMS/BoxTpmsSingeSurfaceAlgorithm.h \
    TPMS/BoxTpmsSingleSurfaceConfig.h \
    TPMS/CommonImplicitFunction.h \
    TPMS/CustomTpmsSingleSurfaceAlgorithm.h \
    TPMS/CustomTpmsSingleSurfaceConfig.h \
    TPMS/Filter/AirHoleSamplePointGroupFilter.h \
    TPMS/Filter/BaseFilter.h \
    TPMS/Filter/BoundaryMarchCubeFilter.h \
    TPMS/Filter/CommonMarchCubeFilter.h \
    TPMS/Filter/CommonSamplePointFilter.h \
    TPMS/Filter/CompressFilter.h \
    TPMS/ImplicitFunction.h \
    TPMS/ImplicitFunctionUtil.h \
    TPMS/MarchBoxTable.h \
    TPMS/MarchBoxUtil.h \
    TPMS/SamplePoint.h \
    TPMS/SamplePointGroup.h \
    TPMS/TpmsImplicitFunction.h \
    Voxel/VoxelModel.h \
    Voxel/VoxelOctree.h \
    Voxel/VoxelTools.h

unix {
    DEFINES += USING_SURFACEMESH
#    DEFINES += DEBUG_INFO
}

contains(DEFINES,USING_SURFACEMESH) {
    include($$PWD/../extern/lydialab.pri)
} else {
    message('Not DEFINE USING_SURFACEMESH')
}

unix {
    #include path and libraries
    INCLUDEPATH += /usr/include/eigen3 \
        /usr/include/assimp \
        /usr/include/opencv4 \
        /usr/include/qt \
        /usr/include/qt/QtGui \
        /usr/include/qt/QtCore \
        /usr/include/qt/QtOpenGL \
        /usr/include/qt/QtWidgets

    LIBS += \
        -L/usr/lib -lOpenMeshCore -lOpenMeshTools \     # openmesh
        -lassimp \      # assimp
        -lopencv_core -lopencv_imgproc \ # opencv
        -lQt5Core -lQt5Gui -lQt5Core -lQt5OpenGL -lQt5Widgets \    # Qt
}


