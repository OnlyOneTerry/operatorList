import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3
import ListModel 1.0

Rectangle {
    visible: true
    width: 340
    height: 480
    property alias list: listView;
    Component{
        id: customdelegate

        Rectangle {
            width: listView.width
            height: 102
            border.width: 1
            border.color: "gray"
            MouseArea{
                anchors.fill: parent
                onClicked: {
                    listView.currentIndex = index
                    panelLoader.setSource(robotContent);
                    loaderTitleLabel = title.text
                    footBar.visible = false
                }
            }

            Row {
                id: row1
                Rectangle {
                    width: listView.width
                    height: 70
                    Text {
                        id: title
                        text: robotIp  //其中一个角色roleNames（）中获得
                        font.bold: true
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                    }

                }
            }
            Row {
                id: row2
                anchors.top: row1.bottom
                Rectangle{

                    width: listView.width
                    height: 30
                    Button {
                        id:sync  //同步
                        width: parent.width/3-10
                        anchors.left: parent.left
                        anchors.leftMargin: 5
                        anchors.bottom: parent.bottom
                        anchors.topMargin: 0
                        text: "同步"
                        font.bold: true
                        anchors.verticalCenter: parent.verticalCenter
                        onClicked: {
                            console.log("同步地图和模型文件")
                        }
                    }
                    Button {
                        id:connect
                        width: parent.width/3-10
                        anchors.left: sync.right
                        anchors.leftMargin: 10
                        anchors.bottom: parent.bottom
                        anchors.topMargin: 0
                        text: "连接"
                        font.bold: true
                        anchors.verticalCenter: parent.verticalCenter
                        onClicked: {
                            console.log("连接机器人")
                        }
                    }
                    Button {
                        id:deleteItem
                        width: parent.width/3-10
                        anchors.left: connect.right
                        anchors.leftMargin: 10
                        anchors.bottom: parent.bottom
                        anchors.topMargin: 0
                        text: "remove"
                        font.bold: true
                        anchors.verticalCenter: parent.verticalCenter
                        onClicked: {
                            mylistModel.remove(index)
                        }
                    }

                }
            }

        }

    }

    ListView {
        id: listView
        focus: true
        currentIndex: -1
        anchors.fill: parent
        model: mylistModel
        delegate: customdelegate
        ScrollIndicator.vertical: ScrollIndicator { }
    }

    Rectangle {
        id:mask
        anchors.fill: parent
        color: "gray"
        opacity: 0.4
        visible: false
        MouseArea{
            anchors.fill: parent
            onClicked:{
                mouse.accepted = false//传递到此为止？
            }
        }
    }

    Connections{
        target: add_robot_dialog
        onClose:{
            mask.visible = false
            console.log("diloag close---------------")
        }
    }

    Connections{
        target: mainWindow
        onAddRobot:{
            mask.visible = true
            console.log("diloag add---------------")
        }
    }
}




