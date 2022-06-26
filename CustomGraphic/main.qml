import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import GraphicScene 1.0
import "./QuickComponent"

Window {
    visible: true
    width: 1200
    height: 600
    title: qsTr("Hello World")

    property var m_skin: Object                         // 皮肤 Object
    property var m_item: null
    property bool isChinese: true
    property bool isQtRendering: true

    // 初始时获取皮肤
    Component.onCompleted: m_skin = getSkin()

    Column {
        id: bttons
        x: 10; y: 10
        width: 120
        spacing: 10

        Button {
            width: 100; height: 30
            text: "圆"
            onClicked: {
                 scene.createGraphItem(0)
            }
        }

        Button {
            width: 100; height: 30
            text: "矩形"
            onClicked: {
                scene.createGraphItem(1)
            }
        }

        Button {
            width: 100; height: 30
            text: "多边形"
            onClicked: scene.createGraphItem(2)
        }

        Button {
            width: 100; height: 30
            text: "显示外接矩形"
            onClicked: scene.showRect(true)
        }

        Button {
            width: 100; height: 30
            text: "隐藏外接矩形"
            onClicked: scene.showRect(false)
        }
    }

    // graphicView
    Rectangle {
        id: graphicView
        x: 130; y: 10
        width: parent.width - x - 10; height: parent.height - 2 * y
        color: "white"
        border.width: 20
        border.color: "#00ff00"

        Flickable {
            id: flickable
            x: graphicView.border.width; y: graphicView.border.width
            width: graphicView.width - 2 * x
            height: graphicView.height - 2 * y
            interactive: false
            clip: true

            GraphicScene {
                id: scene
                width: flickable.width; height: flickable.height
                Component.onCompleted: {
                    setImagePath(":/image/sunyunzhu-001.jpg")
                }
                onCreateItemCompleted: m_item = item

                property real contentX: 0
                property real contentY: 0

                onRightButtonClick: {
                    globalMask.isCrossble = false
                    m_item = item
                    angleSpinbox.spinBoxValue = m_item.itemAngle
                    contextMenu.x = globalMask.mouseX
                    contextMenu.y = globalMask.mouseY
                    contextMenu.visible = true
                }
            }
        }
    }

    // 全局的mask，可动态的获取其xy坐标
    MouseArea {
        id: globalMask
        anchors.fill: parent
        acceptedButtons: Qt.AllButtons
        propagateComposedEvents: true

        property bool isCrossble: true

        onClicked: {
            closePopUp()
            mouse.accepted = !isCrossble
        }
        onPressed: mouse.accepted = !isCrossble
        onReleased: mouse.accepted = !isCrossble
    }

    function closePopUp() {
        globalMask.isCrossble = true
        contextMenu.visible = false
    }

    // 旋转角度的spinbox
    Rectangle {
        id: contextMenu
        width: 140; height: menuColumn.height + 20
        visible: false

        Column {
            id: menuColumn
            x: 10; y: 10
            spacing: 10

            MySpinBox {
                id: angleSpinbox
                width: 120; height: 30
                spinBoxPrefix: "旋转 : "
                spinBoxSuffix: "°"
                spinBoxDecimals: 0
                spinBoxStepSize: 1
                spinBoxValue: 0
                maxValue: 360; minValue: 0
                onResultValueChanged: {
                    if (m_item !== null) {
                        m_item.itemAngle = resultValue
                    }
                }
            }

            Button {
                width: 120; height: 30
                text: "删除"
                onClicked: {
                    closePopUp()
                    scene.deleteItem()
                }
            }
        }
    }

    // 切换主题
    function getSkin()
    {
        var obj = {}
        obj.menuBarBackground = "#2D2D2D"
        obj.menuHoverColor = "#646464"
        obj.menuTextColor = "#FFFFFF"
        obj.unenableTextColor = "#9B9B9B"
        obj.actionBackground = "#373737"
        obj.actionHoverColor = "#4FA0F1"

        // 工具栏
        obj.toolBarBackground = "#AAAAAA"
        obj.toolButtonHoverColor = "#999999"
        obj.toolButtonPressColor = "#777777"
        obj.toolBarSeparatorLineColor = "#FFFFFF"

        // 功能栏及常用按钮
        obj.background = "#858585"
        obj.moduleBarBackground = "#3F3F3F"
        obj.buttonHoverColor = "#999999"
        obj.buttonDefaultColor = "#818181"
        obj.buttonPressColor = "#666666"
        obj.isDark = true

        // 分割线
        obj.separatorLineColor = "#707070"

        // 文本提示颜色
        obj.textPromptColor = "#FFFFFF"

        // 下拉框
        obj.comboBoxBackground = "#2D2D2D"
        obj.comboBoxHoverColor = "#4F4F4F"
        obj.comboBoxPressColor = "#363636"

        // 数据报表
        obj.tableWidgetItemPressColor = "#696969"
        obj.tableWidgetSeparatorLineColor = "#CFCFCF"

        // 边框
        obj.defaultBorderColor = "#9B9B9B"
        obj.activeBorderColor = "#FFFFFF"

        // 3D视图背景色
        obj.viewBackgroundIn3D = "#363636"

        // 图片默认背景色
        obj.imageBackground = "#000000"

        return obj
    }
}
