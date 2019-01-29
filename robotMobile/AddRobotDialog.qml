import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Window 2.0
import QtQuick.Controls 2.2

Window {
    id: askDialog
    width: 300
    height: 150
    minimumHeight: 150
    minimumWidth: 300
    color: "#00000000"
    flags:Qt.FramelessWindowHint   //添加了这一句
    signal close()

    Rectangle {
        id:collectDialogContentItem
        radius:10
        opacity: 1
        anchors.fill: parent
        color: "#338FCC"
        ColumnLayout{
            anchors.fill: parent
            spacing:2
            Row {
                id: robot_title
                width: parent.width
                height: 30
                Layout.rightMargin: 0
                Label{
                    id:titlelabel
                    text: "机器人名称:"
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    id : text_title
                    height: 30
                    text: 'orange item'

                }
            }

            Row {
                id: robot_IP
                width: parent.width
                height: 30
                Layout.leftMargin: 11
                Label{
                    id:iplabel
                    text: "机器人IP:"
                    anchors.verticalCenter: parent.verticalCenter
                }
                TextField{
                    id : text_ip
                    height: 30
                    text: "192.168.1.114"
                }
            }

            Row {
                id: robot_buttons
                width: parent.width
                height: 30
                Layout.leftMargin: 35
                Button{
                    id: btn_cancel
                    anchors.left: parent.left
                    text:"Cancel"
                    height: 25
                    onClicked: {
                        hide()
                        close()
                    }
                }
                Button{
                    id:btn_confirm
                    anchors.left:btn_cancel.right
                    anchors.leftMargin: 25
                    text:"Confirm"
                    height: 25
                    onClicked: {
                        mylistModel.append(text_title.text,text_ip.text,"qrc:/RobotContent.qml")
                        hide()
                        close()
                    }
                }
            }
        }
    }
}
