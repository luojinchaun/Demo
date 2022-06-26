#include "graphicpoint.h"

GraphicPoint::GraphicPoint(QQuickPaintedItem* parent, QPointF pos, PointType type) : QQuickPaintedItem(parent)
{
    // 添加鼠标事件
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setFlag(ItemAcceptsInputMethod, true);

    connect(this, &GraphicPoint::pointChanged,[&]() {
        setPosition(QPointF(m_point.x() - width() / 2, m_point.y() - height() / 2));
    });

    connect(this, &GraphicPoint::typeChanged, [&]() {
        // 确定光标类型
        switch ( m_type )
        {
        case PointType::Center:
            this->setCursor(Qt::PointingHandCursor);
            break;
        case PointType::Edge:
            this->setCursor(Qt::SizeFDiagCursor);
            break;
        case PointType::Special:
            this->setCursor(Qt::SizeAllCursor);
            break;
        default: break;
        }
    });

    setSize(QSizeF(10, 10));
    setPoint(pos);
    setType(type);
}

void GraphicPoint::paint(QPainter *painter)
{
    painter->setBrush(QColor("#00ff00"));
    QRectF rect( 0, 0, this->width(), this->height() );

    switch ( m_type )
    {
    case PointType::Center:
        painter->drawEllipse(rect);
        break;
    case PointType::Edge:
        painter->drawRect(rect);
        break;
    case PointType::Special:
        painter->drawRect(rect);
        break;
    default: break;
    }
}

void GraphicPoint::mouseMoveEvent(QMouseEvent *event)
{
    if ( event->buttons() == Qt::LeftButton ) {
        QPointF pos = m_point + event->pos() - m_pressPoint;
        emit updatePosition(m_type, pos);
    }
}

void GraphicPoint::mousePressEvent(QMouseEvent *event)
{
    if ( event->buttons() == Qt::LeftButton ) {
        m_pressPoint = event->pos();
    }
}
