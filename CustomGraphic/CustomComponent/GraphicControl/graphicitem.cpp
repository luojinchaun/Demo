#include "graphicitem.h"

GraphicItem::GraphicItem(QQuickItem* parent, QRectF rect, ItemType type) : QQuickPaintedItem(parent)
{
    // 添加鼠标事件
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setFlag(ItemAcceptsInputMethod, true);

    // 默认不创建多边形
    m_isPolygonCreating = false;

    // 默认未创建完毕
    m_createFinished = false;

    // 默认未选择
    m_selectState = false;

    m_pen_noSelected.setColor(QColor(255, 255, 255));
    m_pen_noSelected.setWidth(2);

    m_pen_isSelected.setColor(QColor(0, 255, 0));
    m_pen_isSelected.setWidth(2);

    m_rect = rect;
    m_type = type;

    // 设置位置信息
    setPosition(m_rect.topLeft());
    setSize(m_rect.size());

    switch (m_type) {
    case ItemType::Circle: {
        m_center = new GraphicPoint(this, QPointF(width()/2, height()/2), GraphicPoint::PointType::Center);
        m_edge = new GraphicPoint(this, QPointF(width(), height() / 2), GraphicPoint::PointType::Edge);
    } break;
    case ItemType::Rectangle: {
        m_center = new GraphicPoint(this, QPointF(width()/2, height()/2), GraphicPoint::PointType::Center);
        m_edge = new GraphicPoint(this, QPointF(width(), height()), GraphicPoint::PointType::Edge);
    } break;
    case ItemType::Polygon: {
        setFillColor(QColor(255, 0, 0, 100));
        m_isPolygonCreating = true;
        return;
    } break;
    }

    connect(m_center, &GraphicPoint::updatePosition, this, &GraphicItem::updatePosition);
    connect(m_edge, &GraphicPoint::updatePosition, this, &GraphicItem::updatePosition);

    // 创建完毕
    setCreateState(true);
}

void GraphicItem::updatePosition(GraphicPoint::PointType type, QPointF pos)
{
    if ( type == GraphicPoint::PointType::Center ) {
        // 中心点坐标转换为相对于parent的坐标。由于是中心点，所以将pos位置再处理
        pos = mapToItem(this->parentItem(), pos);
        pos.setX(pos.x() - width() / 2);
        pos.setY(pos.y() - height() / 2);
        setPosition(pos);
    }
    else if ( type == GraphicPoint::PointType::Edge )
    {
        switch (m_type) {
        case ItemType::Circle: {
            qreal radius = sqrt(pow(pos.x() - m_center->getPointX(), 2) + pow(pos.y() - m_center->getPointY(), 2));

            qreal dx = radius - width() / 2;    //半径增量
            qreal m_sin = (pos.y() - m_center->getPointY()) / radius;
            qreal m_cos = (pos.x() - m_center->getPointX()) / radius;

            setPosition(QPointF(this->x() - dx, this->y() - dx));
            setWidth(radius * 2);
            setHeight(radius * 2);

            m_center->setPoint(QPointF(width() / 2, height() / 2));
            m_edge->setPoint(QPointF(width() / 2 + radius * m_cos, height() / 2 + radius * m_sin));

        } break;
        case ItemType::Rectangle: {
            qreal dw = abs(pos.x() - m_center->getPointX()) * 2;
            qreal dh = abs(pos.y() - m_center->getPointY()) * 2;
            setPosition(QPointF(this->x() - (dw - width()) / 2, this->y() - (dh - height()) / 2));
            setSize(QSizeF(dw, dh));

            // 在这里会实时的使用m_center的值，所以务必要及时更新
            m_center->setPoint(QPointF(width() / 2, height() / 2));
            m_edge->setPoint(QPointF(pos));
        } break;
        case ItemType::Polygon: {
            GraphicPoint *point = qobject_cast<GraphicPoint *>(sender());
            point->setPoint(pos);

            // 计算最小外接矩形
            qreal left = INT_MAX;
            qreal top = INT_MAX;
            qreal right = INT_MIN;
            qreal bottom = INT_MIN;
            for ( auto &temp : m_points )
            {
                if ( left > temp->getPointX() ) left = temp->getPointX();
                if ( top > temp->getPointY() ) top = temp->getPointY();
                if ( right < temp->getPointX() ) right = temp->getPointX();
                if ( bottom < temp->getPointY() ) bottom = temp->getPointY();
            }

            QPointF tmp(left, top);

            for ( auto &temp : m_points )
            {
                temp->setPoint(temp->getPoint() -  tmp);
            }

            // 旋转之后更新大小会使得视觉上坐标轴不一致，需要使得视觉上坐标轴一致
            QPointF oldPos = mapToItem(this->parentItem(), QPointF(0,0));
            this->setSize(QSizeF(right - left, bottom - top));
            QPointF newPos = mapToItem(this->parentItem(), QPointF(0,0));
            this->setPosition(this->position() - (newPos - oldPos));

            // 更新位置信息
            qreal Pi = 3.14159265;
            qreal x_dx = cos(rotation() * Pi / 180) * left;
            qreal x_dy = sin(rotation() * Pi / 180) * left;

            qreal y_dx = sin((360 - rotation()) * Pi / 180) * top;
            qreal y_dy = cos((360 - rotation()) * Pi / 180) * top;
            setX(this->x() + x_dx + y_dx);
            setY(this->y() + x_dy + y_dy);

            // 更新中心点
            m_center->setPoint(QPointF(width() / 2, height() / 2));

            update();
        } break;
        }
    }
}

void GraphicItem::paint(QPainter *painter)
{
    painter->setPen(m_selectState ? m_pen_isSelected : m_pen_noSelected);
    painter->setRenderHints(QPainter::SmoothPixmapTransform | QPainter::Antialiasing);

    // 由于画笔宽度会影响图形绘制，所有整体内缩画笔宽度的一半
    int penWidth = painter->pen().width() / 2;
    QRectF rect(penWidth, penWidth, width() - penWidth * 2, height() - penWidth * 2);

    switch ( m_type )
    {
    case ItemType::Circle: { painter->drawEllipse(rect); } break;
    case ItemType::Rectangle: { painter->drawRect(rect);} break;
    case ItemType::Polygon: {
        if ( m_points.size() > 1 ) {
            if ( m_isPolygonCreating ) {

                for ( int i = 1; i < m_points.size(); ++i )
                {
                    painter->drawLine(m_points[i-1]->getPoint(), m_points[i]->getPoint());
                }
            } else {
                for ( int i = 1; i < m_points.size(); ++i )
                {
                    painter->drawLine(m_points[i-1]->getPoint(), m_points[i]->getPoint());
                }

                // 形成密闭多边形
                painter->drawLine(m_points.last()->getPoint(), m_points.first()->getPoint());
            }
        }
    } break;
    }

    if (m_showRect) {
        QPen pen;
        pen.setColor(Qt::red);
        pen.setWidth(2);
        painter->setPen(pen);
        painter->drawRect(this->boundingRect());
        painter->drawLine(0, 0, this->width(), this->height());
        painter->drawLine(this->width(), 0, 0, this->height());
    }
}

void GraphicItem::mouseMoveEvent(QMouseEvent *event)
{
    if ( event->buttons() == Qt::LeftButton && !m_isPolygonCreating) {
        // 1. 因为在press时进行转换获取的点是相对于scene的第一次坐标，而在move时要一直对相对于scene进行坐标转换，所以要实时转换
        // 2. 每一次move若不改变position的情况，则event->pos()会逐渐变大，但是每次设置了position，所以每次的event->pos()都是单次move的值，都会很小
        QPointF pos = mapToItem(this->parentItem(), event->pos());
        QPointF press = mapToItem(this->parentItem(), m_pressPoint);
        setPosition(position() + pos - press);
    }
}

void GraphicItem::mousePressEvent(QMouseEvent *event)
{
    // 创建多边形时点击依次放入容器
    if ( event->buttons() == Qt::LeftButton && m_isPolygonCreating ) {
        GraphicPoint *item = new GraphicPoint(this, QPointF(event->x(), event->y()), GraphicPoint::PointType::Edge);

        // 将enable置为false可以使得点处于hover状态时不会出现鼠标变形
        item->setEnabled(false);
        connect(item, &GraphicPoint::updatePosition, this, &GraphicItem::updatePosition);
        m_points.push_back(item);
        update();
    }
    else if ( m_isPolygonCreating && event->buttons() == Qt::RightButton && m_points.size() >= 3) {
        m_isPolygonCreating = false;
        setCreateState(true);
        setFillColor(QColor("transparent"));
        setZ(0);

        // 计算中心点并重新计算尺寸大小信息
        qreal left = 9999;
        qreal top = 9999;
        qreal right = -9999;
        qreal bottom = -9999;
        for ( auto &temp : m_points )
        {
            if ( left > temp->getPointX() ) left = temp->getPointX();
            if ( top > temp->getPointY() ) top = temp->getPointY();
            if ( right < temp->getPointX() ) right = temp->getPointX();
            if ( bottom < temp->getPointY() ) bottom = temp->getPointY();
        }

        QPointF pos(left, top);

        // 将所有的点的x均减去left，y减去top
        for ( auto &temp : m_points )
        {
            temp->setPoint(temp->getPoint() - pos);
            temp->setEnabled(true);
        }

        setPosition(pos);
        setSize(QSizeF(right - left, bottom - top));

        m_center = new GraphicPoint(this, QPointF(width() / 2, height() / 2), GraphicPoint::PointType::Center);
        connect(m_center, &GraphicPoint::updatePosition, this, &GraphicItem::updatePosition);
        update();
    }
    else if ( event->buttons() == Qt::LeftButton ) {
        m_pressPoint = event->pos();
        emit clicked(this);
    }
    else if ( event->buttons() == Qt::RightButton ) {
        emit rightButtonClick(this, event->x(), event->y());
    }
}

