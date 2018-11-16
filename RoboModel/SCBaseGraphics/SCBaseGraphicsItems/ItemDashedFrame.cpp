#include "ItemDashedFrame.h"
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <qstyleoption.h>

ItemDashedFrame::ItemDashedFrame(QRectF rect, QGraphicsItem *parent):
    QGraphicsItem(parent)
{
    setFlags(ItemIsSelectable|ItemIsMovable);

    setCursor(Qt::SizeAllCursor);
    color_brush_.setAlpha(50);
}

ItemDashedFrame::~ItemDashedFrame()
{

}

void ItemDashedFrame::set_rect(const QRectF &rectf)
{
    QPolygonF polyf;
    polyf.append(mapFromScene(rectf.bottomLeft()));
    polyf.append(mapFromScene(rectf.bottomRight()));
    polyf.append(mapFromScene(rectf.topRight()));
    polyf.append(mapFromScene(rectf.topLeft()));
    polyf.append(mapFromScene(rectf.bottomLeft()));

    poly_dash_=polyf;
    update();
}

void ItemDashedFrame::set_pos_list(const QList<QPointF> &pos_list)
{
    pos_list_=pos_list;
    update();
}

QList<QPointF> ItemDashedFrame::get_pos_list()
{
    return pos_list_;
}

void ItemDashedFrame::clear_pos()
{
    pos_list_.clear();
}

QPainterPath ItemDashedFrame::shape() const
{
    QPainterPath path;
    path.addPolygon(poly_dash_);
    return path;
}

void ItemDashedFrame::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(color_brush_);
    painter->drawPolygon(poly_dash_);

    if(pos_list_.size())
    {
        painter->setPen(Qt::magenta);
        painter->setBrush(Qt::NoBrush);
        foreach (auto var_pos, pos_list_)
        {
            painter->drawPoint((var_pos));
        }
    }
}

QRectF ItemDashedFrame::boundingRect() const
{
    return poly_dash_.boundingRect();
}

QVariant ItemDashedFrame::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if(change==ItemSelectedHasChanged)
    {
        if(!value.toBool())
        {
            emit sig_select_release();
        }
    }

    return QGraphicsItem::itemChange(change,value);
}

void ItemDashedFrame::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->buttons()&Qt::RightButton)
    {
        setTransformOriginPoint(poly_dash_.boundingRect().center());
        press_pos_=event->scenePos();
        current_angle_=rotation();
    }
    else
    {
        if(event->modifiers()==Qt::ControlModifier)
        {
            emit sig_copy();
        }
    }
    QGraphicsItem::mousePressEvent(event);
}

void ItemDashedFrame::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->buttons()&Qt::RightButton)
    {
        QPointF c_p(mapToScene(poly_dash_.boundingRect().center()));
        QLineF linef0(c_p,press_pos_);
        QLineF linef(c_p,event->scenePos());
        setRotation(linef.angleTo(linef0)+current_angle_);
    }
    else
    {
        QGraphicsItem::mouseMoveEvent(event);
    }
}

void ItemDashedFrame::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setSelected(true);
    QGraphicsItem::mouseReleaseEvent(event);
;}

void ItemDashedFrame::keyPressEvent(QKeyEvent *event)
{
    QGraphicsItem::keyPressEvent(event);
}
