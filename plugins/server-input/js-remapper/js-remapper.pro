#-------------------------------------------------
#
# Project created by QtCreator 2014-12-21T22:57:25
#
#-------------------------------------------------

QT       -= gui

TARGET = js-remapper
TEMPLATE = lib

DEFINES += JSREMAPPER_LIBRARY

SOURCES += jsremap.cpp

HEADERS += jsremap.h\
        js-remapper_global.h \
    ../QStreamSuite/QStreamServer/inputhandlerobjectinterface.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
