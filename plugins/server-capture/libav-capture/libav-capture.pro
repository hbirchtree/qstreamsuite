#-------------------------------------------------
#
# Project created by QtCreator 2015-01-26T10:23:04
#
#-------------------------------------------------

QT       -= gui

TARGET = libav-capture
TEMPLATE = lib

LIBS += -lavutil -lavformat -lavcodec -lswscale -lswresample

DEFINES += LIBAVCAPTURE_LIBRARY

SOURCES += libavcapture.cpp

HEADERS += libavcapture.h\
        libav-capture_global.h \
    ../../../QStreamServer/capturinginterface.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
