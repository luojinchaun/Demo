#ifndef GRAPHICSCENE_H
#define GRAPHICSCENE_H

#include <QQuickPaintedItem>
#include <QSGSimpleTextureNode>
#include <QQuickWindow>
#include <QTimer>
#include "graphicitem.h"

class GraphicScene : public /*QQuickPaintedItem*/ QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QImage m_image READ getImage WRITE setImage NOTIFY imageChanged)

public:
    explicit GraphicScene(QQuickItem* parent = nullptr);

    QImage getImage() const { return m_image; }
    void setImage(QImage image) { m_image = image; emit imageChanged(); }

    Q_INVOKABLE void createGraphItem(int type);                             // 创建图元
    Q_INVOKABLE void setImagePath(QString path);                            // 设置图片 路径
    Q_INVOKABLE void updateSelect();                                        // 将现有的图元设置为非选中状态
    Q_INVOKABLE void showRect(bool show);                                   // 设置现有图元是否显示最小外接矩形
    Q_INVOKABLE void deleteItem();                                          // 删除当前图元

public slots:
    void setRect();                                                         // 设置图片显示的矩形
    void updateItemSize();                                                  // 当正在创建多边形时，改变外部尺寸，绘制多边形区域也会随着改变

signals:
    void imageChanged();                                                    // 设置显示图片
    void createItemCompleted(GraphicItem* item);                            // 图元创建完毕，发送给外部调用者
    void rightButtonClick(GraphicItem* item, qreal mouseX, qreal mouseY);   // 鼠标右键事件
    void callQmlUpdatePos(qreal dx, qreal dy);
protected:
    virtual QSGNode * updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

private:
    QImage m_image;                         // 原图
    QImage m_paintImage;                    // 需要绘制的图片，经过了缩放，保持原比例
    QRectF m_rect;                          // 图片显示的rect
    QVector<GraphicItem*> m_items;          // 存储当前所有图元
    GraphicItem *m_currentItem;             // 当前图元
    QPointF m_pressPos;                     // 存储鼠标点击时的位置
    QPointF m_wheelPos;                     // 存储鼠标位置
};

#endif // GRAPHICSCENE_H
