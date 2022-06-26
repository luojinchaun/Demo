#include "graphicscene.h"

GraphicScene::GraphicScene(QQuickItem* parent) : QQuickItem(parent)
{
    // 添加鼠标事件
    setAcceptHoverEvents(true);
    setAcceptedMouseButtons(Qt::AllButtons);
    setFlag(ItemAcceptsInputMethod, true);

    // 把图形添加到场景中（继承 QQuickItem，重载 updatePaintNode 时需要，这是另一种方案此处略过）
    setFlag(ItemHasContents, true);

    connect(this, &GraphicScene::widthChanged, this, &GraphicScene::setRect);
    connect(this, &GraphicScene::heightChanged, this, &GraphicScene::setRect);
    connect(this, &GraphicScene::imageChanged, this, &GraphicScene::setRect);

    m_pressPos = QPointF(0, 0);
    m_wheelPos = QPointF(0, 0);
    m_currentItem = nullptr;
}

void GraphicScene::createGraphItem(int type)
{
    // 图元的最小外接矩形
    QRectF rect((width() - 150) / 2, (height() - 150) / 2, 150, 150);

    switch (type) {
    case 0: {
        m_currentItem = new GraphicItem(this, rect, GraphicItem::ItemType::Circle);
    } break;
    case 1: {
        m_currentItem = new GraphicItem(this, rect, GraphicItem::ItemType::Rectangle);
    } break;
    case 2: {
         m_currentItem = new GraphicItem(this, boundingRect(), GraphicItem::ItemType::Polygon);
         m_currentItem->setZ(1);
    } break;
    }

    // 点击某一个item，更新当前item
    connect(m_currentItem, &GraphicItem::clicked, [&](GraphicItem* item) {
        updateSelect(); // 清除图元选中状态
        m_currentItem = item;
        m_currentItem->setSelectState(true); // 将当前点击的item置为选中状态
    });

    // 取消现有item选择状态
    updateSelect();

    if (m_currentItem->getType() != GraphicItem::ItemType::Polygon) {
        emit createItemCompleted(m_currentItem);
    } else {
        // 若当前有一个item在创建多边形，此时改变大小，item的大小还是跟着改变
        connect(this, &GraphicScene::widthChanged, this, &GraphicScene::updateItemSize);
        connect(this, &GraphicScene::heightChanged, this, &GraphicScene::updateItemSize);
    }

    connect(m_currentItem, &GraphicItem::rightButtonClick,
        [&](GraphicItem* item, qreal mouseX, qreal mouseY)
    {
        emit rightButtonClick(item, mouseX, mouseY);
    });

    m_items.push_back(m_currentItem);
}

void GraphicScene::setImagePath(QString path)
{
    setImage(QImage(path));
}

void GraphicScene::updateSelect()
{
    for ( int i = 0; i < m_items.size(); ++i )
    {
        m_items.at(i)->setSelectState(false);
    }
}

void GraphicScene::showRect(bool show)
{
    for ( int i = 0; i < m_items.size(); ++i )
    {
        m_items.at(i)->setShowRect(show);
    }
}

void GraphicScene::deleteItem()
{
    if ( m_currentItem != nullptr ) {
        int index = m_items.indexOf(m_currentItem);
        m_items.remove(index);
        delete m_currentItem;
        m_currentItem = nullptr;
    }
}

void GraphicScene::setRect()
{
    if (m_image.isNull()) {
        m_rect.setRect(0,0,width(), height());
    }
    else {
        m_paintImage = m_image.scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);

        qreal x = (width() - m_paintImage.width()) / 2;
        qreal y = (height() - m_paintImage.height()) / 2;
        m_rect.setRect(x, y, m_paintImage.width(), m_paintImage.height());
    }

    update();
}

void GraphicScene::updateItemSize()
{
    GraphicItem *item;
    for ( int i = 0; i < m_items.size(); ++i ) {
        // 若多边形未创建完毕就改变外部大小，则更改其尺寸
        item = m_items.at(i);
        if ( item->getType() == GraphicItem::ItemType::Polygon && !item->getCreateState() ) {
            item->setSize(this->size());
        }
    }
}

QSGNode *GraphicScene::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *)
{
    auto node = dynamic_cast<QSGSimpleTextureNode *>(oldNode);

    if ( !node ) {
        node = new QSGSimpleTextureNode();
    }

    QSGTexture *m_texture = window()->createTextureFromImage(m_paintImage, QQuickWindow::TextureIsOpaque);   // 通过图片生成纹理
    node->setOwnsTexture(true);                             // 节点拥有纹理的所有权
    node->setRect(m_rect);                                  // 设置纹理节点的目标矩形

    node->markDirty(QSGNode::DirtyForceUpdate);             // 通知渲染器
    node->setTexture(m_texture);
    return node;
}

void GraphicScene::mouseMoveEvent(QMouseEvent *event)
{
    // pass
}

void GraphicScene::mousePressEvent(QMouseEvent *event)
{
    // 更新点击点，以便放大场景，并获取焦点，能够使用键盘事件
    setFocus(true);
    m_pressPos = event->pos();
}

void GraphicScene::keyPressEvent(QKeyEvent *event)
{
    QPointF oldPos = mapToScene(m_pressPos);
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Minus) {
        if (this->scale() > 0.1) {
            this->setScale(this->scale() * 0.95);
            QPointF newPos = mapToScene(m_pressPos);
            this->setPosition(position() + oldPos - newPos);
        }
        event->setAccepted(true);
    }

    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_Equal) {
        this->setScale(this->scale() * 1.05);
        QPointF newPos = mapToScene(m_pressPos);
        this->setPosition(position() + oldPos - newPos);
        event->setAccepted(true);
    }

    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_0) {
        this->setScale(1);
        QPointF newPos = mapToScene(m_pressPos);
        this->setPosition(position() + oldPos - newPos);
        event->setAccepted(true);
    }
}

// 鼠标放大与缩小
void GraphicScene::wheelEvent(QWheelEvent *event)
{
    qDebug()<<event->posF();
    // 若滚动后按压点与当前点不一致，则更新
    if ( m_wheelPos != event->posF()) {
        m_wheelPos = event->posF();
    }

    QPointF oldPos = mapToScene(m_wheelPos);

    // 上滚放大
    if ( event->delta() > 0 ) {
        this->setScale(this->scale() * 1.1);
        QPointF newPos = mapToScene(m_wheelPos);
        this->setPosition(position() + oldPos - newPos);
        event->setAccepted(true);
    }

    // 下滚缩小
    if ( event->delta() < 0 ) {
        if (this->scale() > 0.1) {   
            this->setScale(this->scale() * 0.9);
            QPointF newPos = mapToScene(m_wheelPos);
            this->setPosition(position() + oldPos - newPos);
        }
        event->setAccepted(true);
    }
}
