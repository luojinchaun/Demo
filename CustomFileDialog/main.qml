import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    minimumWidth: 500
    minimumHeight: 500
    width: 1600
    height: 900
    title: qsTr("Hello World")

    CustomFileDialog{
        anchors.fill: parent
    }
}
