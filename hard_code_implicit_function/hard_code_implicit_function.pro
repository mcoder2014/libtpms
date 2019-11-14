TEMPLATE = app
CONFIG += console c++11
#CONFIG -= app_bundle
QT += gui core opengl

DESTDIR = ../bin/

SOURCES += \
        implicitsurface.cpp \
        main.cpp \
        marchbox.cpp \
    simplification.cpp \
        smoothtool.cpp \


HEADERS += \
    implicitsurface.h \
    marchbox.h \
    simplification.h \
    smoothtool.h \


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
    INCLUDEPATH += /usr/include/eigen3 \
        /usr/include/assimp

    LIBS += \
        -L/usr/lib -lOpenMeshCore -lOpenMeshTools -lassimp
}

win32 {

}
