#include "qyfiledialogmodel.h"
#include <QStandardPaths>
#include <QStorageInfo>
#include <QDateTime>
#include <QMessageBox>
#include <QTextStream>

QYFileDialogModel::QYFileDialogModel(QObject *parent) : QAbstractListModel(parent)
{
    m_roleName.insert(isFolder, "isFolder");
    m_roleName.insert(isSelect, "isSelect");
    m_roleName.insert(fileName, "fileName");
    m_roleName.insert(filePath, "filePath");
    m_roleName.insert(fileType, "fileType");
    m_roleName.insert(fileSize, "fileSize");
    m_roleName.insert(lastModified, "lastModified");
}

QVariantList QYFileDialogModel::getStorageInfo()
{
    QVariantList info;

    /*
     * QList<QStorageInfo> QStorageInfo::mountedVolumes()
     * 返回与当前装入的文件系统列表相对应的QStorageInfo对象列表
     * 在Windows上，返回“我的电脑”文件夹中可见的驱动器
     * 在Unix操作系统上，返回所有装入的文件系统（伪文件系统除外）的列表
     */

    foreach ( const QStorageInfo &storage, QStorageInfo::mountedVolumes() ) {
        if ( storage.isValid() && storage.isReady() ) {
            if ( !storage.isReadOnly() ) {
                info.push_back(storage.rootPath());
            }
        }
    }

    return info;
}

QString QYFileDialogModel::getPath(QString type)
{
    if ( type == "Desktop" ) {
        return QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
    }
    else if ( type == "Download" ) {
        return QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);
    }
    else if ( type == "Document" ) {
        return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    }
    else if ( type == "Picture" ) {
        return QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    } else {
        return "Computer";
    }
}

void QYFileDialogModel::loadFolder(QString path, QString filter)
{
    if ( path == "" || path == "Computer" )
    {
        return;
    }

    // 缺少盘符，若path为D:，则无法读出
    if (path.indexOf("/") == -1) {
        path += "/";
    }
    QDir dir(path);
    QStringList filt;

    // 拆分过滤器 "*.png *.bmp *.xyz *.asc *.sirius"
    if ( filter.length() != 0 )
    {
        filt = filter.split(" ", QString::SkipEmptyParts);
    }

    // 根据过滤器获取合适的文件
    QFileInfoList fileList = dir.entryInfoList(filt, QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks,
                                               QDir::Name | QDir::DirsFirst);

    // 清空 ListModel 的数据
    if ( m_date.size() != 0 )
    {
        emit beginRemoveRows(QModelIndex(), 0, m_date.size() - 1);
        m_date.clear();
        emit endRemoveRows();
    }

    // 没有合适的文件
    if ( fileList.size() == 0 )
    {
        return;
    }

    emit beginInsertRows(QModelIndex(), 0, fileList.size() - 1);

    // 把合适的文件信息填入列表
    for ( int i = 0; i != fileList.size(); ++i )
    {
        QString lastModified = fileList.at(i).lastModified().toString("yyyy/MM/dd hh:mm");
        QString type = "";

        if ( !fileList.at(i).isDir() )
        {
            QString fileName = fileList.at(i).fileName();

            if ( -1 != fileName.lastIndexOf(".") ) {
                type = fileName.right( fileName.count() - fileName.lastIndexOf(".") - 1 );
            }
        }

        m_date.push_back( { fileList.at(i).isDir(),
                            false,
                            fileList.at(i).fileName(),
                            fileList.at(i).filePath(),
                            type,
                            fileList.at(i).isDir() ? "" : QString::number(ceil( (double)fileList.at(i).size() / 1024)) + " KB",
                            lastModified } );
    }

    emit endInsertRows();
}

QString QYFileDialogModel::setIsSelectOpposite(int start, int end, bool select)
{
    if ( start > end || start < 0 || end >= m_date.size() || m_date.size() == 0)
    {
        return QString();
    }
    else
    {
        // 返回所选中的文件的名字的字符串，以逗号间隔
        QModelIndex topLeft = createIndex(0, 0);
        QModelIndex bottomRight = createIndex(m_date.size() - 1, 0);
        QString list;

        for ( int i = 0; i < m_date.size(); ++i )
        {
            if ( i >= start && i <= end )
            {
                m_date[i].isSelect = select;

                if ( m_date[i].isFolder ) {
                    continue;
                }

                list += m_date[i].fileName + ", ";
            }
            else
            {
                m_date[i].isSelect = false;
            }

        }

        // 把最后的", "去掉
        if ( list.length() != 0 )
        {
            list.remove(list.length() - 2, 2);
        }

        emit dataChanged(topLeft, bottomRight, QVector<int>() << isSelect);

        return list;
    }
}

QString QYFileDialogModel::getSelectedFileName()
{
    QString list;
    if ( m_date.size() == 0 )
    {
        return list;
    }

    for ( int i = 0; i < m_date.size(); ++i )
    {
        if ( m_date[i].isSelect )
        {
            list += m_date[i].fileName + ", ";
        }
    }

    // 把最后的", "去掉
    if ( list.length() != 0 )
    {
        list.remove(list.length() - 2, 2);
    }

    return list;
}

void QYFileDialogModel::showMessageBox(int type, QString info)
{
    switch ( type )
    {
    case 1: QMessageBox::question(nullptr, "Question", info, QMessageBox::Ok); break;       // 在正常操作中提出问题
    case 2: QMessageBox::information(nullptr, "Information", info, QMessageBox::Ok); break; // 用于报告有关正常操作的信息
    case 3: QMessageBox::warning(nullptr, "Warning", info, QMessageBox::Ok); break;         // 用于报告非关键错误
    case 4: QMessageBox::critical(nullptr, "Error", info, QMessageBox::Ok); break;          // 用于报告关键错误
    default: break;
    }
}

bool QYFileDialogModel::isFileExistStatic(const QString &fileName)
{
    QFileInfo fileInfo(fileName);
    return fileInfo.isFile();
}

QString QYFileDialogModel::read(const QString path)
{
    if ( path.isEmpty() ) {
        return "The path is empty !";
    }

    QFile file(path);
    QString result;

    // 以只读的方式打开本地文件，一行一行的读取内容并追加在后面，最后返回一个一行的字符串
    if ( file.open(QIODevice::ReadOnly) ) {
        QString line;
        QTextStream t( &file );
        do {
            line = t.readLine();
            result += line;
        } while (!line.isNull());

        file.close();
    } else {
        return "Unable to open the file !";
    }

    return result;
}

int QYFileDialogModel::rowCount(const QModelIndex &parent) const
{
    if ( parent.isValid() ) {
        return 0;
    } else {
        return m_date.size();
    }
}

QHash<int, QByteArray> QYFileDialogModel::roleNames() const
{
   return m_roleName;
}

QVariant QYFileDialogModel::data(const QModelIndex &index, int role) const
{
    if ( !index.isValid() )
    {
        return QVariant();
    }

    switch (role)
    {
    case isFolder: return m_date.value(index.row()).isFolder;
    case isSelect: return m_date.value(index.row()).isSelect;
    case fileName: return m_date.value(index.row()).fileName;
    case filePath: return m_date.value(index.row()).filePath;
    case fileType: return m_date.value(index.row()).fileType;
    case fileSize: return m_date.value(index.row()).fileSize;
    case lastModified: return m_date.value(index.row()).lastModified;
    default: return QVariant();
    }
}

bool QYFileDialogModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if ( index.isValid() )
    {
        switch (role)
        {
        case isFolder: m_date[index.row()].isFolder = value.toBool(); break;
        case isSelect: m_date[index.row()].isSelect = value.toBool(); break;
        case fileName: m_date[index.row()].fileName = value.toString(); break;
        case filePath: m_date[index.row()].filePath = value.toString(); break;
        case fileType: m_date[index.row()].fileType = value.toString(); break;
        case fileSize: m_date[index.row()].fileSize = value.toString(); break;
        case lastModified: m_date[index.row()].lastModified = value.toString(); break;
        default:return false;
        }

        emit dataChanged(index, index, QVector<int>() << role); // 数值改变以后需要发送这个信号，这时在qml端能即时刷新
        return true;
    }

    return false;
}
