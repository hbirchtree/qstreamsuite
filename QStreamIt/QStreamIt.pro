TEMPLATE = app

QT += widgets network opengl qml quickwidgets multimedia

SOURCES += main.cpp \
    streamerfrontend.cpp \
    ../socketworker.cpp \
    datastore.cpp

RESOURCES += qml-components/qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    streamerfrontend.h \
    ../streamer_enums.h \
    ../socketworker.h \
    ../jsoncomm.h \
    datastore.h \
    inputplugininterface.h \
    ui-components/analogstick.h \
    ui-components/trackpoint.h \
    ui-components/eventcapture.h \
    ui-components/digitalkey.h \
    ui-components/qt-components.h \
    ../streamer_global.h

#DEFINES += QT_NO_DEBUG_OUTPUT

DISTFILES += \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/AndroidManifest.xml \
    android/gradlew.bat \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
