import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.1
import QtMultimedia 5.4

ApplicationWindow{
    title: qsTr("Streamer")
    id: applicationWindow
    visible:true
    width:960
    height:540

    //Used to define screen area
    property int s_x: 0
    property int s_y: 0
    property int s_w: 0
    property int s_h: 0

    function setDisplayRect(x,y,w,h){
        if(x>0)s_x = x
        if(y>0)s_y = y
        if(w>0)s_w = w
        if(h>0)s_h = h
    }
    function changeSurfaceScale(x_real,y_real){
        console.log(x_real)
        sview.inSurface.xscale = x_real
        sview.inSurface.yscale = y_real
        console.log(sview.inSurface.xscale)
    }
    function applyScreenRect(){
        console.log("Applying rect")
        changeSurfaceScale(s_w/sview.outSurface.width,s_h/sview.outSurface.height)
    }

    signal insertPlugin()

    signal connectServer(string host,int port)
    onConnectServer: {loading.startLoading();statusBubbleShow("Connecting to host")}
    function connectedServer(){ //Called when the server is actually connected
        statusBubbleShow(qsTr("Connected!"))
        sview.enabled = true
        sview.outSurface.visible = true
        loading.stopLoading()
        t_dis.visible=true
    }

    function main_disconnect(){
        sview.outSurface.stop()
        sview.outSurface.visible = false
        sview.enabled = false
        t_con.visible=true
        t_dis.visible=false
        loading.stopLoading()
        statusBubbleShow("Disconnected from server")
    }
    function fail_disconnect(){
        sview.outSurface.stop()
        sview.outSurface.visible = false
        sview.inSurface.enabled = false
        t_con.visible=true
        t_dis.visible=false
        loading.stopLoading()
        console.log("Failure")
    }

    signal disconnectServer()
    onDisconnectServer: {
        main_disconnect()
    }

    signal captureEvent(int type,int val1,int val2)
    function displayStream(url){ //Called when server is connected, should receive a URL to an RTMP stream
        sview.outSurface.stop()
        sview.outSurface.source = url
        sview.outSurface.play()
    }
    signal testSignal() //Just for science

    signal statusBubblePop();onStatusBubblePop:{status.aniFadeOut.start()} //Used to close status bubble
    function statusBubbleShow(message){
        status.updateStatusText(message)
        console.log(message)
        status.visible = true
        status.aniFadeIn.start()
    }

    StreamView {
        id:sview
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.fill:parent
        outSurface.visible: false
        enabled: false
        onHeightChanged: applyScreenRect()
        inSurface.onKeyPressed: captureEvent(type,keycode,modifiers)
        inSurface.onWheelMoved: captureEvent(type,delta.x,delta.y)
        inSurface.onMouseMovedAbs: captureEvent(type,x+s_x,y+s_y)
        inSurface.onMouseMovedRel: captureEvent(type,x,y)
        function inputGrab(){focus = true;inSurface.focus = true}
        function toggleOverlay(){
            if(toverlay.enabled){
                toverlay.enabled=false
                toverlay.visible=false
            }else{
                toverlay.enabled=true
                toverlay.visible=true
            }
        }
        TouchOverlay{
            id:toverlay
            anchors.fill:parent
            onNewEvent: captureEvent(type,v1,v2)
        }
    }


    ClientConnect {
        id: clientConnect
        x: parent.width/2-width/2
        y: parent.height/2-height/2
        width:parent.width/1.5
        height:parent.height/1.5
        enabled:false
        visible:false
        function checkConnect(){
            if(!ipField.text==""||!portField.text==""){
                connectServer(ipField.text,portField.text)
                ipField.text = ""
                portField.text = ""
                clientConnect.aniFadeOut.start()
                clientConnect.enabled = false
                sview.inputGrab()
            }
        }
        function showUi(){
            visible = true
            clientConnect.aniFadeIn.start()
            clientConnect.enabled = true
        }
        aniFadeOut.onStopped: visible = false
        button.onClicked: {checkConnect()}
    }



    LoaderAnim{
        id:loading
        visible:false
        animation.loops: Animation.Infinite
        x:parent.width/2-width
        y:parent.height/2-height
        width:parent.height/12
        height:width
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        enabled: false
        function startLoading(message){
            visible = true
            animation.start()
            if(!message==""){
                statusBubbleShow(message)
            }
        }
        function stopLoading(){
            animation.stop()
            visible = false
        }
    }
    StatusBubble{
        id:status
        anchors.horizontalCenter: parent.horizontalCenter
        visible:false
        width: 240+parent.width/6
        height: 64+parent.height/9
        anchors.bottom: loading.top
        popper.onClicked: {aniFadeOut.start();mouse.accepted=true}
        aniFadeOut.onStopped: visible = false
    }

    toolBar:ToolBar{RowLayout{anchors.fill:parent
            ToolButton{id:t_con;/*text: qsTr("Connect");*/iconSource:"/icons/conn";onClicked: {clientConnect.showUi();visible=false}}
            ToolButton{id:t_dis;visible:false;/*text: qsTr("Disconnect");*/iconSource:"/icons/disco";onClicked: {disconnectServer()}}
            ToolButton{
                //                text: qsTr("Test messages/loader")
                iconSource: "/icons/test"
                onClicked: {
                    changeSurfaceScale(2,2)
                    testSignal()
                    if(loading.animation.running){loading.stopLoading()
                    }else{loading.startLoading()}
                }
            }

            Item{Layout.fillWidth: true}

            ToolButton{
                //                text: qsTr("Fill mode")
                iconSource: "/icons/fit"
                onClicked: {
                    if(sview.outSurface.fillMode == VideoOutput.Stretch){sview.outSurface.fillMode = VideoOutput.PreserveAspectFit
                    }else if(sview.outSurface.fillMode == VideoOutput.PreserveAspectFit) sview.outSurface.fillMode = VideoOutput.Stretch
                }
            }
            //            ToolButton{text: "Focus";onClicked: {sview.inputGrab()}}
            ToolButton{
                //                text: "Tap-click"
                iconSource: "/icons/relative"
                checkable: true
                checked: true
                onCheckedChanged: {
                    if(checked){sview.inSurface.tapClick = true;iconSource = "/icons/relative"}
                    else{sview.inSurface.tapClick = false;iconSource = "/icons/absolute"}
                }
            }
            ToolButton{
                iconSource: "/icons/tools"
                onClicked: adM.popup()
            }

            Menu{
                id:adM
                title: "LOL"
                iconSource: "/icons/tools"
                items: [
                    MenuItem{
                        text: "Audio"
                        iconSource: "/icons/sound"
                        checkable: true
                        checked:true
                        onTriggered: {
                            if(checked){
                                sview.outSurface.volume = 1
                                iconSource = "/icons/sound"
                            }else{
                                sview.outSurface.volume = 0
                                iconSource = "/icons/mute"
                            }
                        }
                    },
                    MenuItem{
                        text:"Capture keys"
                        iconSource: "/icons/kbd"
                        checkable: true
                        checked:true
                        onTriggered: {
                            if(checked){
                                sview.inSurface.keyCapture = true
                            }else{
                                sview.inSurface.keyCapture = false
                            }
                        }
                    },
                    MenuItem{
                        text:"Focus"
                        iconSource: "/icons/focus"
                        onTriggered: sview.inputGrab()
                    },
                    MenuItem{
                        text:"Fullscreen"
                        checked:false
                        checkable:true
                        onTriggered:{
                            if(checked){
                                applicationWindow.visibility=5
                            }else
                                applicationWindow.visibility=2
                        }
                    },
                    MenuItem{
                        text:"Analog stick"
                        checked:true
                        checkable:true
                        onTriggered:{
                            toverlay.sticksToggle()
                        }
                    },
                    MenuItem{
                        text:"Buttons"
                        checked:true
                        checkable:true
                        onTriggered:{
                            toverlay.buttonsToggle()
                        }
                    },
                    MenuItem{
                        text:"Touch overlay"
                        checked:true
                        checkable:true
                        onTriggered:{
                            sview.toggleOverlay()
                        }
                    },
                    MenuItem{
                        text:"Mouse buttons"
                        checked:true
                        checkable:true
                        onTriggered:{
                            toverlay.mBtnsToggle()
                        }
                    },
                    MenuItem{
                        text:"Insert plugin"
                        onTriggered:{
                            insertPlugin()
                        }
                    },
                    MenuItem{
                        text:"Quit"
                        onTriggered: Qt.quit()
                    }

                ]
            }
        }
    }
}
