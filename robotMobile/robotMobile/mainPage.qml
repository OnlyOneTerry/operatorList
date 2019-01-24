import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import QtQuick.Window 2.3

Rectangle {
    visible: true
    width: 340
    height: 480
    property alias list: listView;
    ListView {
        id: listView
        focus: true
        currentIndex: -1
        anchors.fill: parent
        delegate: ItemDelegate {
            width: parent.width
            text: model.title
            highlighted: ListView.isCurrentItem
            onClicked: {
                listView.currentIndex = index
                panelLoader.setSource(model.source);
                loaderTitleLabel = listView.currentItem.text
            }
        }

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
