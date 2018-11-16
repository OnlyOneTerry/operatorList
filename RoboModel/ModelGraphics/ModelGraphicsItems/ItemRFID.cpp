#include "ItemRFID.h"
#include "WidgetRFIDsetting.h"
#include "ModelGraphicsScene.h"
#include "ModelCustomCommand.h"
#include "ItemAngelCtroller.h"
ItemRFIDSensor::ItemRFIDSensor(ModelGraphicsScene* scene,QJsonObject json):ItemBase(scene)
{
    setCursor(Qt::SizeAllCursor);
    brush_=QBrush(QColor(140,137,137));
    painter_path_.addRect (-k_radius_size_-2,-k_radius_size_-2,k_radius_size_*2+4,k_radius_size_*2+4);
    if(!json.isEmpty ())
    {
        data_RFID_.x_=json["x"].toDouble ();
        data_RFID_.y_=json["y"].toDouble ();
        data_RFID_.z_=json["z"].toDouble ();
        m_ori_pos = QPointF(data_RFID_.x_*100,data_RFID_.y_*100);
        update_item ();
        //加载模型文件创建
        isCreatedByLoading_ = true;
    }
    else
    {
        //向场景中拖拽方式创建
        isCreatedByLoading_ = false;
    }
    update_item ();
    update();
}

ItemRFIDSensor::~ItemRFIDSensor()
{
}

void ItemRFIDSensor::add_setting_widget()
{
    if(widget_setting_==NULL)
    {
        widget_setting_=new WidgetRFIDsetting(this);
    }
}

void ItemRFIDSensor::updateWidgetValue()
{
    qobject_cast<WidgetRFIDsetting *>(widget_setting_)->update_value ();
}

void ItemRFIDSensor::position_changed()
{
    isPostion_changed = true;
    data_RFID_.x_=get_scene_pos_m ().x ();
    data_RFID_.y_=get_scene_pos_m ().y ();
    if(is_ppt_ready (widType::WIDGET_RFID))
    {
        if(getIsBeingDraged())
        {
            qobject_cast<WidgetRFIDsetting *>(widget_setting_)->update_value ();
        }
    }
}

QVariantMap ItemRFIDSensor::get_RFID_data() const
{
    QVariantMap variant_map_RFID;

    variant_map_RFID.insert ("x",data_RFID_.x_);
    variant_map_RFID.insert ("y",data_RFID_.y_);
    variant_map_RFID.insert ("z",data_RFID_.z_);

    return variant_map_RFID;
}

void ItemRFIDSensor::update_data()
{
    data_RFID_.x_=get_scene_pos_m ().x ();
    data_RFID_.y_=get_scene_pos_m ().y ();
}

void ItemRFIDSensor::update_item()
{
    setPos(data_RFID_.x_*100,data_RFID_.y_*100);
}

void ItemRFIDSensor::mousePressEvent (QGraphicsSceneMouseEvent *event)
{
    model_scene()->setCurrentPressItem(this);
    ItemBase::mousePressEvent (event);
    if(event->button()==Qt::LeftButton)
    {
        setNewPos(event->pos());
    }
}

void ItemRFIDSensor::mouseMoveEvent (QGraphicsSceneMouseEvent *event)
{
    ItemBase::mouseMoveEvent (event);

}

void ItemRFIDSensor::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    ItemBase::mouseReleaseEvent(event);
    //同时拖动多个item undo redo
    if(!isPostion_changed)
    {
        return;
    }

    if(m_undoStack == NULL) m_undoStack = UndoStack::InitStack();
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
    moveItemCommand* mv_command = new moveItemCommand(m_ori_pos,m_new_pos,model_scene(), this,NULL);
    m_undoStack->push(mv_command);
    m_ori_pos = this->pos();

}

void ItemRFIDSensor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
    painter->drawText(QRectF(-8,-8,16,16),Qt::AlignCenter,"RFID");
    painter->resetMatrix();

}
