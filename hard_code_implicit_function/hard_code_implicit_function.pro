TEMPLATE = app
CONFIG += console c++11
#CONFIG -= app_bundle
QT += gui core

DESTDIR = ../bin/

SOURCES += \
        implicitsurface.cpp \
        main.cpp \
        marchbox.cpp

HEADERS += \
    implicitsurface.h \
    marchbox.h
