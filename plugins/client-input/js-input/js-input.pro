#-------------------------------------------------
#
# Project created by QtCreator 2014-12-21T06:26:54
#
#-------------------------------------------------

QT += core
QT -= gui

TARGET = js-input
TEMPLATE = lib

LIBS += -lSDL2

SOURCES += joystickinput.cpp

HEADERS += joystickinput.h \
    ../QStreamSuite/QStreamIt/inputplugininterface.h
DISTFILES += js-input.json

unix {
    target.path = /usr/lib
    INSTALLS += target
}
