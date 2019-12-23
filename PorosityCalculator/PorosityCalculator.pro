TEMPLATE = app
TARGET = PorosityCalculator
CONFIG += console c++11
#CONFIG -= app_bundle
QT += gui core opengl

DESTDIR = ../bin/
# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

MBPATH = $$PWD/../hard_code_implicit_function

SOURCES += \
    $${MBPATH}/implicitsurface.cpp \
    $${MBPATH}/isolevelgenerator.cpp \
    $${MBPATH}/marchbox.cpp \
    $${MBPATH}/qualityinspection.cpp \
    $${MBPATH}/simplification.cpp \
    $${MBPATH}/smoothtool.cpp \
    generalporositycalculator.cpp \
    main.cpp \

HEADERS += \
    $${MBPATH}/implicitsurface.h \
    $${MBPATH}/isolevelgenerator.h \
    $${MBPATH}/marchbox.h \
    $${MBPATH}/qualityinspection.h \
    $${MBPATH}/simplification.h \
    $${MBPATH}/smoothtool.h \
    generalporositycalculator.h \


unix {
    DEFINES += USING_SURFACEMESH
#    DEFINES += DEBUG_INFO
}

contains(DEFINES,USING_SURFACEMESH) {
    message('DEFINES USING_SURFACEMESH')

    HEADERS += \
        surfacemesh_load.h \
        $$PWD/../extern/octree/Octree.h \
        $$PWD/../extern/octree/primitives.h \
        $$PWD/../extern/octree/TriAccel.h

    SOURCES += \
        surfacemesh_load.cpp

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

    win32 {

    }


} else {
    message('Not DEFINE USING_SURFACEMESH')
}

unix {
    #include path and libraries
    INCLUDEPATH += /usr/include/eigen3 \
        /usr/include/assimp \
        /usr/include/opencv4

    LIBS += \
        -L/usr/lib -lOpenMeshCore -lOpenMeshTools \     # openmesh
        -lassimp \      # assimp
        -lopencv_core -lopencv_imgproc  # opencv
}

win32 {

}
