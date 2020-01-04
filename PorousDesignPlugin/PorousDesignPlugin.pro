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
