import QtQuick 2.4
import QtMultimedia 5.4
import EventCapture 1.0

Rectangle {
    width: 400
    height: 400
    color: "black"

    property alias inSurface: eventCapture
    property alias outSurface: vplayer

    Video{
        id: vplayer
        anchors.fill: parent
    }

    EventCaptor {
        id: eventCapture
        anchors.fill:parent
        focus:true
    }
}

