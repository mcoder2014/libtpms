TEMPLATE = lib
TARGET = tpmslib
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

DESTDIR = ../bin/

SOURCES += \
    IO/exporter.cpp \
    IO/importer.cpp \
    TPMS/BaseTpmsConfig.cpp \
    TPMS/BoxTpmsSingeSurfaceAlgorithm.cpp \
    TPMS/BoxTpmsSingleSurfaceConfig.cpp \
    TPMS/TpmsImplicitSurface.cpp \
    implicitsurface.cpp \
    isolevelgenerator.cpp \
    marchbox.cpp \
    mesh.cpp \
    qualityinspection.cpp \
    simplification.cpp \
    smoothtool.cpp \
    generalporositycalculator.cpp \

HEADERS += \
    IO/exporter.h \
    IO/importer.h \
    TPMS/BaseTpmsAlgorithm.h \
    TPMS/BaseTpmsConfig.h \
    TPMS/BoxTpmsSingeSurfaceAlgorithm.h \
    TPMS/BoxTpmsSingleSurfaceConfig.h \
    TPMS/TpmsImplicitSurface.h \
    implicitsurface.h \
    isolevelgenerator.h \
    marchbox.h \
    mesh.h \
    qualityinspection.h \
    simplification.h \
    smoothtool.h \
    generalporositycalculator.h \

unix {
    DEFINES += USING_SURFACEMESH
#    DEFINES += DEBUG_INFO
}

contains(DEFINES,USING_SURFACEMESH) {
    message('DEFINES USING_SURFACEMESH')

#    HEADERS += \
#        $$PWD/../extern/octree/Octree.h \
#        $$PWD/../extern/octree/primitives.h \
#        $$PWD/../extern/octree/TriAccel.h

#    SOURCES +=

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
        -lQt5Core -lQt5Gui -lQt5Core -lQt5OpenGL -lQt5Widgets    # Qt
}
