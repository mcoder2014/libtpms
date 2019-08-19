TEMPLATE = app
CONFIG += console c++11
#CONFIG -= app_bundle
QT += gui core

DESTDIR = ../bin/

SOURCES += \
        implicitsurface.cpp \
        main.cpp \
        marchbox.cpp \
        smoothtool.cpp

HEADERS += \
    implicitsurface.h \
    marchbox.h \
    smoothtool.h

#INCLUDEPATH += /usr/include

LIBS += \
    -L/usr/lib -lOpenMeshCore -lOpenMeshTools
