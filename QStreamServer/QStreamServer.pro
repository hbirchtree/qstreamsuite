#-------------------------------------------------
#
# Project created by QtCreator 2014-11-18T12:24:05
#
#-------------------------------------------------

QT       += core gui network qml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QStreamServer
TEMPLATE = app


SOURCES += main.cpp\
        streamserver.cpp \
    networker.cpp \
    inputhandler.cpp \
    configreader.cpp \
    ../socketworker.cpp \
    ../jsoncomm.cpp

HEADERS  += streamserver.h \
    networker.h \
    inputhandler.h \
    configreader.h \
    ../streamer_enums.h \
    ../socketworker.h \
    ../jsoncomm.h \
    inputhandlerobjectinterface.h

FORMS    += streamserver.ui

#DEFINES += QT_NO_DEBUG_OUTPUT

OTHER_FILES += \
    test-config.json
