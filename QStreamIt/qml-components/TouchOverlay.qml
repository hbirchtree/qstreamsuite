import QtQuick 2.0

MultiPointTouchArea{
    id:touchoverlay
    anchors.fill:parent
    width:1000
    height:1000
    property int keysize: stick.width/2
    signal newEvent(int type,int v1,int v2)
    onNewEvent: console.log(type,v1,v2)
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
    DigitalKey{
        id:key1
        anchors.right:key2.left
        anchors.top:key4.verticalCenter
        width:keysize
        height:keysize
        onNewEvent: parent.newEvent(type,keycode,0)
        label: "1"
    }
    DigitalKey{
        id:key2
        anchors.right: key3.left
        anchors.top:key3.verticalCenter
        width:keysize
        height:keysize
        onNewEvent: parent.newEvent(type,keycode,0)
        label: "2"
    }
    DigitalKey{
        id:key3
        anchors.right: parent.right
        anchors.top:key4.verticalCenter
        width:keysize
        height:keysize
        onNewEvent: parent.newEvent(type,keycode,0)
        label: "3"
    }
    DigitalKey{
        id:key4
        anchors.right: key3.left
        anchors.top: parent.top
        width:keysize
        height:keysize
        onNewEvent: parent.newEvent(type,keycode,0)
        label: "4"
    }
    DigitalKey {
        id: digitalKey1
        width: wasd.width/3
        height: width
        anchors.top: wasd.top
        anchors.topMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        label: "Q"
        onNewEvent: parent.newEvent(type,keycode,0)
    }

    DigitalKey {
        id: digitalKey2
        x: 424
        width: keysize
        height: width
        anchors.top: wasd.top
        anchors.topMargin: 0
        anchors.right: wasd.right
        anchors.rightMargin: 0
        label: "E"
        onNewEvent: parent.newEvent(type,keycode,0)
    }
    DigitalKey {
        id: digitalKey3
        x: 354
        width: keysize
        height: width
        anchors.right: digitalKey4.right
        anchors.rightMargin: 0
        anchors.top: digitalKey4.bottom
        anchors.topMargin: 0
        label: "F"
        onNewEvent: parent.newEvent(type,keycode,0)
    }
    DigitalKey {
        id: digitalKey4
        width: keysize
        height: width
        anchors.left: digitalKey2.right
        anchors.leftMargin: 0
        anchors.top: wasd.top
        anchors.topMargin: 0
        label: "R"
        onNewEvent: parent.newEvent(type,keycode,0)
    }
    WASDComponent{
        id:wasd
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        width:keysize*3
        height: keysize*2
        onNewEvent: parent.newEvent(type,v1,v2)
    }
    AnalogStickImplementation{
        id:stick
        x: 463
        y: 363
        height:width
        anchors.right: parent.right
        anchors.rightMargin: 2
        anchors.bottom: mouseButtons1.top
        anchors.bottomMargin: 6
        width: parent.width/14*3
        onNewEvent: parent.newEvent(type,v1,v2)
    }

    MouseButtons {
        id: mouseButtons1
        x: 215
        y: 515
        width: 385
        height: 100
        anchors.right: parent.right
        anchors.rightMargin: 2
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 2
        rotation: 0
        leftClick.onPressed: captureEvent(2,1,0)
        leftClick.onReleased: captureEvent(3,1,0)
        rightClick.onPressed: captureEvent(2,2,0)
        rightClick.onReleased: captureEvent(3,2,0)
    }
}
