#ifndef GRAPHICITEM_H
#define GRAPHICITEM_H

#include <QQuickPaintedItem>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include "graphicpoint.h"

class GraphicItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(ItemType m_type READ getType WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(bool createState READ getCreateState WRITE setCreateState NOTIFY createStateChanged)
    Q_PROPERTY(bool selectState READ getSelectState WRITE setSelectState NOTIFY selectStateChanged)
    Q_PROPERTY(bool showRect READ getShowRect WRITE setShowRect NOTIFY showRectChanged)
    Q_PROPERTY(int itemAngle READ getAngle WRITE setAngle NOTIFY angleChanged)

public:

    enum class ItemType {
        Circle = 0,         // 圆
        Rectangle,          // 矩形
        Polygon,            // 多边形
    };

    explicit GraphicItem(QQuickItem* parent, QRectF rect, ItemType type);

    ItemType getType() const { return m_type; }
    void setType(ItemType type) { m_type = type; emit typeChanged(); }

    bool getCreateState() const { return m_createFinished; }
    void setCreateState(bool state) { m_createFinished = state; emit createStateChanged(); }

    bool getSelectState() const { return m_selectState; }
    void setSelectState(bool state) { m_selectState = state; update(); emit selectStateChanged(); }

    bool getShowRect() const { return m_showRect; }
    void setShowRect(bool state) { m_showRect = state; update(); emit showRectChanged(); }

    int getAngle() const { return this->rotation(); }
    void setAngle(int angle) { this->setRotation(angle); emit angleChanged();}

public slots:
    void updatePosition(GraphicPoint::PointType type, QPointF pos);         // 由点更新位置或形状

protected:
    void paint(QPainter *painter) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;

signals:
    void typeChanged();
    void createStateChanged();                                              // 图元是否已经创建完毕
    void selectStateChanged();                                              // 是否已经选中
    void showRectChanged();                                                 // 是否显示QQuickItem的外接矩形
    void angleChanged();                                                    // 旋转角度发生变化
    void startCreatePolygon();                                              // 通知外部开始创建多边形
    void rightButtonClick(GraphicItem* item, qreal mouseX, qreal mouseY);   // 鼠标右键事件
    void clicked(GraphicItem* item);                                        // 点击item发出此信号

private:
    ItemType m_type;
    GraphicPoint *m_center;                   // 中心点
    GraphicPoint *m_edge;                     // 边缘点
    QVector<GraphicPoint*> m_points;          // 存储多边形点集

    QRectF m_rect;                            // 整个rect在Scene所处的位置
    QPointF m_pressPoint;                     // 鼠标按压时的点
    QPen m_pen_isSelected;                    // 选中画笔
    QPen m_pen_noSelected;                    // 未选中画笔
    bool m_selectState;                       // 是否选中
    bool m_isPolygonCreating;                 // 是否正在创建多边形
    bool m_createFinished;                    // 图形创建完毕
    bool m_showRect;                          // 是否显示外接矩形
};

#endif // GRAPHICITEM_H
