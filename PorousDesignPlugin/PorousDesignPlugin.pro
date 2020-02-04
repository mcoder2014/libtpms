# 这些在变量 starlab core.pro 的文件中被设置为环境变量

include($$[STARLAB])
include($$[SURFACEMESH])
include($$[NANOFLANN])
include($$[OCTREE])

##--- SPECIFIES CORE CONFIGURATION
#system(qmake -set STARLAB       $$PWD/starlab.prf)
#system(qmake -set EIGENPATH     $$PWD/external/eigen-3.2.5)
#system(qmake -set CHOLMOD       $$PWD/external/cholmod-4.0.0/cholmod.prf)
#system(qmake -set QHULL         $$PWD/external/qhull-2012.1/qhull.prf)
#system(qmake -set CGAL          $$PWD/external/cgal-4.2/cgal.prf)
#system(qmake -set OPENNI        $$PWD/external/openni-2.1alpha/openni.prf)
#system(qmake -set NANOFLANN     $$PWD/external/nanoflann-1.1.9/nanoflann.prf)
#system(qmake -set FLANN         $$PWD/external/flann-1.8.4/flann.prf)
#system(qmake -set MATLAB        $$PWD/external/matlab/matlab.prf)
#system(qmake -set KDTREEMATLAB  $$PWD/external/kdtree-matlab/kdtree-matlab.prf)
#system(qmake -set OCTREE        $$PWD/external/octree/octree.prf)
#system(qmake -set PQP           $$PWD/external/pqp-2.0/pqp.prf)

# 设置工程为 plugin 配置，具体出现在 $$[STARLAB] 中
StarlabTemplate(plugin)

QT += gui opengl xml svg printsupport widgets


win32{
    # Project Setting on windows system
    include($$[QHULL])
    message("Qhull" $$[QHULL])

    # Assimp
    INCLUDEPATH += \
        E:/libs/Assimp/include     # Assimp

    LIBS += \
        -lE:/libs/Assimp/lib/x64/assimp-vc140-mt
}

unix{
    # project setting on unix-like system(linux)
    # Add the qhull manually
    INCLUDEPATH += \
        /usr/include/libqhullcpp

    LIBS += \
        -L/usr/lib -lqhull -lqhull_p -lqhull_r \
        -lqhull_p -lqhullcpp -lqhull_r \      # qhull
        -lassimp    # assimp
}

HEADERS += \
    controlwidget.h \
    parameter.h \
    porousdesignplugin.h \
    qmesh/qabstractmesh.h \
    qmesh/qboxmesh.h \
    qmesh/qtrianglemesh.h \
    scene.h \
    QMesh \
    QuickMeshDraw.h

SOURCES += \
    controlwidget.cpp \
    porousdesignplugin.cpp \
    qmesh/qabstractmesh.cpp \
    qmesh/qboxmesh.cpp \
    qmesh/qtrianglemesh.cpp \
    scene.cpp

FORMS += \
    controlwidget.ui

RESOURCES += \
    porousdesign.qrc

## Include libtpms ##
unix {
    DEFINES += USING_SURFACEMESH
}

contains(DEFINES,USING_SURFACEMESH) {
    message('DEFINES USING_SURFACEMESH')

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
        /usr/include/opencv4 \
        /usr/include/qt \
        /usr/include/qt/QtGui \
        /usr/include/qt/QtCore \
        /usr/include/qt/QtOpenGL \
        /usr/include/qt/QtWidgets \
        $$PWD/../libtpms

    LIBS += \
        -L/usr/lib -lOpenMeshCore -lOpenMeshTools \     # openmesh
        -lassimp \      # assimp
        -lopencv_core -lopencv_imgproc \ # opencv
        -lQt5Core -lQt5Gui -lQt5Core -lQt5OpenGL -lQt5Widgets \   # Qt
        -L$$OUT_PWD/../bin -ltpmslib
}
