#include "BaseControlPosGraphicsitem.h"
#include <QPainter>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>

BaseControlPosGraphicsitem::BaseControlPosGraphicsitem(QGraphicsItem *parent_item):
    BaseDrawingFixGraphicsItem(parent_item)
{
    setCursor(QCursor(Qt::CrossCursor));
    radius_=3.;
    setVisible(false);
}

QRectF BaseControlPosGraphicsitem::boundingRect() const
{
    return QRectF(-radius_,-radius_,radius_*2,radius_*2);
}

QPainterPath BaseControlPosGraphicsitem::shape() const
{
    QPainterPath path;
    path.addRoundRect(QRectF(-radius_,-radius_,radius_*2,radius_*2),50,50);
    return path;
}

void BaseControlPosGraphicsitem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(sucolor(color_pen_,100));

    QRadialGradient radialGrad(0,0,8,0,0);
    radialGrad.setColorAt( 0.0, QColor(101,224,255) );
    radialGrad.setColorAt( 1.0, QColor(Qt::blue));
    QBrush b(radialGrad);
    b.setStyle(Qt::RadialGradientPattern);
    painter->setBrush(b);

    painter->drawRoundRect(QRectF(-radius_,-radius_,radius_*2,radius_*2),50,50);

    if(is_hovered_)
    {
        painter->setPen(Qt::cyan);
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(QRectF(-radius_,-radius_,radius_*2,radius_*2).adjusted(-5,-5,5,5));
    }
}

void BaseControlPosGraphicsitem::mousePressEvent(QGraphicsSceneMouseEvent *event){}
void BaseControlPosGraphicsitem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    setPos(parentItem()->mapFromScene(event->scenePos()));
}
void BaseControlPosGraphicsitem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){}
