import QtQuick 2.0

Item{
    id: loadingThang
    x: 0
    y: 0
    width: 100
    height: 100

    signal resetPoints()

    property alias animation: loadAni
    property color recolor:"#0076a0"
    property int recRad:width/8

    onResetPoints: {
        c1.x = width/2-c1.width/2
        c1.y = 0
        c2.x = width-c2.width
        c2.y = width/2-c2.width/2
        c3.x = width/2-c3.width/2
        c3.y = width-c3.width
        c4.x = 0
        c4.y = width/2-c4.width/2
    }

    Rectangle {
        id: c4
        x: 0
        y: parent.width/2-c4.width/2
        width: recRad*2
        height: width
        color: recolor
        radius: recRad
    }

    Rectangle {
        id: c2
        x: parent.width-c2.width
        y: parent.width/2-c2.width/2
        width: recRad*2
        height: width
        color: recolor
        radius: recRad
    }

    Rectangle {
        id: c3
        x: parent.width/2-c3.width/2
        y: parent.width-c3.width
        width: recRad*2
        height: width
        color: recolor
        radius: recRad
    }

    Rectangle {
        id: c1
        x: parent.width/2-c1.width/2
        y: 0
        width: recRad*2
        height: width
        color: recolor
        radius: recRad
    }
    ParallelAnimation {
        id: loadAni
        running:false
        alwaysRunToEnd: true
        onStopped: resetPoints()

        //C1 animation
        PropertyAnimation {
            target: c1
            properties: "x"
            from: c1.x
            to: c2.x
        }
        PropertyAnimation {
            target: c1
            properties: "y"
            from: c1.y
            to: c2.y
        }
        NumberAnimation {target: c1; easing.type: Easing.InExpo; duration: 400;}
        //C3 animation
        PropertyAnimation {
            target: c3
            properties: "x"
            from: c3.x
            to: c4.x
        }
        PropertyAnimation {
            target: c3
            properties: "y"
            from: c3.y
            to: c4.y
        }
        NumberAnimation {target: c3; easing.type: Easing.InExpo; duration: 400;}
        //C2 animation
        PropertyAnimation {
            target: c2
            properties: "x"
            from: c2.x
            to: c3.x
        }
        PropertyAnimation {
            target: c2
            properties: "y"
            from: c2.y
            to: c3.y
        }

        NumberAnimation { target: c2; easing.type: Easing.InExpo; duration: 400;}
        //C4 animation
        PropertyAnimation {
            target: c4
            properties: "x"
            from: c4.x
            to: c1.x
        }
        PropertyAnimation {
            target: c4
            properties: "y"
            from: c4.y
            to: c1.y
        }
        NumberAnimation {target: c4; easing.type: Easing.InExpo; duration: 400;}
    }
}
