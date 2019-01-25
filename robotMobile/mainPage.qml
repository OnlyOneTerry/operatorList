import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3

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
                    panelLoader.setSource(model.source);
                    loaderTitleLabel = title.text
                }
            }

            Row {
                id: row1
                Rectangle {
                    width: listView.width
                    height: 70
                    Text {
                        id: title
                        text:  model.title
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
                        width: parent.width/2-10
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
                        width: parent.width/2-10
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
                }
            }



        }

    }

    ListView {
        id: listView
        focus: true
        currentIndex: -1
        anchors.fill: parent
        delegate: customdelegate
        //        delegate: ItemDelegate {
        //            width: parent.width
        //            text: model.title
        //            highlighted: ListView.isCurrentItem
        //            onClicked: {
        //                listView.currentIndex = index
        //                panelLoader.setSource(model.source);
        //                loaderTitleLabel = listView.currentItem.text
        //            }
        //        }
        //        model: customModel
        model: ListModel {
            ListElement {title: "BusyIndicator";source: "qrc:/Login.qml"}
            ListElement { title: "Button";source: "qrc:/RobotContent.qml" }
            ListElement { title: "CheckBox";source: "qrc:/RobotContent.qml" }
            ListElement { title: "ComboBox";source: "qrc:/RobotContent.qml" }
            ListElement { title: "DelayButton"; source: "qrc:/RobotContent.qml" }
            ListElement { title: "Dial"; source: "qrc:/RobotContent.qml" }
            ListElement { title: "Dialog"; source: "qrc:/RobotContent.qml" }
            ListElement { title: "Delegates"; source: "qrc:/RobotContent.qml" }
            ListElement { title: "Frame"; source: "qrc:/RobotContent.qml" }
            ListElement { title: "GroupBox"; source: "qrc:/RobotContent.qml" }
            ListElement { title: "PageIndicator"; source: "qrc:/RobotContent.qml" }
            ListElement { title: "ProgressBar"; source: "qrc:/RobotContent.qml" }
            ListElement { title: "RadioButton"; source: "qrc:/RobotContent.qml" }
            ListElement { title: "RangeSlider"; source: "qrc:/RobotContent.qml" }
            ListElement { title: "ScrollBar"; source: "qrc:/RobotContent.qml" }
            ListElement { title: "ScrollIndicator"; source: "qrc:/RobotContent.qml" }
            ListElement { title: "Slider"; source: "qrc:/RobotContent.qml" }
            ListElement { title: "SpinBox"; source: "qrc:/RobotContent.qml" }
            ListElement { title: "StackView"; source: "qrc:/RobotContent.qml" }
            ListElement { title: "SwipeView"; source: "qrc:/RobotContent.qml" }
            ListElement { title: "Switch"; source: "qrc:/RobotContent.qml" }
            ListElement { title: "TabBar"; source: "qrc:/RobotContent.qml" }
            ListElement { title: "TextArea"; source: "qrc:/RobotContent.qml" }
            ListElement { title: "TextField"; source: "qrc:/RobotContent.qml" }
            ListElement { title: "ToolTip"; source: "qrc:/RobotContent.qml" }
            ListElement { title: "Tumbler"; source: "qrc:/RobotContent.qml" }
        }

        ScrollIndicator.vertical: ScrollIndicator { }
    }
}
