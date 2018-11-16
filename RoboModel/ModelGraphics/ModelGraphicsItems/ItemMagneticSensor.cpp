#include "ItemMagneticSensor.h"
#include "WidgetMagneticSensorSetting.h"
#include "ItemAngelCtroller.h"
#include "ModelGraphicsScene.h"
#include "ModelCustomCommand.h"

#include"./GuidenceWidgets/MagSensorContentSetting.h"

int ItemMagneticSensor::count = 1;//0表示引导创建，1表示非引导创建
ItemMagneticSensor::ItemMagneticSensor(ModelGraphicsScene* scene,QJsonObject json):ItemBase(scene)
{
    setCursor(Qt::SizeAllCursor);
    brush_=QBrush(QColor(140,137,137));
    painter_path_.addRect (-k_radius_size_/2,-k_radius_size_,k_radius_size_,k_radius_size_*2);

    item_angel_ctroller_=new ItemAngelCtroller(scene,this);
    if(!json.isEmpty ())
    {
        data_magnetic_.id_=json["id"].toInt ();
        data_magnetic_.x_=json["x"].toDouble ();
        data_magnetic_.y_=json["y"].toDouble ();
        data_magnetic_.z_=json["z"].toDouble ();
        data_magnetic_.r_=json["r"].toDouble ();
        data_magnetic_.resolution_=json["resolution"].toDouble ();
        data_magnetic_.step_=json["step"].toDouble ();
        m_ori_pos = QPointF(data_magnetic_.x_*100,data_magnetic_.y_*100);
        update_item ();
        //加载模型文件创建
        isCreatedByLoading_ = true;
    }
    else
    {
        while(model_scene()->get_id_list(3).contains(QString("%1").arg(data_magnetic_.id_)))
        {
            data_magnetic_.id_++;
        }

        //向场景中拖拽方式创建
        isCreatedByLoading_ = false;
    }
    setToolTip(QString("MagneticSensor_%1").arg(data_magnetic_.id_));
    setID(data_magnetic_.id_);
    update_item ();
}

ItemMagneticSensor::~ItemMagneticSensor()
{

}

void ItemMagneticSensor::add_setting_widget()
{
    if(widget_setting_==NULL)
    {
        widget_setting_=new WidgetMagneticSensorSetting(this);
    }
}

void ItemMagneticSensor::updateWidgetValue()
{
    qobject_cast<WidgetMagneticSensorSetting *>(widget_setting_)->update_value ();
}

void ItemMagneticSensor::position_changed()
{
    isPostion_changed = true;
    data_magnetic_.x_=get_scene_pos_m ().x ();
    data_magnetic_.y_=get_scene_pos_m ().y ();
    if(is_ppt_ready (widType::WIDGET_MAGENETIC))
    {
        if(getIsBeingDraged())
        {
            qobject_cast<WidgetMagneticSensorSetting *>(widget_setting_)->update_value ();
        }
    }
}

QVariantMap ItemMagneticSensor::get_magnetic_data() const
{
    QVariantMap variant_map_magnetic;

    variant_map_magnetic.insert ("id",data_magnetic_.id_);
    variant_map_magnetic.insert ("x",data_magnetic_.x_);
    variant_map_magnetic.insert ("y",data_magnetic_.y_);
    variant_map_magnetic.insert ("z",data_magnetic_.z_);
    variant_map_magnetic.insert ("r",data_magnetic_.r_);
    variant_map_magnetic.insert ("resolution",data_magnetic_.resolution_);
    variant_map_magnetic.insert ("step",data_magnetic_.step_);

    return variant_map_magnetic;
}

void ItemMagneticSensor::update_data()
{
    data_magnetic_.x_=get_scene_pos_m ().x ();
    data_magnetic_.y_=get_scene_pos_m ().y ();
    data_magnetic_.r_=rotation ();
}

void ItemMagneticSensor::update_item()
{

    setPos (data_magnetic_.x_*100,data_magnetic_.y_*100);

    //旋转角标
    qreal k_radius  = item_angel_ctroller_->getRoationRadius();
    item_angel_ctroller_->setPos(k_radius*qCos(data_magnetic_.r_*M_PI/180.),k_radius*qSin(data_magnetic_.r_*M_PI/180.));
    item_angel_ctroller_->setRotation(data_magnetic_.r_);
    set_d_r(data_magnetic_.r_);
    text_infor_=QString("%1").arg(data_magnetic_.id_);
    setID(data_magnetic_.id_);
    update();
}

void ItemMagneticSensor::slot_angel_changed(qreal angel)
{
    set_d_r(angel);

    if(is_ppt_ready (widType::WIDGET_MAGENETIC))
    {
            widget_setting_->set_r(angel);
    }
}

void ItemMagneticSensor::slot_guidconfig_finished()
{
    this->data_magnetic_.step_ = magSensorSetting_->magneticData_.step_;
    this->data_magnetic_.resolution_ = magSensorSetting_->magneticData_.resolution_;
    add_setting_widget();
    WidgetMagneticSensorSetting* temp = dynamic_cast<WidgetMagneticSensorSetting*>(widget_setting_);
    temp->set_step_byGuidConfigure(this->data_magnetic_.step_);
    temp->set_resolution_byGuidConfigure(this->data_magnetic_.resolution_);
    temp->set_Type_byGuidConfigure(magSensorSetting_->magneticSensorType_);
    magSensorSetting_->close();
}

void ItemMagneticSensor::mousePressEvent (QGraphicsSceneMouseEvent *event)
{
    model_scene()->setCurrentPressItem(this);
    ItemBase::mousePressEvent (event);
    if(event->button()==Qt::LeftButton)
    {
        setNewPos(event->pos());
    }
    if(count == 1) return;
    magSensorSetting_ = new MagSensorContentSetting;
    connect(magSensorSetting_,SIGNAL(sig_finished()),this,SLOT(slot_guidconfig_finished()));
    magSensorSetting_->show();
    count++;
}

void ItemMagneticSensor::mouseMoveEvent (QGraphicsSceneMouseEvent *event)
{
    ItemBase::mouseMoveEvent (event);
}

void ItemMagneticSensor::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    ItemBase::mouseReleaseEvent(event);
    //同时拖动多个item undo redo
    if(!isPostion_changed)
    {
        return;
    }
    if(m_undoStack==NULL) m_undoStack = UndoStack::InitStack();
    //框选多个
    if(model_scene()->get_selected_items()->size()>1)
    {
        if(m_undoStack == NULL) m_undoStack = UndoStack::InitStack();

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
