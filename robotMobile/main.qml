import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3

ApplicationWindow {
    visible: true
    width: 340
    height: 480
    title: qsTr("robotMobile")
    property alias loaderTitleLabel :titleLabel.text  ;

    header: ToolBar {

        RowLayout {
            spacing: 20
            anchors.fill: parent

            ToolButton {
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source:  "images/back.png"
                }
                onClicked: {
                    panelLoader.setSource("qrc:/mainPage.qml")
                    titleLabel.text = "RobotMobile"
                }
            }

            Label {
                id: titleLabel
                text: "RobotMobile"
                font.pixelSize: 20
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }

            ToolButton {
                contentItem: Image {
                    fillMode: Image.Pad
                    horizontalAlignment: Image.AlignHCenter
                    verticalAlignment: Image.AlignVCenter
                    source: "images/menu.png"
                }
                onClicked: optionsMenu.open()

                Menu {
                    id: optionsMenu
                    x: parent.width - width
                    transformOrigin: Menu.TopRight

                    MenuItem {
                        text: "Settings"
                        onTriggered: settingsDialog.open()
                    }
                    MenuItem {
                        text: "About"
                        onTriggered: aboutDialog.open()
                    }
                }
            }
        }
    }

    footer: TabBar {
        id: footBar
        currentIndex: 0
        TabButton {
            text: qsTr("First")
        }
        TabButton {
            text: qsTr("Second")
        }
        TabButton {
            text: qsTr("Add")
            onClicked: {
                mylistModel.append("AMB150","192.168.1.72","qrc:/RobotContent.qml")
            }
        }
    }

    Loader {
        id: panelLoader
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        function setSource(source) {
            panelLoader.source = source
        }
    }
    Component.onCompleted: {
        panelLoader.source = "qrc:/mainPage.qml"
        console.log("mainPage.qml")
    }
}
