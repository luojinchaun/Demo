import QtQuick 2.12
import QtQuick.Controls 2.5
import QYTableWidgetModel 1.0
import QYSimpleImage 1.0

// 自定义数据报表
Rectangle {
    id: tableWidget
    color: m_skin.moduleBarBackground

    property string fontFamily: "微软雅黑"
    property int pixelSize: 14
    property int currentSelectRow: -1
    property real itemHeight: 35
    property real titleHeight: 40
    property real horizontalScrollbarWidth: 10
    property real verticalScrollbarWidth: 10

    property QYTableWidgetModel currentModel: staticModel ? tableWidgetModel : tableModel

    property var horizontalHeaderData: []           // 列头数据
    property var horizontalHeaderWidth: []          // 列宽
    property bool verticalHeaderVisible: false      // 是否显示行头
    property bool hHeaderAdaptive: true             // 水平列头是否开启自适应宽度
    property bool contentAdaptive: true             // 表格内容是否开启自适应
    property bool needClearSelectState: false       // 在表格行数变化时是否需要清空选择状态
    property var verticalHeaderWidth: 25            // 行宽
    property var tableWidgetData: []                // 报表的数据
    property bool staticModel: false                // 使用全局 Model

    signal rightButtonPressed(var rx, var ry)

    onHorizontalHeaderDataChanged: {
        horizontalHeaderModel.clear()

        for ( var i = 0; i < horizontalHeaderData.length; ++i )
        {
            if ( i < horizontalHeaderWidth.length ) {
                var itemWidth = horizontalHeaderWidth[i]
            } else {
                itemWidth = 80
            }

            horizontalHeaderModel.append({typeName: horizontalHeaderData[i], itemWidth: itemWidth})
        }

        // 根据列头数据设置列数
        currentModel.setColumnCount(horizontalHeaderData.length)
    }

    // 鼠标事件
    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.AllButtons
        onClicked: {
            if ( mouse.button === Qt.LeftButton ) {
                mouse.accepted = false
            } else if ( mouse.button === Qt.RightButton ) {
                rightButtonPressed(mouseX, mouseY)
            }
        }
    }

    // 行与列的交叉点
    Rectangle {
        id: intersect
        width: verticalHeaderWidth; height: titleHeight
        color: intersectArea.pressed ? m_skin.buttonPressColor : "transparent"
        visible: verticalHeader.visible

        MouseArea {
            id: intersectArea
            anchors.fill: parent
            hoverEnabled: true
            onClicked: {
                currentModel.setIsSelect(0, 0, tableView.columns, tableView.rows)
                currentSelectRow = -1
            }
        }
    }

    // 列头
    Rectangle {
        id: horizontalHeader
        x: intersect.visible ? intersect.width : 0
        width: intersect.visible ? parent.width - intersect.width : parent.width
        height: titleHeight
        color: "transparent"
        clip: true

        Row {
            id: horizontalColumn
            clip: true

            Repeater {
                model: ListModel { id: horizontalHeaderModel }

                delegate: Rectangle {
                    width: itemWidth; height: titleHeight
                    color: horizontalItemArea.pressed ? m_skin.buttonPressColor :
                                                        horizontalItemArea.containsMouse ? m_skin.buttonHoverColor : "transparent"

                    Text {
                        anchors.fill: parent
                        anchors.rightMargin: 1
                        font.family: fontFamily
                        font.pixelSize: pixelSize
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        renderType: isQtRendering ? Text.QtRendering : Text.NativeRendering
                        color: m_skin.menuTextColor
                        elide: hHeaderAdaptive ? Text.ElideNone : Text.ElideRight
                        text: typeName
                        clip: true

                        // 自适应头部距离
                        onContentWidthChanged: {
                            if ( hHeaderAdaptive && ( (horizontalHeaderModel.get(index).itemWidth - 10) < contentWidth) ) {
                                horizontalHeaderModel.setProperty(index, "itemWidth", contentWidth + 10)
                            }
                        }
                    }

                    MouseArea {
                        id: horizontalItemArea
                        anchors.fill: parent
                        hoverEnabled: true
                        onClicked: {
                            currentModel.setIsSelect(model.index, 0, 1, tableView.rows)
                            currentSelectRow = -1
                        }
                    }
                }
            }
        }
    }

    // 行头
    ListView {
        id: verticalHeader
        x: 0; y: titleHeight
        width: verticalHeaderWidth; height: parent.height - y
        orientation: ListView.Vertical
        interactive: false
        bottomMargin: horizontalScrollbarWidth	// 给滚动条留出的距离
        clip: true
        visible: verticalHeaderVisible && verticalHeaderModel.count > 0

        ScrollBar.vertical: vertical

        model: ListModel { id: verticalHeaderModel }

        delegate: Rectangle {
            width: verticalHeaderWidth; height: itemHeight
            color: verticalItemArea.pressed ? m_skin.buttonPressColor :
                                              verticalItemArea.containsMouse ? m_skin.buttonHoverColor : "transparent"

            Text {
                anchors.fill: parent
                font.family: fontFamily
                font.pixelSize: pixelSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                renderType: isQtRendering ? Text.QtRendering : Text.NativeRendering
                color: m_skin.menuTextColor
                text: number

                // 自适应头部距离,当行数变得很多时就不够显示
                onContentWidthChanged: {
                    if ( contentWidth > verticalHeaderWidth ) {
                        verticalHeaderWidth = contentWidth + 20
                    }
                }
            }

            MouseArea {
                id: verticalItemArea
                anchors.fill: parent
                hoverEnabled: true
                onClicked: {
                    currentModel.setIsSelect(0, model.index, tableView.columns, 1)
                    currentSelectRow = index
                }
            }
        }
    }

    // TableView
    TableView {
        id: tableView
        x: verticalHeaderVisible ? verticalHeader.width : 0
        y: titleHeight
        width: parent.width - x
        height: parent.height - y
        bottomMargin: horizontalScrollbarWidth
        rightMargin: verticalScrollbarWidth			// 给滚动条留出的距离
        boundsBehavior: Flickable.StopAtBounds
        flickableDirection: Flickable.VerticalFlick
        clip: true

        model: currentModel

        onRowsChanged: updateRows(rows)

        onContentXChanged: horizontalColumn.x = -contentX

        ScrollBar.horizontal: ScrollBar {
            id: horizontal
            policy: tableView.contentWidth > width ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
            minimumSize: 0.1    // 滚动条显示最小尺寸相对于整个滚动区域长度

            // 增加自定义滚动条hander颜色
            contentItem: Rectangle {
                implicitWidth: horizontal.interactive ? 5 : 1
                implicitHeight: horizontal.interactive ?  5 : 1
                radius: width / 2
                color: horizontal.pressed ? m_skin.menuTextColor : m_skin.separatorLineColor
            }
        }

        ScrollBar.vertical: ScrollBar {
            id: vertical
            policy: verticalHeader.contentHeight > verticalHeader.height ? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
            minimumSize: 0.1

            contentItem: Rectangle {
                implicitWidth: vertical.interactive ? 5 : 1
                implicitHeight: vertical.interactive ?  5 : 1
                radius: width / 2
                color: vertical.pressed ? m_skin.menuTextColor : m_skin.separatorLineColor
            }
        }

        delegate: Rectangle {
            implicitWidth: horizontalHeaderModel.get(model.column).itemWidth
            implicitHeight: itemHeight
            border.width: 1
            border.color: m_skin.tableWidgetSeparatorLineColor
            color: isSelect ? m_skin.tableWidgetItemPressColor : "transparent"

            // 文本与编辑
            TextInput {
                id: disPlaytext
                anchors.fill: parent
                font.family: fontFamily
                font.pixelSize: pixelSize
                verticalAlignment: TextInput.AlignVCenter
                horizontalAlignment: TextInput.AlignHCenter
                renderType: isQtRendering ? Text.QtRendering : Text.NativeRendering
                color: m_skin.menuTextColor
                enabled: model.editable
                visible: model.isText
                clip: true
                text: {
                    if ( model.value === undefined ) {
                        return "nan"
                    } else {
						return model.value
					}
                }

                onEditingFinished: model.value = text
            }

            // 图片
            QYSimpleImage {
                anchors.fill: parent
				anchors.margins: 2
				m_image: model.image
				clip: true
				visible: !isText
            }

            MouseArea {
                anchors.fill: parent
                hoverEnabled: true

                onClicked: {
                    model.isSelect = true
                    mask.enabled = false
                    currentSelectRow = model.row
                }

                onDoubleClicked: {
                    if (model.editable) {
                        disPlaytext.forceActiveFocus()
                        mask.enabled = true
                    }
                }
            }
        }

        // tabview 的最小外接矩形，为了使边缘的宽度为2
        Rectangle {
            z: 2
            width: tableView.contentWidth
            height:  tableView.rows ===  0 ? 0 : tableView.contentHeight
            border.width: 2
            border.color: m_skin.tableWidgetSeparatorLineColor
            color: "transparent"

            // 进入可编辑状态的掩膜
            MouseArea {
                id: mask
                anchors.fill: parent;
                enabled: false
                propagateComposedEvents: true   // 允许事件穿透
                onClicked: {
                    tableWidget.focus = true
                    mask.enabled = false
                    currentModel.setIsSelect(0, 0, 0, 0)
                    mouse.accepted = false
                }
            }
        }
    }

    // TableView model, Non-global model
    QYTableWidgetModel { id: tableModel }

    // 用于测量字符串占据宽度
    Text {
        id: testStrlen
        font.family: fontFamily
        font.pixelSize: pixelSize
        renderType: isQtRendering ? Text.QtRendering : Text.NativeRendering
        visible: false
    }

    // 初始化报表数据
    function initTableData() {
        // 清空报表数据
        verticalHeaderModel.clear()
        currentModel.clear()

        // 添加报表数据
        for ( var i = 0; i < tableWidgetData.length; ++i )
        {
            if (contentAdaptive) {
                for ( var j = 0; j < tableWidgetData[i].length; ++j )
                {
                    testStrlen.text = tableWidgetData[i][j]
                    if ( (horizontalHeaderModel.get(j).itemWidth - 10) < testStrlen.contentWidth ) {
                        horizontalHeaderModel.setProperty(j, "itemWidth", testStrlen.contentWidth + 10)
                    }
                }
            }
            currentModel.push_backRow(tableWidgetData[i])
        }
        updateRows(tableView.rows)

        currentModel.resetModel() // 更新表格宽度
    }

    // 删除选中行
    function removeRow(row) {
        currentModel.removeRow(row)
        currentSelectRow = -1
    }

    // 更新行数据
    function updateRows(row) {
        if ( row > verticalHeaderModel.count ) {
            for ( var i = verticalHeaderModel.count; i < row; ++i )
            {
                verticalHeaderModel.append({ "number": i+1 })
            }
        }

        if ( row < verticalHeaderModel.count ) {
            verticalHeaderModel.remove(row, verticalHeaderModel.count - row)
        }

        // 清除选择状态
        if (needClearSelectState) {
            currentModel.setIsSelect(0, 0, 0, 0)
        }
    }

    // 获取某一列数据
    function getColumnData(column) { return currentModel.getColumnData(column) }

    // 更新报表中某一列数据
    function updateColumnData(list, col, state) { currentModel.updateColumnData(list, col, state) }

    // 更新报表中某一个数据
    function updateOneData(row, col, value) { currentModel.updateOneData(row, col, value) }

    // 获取和行数相等的数据
    function getRowNullData() {
        var array = []
        for ( var i = 0; i < currentModel.rowCount(); ++i )
        {
            array.push(i)
        }
        return array
    }

    // 导出CSV
    function exportCSV() {
        var path = m_config.file_exportCSVPath === "" ? globalFun.isDirExist("D:/") ? "D:/" : "C:/" : m_config.file_exportCSVPath

        var file = globalFun.getSaveFileName(isChinese ? "导出CSV" : "Export CSV",
                                             path + "/" + globalFun.getCurrentTime(5) + ".csv",
                                             "CSV File (*.csv)")

        if ( file !== "" ) {
            // 存储路径
            m_config.file_exportCSVPath = file.substring(0, file.lastIndexOf('/'))

            // 导出CSV
            currentModel.exportCSV(file, horizontalHeaderData)
        }
    }

    // 滚动到指定行
    function scrollToRow(index) {
        if ( index >= currentModel.rowCount() || index < 0 ) {
            return
        }

        var currentPos = (index + 1) * itemHeight

        if ( tableView.height < currentPos ) {
            // 需要向下滚动
            tableView.contentY = currentPos - tableView.height
        } else if ( currentPos < tableView.height ) {
            // 需要看的点在第一页，contentY为0
            tableView.contentY = 0
        }

        currentModel.setIsSelect(0, index, currentModel.columnCount(), 1, true)
    }

    // 设置可编辑的列，用数组传递可编辑的列
    function setEditableColumn(column) { currentModel.setEditableColumn(column) }

    // 使用比例因子数组进行缩放，factorArray 数组和一定要等于1，否则会出现异常
    function updateLayoutInFactor(factorArray) {
        var w = width - (verticalHeaderVisible ? verticalHeaderWidth : 0) - verticalScrollbarWidth

        for ( var i = 0; i < horizontalHeaderData.length; ++i ) 
		{
            horizontalHeaderModel.setProperty(i, "itemWidth", w * factorArray[i])
        }

        currentModel.resetModel()
    }
}
