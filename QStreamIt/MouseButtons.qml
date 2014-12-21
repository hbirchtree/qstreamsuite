import QtQuick 2.0

Item{
    id: item1
    width: 360
    height: 50

    property alias leftClick: leftClick
    property alias rightClick: rightClick

    property color normal: "#2b2b2b"
    property color click: "#868686"

    Rectangle {
        id: rectangle1
        color: "#2b2b2b"
        radius: height/2
        border.width: 2
        anchors.right: parent.horizontalCenter
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left

        MultiPointTouchArea {
            id: leftClick
            anchors.fill: parent
            onPressed: {
                rectangle1.color = click
            }
            onReleased: {
                rectangle1.color = normal
            }
        }
    }

    Rectangle {
        id: rectangle2
        x: 4
        y: 2
        color: "#2b2b2b"
        radius: height/2
        border.width: 2
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: parent.horizontalCenter

        MultiPointTouchArea {
            id: rightClick
            anchors.fill: parent
            onPressed: {
                rectangle2.color = click
            }
            onReleased: {
                rectangle2.color = normal
            }
        }
    }

}
