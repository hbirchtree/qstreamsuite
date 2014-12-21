import QtQuick 2.0

MouseArea{
    id:touchoverlay
    anchors.fill:parent
    width:600
    height:600
    signal newEvent(int type,int v1,int v2)
    signal clockOn(bool enable)
    onClockOn: {
        if(enable){
            stick.clock=true
        }else
            stick.clock=false
    }
    onEnabledChanged: {
        if(enabled){
            clockOn(true)
        }else{
            clockOn(false)
        }
    }

    function buttonsToggle(){
        if(buttonContainer.visible){
            buttonContainer.enabled=false
            buttonContainer.visible=false
        }else{
            buttonContainer.enabled=true
            buttonContainer.visible=true
        }
    }
    function sticksToggle(){
        if(stick.visible){
            stick.enabled=false
            stick.visible=false
        }else{
            stick.enabled=true
            stick.visible=true
        }
    }
    function mBtnsToggle(){
        if(mouseButtons1.visible){
            mouseButtons1.enabled=false
            mouseButtons1.visible=false
        }else{
            mouseButtons1.enabled=true
            mouseButtons1.visible=true
        }
    }

    Item{
        anchors.right: parent.right
        id:buttonContainer
        DigitalKey{
            anchors.right: parent.right
            width:200
            height:200
            btn.onPressed: newEvent(6,32,0)
            btn.onReleased: newEvent(7,32,0)
        }
    }
    AnalogStickImplementation{
        id:stick
        height:width
        anchors.rightMargin: 180
        anchors.right: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        onNewEvent: parent.newEvent(type,v1,v2)
    }

    MouseButtons {
        id: mouseButtons1
        x: 300
        y: 445
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 8
        leftClick.onPressed: captureEvent(2,1,0)
        leftClick.onReleased: captureEvent(3,1,0)
        rightClick.onPressed: captureEvent(2,2,0)
        rightClick.onReleased: captureEvent(3,2,0)
    }
    onPressed: mouse.accepted=false
    onReleased: mouse.accepted=false
}
