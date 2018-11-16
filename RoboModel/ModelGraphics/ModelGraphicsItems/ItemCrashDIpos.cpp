#include "ItemCrashDIpos.h"
#include"ItemCrashDI.h"
#include"WidgetCrashDiPosSetting.h"
#include"ModelGraphicsScene.h"
#include"../ModelHelper/ModelCustomCommand.h"
ItemCrashDIpos::ItemCrashDIpos(ItemCrashDI *parent, QColor color):
    crashDi_(parent),
    ItemBase(parent->model_scene(),parent),
    radius_(3),
    drawFlag_(false),
    color_(color)
{
    setFlags(ItemIsSelectable| ItemIsMovable|ItemSendsGeometryChanges);

    setAcceptHoverEvents(true);
    setCursor(Qt::CrossCursor);

    color_.setAlpha(100);
}

ItemCrashDIpos::~ItemCrashDIpos()
{

}

qreal ItemCrashDIpos::getRadius()
{
    return radius_;
}

void ItemCrashDIpos::setRadius(qreal r)
{
    radius_ = r;
}

ItemCrashDI *ItemCrashDIpos::getCrashDi()
{
    return crashDi_;
}


QPainterPath ItemCrashDIpos::shape() const
{
    QPainterPath paint_path;
    paint_path.addEllipse(-radius_,-radius_,2*radius_,2*radius_);
    return paint_path;
}

QRectF ItemCrashDIpos::boundingRect() const
{
    return QRectF(-2*radius_,-2*radius_,4*radius_,4*radius_);
}

void ItemCrashDIpos::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(pen_);
    painter->setBrush (color_);
    painter->drawEllipse(-radius_,-radius_,2*radius_,2*radius_);
    QPen pen(Qt::gray);//画编号
    if(drawFlag_)
    {
        QTransform trans(1,0,0,-1,0,0);
        trans.rotate(90);
        painter->setTransform(trans,true);
        painter->setPen(pen);
        int parentId = this->getParent()->data_crashDi_.id_;
        painter->drawText(QPointF(5,5),QString("%1").arg(parentId));
    }
}

void ItemCrashDIpos::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    model_scene()->setCurrentPressItem(this);
    ItemBase::mousePressEvent(event);
    //    if(event->button()==Qt::LeftButton)
    //    {
    //        setNewPos(event->pos());
    //    }
}

void ItemCrashDIpos::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    ItemBase::mouseMoveEvent(event);

}

void ItemCrashDIpos::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    updatePosWidget();//释放后更新属性框点坐标
    ItemBase::mouseReleaseEvent(event);

    //单个拖动undo redo
    if(m_undoStack==NULL||!isPostion_changed)
    {
        return;
    }
    m_new_pos = this->pos();
    moveItemCommand* mv_command = new moveItemCommand(m_ori_pos,m_new_pos,model_scene(),this,NULL);
    m_undoStack->push(mv_command);
    m_ori_pos = this->pos();

    isPostion_changed = false;
}

void ItemCrashDIpos::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    radius_ = 5;
    ItemBase::hoverEnterEvent(event);
    if(!getParent()->isSelected())
    {
        getParent()->setPen(Qt::NoPen);
    }
    getParent()->setToolTip(tr("Hold down then you can drag "));
}

void ItemCrashDIpos::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    radius_=3;
    ItemBase::hoverLeaveEvent (event);
}

void ItemCrashDIpos::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    ItemBase::hoverMoveEvent(event);
    getParent()->setToolTip(tr("Hold down then you can drag "));
}

void ItemCrashDIpos::position_changed()
{
    isPostion_changed = true;
    if(is_ppt_ready (widType::WIDGET_CRASHDIPOS))
    {
        if(getIsBeingDraged())
        {
            qobject_cast<WidgetCrashDiPosSetting *>(widget_setting_)->set_value(scenePos ());
            scene ()->update ();
        }
    }
}

void ItemCrashDIpos::add_setting_widget()
{
    if(widget_setting_==NULL)
    {
        widget_setting_ = new WidgetCrashDiPosSetting(this,scenePos());
    }
}

void ItemCrashDIpos::item_selected(const bool &is_selected)
{
    if (is_selected)
    {
        color_.setAlpha (255);
    }
    else
    {
        color_.setAlpha (100);
    }

    crashDi_->update ();
    model_scene ()->update ();

    ItemBase::item_selected (is_selected);
}

void ItemCrashDIpos::setIndexInLine(int idx)
{
    indexInLine_ = idx;
}

int ItemCrashDIpos::getIndexfromLine()
{
    return indexInLine_;
}

void ItemCrashDIpos::setDrawFlag(bool flag)
{
    drawFlag_ = flag;
}

ItemCrashDI *ItemCrashDIpos::getParent()
{
    return  crashDi_;
}

void ItemCrashDIpos::updatePosWidget()
{
     if(is_ppt_ready (widType::WIDGET_CRASHDIPOS))
    {
        qobject_cast<WidgetCrashDiPosSetting *>(widget_setting_)->set_value (scenePos ());
        scene ()->update ();
    }
}
