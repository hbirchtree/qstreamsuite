import QtQuick 2.4
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2
import QtMultimedia 5.4
import "qrc:/qmlui/components"
import "qrc:/qmlui/input"

ApplicationWindow{
    title: qsTr("Streamer")
    id: applicationWindow
    visible:true
    width:960
    height:540

    property int s_x: 0
    property int s_y: 0
    property int s_w: 0
    property int s_h: 0
    property int netlate: 0

    function setDisplayRect(x,y,w,h){
        if(x>0)s_x = x
        if(y>0)s_y = y
        if(w>0)s_w = w
        if(h>0)s_h = h
    }
    MessageDialog {
        id: messageDial
        visible: false
        title: "QStream client"
        modality: Qt.WindowModal
        icon: StandardIcon.Information
        standardButtons: StandardButton.Ok

        function smack(caption,subcaption){
            messageDial.text = caption;
            subcaption = subcaption ? subcaption : ""
            messageDial.informativeText = subcaption;
            messageDial.open()
        }
    }

    function applyScreenRect(){
        changeSurfaceScale(s_w/sview.outSurface.width,s_h/sview.outSurface.height)
    }
    function changeSurfaceScale(x_real,y_real){
        sview.inSurface.xscale = x_real
        sview.inSurface.yscale = y_real
    }
    function connectingServer(){
        messageDial.smack("Connecting to host")
    }
    function connectedServer(){ //Called when the server is actually connected
        messageDial.smack(qsTr("Connected!"))
        sview.enabled = true
        sview.outSurface.visible = true
        t_dis.visible=true
        t_con.visible=false
    }
    function main_disconnect(){
        sview.outSurface.stop()
        sview.outSurface.visible = false
        sview.enabled = false
        t_con.visible=true
        t_dis.visible=false
        messageDial.smack("Disconnected from server")
    }
    function fail_disconnect(){
        sview.outSurface.stop()
        sview.outSurface.visible = false
        sview.inSurface.enabled = false
        t_con.visible=true
        t_dis.visible=false
        console.log("Failure")
    }
    function displayStream(url){ //Called when server is connected, should receive a URL to an RTMP stream
        sview.outSurface.stop()
        sview.outSurface.source = url
        sview.outSurface.play()
    }

    function statusBubbleShow(message){ //For legacy functionality
        messageDial.smack(message)
    }
    function inputGrab(){
        sview.focus = true
        sview.inSurface.focus = true
    }
    function toggleOverlay(){
        if(toverlay.enabled){
            toverlay.enabled=false
            toverlay.visible=false
        }else{
            toverlay.enabled=true
            toverlay.visible=true
        }
    }

    signal insertPlugin()
    signal connectServer(string host,int port)
    signal disconnectServer()
    signal captureEvent(int type,int val1,int val2)
    signal testSignal() //Just for science

    onHeightChanged: applyScreenRect()

    StreamView {
        id:sview
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0
        anchors.fill:parent
        outSurface.visible: false
        enabled: false
        inSurface.onKeyPressed: captureEvent(type,keycode,modifiers)
        inSurface.onWheelMoved: captureEvent(type,delta.x,delta.y)
        inSurface.onMouseMovedAbs: captureEvent(type,x+s_x,y+s_y)
        inSurface.onMouseMovedRel: captureEvent(type,x,y)
        onEnabledChanged: toverlay.enabled = enabled
        onVisibleChanged: toverlay.visible = visible
        Text{
            text: netlate
            styleColor: "#000000"
            style: Text.Outline
            color:"#e200ff"
        }
        TouchOverlay{
            id:toverlay
            anchors.fill:parent
            onNewEvent: captureEvent(type,v1,v2)
        }
    }
    ClientConnect {
        id: clientConnect
        onRequestConnect: {
            connectServer(ipS,portS)
            inputGrab()
        }
    }

    toolBar:ToolBar{RowLayout{anchors.fill:parent
            ToolButton{id:t_con;/*text: qsTr("Connect");*/iconSource:"/icons/conn";onClicked: {clientConnect.open()}}
            ToolButton{id:t_dis;visible:false;/*text: qsTr("Disconnect");*/iconSource:"/icons/disco";onClicked: {disconnectServer()}}
            ToolButton{
                //                text: qsTr("Test messages/loader")
                iconSource: "/icons/test"
                onClicked: {
                    changeSurfaceScale(2,2)
                    testSignal()
                    messageDial.smack("HEHE")
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
                        onTriggered: inputGrab()
                    },
                    MenuItem{
                        text:"Fullscreen"
                        checked:false
                        checkable:true
                        onTriggered:{
                            if(checked){
                                applicationWindow.visibility=5
                            }else
                                applicationWindow.visibility=1
                        }
                    },
                    MenuItem{
                        text:"Touch overlay"
                        checked:true
                        checkable:true
                        onTriggered:{
                            toggleOverlay()
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
