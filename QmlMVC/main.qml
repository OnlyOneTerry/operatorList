import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import DataModel 1.0
import QtQuick.Window 2.3
import QtQuick.Controls 1.4

Window{
    visible: true
    width: 640
    height: 480
    title: qsTr("Model")

    ListView{
        id:view
        width: parent.width
        height: parent.height -rect_input.height

        model: $Model

        delegate: Rectangle{
            width: view.width
            height: 40
            border.color:"gray" // Qt.gray(color)
            color: dmColor

            Text{
                anchors.left: parent.left
                text: dmTitle
            }

            Image{
                id:icon
                anchors.verticalCenter: parent
                anchors.right: parent.right
                source: "qrc:/images/remove.png"
                MouseArea{
                    anchors.fill: parent
                    onClicked: $Model.remove(index)
                }
            }
        }
    }

    RowLayout {
        id: rect_input
        anchors.bottom: parent.bottom
        width: parent.width
        height: 50
        TextField{
            id : text_title
            Layout.fillWidth:  true
            text: 'orange item'
        }
        TextField{
            id : text_color
            Layout.fillWidth:  true
            text: 'orange'
        }

        Button{
            text:'Add'
            onClicked: {
                $Model.append(text_title.text,text_color.text)
            }
        }
    }



}
