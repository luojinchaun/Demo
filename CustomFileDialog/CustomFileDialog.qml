import QtQuick 2.12
import QtQuick.Controls 2.5

// 自定义文件管理器
/*
 * 单选:
 * 1. 是文件夹，则清空文件名称
 * 2. 是文件，则显示文件名称
 * 3. 是图片，则直接显示
 * 4. 是ASC、XYZ文件，则显示图片和信息
 * 5. 是Sirius文件，则显示图片
 *
 * 双击:
 * 1. 是文件夹，则进入下一级
 * 2. 是图片，则提示无法打开
 * 3. 是ASC、XYZ、Sirius文件，则直接打开
 *
 * 打开:
 * 1. 文件名称为空，则提示选择文件
 * 2. 单选-图片，则提示不能选择单张图片
 * 3. 单选-ASC、XYZ、Sirius文件，则打开文件
 * 4. 多选，需判断文件类型是否一致，不一致提示类型错误
 * 5. 多选，如果是文件则提示错误，是图片则加载图片
 */
Image {
    id: fileDialog
    z: 10
    anchors.fill: parent
    source: "qrc:/image/Shadow.png"

    property string fontFamily: "微软雅黑"
    property int pixelSize: 14
    property int margin: 5
    property int lineWidth: 4

    property bool is_load_ASCOrXYZ: false
    property bool isDark: true
    property bool isChinese: true
    property bool showImg: true             // 是否显示图片信息
    property string currentPath: ""         // 当前文件的路径
    property string selectedFile: ""        // 用户点击选择的文件
    property bool ctrlPressed: false        // ctrl 按钮是否按下
    property bool shiftPressed: false       // shift 按钮是否按下

    property var previousPath: []           // 保存历史文件路径，最多可以保存10条
    property int shiftSelectIndex: 0		// 默认值为0

    signal openFile(var path, var name, var type)

    onPreviousPathChanged: {
        // 最多存储10个路径
        if ( previousPath.length > 10 ) {
            previousPath.shift()
        }
    }

    // Init file data
    Component.onCompleted: {
        currentPath = fileDialogModel.getCurrentPath()
        loadFolder(currentPath)
    }

    // KeyEvent control pressed
    Keys.onPressed: {
        if ( event.modifiers === Qt.ControlModifier && event.key === Qt.Key_A && listView.visible )
        {
            fileNameComboBox.inputText = fileDialogModel.setIsSelectOpposite(0, fileDialogModel.count() - 1, true)
        }

        if ( event.key === Qt.Key_Control )
        {
            ctrlPressed = true
        }

        if ( event.key === Qt.Key_Shift )
        {
            shiftPressed = true
        }
    }

    // KeyEvent control released
    Keys.onReleased: {
        if ( event.key === Qt.Key_Control )
        {
            ctrlPressed = false
        }

        if ( event.key === Qt.Key_Shift )
        {
            shiftPressed = false
        }
    }

    // File Dialog
    Rectangle {
        z: 11
        width: parent.width; height: parent.height
        anchors.centerIn: parent
        color: "#858585"

        // Title
        Rectangle {
            id: title
            width: parent.width; height: 30
            color: "#AAAAAA"

            // 标题图标
            Image {
                width: 20; height: 20
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.margins: 5
                fillMode: Image.PreserveAspectFit
                source: "qrc:/image/Logo.png"
            }

            // 标题文本
            Text {
                x: 35
                width: contentWidth; height: parent.height
                font.family: fontFamily
                font.pixelSize: pixelSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                color: "#FFFFFF"
                text: isChinese ? "加 载" : "Load"
            }

            // 关闭按钮
            Rectangle {
                id: closeBtn
                width: 60; height: 30
                anchors.top: parent.top
                anchors.right: parent.right
                color: mouseArea.containsPress ? "#F1707A" : mouseArea.containsMouse ? "#E81123" : "#AAAAAA"

                Image {
                    width: 20; height: 20
                    anchors.centerIn: parent
                    source: "qrc:/image/Close.png"
                }

                MouseArea {
                    id: mouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onReleased: {
                        if (containsMouse) {
                            fileDialog.visible = false
                        }
                    }
                }
            }
        }

        // Catalogue
        Rectangle {
            id: catalogue
            width: parent.width; height: 30
            anchors.top: title.bottom
            anchors.left: parent.left
            anchors.topMargin: margin
            color: "transparent"

            // 提示文本
            Text {
                id: catalogueText
                width: 125; height: parent.height
                anchors.bottom: parent.bottom
                font.family: fontFamily
                font.pixelSize: pixelSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                color: "#FFFFFF"
                text: isChinese ? "目录 : " : "Look in : "
            }

            // 文件路径
            Rectangle {
                id: filePathRec
                width: content.width - 150; height: parent.height
                anchors.left: catalogueText.right
                anchors.leftMargin: margin
                anchors.bottom: parent.bottom
                border.width: 1
                border.color: "#000000"
                color: "#3F3F3F"

                Image {
                    width: 20; height: 20
                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/image/Folder.png"
                }

                Text {
                    x: 32
                    width: parent.width - 32; height: parent.height
                    font.family: fontFamily
                    font.pixelSize: pixelSize
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    elide: Text.ElideLeft
                    color: "#FFFFFF"
                    text: currentPath
                }
            }

            // 回退按钮
            Rectangle {
                id: returnBtn
                width: parent.height; height: parent.height
                anchors.left: filePathRec.right
                anchors.leftMargin: margin
                color: returnBtnArea.pressed ? "#666666" :
                                               (returnBtnArea.containsMouse ? "#999999" : "transparent")

                Image {
                    anchors.fill: parent
                    anchors.margins: 3
                    source: isDark ? "qrc:/image/LeftL.png" : "qrc:/image/Left.png"
                }

                MouseArea {
                    id: returnBtnArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        if ( previousPath.length === 0 ) {
                            console.log("Last path is empty !")
                        } else {
                            currentPath = previousPath.pop()
                            loadFolder(currentPath)
                        }
                    }
                }
            }

            // 上一页按钮
            Rectangle {
                id: previousBtn
                width: parent.height; height: parent.height
                anchors.left: returnBtn.right
                anchors.leftMargin: margin
                color: previousBtnArea.pressed ? "#666666" :
                                                 (previousBtnArea.containsMouse ? "#999999" : "transparent")

                Image {
                    anchors.fill: parent
                    anchors.margins: 3
                    source: isDark ? "qrc:/image/UpL.png" : "qrc:/image/Up.png"
                }

                MouseArea {
                    id: previousBtnArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: {
                        var path = currentPath

                        if ( path === "Computer" )
                        {
                            console.log("This is the top floor !")
                        }
                        else if ( path.slice(path.length - 2, path.length) === ":/" )
                        {
                            currentPath = "Computer"
                        }
                        else
                        {
                            var index = path.lastIndexOf("/")

                            // 删除最后一层目录 "/xxx"
                            if ( index !== -1 ) {
                                path = path.slice(0, index) // 如path = "D:/WaitDeal"，slice之后为"D:",这时没有/，则补上
                                index = path.lastIndexOf("/")
                                if (index === -1) {
                                     path += "/"
                                }
                            }
                            // 记录当前路径
                            previousPath.push(currentPath)

                            // 获取新路径，并加载文件
                            currentPath = path
                            loadFolder(currentPath)
                        }
                    }
                }
            }

            // 图像显示隐藏按钮
            Rectangle {
                id: showPicBtn
                anchors.right: parent.right
                anchors.rightMargin: showImg ? picture.width + margin*2 : margin
                width: parent.height; height: parent.height
                color: showPicBtnArea.pressed ? "#666666" :
                                                (showPicBtnArea.containsMouse ? "#999999" : "transparent")

                Image {
                    anchors.fill: parent
                    anchors.margins: 3
                    source: showImg ? (isDark ? "qrc:/image/RightSL.png" : "qrc:/image/RightS.png") :
                                      (isDark ? "qrc:/image/LeftSL.png" : "qrc:/image/LeftS.png")
                }

                MouseArea {
                    id: showPicBtnArea
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.PointingHandCursor
                    onClicked: showImg = !showImg
                }
            }
        }

        // Path icon
        Rectangle {
            id: path
            width: 120
            anchors.top: catalogue.bottom
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.margins: margin
            color: "#3F3F3F"

            Column {
                anchors.centerIn: parent
                spacing: (path.height - 400) / 6

                Repeater {
                    model: [
                        {imgpathD: "qrc:/image/Desktop.png", imgpathL: "qrc:/image/DesktopL.png",
                            txtC: "桌 面", txtE: "Desktop"},
                        {imgpathD: "qrc:/image/Download.png", imgpathL: "qrc:/image/DownloadL.png",
                            txtC: "下 载", txtE: "Download"},
                        {imgpathD: "qrc:/image/Document.png", imgpathL: "qrc:/image/DocumentL.png",
                            txtC: "文 档", txtE: "Document"},
                        {imgpathD: "qrc:/image/Picture.png", imgpathL: "qrc:/image/PictureL.png",
                            txtC: "图 片", txtE: "Picture"},
                        {imgpathD: "qrc:/image/Computer.png", imgpathL: "qrc:/image/ComputerL.png",
                            txtC: "电 脑", txtE: "Computer"},
                    ]

                    delegate: Rectangle {
                        id: pathBtn
                        width: 80; height: 80
                        color: pathBtnArea.pressed ? "#666666" :
                                                     (pathBtnArea.containsMouse ? "#999999" : "transparent")

                        Image {
                            y: 10
                            width: 40; height: 40
                            anchors.horizontalCenter: parent.horizontalCenter
                            fillMode: Image.PreserveAspectFit
                            source: isDark ? modelData.imgpathL : modelData.imgpathD
                        }

                        Text {
                            y: 55
                            width: parent.width; height: 20
                            font.family: fontFamily
                            font.pixelSize: pixelSize
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHCenter
                            color: "#FFFFFF"
                            text: isChinese ? modelData.txtC : modelData.txtE
                        }

                        MouseArea{
                            id: pathBtnArea
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: {
                                // 记录当前路径
                                previousPath.push(currentPath)

                                // 获取新路径，并加载文件
                                currentPath = fileDialogModel.getPath(modelData.txtE)
                                loadFolder(currentPath)
                            }

                        }

                    }

                }

            }
        }

        // Content
        Rectangle {
            id: content
            anchors.top: catalogue.bottom
            anchors.bottom: grid.top
            anchors.left: path.right
            anchors.right: showImg ? picture.left : parent.right
            anchors.margins: margin
            color: "#3F3F3F"

            // Title
            Rectangle {
                x: -3
                width: listView.width; height: 30
                color: "transparent"
                clip: true

                // 文件名
                Text {
                    id: template_fileName
                    x: 6
                    width: parent.width/2; height: 30
                    font.family: fontFamily
                    font.pixelSize: pixelSize
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    elide: Text.ElideRight
                    color: "#FFFFFF"
                    text: isChinese ? "文件名称" : "FileName"

                    Rectangle {
                        width: 2; height: parent.height
                        anchors.right: parent.right
                        anchors.rightMargin: margin + 3
                        color: "gray"
                    }
                }

                // 最后修改时间
                Text {
                    id: template_time
                    x: 3 + template_fileName.width
                    width: 150; height: 30
                    font.family: fontFamily
                    font.pixelSize: pixelSize
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    elide: Text.ElideRight
                    color: "#FFFFFF"
                    text: isChinese ? "修改时间" : "Data modified"

                    Rectangle {
                        width: 2; height: parent.height
                        anchors.right: parent.right
                        anchors.rightMargin: margin
                        color: "gray"
                    }
                }

                // 文件类型
                Text {
                    id: template_fileType
                    x: 3 + template_fileName.width + template_time.width
                    width: 80; height: 30
                    font.family: fontFamily
                    font.pixelSize: pixelSize
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    elide: Text.ElideRight
                    color: "#FFFFFF"
                    text: isChinese ? "文件类型" : "FileType"

                    Rectangle {
                        width: 2; height: parent.height
                        anchors.right: parent.right
                        anchors.rightMargin: margin
                        color: "gray"
                    }
                }

                // 文件大小
                Text {
                    id: template_fileSize
                    x: 3 + template_fileName.width + template_time.width + template_fileType.width
                    width: 80; height: 30
                    font.family: fontFamily
                    font.pixelSize: pixelSize
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    elide: Text.ElideRight
                    color: "#FFFFFF"
                    text: isChinese ? "文件大小" : "FileSize"

                    Rectangle {
                        width: 2; height: parent.height
                        anchors.right: parent.right
                        anchors.rightMargin: margin
                        color: "gray"
                    }
                }
            }

            // ListView
            ListView {
                id: listView
                y: 30
                width: parent.width; height: parent.height - 30
                boundsBehavior: Flickable.StopAtBounds
                spacing: 5
                clip: true
                visible: !gridView.visible

                model: fileDialogModel

                ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

                delegate: Component {

                    Rectangle {
                        width: listView.width; height: 30
                        color: isSelect ? "#666666" : mouseArea.containsMouse ? "#999999" : "transparent"

                        // 文件图像
                        Image {
                            x: 3; y: 3
                            width: 24; height: 24
                            source: "qrc:/image/" + (isFolder === true ? (isDark ? "FolderL.png" : "Folder.png")
                                                                                 : (isDark ? "FileL.png" : "File.png") )
                        }

                        // 文件名
                        Text {
                            x: 30
                            width: template_fileName.width - 40; height: 30
                            font.family: fontFamily
                            font.pixelSize: pixelSize
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            elide: Text.ElideRight
                            color: "#FFFFFF"
                            text: fileName
                        }

                        // 最后修改时间
                        Text {
                            x: template_fileName.width
                            width: template_time.width; height: 30
                            font.family: fontFamily
                            font.pixelSize: pixelSize
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            elide: Text.ElideRight
                            color: "#FFFFFF"
                            text: lastModified
                        }

                        // 文件类型
                        Text {
                            x: template_fileName.width + template_time.width
                            width: template_fileType.width; height: 30
                            font.family: fontFamily
                            font.pixelSize: pixelSize
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            elide: Text.ElideRight
                            color: "#FFFFFF"
                            text: isFolder == true ? "文件夹" : fileType
                        }

                        // 文件大小
                        Text {
                            x: template_fileName.width + template_time.width + template_fileType.width
                            width: template_fileSize.width; height: 30
                            font.family: fontFamily
                            font.pixelSize: pixelSize
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignLeft
                            elide: Text.ElideRight
                            color: "#FFFFFF"
                            text: fileSize
                        }

                        MouseArea {
                            id: mouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                            onClicked: {
                                // 清空图片信息
                                clearImageInfo()

                                // 若矩形框的框选刚好已经结束，则不执行任何操作
                                if ( selectMouse.isComplete === 2 ) {
                                    selectMouse.isComplete = 3
                                    return
                                }

                                // 选择单个文件的情况
                                if ( !ctrlPressed && !shiftPressed )
                                {
                                    shiftSelectIndex = model.index
                                    // 若框选之后再点击某一个item，这个item的isSelect不变，其他的均为false
                                    if ( selectMouse.isComplete === 3 ) {
                                        selectMouse.isComplete = 0
                                        fileDialogModel.setIsSelectOpposite(model.index, model.index, model.isSelect)
                                    } else {
                                        // 正常情况下点击某一个的是!model.isSelect
                                        fileDialogModel.setIsSelectOpposite(model.index, model.index, true)
                                    }
                                }
                                else if(ctrlPressed)
                                {
                                    // 按住ctrl的情况
                                    model.isSelect = !isSelect
                                    fileNameComboBox.inputText = fileDialogModel.getSelectedFileName()
                                    return
                                }
                                else if (shiftPressed)
                                {
                                    // 按住shift的情况
                                    var start = shiftSelectIndex < model.index ? shiftSelectIndex : model.index
                                    var end = shiftSelectIndex > model.index ? shiftSelectIndex : model.index
                                    fileNameComboBox.inputText = fileDialogModel.setIsSelectOpposite(start, end, true)
                                    return
                                }

                                // 剩下的选择是单个文件的选择
                                // 如果选择的是文件，则把文件名复制给文件下拉框
                                if ( isFolder ) {
                                    fileNameComboBox.inputText = ""
                                } else {
                                    fileNameComboBox.inputText = fileName

                                    // 如果选中的是 ASC、XYZ、Sirius 可显示图片和文件信息
                                    if ( showImg ) {
                                        if ( fileType == "png" || fileType == "PNG" || fileType == "bmp" || fileType == "BMP" )
                                        {
                                            var path = "file:///" + currentPath + "/" + fileNameComboBox.inputText
                                            image.source = path
                                            pic_size.size = image.sourceSize.width + " x " + image.sourceSize.height
                                        }

                                        else if ( fileType == "sirius" )
                                        {
                                            var pic = fileName.replace("sirius", "jpg")
                                            path = currentPath + "/" + pic

                                            if ( fileDialogModel.isFileExist(path) ) {
                                                image.source = "file:///" + path
                                                pic_size.size = image.sourceSize.width + " x " + image.sourceSize.height
                                            }
                                        }

                                        else if ( fileType == "asc" || fileType == "xyz" )
                                        {
                                            pic = fileName.replace(fileType == "asc" ? "asc" : "xyz", "jpg")
                                            path = currentPath + "/" + pic

                                            if ( fileDialogModel.isFileExist(path) ) {
                                                image.source = "file:///" + path
                                                pic_size.size = image.sourceSize.width + " x " + image.sourceSize.height
                                            }

                                            var jsFile = fileName.replace(fileType == "asc" ? "asc" : "xyz", "json")
                                            path = currentPath + "/" + jsFile

                                            if ( fileDialogModel.isFileExist(path) ) {
                                                var obj = JSON.parse(fileDialogModel.read(path))

                                                if ( obj.psiMethod === 1 ) {
                                                    psiMethod.method = "BUCKET5A_5A"
                                                } else if ( obj.psiMethod === 3 ) {
                                                    psiMethod.method = "BUCKET5A_9A"
                                                } else if ( obj.psiMethod === 5 ) {
                                                    psiMethod.method = "OPT_SEQUENCE"
                                                } else if ( obj.psiMethod === 6 ) {
                                                    psiMethod.method = "9A_AIA"
                                                }

                                                if ( obj.unwrapMethod === 0 ) {
                                                    unwrapMethod.method = isChinese ? "枝切法" : "BRANCH_CUT"
                                                } else if ( obj.unwrapMethod === 1 ) {
                                                    unwrapMethod.method = isChinese ? "质量图" : "HISTOGRAM"
                                                } else if ( obj.unwrapMethod === 1 ) {
                                                    unwrapMethod.method = isChinese ? "跨区枝切法" : "SPAN_BRANCH_CUT"
                                                } else if ( obj.unwrapMethod === 1 ) {
                                                    unwrapMethod.method = isChinese ? "DCT全局法" : "DCT"
                                                } else if ( obj.unwrapMethod === 1 ) {
                                                    unwrapMethod.method = isChinese ? "质量图Pro" : "HISTOGRAM_PRO"
                                                }

                                                removeSpikes.method = obj.removeSpikes ? (isChinese ? "启用" : "Yes") :
                                                                                         (isChinese ? "未启用" : "No")
                                                removeResidual.method = obj.removeResidual ? (isChinese ? "启用" : "Yes") :
                                                                                             (isChinese ? "未启用" : "No")
                                                fillSpikes.method = obj.fillSpikes ? (isChinese ? "启用" : "Yes") :
                                                                                     (isChinese ? "未启用" : "No")

                                                var decimal = 5 / Math.pow(10, 3)
                                                var divisor = Math.pow(10, 2)
                                                pixel.size = Math.floor((obj.pixel + decimal) * divisor) / divisor + " mm"
                                            }
                                        }

                                    }
                                }
                            }

                            onDoubleClicked: {
                                if ( isFolder ) {
                                    // 如果是文件夹则进入到下级目录
                                    previousPath.push(currentPath)
                                    currentPath = filePath
                                    loadFolder(currentPath)
                                } else {
                                    // 如果选中的是 ASC、XYZ、Sirius，则直接加载
                                    if ( fileType == "asc" || fileType == "xyz" || fileType == "sirius" ) {
                                        is_load_ASCOrXYZ = fileType == "sirius" ? false : true

                                    } else {
                                        // 单张图片无法打开
                                        fileDialogModel.showMessageBox(3, "Please select the correct number of image files !")
                                    }
                                }
                            }
                        }

                    }
                }
            }

            // 框选的矩形框
            Rectangle{
                id: selectRect
                width: 0; height: 0
                border.width: 2
                border.color: Qt.darker("#666666", 5)
                color: Qt.darker("#666666", 1.5)
                opacity: 0.5
                visible: false
            }

            // 可拖动区域
            MouseArea{
                id: selectMouse
                anchors.fill: parent
                propagateComposedEvents: true
                enabled: listView.visible

                property int startX: 0
                property int startY: 0
                property int isComplete: 0  // 0-不进行操作  1-正在选择  2-选择完毕  3-当选择完毕之后点击任意一个item是选中的状态

                onPositionChanged: {
                    // 画的矩形最大值不可以超过listview
                    if ( selectRect.visible && containsMouse && mouseY >= 30 ) {
                        var X = mouseX < startX ? mouseX : startX
                        var Y = mouseY < startY ? mouseY : startY
                        var W = Math.abs(Number(mouseX) - Number(startX))
                        var H = Math.abs(Number(mouseY) - Number(startY))
                        selectRect.x = X
                        selectRect.y = Y
                        selectRect.width = W
                        selectRect.height = H

                        // 这里的Y默认的就是大于0的数，但是要限制值必须小于listview.height，H的最大值也只能是listview.height
                        Y = listView.contentY + selectRect.y - 30
                        H = Y + selectRect.height

                        // 若框选的矩形的Y值超过了contentHeight，则不进行处理
                        if ( Y > listView.contentHeight ) {
                            fileDialogModel.setIsSelectOpposite(0, 0, false)
                            fileNameComboBox.inputText = ""
                            return
                        }

                        // 若高度大于了listView.contentHeight，则按照listView.contentHeight的值进行计算
                        if ( H > listView.contentHeight ) {
                            H = listView.contentHeight
                        }

                        // 这里自动算入了listview.contentY值
                        var start = Math.ceil(Y / 35)
                        var end = Math.ceil(H /35)
                        fileNameComboBox.inputText = fileDialogModel.setIsSelectOpposite((start - 1) > 0 ? (start - 1) : 0, end -1, true)
                        isComplete = 1
                    }
                }

                onReleased: {
                    selectRect.visible = false
                    selectRect.width = 0
                    selectRect.height = 0

                    if ( isComplete === 1 ) {
                        // 这里用来判断是否在内部区域，因为listview的0点是mouseY=30的点
                        // 这里有个小坑，当最后的点不在mousearea里面，release就不会触发item的click，若在之内，就出触发
                        if ( mouseY >= 30 && (mouseY - 30) <= listView.height ) {
                            isComplete = 2
                        } else {
                            isComplete = 3
                        }
                    }
                }

                onPressed: {
                    // 要在高度为30以上的地方才允许绘制矩形
                    if ( containsPress && mouseY >= 30 ) {
                        startX = mouseX
                        startY = mouseY
                        selectRect.visible = true
                    }

                    if ( mouseY - 30 > listView.contentHeight ) {
                        fileDialogModel.setIsSelectOpposite(0, fileDialogModel.count() - 1, false)
                        fileNameComboBox.inputText = ""
                    }
                }
            }

            // 显示磁盘信息的界面
            GridView {
                id: gridView
                x: 30; y: 60
                width: parent.width - 60; height: parent.height - 90
                cellWidth: 180; cellHeight: 80
                boundsBehavior: Flickable.StopAtBounds
                visible: currentPath === "Computer"

                onVisibleChanged: {
                    if ( visible ) {
                        gridView.model = fileDialogModel.getStorageInfo()
                    }
                }

                delegate: Rectangle {
                    width: 140 ; height: 60
                    color: selectedFile == modelData ? "#666666" :
                                                       gridViewArea.containsMouse ? "#999999" : "transparent"

                    Image {
                        x: 10; y: 10
                        width: 40 ; height: 40
                        fillMode: Image.PreserveAspectFit
                        source: isDark ? "qrc:/image/HarddiskL.png" : "qrc:/image/Harddisk.png"
                    }

                    Text {
                        x: 60
                        width: 100; height: 60
                        font.family: fontFamily
                        font.pixelSize: pixelSize
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft
                        color: "#FFFFFF"
                        text: (isChinese ? "磁盘 ( " : "Disk ( ") + modelData.slice(0, modelData.length - 2) + " : )"
                    }

                    MouseArea {
                        id: gridViewArea
                        anchors.fill: parent
                        hoverEnabled: true

                        onClicked: selectedFile = modelData

                        onDoubleClicked: {
                            // 记录当前路径
                            previousPath.push(currentPath)

                            // 加载新路径
                            currentPath = modelData
                            loadFolder(modelData)
                        }
                    }
                }
            }

        }

        // ComboBox and button
        Rectangle {
            id: grid
            width: content.width; height: 80
            anchors.bottom: parent.bottom
            anchors.left: path.right
            anchors.margins: margin
            color: "#3F3F3F"

            Text {
                id: fileNameText
                width: contentWidth; height: 40
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: margin*2
                font.family: fontFamily
                font.pixelSize: pixelSize
                color: "#FFFFFF"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                text: isChinese ? "文件名称 :" : "File name :"
            }

            Text {
                id: fileTypeText
                width: contentWidth; height: 40
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.leftMargin: margin*2
                font.family: fontFamily
                font.pixelSize: pixelSize
                color: "#FFFFFF"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignRight
                text: isChinese ? "文件类型 :" : "File type :"
            }

            QYCombobox {
                id: fileNameComboBox
                height: 30
                anchors.top: parent.top
                anchors.left: fileNameText.right
                anchors.right: openBtn.left
                anchors.topMargin: margin
                anchors.leftMargin: margin*2
                anchors.rightMargin: margin*2
                font.family: fontFamily
                font.pixelSize: pixelSize
                editable: true
                comboboxText.onEditingFinished: {
                    model = []
                    popup.close()
                }

                comboboxText.onTextEdited: {
                    var data = []
                    model = []

                    if ( comboboxText.text.length === 0 ) {
                        return
                    }

                    for ( var i = 0; i < fileDialogModel.count(); ++i )
                    {
                        var patt = new RegExp(comboboxText.text, 'i')
                        if ( patt.test(fileDialogModel.getFileName(i)) ) {
                            data.push(fileDialogModel.getFileName(i))
                        }
                    }

                    if ( data.length === 0 ) {
                        return
                    }

                    model = data
                    popup.open()
                }
            }

            QYCombobox {
                id: fileTypeComboBox
                height: 30
                anchors.bottom: parent.bottom
                anchors.left: fileNameText.right
                anchors.right: openBtn.left
                anchors.bottomMargin: margin
                anchors.leftMargin: margin*2
                anchors.rightMargin: margin*2
                font.family: fontFamily
                font.pixelSize: pixelSize

                model: ["All Files (*.*)",
                        "All Files (*.asc *.xyz *.sirius *.png *.bmp)",
                        "ASC Files (*.asc)",
                        "XYZ Files (*.xyz)",
                        "Sirius File (*.sirius)",
                        "Image Files (*.png)",
                        "Image Files (*.bmp)"]

                onCurrentTextChanged: loadFolder(currentPath)
            }

            Button {
                id: openBtn
                width: 60; height: 30
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.topMargin: margin
                anchors.rightMargin: margin*2
                text:  isChinese ? "打 开" : "Open"
                onClicked: {
                    if ( fileNameComboBox.inputText === "" ) {
                        fileDialogModel.showMessageBox(3, "Please select the file to load !")
                    } else {
                        var file = fileNameComboBox.inputText

                        if ( file.lastIndexOf(", ") === -1 ) {
                            // 单个文件
                            var type = file.substring(file.lastIndexOf(".") + 1, file.length)

                            if ( type === "asc" || type === "xyz" || type === "sirius" ) {
                                is_load_ASCOrXYZ = type === "sirius" ? false : true
                            } else {
                                // 单张图片无法打开
                                fileDialogModel.showMessageBox(3, "Please select the correct number of image files !")
                            }
                        } else {
                            // 多个文件
                            var list = file.split(", ")
                            type = list[0].substring(list[0].lastIndexOf(".") + 1, list[0].length)

                            // 先确定文件类型
                            for ( var i = 1; i < list.length; ++i )
                            {
                                var temp = list[i].substring(list[i].lastIndexOf(".") + 1, list[i].length)
                                if ( type !== temp ) {
                                    // 文件类型不同
                                    fileDialogModel.showMessageBox(3, "Different types of files cannot be loaded at the same time !")
                                    return
                                }
                            }

                            if ( type !== "png" && type !== "bmp" ) {
                                fileDialogModel.showMessageBox(3, type.toUpperCase() + " file can only load one at a time !")
                            } else {
                                is_load_ASCOrXYZ = false
                            }
                        }
                    }
                }
            }

            Button {
                id: cancelBtn
                width: 60; height: 30
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.bottomMargin: margin
                anchors.rightMargin: margin*2
                text: isChinese ? "取 消" : "Cancel"
                onClicked: fileDialog.visible = false
            }
        }

        // Picture info
        Rectangle {
            id: picture
            width: 300
            anchors.top: catalogue.bottom
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            anchors.margins: margin
            color: "#3F3F3F"
            visible: showImg

            // Picture
            Image {
                id: image
                width: 280; height: 280
                anchors.top: parent.top
                anchors.topMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                fillMode: Image.PreserveAspectFit
            }

            // Separator line - landscape
            Rectangle {
                width: 290; height: lineWidth
                radius: lineWidth
                anchors.top: image.bottom
                anchors.topMargin: 10
                anchors.horizontalCenter: parent.horizontalCenter
                color: "#707070"
                visible: pic_size.size != ""
            }

            // 宽高
            Text {
                id: pic_size
                width: 280; height: 20
                anchors.top: image.bottom
                anchors.left: image.left
                anchors.topMargin: 20 + lineWidth
                font.family: fontFamily
                font.pixelSize: pixelSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                elide: Text.ElideRight
                color: "#FFFFFF"
                text: (isChinese ? "图片尺寸 :  " : "Picture size :  ") + pic_size.size
                visible: pic_size.size != ""

                property string size: ""
            }

            // 相位提取算法
            Text {
                id: psiMethod
                width: 280; height: 20
                anchors.top: pic_size.bottom
                anchors.left: image.left
                anchors.topMargin: lineWidth * 2
                font.family: fontFamily
                font.pixelSize: pixelSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                elide: Text.ElideRight
                color: "#FFFFFF"
                text: (isChinese ? "相位提取算法 :  " : "PSI method :  ") + psiMethod.method
                visible: psiMethod.method != ""

                property string method: ""
            }

            // 解包裹算法
            Text {
                id: unwrapMethod
                width: 280; height: 20
                anchors.top: psiMethod.bottom
                anchors.left: image.left
                anchors.topMargin: lineWidth * 2
                font.family: fontFamily
                font.pixelSize: pixelSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                elide: Text.ElideRight
                color: "#FFFFFF"
                text: (isChinese ? "解包裹算法 :  " : "Unwrap method :  ") + unwrapMethod.method
                visible: unwrapMethod.method != ""

                property string method: ""
            }

            // 是否去毛刺
            Text {
                id: removeSpikes
                width: 280; height: 20
                anchors.top: unwrapMethod.bottom
                anchors.left: image.left
                anchors.topMargin: lineWidth * 2
                font.family: fontFamily
                font.pixelSize: pixelSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                elide: Text.ElideRight
                color: "#FFFFFF"
                text: (isChinese ? "去毛刺 :  " : "Remove spikes :  ") + removeSpikes.method
                visible: removeSpikes.method != ""

                property string method: ""
            }

            // 是否去残差
            Text {
                id: removeResidual
                width: 280; height: 20
                anchors.top: removeSpikes.bottom
                anchors.left: image.left
                anchors.topMargin: lineWidth * 2
                font.family: fontFamily
                font.pixelSize: pixelSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                elide: Text.ElideRight
                color: "#FFFFFF"
                text: (isChinese ? "去残差 :  " : "Remove residual :  ") + removeResidual.method
                visible: removeResidual.method != ""

                property string method: ""
            }

            // 是否毛刺填充
            Text {
                id: fillSpikes
                width: 280; height: 20
                anchors.top: removeResidual.bottom
                anchors.left: image.left
                anchors.topMargin: lineWidth * 2
                font.family: fontFamily
                font.pixelSize: pixelSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                elide: Text.ElideRight
                color: "#FFFFFF"
                text: (isChinese ? "毛刺填充 :  " : "Fill spikes :  ") + fillSpikes.method
                visible: fillSpikes.method != ""

                property string method: ""
            }

            // 像素标定值
            Text {
                id: pixel
                width: 280; height: 20
                anchors.top: fillSpikes.bottom
                anchors.left: image.left
                anchors.topMargin: lineWidth * 2
                font.family: fontFamily
                font.pixelSize: pixelSize
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
                elide: Text.ElideRight
                color: "#FFFFFF"
                text: (isChinese ? "像素标定值 :  " : "Pixel calibration :  ") + pixel.size
                visible: pixel.size != ""

                property string size: ""
            }

        }

    }

    // 根据路径和过滤器获取合适的文件
    function loadFolder(path) {
        clearImageInfo()
        shiftSelectIndex = 0
        var data = []
        var filters = fileTypeComboBox.currentText

        // "All Files (*.png *.bmp *.xyz *.asc *.sirius)"
        // 通过 '(' 来截取需要的内容 "*.png *.bmp *.xyz *.asc *.sirius"
        filters = filters.slice(filters.indexOf("(") + 1, filters.length - 1)

        fileDialogModel.loadFolder(path, filters)

        // 滚动列表回到第一行
        listView.positionViewAtBeginning()

        // 清空选中的文件名
        fileNameComboBox.inputText = ""
    }

    // 清空图片信息
    function clearImageInfo() {
        image.source = ""
        pic_size.size = ""
        psiMethod.method = ""
        unwrapMethod.method = ""
        removeSpikes.method = ""
        removeResidual.method = ""
        fillSpikes.method = ""
        pixel.size = ""
    }
}
