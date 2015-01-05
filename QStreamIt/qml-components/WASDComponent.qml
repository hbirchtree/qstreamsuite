import QtQuick 2.0

Item {
    width: height*1.5
    height: 200
    signal newEvent(int type,int v1,int v2)
    DigitalKey {
        id: w
        width: parent.width/3
        height: width
        label: "W"
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        onNewEvent: parent.newEvent(type,keycode,0)
    }

    DigitalKey {
        id: s
        width: parent.width/3
        height: width
        label: "S"
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        onNewEvent: parent.newEvent(type,keycode,0)
    }

    DigitalKey {
        id: a
        width: parent.width/3
        height: width
        label: "A"
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        onNewEvent: parent.newEvent(type,keycode,0)
    }

    DigitalKey {
        id: d
        width: parent.width/3
        height: width
        label: "D"
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        onNewEvent: parent.newEvent(type,keycode,0)
    }
}

