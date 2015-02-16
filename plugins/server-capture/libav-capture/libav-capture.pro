#-------------------------------------------------
#
# Project created by QtCreator 2015-01-26T10:23:04
#
#-------------------------------------------------

QT       -= gui

TARGET = libav-capture
TEMPLATE = lib

LIBS += -lavcodec -lavformat -lswscale

DEFINES += LIBAVCAPTURE_LIBRARY

SOURCES += libavcapture.cpp

HEADERS += libavcapture.h\
        libav-capture_global.h \
    ../../../QStreamServer/capturinginterface.h \
    muxer.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
