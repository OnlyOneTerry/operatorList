#include "ItemPolypos.h"
#include "ItemPolygon.h"
#include "ItemCrashDI.h"
#include "ModelGraphicsView.h"
#include "ModelGraphicsScene.h"
#include "WidgetPosSetting.h"

ItemPolypos::ItemPolypos(ItemPolygon *parent,QColor color):
    polygon_(parent),
    ItemBase(parent->model_scene (),parent),
    radius_(3),
    indexInPolyon_(0),
    color_(color)
{
    setFlags(ItemIsSelectable | ItemIsMovable|ItemSendsGeometryChanges);

    setAcceptHoverEvents (true);
    setCursor(Qt::CrossCursor);
    color_.setAlpha (100);
}


ItemPolypos::~ItemPolypos()
{
}

QPainterPath ItemPolypos::shape() const
{
    QPainterPath paint_path;
    paint_path.addEllipse (-radius_,-radius_,2*radius_,2*radius_);
    return paint_path;
}

QRectF ItemPolypos::boundingRect() const
{
    return QRectF(-2*radius_,-2*radius_,4*radius_,4*radius_);
}

void ItemPolypos::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen (Qt::NoPen);
    painter->setBrush (color_);
    painter->drawEllipse (-radius_,-radius_,2*radius_,2*radius_);
}

void ItemPolypos::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    model_scene()->setCurrentPressItem(this);
    ItemBase::mousePressEvent (event);
    if(event->button()==Qt::LeftButton)
    {
        setNewPos(event->pos());
    }
}

void ItemPolypos::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    ItemBase::mouseMoveEvent (event);
}

void ItemPolypos::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    ItemBase::mouseReleaseEvent (event);

    if(isPostion_changed)//位置有移动
    {
        emit pos_changed(event->pos().x(),event->pos().y());
    }
}

void ItemPolypos::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    radius_=6;
    ItemBase::hoverEnterEvent (event);

}

void ItemPolypos::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    radius_=3;
    ItemBase::hoverLeaveEvent (event);
}

void ItemPolypos::add_setting_widget()
{
    if(widget_setting_==NULL)
    {
        widget_setting_=new WidgetPosSetting(this,scenePos ());
    }
}

void ItemPolypos::position_changed()
{
    isPostion_changed = true;
    setNewPos(this->pos());
    if(is_ppt_ready (WidgetPosSetting::Type))
    {
        if(getIsBeingDraged())
        {
            qobject_cast<WidgetPosSetting *>(widget_setting_)->set_value (scenePos ());
            polygon_->update ();
            scene ()->update ();
        }
    }
}

ItemPolygon *ItemPolypos::getparentPolygon()
{
    return polygon_;
}

int ItemPolypos::getIndexInPolygon()
{
    return indexInPolyon_;
}

void ItemPolypos::setIndexInPolygon(int index)
{
    indexInPolyon_ = index;
}

qreal ItemPolypos::getRadius()
{
    return radius_;
}

void ItemPolypos::setRadius(qreal r)
{
    radius_ = r;
}

void ItemPolypos::set_color(const QColor &color)
{
    color_=color;
}

void ItemPolypos::slot_change_pos(qreal vx, qreal vy)
{
    setPos (QPointF(vx,vy)-parentItem ()->scenePos ());
    polygon_->update ();
    scene()->update ();
}

void ItemPolypos::item_selected (const bool &is_selected)
{
    if (is_selected)
    {
        color_.setAlpha (255);
    }
    else
    {
        color_.setAlpha (100);
    }

    polygon_->update ();
    model_scene ()->update ();

    ItemBase::item_selected (is_selected);
}
