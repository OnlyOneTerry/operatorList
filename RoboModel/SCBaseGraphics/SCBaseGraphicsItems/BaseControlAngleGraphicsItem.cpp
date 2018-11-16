#include "BaseControlAngleGraphicsItem.h"
#include <QPolygonF>
#include <QPainter>
#include <QCursor>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QtMath>

BaseControlAngleGraphicsItem::BaseControlAngleGraphicsItem(QGraphicsItem *parent_item):
    BaseDrawingFixGraphicsItem(parent_item)
{
    setCursor(QCursor(Qt::CrossCursor));
}

QRectF BaseControlAngleGraphicsItem::boundingRect() const
{
    return QRectF(polyf_.boundingRect());
}

QPainterPath BaseControlAngleGraphicsItem::shape() const
{
    QPainterPath path;
    path.addPolygon(polyf_);
    return path;
}

void BaseControlAngleGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    painter->setPen(Qt::NoPen);
    painter->setBrush(is_hovered_?Qt::green:Qt::magenta);
    painter->drawPolygon(polyf_);
    //    painter->drawEllipse(QRectF(-1,-1,2,2));
}

void BaseControlAngleGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    press_pos_=event->pos();
}

void BaseControlAngleGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QLineF line(parentItem()->scenePos(),event->scenePos());
    setRotation(line.angle());
    qreal angle_h=line.angle()/180.*M_PI;
    setPos(radius_*qCos(angle_h),radius_*qSin(angle_h));
}

void BaseControlAngleGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

}

void BaseControlAngleGraphicsItem::initPolyF()
{
    polyf_.clear();
    polyf_.append(QPointF(0,-4));
    polyf_.append(QPointF(4,0));
    polyf_.append(QPointF(0,4));
    setPos(radius_,0);
}
