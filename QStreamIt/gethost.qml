import QtQuick 2.3
import QtQuick.Controls 1.2

ApplicationWindow {
    id: applicationWindow
    visible: true
    width: 446
    height: 224
    title: qsTr("Hello World")

    signal receiveString(int enumerator,string qmlstring)
    signal receiveInt(int enumerator,int qmlint)
    signal finishedQuery()

    Item {
        id: buttonItem
        y: parent.height/16*3
        height: parent.height/3-2
        anchors.left: parent.left
        anchors.leftMargin: 4
        anchors.right: parent.right
        anchors.rightMargin: 4
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4
        opacity: 1

        Button {
            id: button1
            x: parent.width-width
            y: 0
            width: (parent.width-56)/3+56
            height: parent.height
            text: qsTr("Connect")
            onClicked: {
                if(ipAddrField.text!="")
                    if(portField.text!=""){
                        receiveString(0,ipAddrField.text)
                        receiveInt(1,portField.text*1)
                        finishedQuery()
                    }
            }
        }
    }

    Item {
        id: portItem
        height: parent.height/8*2
        anchors.right: parent.right
        anchors.rightMargin: 4
        anchors.left: parent.left
        anchors.leftMargin: 4
        anchors.top: ipAddrItem.bottom
        anchors.topMargin: 4
        transformOrigin: Item.Left


        Label {
            id: label2
            x: 0
            y: 0
            width: 142
            height: parent.height
            text: qsTr("Port")
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            style: Text.Raised
            font.pointSize: 22
        }

        TextField {
            id: portField
            y: 0
            height: parent.height
            anchors.left: label2.right
            anchors.leftMargin: 110
            anchors.right: parent.right
            anchors.rightMargin: 0
            placeholderText: qsTr("TCP port")
        }

    }

    Item {
        id: ipAddrItem
        height: parent.height/8*2
        anchors.right: parent.right
        anchors.rightMargin: 4
        anchors.left: parent.left
        anchors.leftMargin: 4
        anchors.top: parent.top
        anchors.topMargin: 4

        Label {
            id: label1
            x: 0
            width: 142
            height: parent.height
            text: qsTr("IP address")
            z: 2
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
            style: Text.Raised
            font.pointSize: 22
        }

        TextField {
            id: ipAddrField
            height: parent.height
            anchors.left: label1.right
            anchors.leftMargin: 12
            anchors.right: parent.right
            anchors.rightMargin: 0
            z: 2
            placeholderText: qsTr("IPv4 address or hostname")
        }
    }



}
