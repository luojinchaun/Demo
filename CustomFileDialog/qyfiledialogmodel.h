#ifndef QYFILEDIALOGMODEL_H
#define QYFILEDIALOGMODEL_H

#include <QAbstractListModel>
#include <QDir>

typedef struct fileInfo {
    bool isFolder;
    bool isSelect;
    QString fileName;
    QString filePath;
    QString fileType;
    QString fileSize;
    QString lastModified;
} FileInfo;

class QYFileDialogModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum FILE_INFO_ROLE
    {
        isFolder = 0,
        isSelect,
        fileName,
        filePath,
        fileType,
        fileSize,
        lastModified,
    };
    Q_ENUM(FILE_INFO_ROLE)

    explicit QYFileDialogModel(QObject *parent = nullptr);

    Q_INVOKABLE QVariantList getStorageInfo();                                          // 获取磁盘信息
    Q_INVOKABLE QString getPath(QString type);                                          // 获取路径
    Q_INVOKABLE void loadFolder(QString path = "", QString filter = "");                // 根据路径和过滤器，获取当前路径下所有文件
    Q_INVOKABLE QString setIsSelectOpposite(int start, int end, bool select);           // 提供的索引设置select，其他的设置为false
    Q_INVOKABLE QString getSelectedFileName();                                      	// 获取已经选择的文件名

    Q_INVOKABLE int count() { return m_date.size(); }                                   // 获取文件数量
    Q_INVOKABLE QString getFileName(int index) { return m_date.at(index).fileName; }    // 获取文件名称
    Q_INVOKABLE QString getCurrentPath() { return QDir::currentPath(); }
    Q_INVOKABLE void showMessageBox(int type, QString info);
    Q_INVOKABLE bool isFileExistStatic(const QString &fileName);
    Q_INVOKABLE QString read(const QString path);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QHash<int, QByteArray> roleNames() const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

private:
    QList<FileInfo> m_date;
    QHash<int, QByteArray> m_roleName;
};

#endif // FILEDIALOGMODEL_H
