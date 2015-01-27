#-------------------------------------------------
#
# Project created by QtCreator 2014-12-23T21:45:15
#
#-------------------------------------------------

QT       -= gui
QT += multimedia

TARGET = pulse-capture
TEMPLATE = lib

LIBS += -lpulse-simple

DEFINES += PULSECAPTURE_LIBRARY

SOURCES += pulsecapture.cpp

HEADERS += pulsecapture.h\
        pulse-capture_global.h \
    ../QStreamSuite/QStreamServer/capturinginterface.h \
    ../../../QStreamServer/capturinginterface.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
