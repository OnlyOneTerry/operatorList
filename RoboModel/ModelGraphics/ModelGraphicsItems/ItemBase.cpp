#include "ItemBase.h"
#include "ModelGraphicsView.h"
#include "WidgetSetting.h"
#include "WidgetModelSetting.h"
#include "ModelGraphicsScene.h"
#include "ItemAngelCtroller.h"
#include"UndoStack.h"
#include"GlobalVariant.h"
#include"ModelCommon.h"
ItemBase::ItemBase(ModelGraphicsScene *scene,ItemBase *parent):
    QGraphicsItem(parent),
    widget_setting_(NULL),
    model_scene_(scene),
    pen_(Qt::NoPen),
    adjust_type_(NONE),
    item_angel_ctroller_(NULL),
    d_r_(0.),
    layer_(-1),
    m_undoStack(NULL),
    isPostion_changed(false),
    m_id(0),
    isBeingDraged_(false),
    isCreatedByLoading_ (false),
    m_ori_pos(0,0)
{

    setFlag (ItemIsMovable);
    setFlag (ItemIsSelectable);
    setFlag (ItemSendsGeometryChanges);

    setAcceptHoverEvents (true);

    m_undoStack = UndoStack::InitStack();
}

ItemBase::~ItemBase()
{

}

void ItemBase::setID(int newid)
{
    m_id = newid;
}

int ItemBase::getId()
{
    return m_id;
}

QPainterPath ItemBase::shape() const
{
    return painter_path_;
}

QRectF ItemBase::boundingRect() const
{
    return painter_path_.boundingRect ();//.adjusted(-10,-10,20,20);
}

void ItemBase::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->rotate(d_r_);
    painter->setPen (pen_);
    painter->setBrush (brush_);
    painter->drawPath (painter_path_);
    painter->rotate(-d_r_);

    QTransform tran(QMatrix(1, 0, 0, -1, 0, 0.));
    tran.rotate(90);
    painter->setTransform(tran,true);
    painter->setPen(Qt::white);
    painter->setFont( QFont("Times",4));
    painter->drawText(QRectF(-6,-6,12,12),Qt::AlignCenter,text_infor_);
    //    painter->drawText(QRectF(-6,-6,12,12),Qt::AlignCenter,QString("%1").arg(getId()));
    painter->resetMatrix();

}

QVariant ItemBase::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    if (change==QGraphicsItem::ItemSelectedHasChanged/*||change==ItemSelectedChange*/)
    {
        item_selected(value.toBool());
    }
    else if(change==ItemPositionHasChanged/*||change==ItemPositionChange*/)
    {
        if(isSelected ())
        {
            position_changed();
        }
    }
    return QGraphicsItem::itemChange(change, value);
}

void ItemBase::mousePressEvent (QGraphicsSceneMouseEvent *event)
{

    if(event->modifiers()==Qt::AltModifier)
    {
        event->ignore();
        return;
    }
    press_scene_pos_=event->scenePos ();
    QGraphicsItem::mousePressEvent (event);
}

void ItemBase::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    setIsBeingDraged(true);
    QGraphicsItem::mouseMoveEvent(event);
}

void ItemBase::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setIsBeingDraged(false);
    QGraphicsItem::mouseReleaseEvent(event);
}

void ItemBase::item_selected(const bool &is_selected)
{

    if(is_selected)
    {
        //点击选中个数为1
        if(model_scene ()->selectedItems().size ()==1)
        {
            add_setting_widget();
            model_scene_->model_view ()->property_widget ()->update_property_widget (widget_setting_);
        }
        else
        {
            if(model_scene ()->model_view ()->property_widget ()->get_current_property_widget ())
            {
                if(model_scene ()->model_view ()->property_widget ()->get_current_property_widget ()->getType()!=widType::WIDGET_ROBOTMODEL )
                {
                    WidgetModelSetting* tep_WidModelSeting = model_scene()->model_view()->property_widget()->getWidgetModelSetting();
                    model_scene()->model_view()->property_widget()->update_property_widget(tep_WidModelSeting);
                }
            }
        }

    }
    else
    {
        if(model_scene ()->model_view ()->property_widget ()->get_current_property_widget ())
        {
            if(model_scene ()->model_view ()->property_widget ()->get_current_property_widget ()->getType()!=widType::WIDGET_ROBOTMODEL )
            {
                WidgetModelSetting* tep_WidModelSeting = model_scene()->model_view()->property_widget()->getWidgetModelSetting();
                model_scene()->model_view()->property_widget()->update_property_widget(tep_WidModelSeting);
            }
        }
    }

    if(is_selected)
    {
        pen_=QPen(Qt::magenta);
    }
    else
    {
        pen_=Qt::NoPen;
    }
}

WidgetSetting *ItemBase::getWidgetSetting()
{
    return widget_setting_;
}

void ItemBase::slot_widget_setting_deleted()
{
    widget_setting_=NULL;
}

ModelGraphicsScene* ItemBase::model_scene() const
{
    return model_scene_;
}

QPointF ItemBase::get_scene_pos_m() const
{
    return QPointF(qRound(scenePos ().x ()*10)*0.001,qRound(scenePos ().y ()*10)*0.001);
}

void ItemBase::set_d_r(const qreal &d_r)
{
    d_r_=d_r;
}

bool ItemBase::is_ppt_ready(widType type) const
{
    if(model_scene ()->model_view ()->property_widget ()->get_current_property_widget ())
    {
        if(model_scene ()->model_view ()->property_widget ()->get_current_property_widget ()->getType() ==type)
        {
                if(model_scene ()->selectedItems ().size ()==1)
                {
                    return true;
                }
        }
    }

    return false;
}
ItemAngelCtroller *ItemBase::getAngelCtroller()
{
    return  item_angel_ctroller_;
}
/*******************************************************
 * Description: 检测属性窗是否准备好
 *
 * @author
 * @date
 *******************************************************/
bool ItemBase::is_ppt_ready(const int& type) const
{
    if(model_scene ()->model_view ()->property_widget ()->get_current_property_widget ())
    {
        if(model_scene ()->model_view ()->property_widget ()->get_current_property_widget ()->type ()==type)
        {
                if(model_scene ()->selectedItems ().size ()==1)
                {
                    return true;
                }
        }
    }

    return false;
}

quint32 ItemBase::selected_items_size() const
{
    return model_scene ()->selectedItems ().size ();
}

void ItemBase::setOriPos(QPointF pos)
{
    m_ori_pos = pos;
}

QPointF ItemBase::getOriPos()
{
    return m_ori_pos;
}

void ItemBase::setNewPos(QPointF pos)
{
    m_new_pos = pos;
}

QPointF ItemBase::getNewPos()
{
    return m_new_pos;
}

QPen ItemBase::getPen()
{
    return pen_;
}

void ItemBase::setPen(QPen p)
{
    pen_ = p;
}

void ItemBase::setIsBeingDraged(bool flag)
{
    isBeingDraged_ = flag;
}

bool ItemBase::getIsBeingDraged()
{
    return isBeingDraged_;
}

void ItemBase::slot_angel_changed(qreal angel)
{

}

void ItemBase::hoverEnterEvent (QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverEnterEvent (event);
    if(!isSelected ())
    {
        pen_=QPen(Qt::yellow);
    }
    update ();
}
void ItemBase::hoverMoveEvent (QGraphicsSceneHoverEvent *event)
{
    GlobalVariant::getInst()->globalCursorFlag = false;
    QGraphicsItem::hoverMoveEvent (event);
}
void ItemBase::hoverLeaveEvent (QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverLeaveEvent (event);

    if(!isSelected ())
    {
        pen_=Qt::NoPen;
    }
    update ();
}

void ItemBase::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{

}
