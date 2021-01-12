# lydialab extern

# 为了和 lydialab 一起使用
unix {
    LYDIALAB_BASEPATH = /home/chaoqun/workspace/3d_insole_design/projects/LydiaLab
    LYDIALAB_EXECUTE_BASEPATH = /home/chaoqun/Desktop/LydiaLab/LydiaLabD

    message($${LYDIALAB_BASEPATH})

    INCLUDEPATH += \
        $${LYDIALAB_BASEPATH}/surfacemesh/surfacemesh \
        $${LYDIALAB_BASEPATH}/core/starlib \
        $${LYDIALAB_BASEPATH}/core/external/octree

    DEPENDPATH += \
        $${LYDIALAB_BASEPATH}/surfacemesh/surfacemesh \
        $${LYDIALAB_BASEPATH}/core/starlib \
        $${LYDIALAB_BASEPATH}/core/external/octree

    LIBS += \
        -L$${LYDIALAB_EXECUTE_BASEPATH} -lsurfacemesh -lstarlib
}

# 如果单独编译，可以使用本地源码

#    unix {
#        INCLUDEPATH += \
#            $$PWD/../extern/surfacemesh \
#            $$PWD/../extern/surfacemesh/helpers \
#            $$PWD/../extern/surfacemesh/parameters \
#            $$PWD/../extern/starlib \
#            $$PWD/../extern/octree

#        DEPENDPATH += \
#            $$PWD/../extern/surfacemesh \
#            $$PWD/../extern/surfacemesh/helpers \
#            $$PWD/../extern/surfacemesh/parameters \
#            $$PWD/../extern/starlib

#        LIBS += \
#            -L$$OUT_PWD/../bin -lsurfacemesh -lstarlib
#    }
