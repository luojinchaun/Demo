#ifndef GRAPHICPOINT_H
#define GRAPHICPOINT_H

#include <QQuickPaintedItem>
#include <QMouseEvent>
#include <QPainter>
#include <QDebug>
#include <QCursor>

class GraphicPoint : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(PointType m_type READ getType WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QPointF m_point READ getPoint WRITE setPoint NOTIFY pointChanged)
    Q_PROPERTY(qreal m_pointX READ getPointX WRITE setPointX NOTIFY pointXChanged)
    Q_PROPERTY(qreal m_pointY READ getPointY WRITE setPointY NOTIFY pointYChanged)

public:
    enum class PointType {
        Center = 0,     // 中心点
        Edge,           // 边缘点（可拖动改变图形的形状、大小）
        Special         // 多边形点
    };

    GraphicPoint(QQuickPaintedItem* parent, QPointF pos, PointType type);

    PointType getType() const { return m_type; }
    void setType(PointType type) { m_type = type; emit typeChanged(); }

    QPointF getPoint() const { return m_point; }
    void setPoint(QPointF point) { m_point = point; emit pointChanged(); }

    qreal getPointX() const { return m_point.x(); }
    void setPointX(qreal x) { m_point.setX(x); emit pointXChanged(); }

    qreal getPointY() const { return m_point.y(); }
    void setPointY(qreal y) { m_point.setY(y); emit pointYChanged(); }


protected:
    void paint(QPainter *painter) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;

signals:
    void pointChanged();
    void updatePosition(PointType type, QPointF pos);  // 位置变化，通知外部
    void typeChanged();
    void pointXChanged();
    void pointYChanged();

private:
    QPointF m_point;        // 点的中心坐标
    PointType m_type;       // 点的类型
    QPointF m_pressPoint;   // 鼠标按压时的点
};

#endif // GRAPHICPOINT_H
