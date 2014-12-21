import QtQuick 2.0

Rectangle {
    id:stB
    width: 240
    height: 64
    color: defaultBg
    radius: height/3
    border.width: 3
    border.color: defaultBgB

    property color defaultBg:"#5d5d5d"
    property color defaultBgB:"#000000"
    property color defaultFg:"#ffffff"
    signal updateStatusTextC(string message,color col)
    signal updateStatusText(string message)

    property alias aniFadeIn: tIn
    property alias aniFadeOut: tOut
    property alias popper: deactivator

    onUpdateStatusTextC: {statusText.text = message; color = col}
    onUpdateStatusText: {statusText.text = message}

    Text {
        id: statusText
        color: defaultFg
        text: qsTr("STATUS TEXT")
        anchors.fill: parent
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WordWrap
    }

    MouseArea{ //Used to pop the bubble
        id: deactivator
        anchors.fill: parent
    }

    PropertyAnimation{
        target:stB
        properties: "opacity"
    }

    NumberAnimation {
        id:tIn
        target: stB
        easing.type: Easing.InQuad
        properties: "opacity"
        duration: 200
        to:1
    }
    NumberAnimation {
        id:tOut
        target: stB
        easing.type: Easing.OutQuad
        properties: "opacity"
        duration: 200
        to:0
    }
}
