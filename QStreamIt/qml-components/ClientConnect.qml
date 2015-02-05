import QtQuick 2.3
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.2

Dialog {
    signal requestConnect(string ipS,string portS)
    id: rectangle1
    standardButtons: StandardButton.Open | StandardButton.Cancel
    title: qsTr("Connect to host")
    modality: Qt.WindowModal
    SystemPalette {id:palette}

    contentItem: Rectangle{
        color:palette.dark
        implicitHeight: 300
        implicitWidth: 800
        ColumnLayout{
            RowLayout {
                TextField {id: ip;placeholderText: qsTr("IP address");onTextChanged: err.visible = false}
                Label{text:":"}
                TextField {id: port;placeholderText: qsTr("Port");onTextChanged: err.visible = false}
            }
            Button{id:connectBtn;text:qsTr("Connect");visible:!err.visible;
                onClicked: {
                    if(ip.text==""||port.text==""){
                        err.text = qsTr("Invalid parameters")
                        err.visible = true
                    }else{
                        requestConnect(ip.text,port.text)
                        rectangle1.close()
                    }
                }
            }
            Label{id:err;visible:false;color:"red"}
        }
        Keys.onBackPressed: rectangle1.close()
        Keys.onEnterPressed: rectangle1.accept()
    }
}

