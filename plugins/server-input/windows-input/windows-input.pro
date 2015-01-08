#-------------------------------------------------
#
# Project created by QtCreator 2014-12-29T12:22:58
#
#-------------------------------------------------

QT       += core gui

TARGET = windows-input
TEMPLATE = lib

LIBS += -luser32

DEFINES += WINDOWSINPUT_LIBRARY

SOURCES += windowsinput.cpp

HEADERS += windowsinput.h\
        windows-input_global.h \
    ../StreamSuite/QStreamServer/inputhandlerobjectinterface.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
