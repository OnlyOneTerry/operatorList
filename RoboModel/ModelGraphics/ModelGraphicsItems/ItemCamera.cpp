#include "ItemCamera.h"
#include"ModelCustomCommand.h"
ItemCamera::ItemCamera(ModelGraphicsScene *scene, QJsonObject json)
    :ItemBase(scene)
{
    setFlag (ItemIsMovable);
    setFlag (ItemIsSelectable);
    setFlag (ItemSendsGeometryChanges);

    if(!json.isEmpty())
    {
        data_camera_.type_ =json["type"].toInt ();
        data_camera_.x_=json["x"].toDouble ();
        data_camera_.y_=json["y"].toDouble ();
        data_camera_.z_=json["z"].toDouble ();
        data_camera_.pitch_ = json["pitch"].toInt();
        data_camera_.roll_ = json["roll"].toInt();
        data_camera_.yaw_ = json["yaw"].toInt();
        m_ori_pos = QPointF(data_camera_.x_*100,data_camera_.y_*100);
        isCreatedByLoading_ = true;
    }
    else
    {
        isCreatedByLoading_ = false;
    }
    setToolTip(tr("Camera"));
}

ItemCamera::~ItemCamera()
{

}

void ItemCamera::add_setting_widget()
{
    if(widget_setting_==NULL)
    {
        widget_setting_ = new WidgetCameraSetting(this);
    }
}

void ItemCamera::updateWidgetValue()
{
    if(widget_setting_)
    {
        dynamic_cast<WidgetCameraSetting*>(widget_setting_)->update_value();
    }
}
//属性窗控制item
void ItemCamera::update_item()
{
    this->setPos(data_camera_.x_*100,data_camera_.y_*100);
    update();
}

void ItemCamera::update_data()
{
    data_camera_.x_ = get_scene_pos_m().x();
    data_camera_.y_ = get_scene_pos_m().y();
}

QPainterPath ItemCamera::shape() const
{
    QPainterPath path;
    path.addRect(0,0,14,25);
    return path;
}

QRectF ItemCamera::boundingRect() const
{
    return shape().boundingRect();
}

void ItemCamera::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(pen_);
    QBrush brush(QColor(114,153,253,255));
    painter->setBrush(brush);
    painter->drawRect(boundingRect());
    painter->setPen(Qt::green);
    painter->drawEllipse(5,15,8,8);
    painter->drawEllipse(7,17,4,4);
}
void ItemCamera::position_changed()
{
    isPostion_changed = true;
    update_data();
    if(widget_setting_)
    {
        dynamic_cast<WidgetCameraSetting*>(widget_setting_)->update_value();
    }
}

void ItemCamera::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    ItemBase::mousePressEvent(event);
}

void ItemCamera::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    ItemBase::mouseMoveEvent(event);
}

void ItemCamera::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    ItemBase::mouseReleaseEvent(event);

    if(!isPostion_changed)
    {
        return;
    }
    if(m_undoStack == NULL) m_undoStack = UndoStack::InitStack();
    //同时拖动多个item undo redo
    //框选多个
    if(model_scene()->get_selected_items()->size()>1)
    {

        QList<QGraphicsItem*> *templist = model_scene()->get_selected_items();
        model_scene()->m_newItemMap.clear();
        foreach (auto var, *templist) {
            model_scene()->m_newItemMap.insert(dynamic_cast<ItemBase*>(var),var->pos());
        }

        moveItemsCommand* itemsCmd = new moveItemsCommand(model_scene(),model_scene()->m_oriItemMap,model_scene()->m_newItemMap,NULL);
        m_undoStack->push(itemsCmd);

        model_scene()->m_oriItemMap = model_scene()->m_newItemMap;

        return;
    }

    //ctrl多选
    if(model_scene()->selectedItems().size()>1)
    {
        QList<QGraphicsItem*> templist = model_scene()->selectedItems();
        model_scene()->m_newItemMap.clear();
        foreach (auto var, templist) {
            model_scene()->m_newItemMap.insert(dynamic_cast<ItemBase*>(var),var->pos());
        }

        moveItemsCommand* itemsCmd = new moveItemsCommand(model_scene(),model_scene()->m_oriItemMap,model_scene()->m_newItemMap,NULL);
        m_undoStack->push(itemsCmd);

        model_scene()->m_oriItemMap = model_scene()->m_newItemMap;
        return;
    }

    //单个拖动undo redo
    m_new_pos = this->pos();
    moveItemCommand* mv_command = new moveItemCommand(m_ori_pos,m_new_pos,model_scene(),this,NULL);
    m_undoStack->push(mv_command);
    m_ori_pos = this->pos();
}
