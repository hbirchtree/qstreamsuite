import QtQuick 2.0
import DigitalKeyComponent 1.0

MultiPointTouchArea{
    property string label: "A"
    property color bNormal:"#292929"
    property color bPressed:"#696969"
    signal newEvent(int type,int keycode)
    onNewEvent: {if(type==6) keyBase.color=bPressed;if(type==7) keyBase.color=bNormal}
    width: 160
    height: width
    onPressed: newEvent(6,keyObj.keycode)
    onReleased: newEvent(7,keyObj.keycode)
    DigitalKeySurface{
        id:keyObj
        keySignal: label
    }
    Rectangle {
        anchors.fill:parent
        id: keyBase
        radius: parent.width/4
        border.width: 2
        color:bNormal
    }
    Text {
        id: text1
        color: "#ffffff"
        text: label
        anchors.fill: parent
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: height/2
    }
}

