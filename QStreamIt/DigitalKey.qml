import QtQuick 2.0

Rectangle {

    property alias btn:button
    property color bNormal:"#292929"
    property color bPressed:"#696969"
    id: keyBase
    width: 160
    height: width
    radius: width/4
    border.width: 2
    color:bNormal
    MultiPointTouchArea{
        id:button
        maximumTouchPoints: 1
        minimumTouchPoints: 1
        anchors.fill:parent
        onPressed: {keyBase.color=bPressed}
        onReleased: {keyBase.color=bNormal}
    }
}

