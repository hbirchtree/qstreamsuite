#-------------------------------------------------
#
# Project created by QtCreator 2014-12-17T16:18:13
#
#-------------------------------------------------

QT += core gui

TARGET = x11-xinput-handler
TEMPLATE = lib

LIBS += -lX11 -lXtst

DEFINES += LIBX11XINPUTHANDLER_LIBRARY

SOURCES += xinputhandler.cpp

HEADERS += xinputhandler.h\
        libx11-xinput-handler_global.h \
    ../QStreamSuite/QStreamServer/inputhandlerobjectinterface.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
