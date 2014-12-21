TEMPLATE = app

QT += widgets network opengl qml quickwidgets multimedia

SOURCES += main.cpp \
    streamerfrontend.cpp \
    ../socketworker.cpp \
    ../jsoncomm.cpp \
    datastore.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    streamerfrontend.h \
    ../streamer_enums.h \
    ../socketworker.h \
    ../jsoncomm.h \
    eventcapture.h \
    datastore.h \
    analogstick.h \
    trackpoint.h \
    inputplugininterface.h

FORMS +=

#DEFINES += QT_NO_DEBUG_OUTPUT

DISTFILES += \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/AndroidManifest.xml \
    android/gradlew.bat \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    TECHNICAL.md

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
