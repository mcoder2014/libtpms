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
    Mesh/MeshShellTool.cpp \
    Mesh/MeshSmoothTool.cpp \
    Mesh/PorosityCalculator.cpp \
    SurfaceMesh/OctreeUtil.cpp \
    TPMS/BaseTpmsConfig.cpp \
    TPMS/BaseTpmsSingleSurfaceConfig.cpp \
    TPMS/BoxTpmsSingeSurfaceAlgorithm.cpp \
    TPMS/BoxTpmsSingleSurfaceConfig.cpp \
    TPMS/CustomTpmsSingleSurfaceAlgorithm.cpp \
    TPMS/CustomTpmsSingleSurfaceConfig.cpp \
    TPMS/Filter/CompressFilter.cpp \
    TPMS/MarchBoxTable.cpp \
    TPMS/MarchBoxUtil.cpp \
    TPMS/TpmsImplicitFunction.cpp \
    Voxel/VoxelModel.cpp \
    Voxel/VoxelOctree.cpp \
    implicitsurface.cpp \
    isolevelgenerator.cpp \
    marchbox.cpp \
    qualityinspection.cpp \
    simplification.cpp \
    smoothtool.cpp \
    generalporositycalculator.cpp \

HEADERS += \
    IO/Convertor.h \
    IO/Exporter.h \
    IO/Importer.h \
    Math/EigenUtil.h \
    Mesh/Mesh.h \
    Mesh/MeshCleaningTool.h \
    Mesh/MeshShellTool.h \
    Mesh/MeshSmoothTool.h \
    Mesh/PorosityCalculator.h \
    SurfaceMesh/OctreeUtil.h \
    TPMS/BaseTpmsAlgorithm.h \
    TPMS/BaseTpmsConfig.h \
    TPMS/BaseTpmsSingleSurfaceConfig.h \
    TPMS/BoxTpmsSingeSurfaceAlgorithm.h \
    TPMS/BoxTpmsSingleSurfaceConfig.h \
    TPMS/CustomTpmsSingleSurfaceAlgorithm.h \
    TPMS/CustomTpmsSingleSurfaceConfig.h \
    TPMS/Filter/BaseSampleMatrixFilter.h \
    TPMS/Filter/CompressFilter.h \
    TPMS/MarchBoxTable.h \
    TPMS/MarchBoxUtil.h \
    TPMS/SamplePoint.h \
    TPMS/TpmsImplicitFunction.h \
    Voxel/VoxelModel.h \
    Voxel/VoxelOctree.h \
    implicitsurface.h \
    isolevelgenerator.h \
    marchbox.h \
    qualityinspection.h \
    simplification.h \
    smoothtool.h \
    generalporositycalculator.h \

unix {
    DEFINES += USING_SURFACEMESH
#    DEFINES += DEBUG_INFO
}

contains(DEFINES,USING_SURFACEMESH) {
    unix {
        INCLUDEPATH += \
            $$PWD/../extern/surfacemesh \
            $$PWD/../extern/surfacemesh/helpers \
            $$PWD/../extern/surfacemesh/parameters \
            $$PWD/../extern/starlib \
            $$PWD/../extern/octree

        DEPENDPATH += \
            $$PWD/../extern/surfacemesh \
            $$PWD/../extern/surfacemesh/helpers \
            $$PWD/../extern/surfacemesh/parameters \
            $$PWD/../extern/starlib

        LIBS += \
            -L$$OUT_PWD/../bin -lsurfacemesh -lstarlib
    }

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
