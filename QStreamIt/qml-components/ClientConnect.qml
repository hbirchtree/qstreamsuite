import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Dialogs 1.2

Dialog {
    id: rectangle1

    property alias button: button1
    property alias ipField: textField1
    property alias portField: textField2

    contentItem: Column {
        id: column1
        spacing: 0
        Item {
            id: ip
            height: parent.height/3
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0

            TextField {
                id: textField1
                anchors.left: parent.horizontalCenter
                anchors.leftMargin: 0
                anchors.top: parent.top
                anchors.topMargin: 4
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 4
                anchors.right: parent.right
                anchors.rightMargin: 0
                placeholderText: qsTr("Text Field")
            }

            Label {
                id: label1
                enabled: false
                color: "#ffffff"
                text: qsTr("IP addr.")
                anchors.right: parent.horizontalCenter
                anchors.rightMargin: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 4
                anchors.top: parent.top
                anchors.topMargin: 4
                anchors.left: parent.left
                anchors.leftMargin: 0
                font.pointSize: 36
            }
        }

        Item {
            id: port
            height: parent.height/3
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0

            TextField {
                id: textField2
                anchors.left: parent.horizontalCenter
                anchors.leftMargin: 0
                anchors.top: parent.top
                anchors.topMargin: 4
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 4
                anchors.right: parent.right
                anchors.rightMargin: 0
                placeholderText: qsTr("Text Field")
            }

            Label {
                id: label2
                enabled: false
                color: "#ffffff"
                text: qsTr("Port")
                anchors.right: parent.horizontalCenter
                anchors.rightMargin: 0
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 4
                anchors.top: parent.top
                anchors.topMargin: 4
                anchors.left: parent.left
                anchors.leftMargin: 0
                font.pointSize: 36
            }
        }

        Item{
            id: btn
            height: parent.height/3
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.right: parent.right
            anchors.rightMargin: 0
            Button {
                id: button1
                text: qsTr("Connect")
                anchors.left: parent.horizontalCenter
                anchors.leftMargin: 0
                anchors.top: parent.top
                anchors.topMargin: 16
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 16
                anchors.right: parent.right
                anchors.rightMargin: 28
                isDefault: true
            }
        }
    }


}

