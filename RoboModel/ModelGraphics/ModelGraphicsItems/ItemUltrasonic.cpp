
#include "ItemUltrasonic.h"
#include "WidgetUltrasonicSetting.h"
#include "ItemAngelCtroller.h"
#include "ModelGraphicsScene.h"
#include"ItemScanArea.h"
#include"./GuidenceWidgets/UltrasonicContentSetting.h"
int ItemUltrasonic::count = 1;//0表示引导创建，1表示非引导创建
ItemUltrasonic::ItemUltrasonic(ModelGraphicsScene* scene,QJsonObject json):ItemBase(scene)
{
    setCursor(Qt::SizeAllCursor);
    brush_=QBrush(Qt::darkYellow);
    painter_path_.addEllipse (-k_radius_size_,-k_radius_size_,k_radius_size_*2,k_radius_size_*2);
    scanArea_ = new ItemScanArea(scene,this);

    scanArea_->setPos(6,0);//调整扫描区域的相对于设备的位置
    scanArea_->set_rotateRadius(6);

    item_angel_ctroller_=new ItemAngelCtroller(scene,this);
    if(!json.isEmpty ())
    {
        data_ultrasonic_.id_=json["id"].toInt ();
        data_ultrasonic_.func_=json["func"].toInt();
        data_ultrasonic_.x_=json["x"].toDouble ();
        data_ultrasonic_.y_=json["y"].toDouble ();
        data_ultrasonic_.z_=json["z"].toDouble ();
        data_ultrasonic_.r_=json["r"].toDouble ();
        data_ultrasonic_.min_dist_=json["minDist"].toDouble ();
        data_ultrasonic_.max_dist_=json["maxDist"].toDouble ();
        data_ultrasonic_.range_=json["range"].toDouble ();
        data_ultrasonic_.max_speed_=json["maxSpeed"].toDouble();
        m_ori_pos = QPointF(data_ultrasonic_.x_*100,data_ultrasonic_.y_*100);
        scanArea_->setRotation(90+data_ultrasonic_.r_);
        scanArea_->setRange(data_ultrasonic_.range_);
        scanArea_->setPos(6*qCos(data_ultrasonic_.r_*M_PI/180.),6*qSin(data_ultrasonic_.r_*M_PI/180.));
        item_angel_ctroller_->set_rotation(data_ultrasonic_.r_);
        update_item ();
        //加载模型文件创建
        isCreatedByLoading_ = true;
    }
    else
    {
        scanArea_->setRotation(90);
        while(model_scene()->get_id_list(1).contains(QString("%1").arg(data_ultrasonic_.id_)))
        {
            data_ultrasonic_.id_++;
        }
        //向场景中拖拽方式创建
        isCreatedByLoading_ = false;
    }
    scanArea_->setID(data_ultrasonic_.id_);
    scanArea_->setToolTip(QString("UltrasonicScanArea_%1").arg(data_ultrasonic_.id_));
    setToolTip(QString("ItemUltrasonic_%1").arg(data_ultrasonic_.id_));
    setID(data_ultrasonic_.id_);
    update_item();
    update();
}

ItemUltrasonic::~ItemUltrasonic()
{
}

void ItemUltrasonic::add_setting_widget()
{
    if(widget_setting_==NULL)
    {
        widget_setting_=new WidgetUltrasonicSetting(this);
    }
}

void ItemUltrasonic::updateWidgetValue()
{
    qobject_cast<WidgetUltrasonicSetting *>(widget_setting_)->update_value ();
}

void ItemUltrasonic::position_changed()
{
    isPostion_changed = true;
    data_ultrasonic_.x_=get_scene_pos_m ().x ();
    data_ultrasonic_.y_=get_scene_pos_m ().y ();

    if(is_ppt_ready (widType::WIDGET_ULTRASIONIC))
    {
        if(getIsBeingDraged())
        {
            qobject_cast<WidgetUltrasonicSetting *>(widget_setting_)->update_value ();
        }
    }

}

QVariantMap ItemUltrasonic::get_ultrasonic_data()
{
    QVariantMap variant_map_ultrasonic;

    variant_map_ultrasonic.insert ("id",data_ultrasonic_.id_);
    variant_map_ultrasonic.insert ("func",data_ultrasonic_.func_);
    variant_map_ultrasonic.insert ("x",data_ultrasonic_.x_);
    variant_map_ultrasonic.insert ("y",data_ultrasonic_.y_);
    variant_map_ultrasonic.insert ("z",data_ultrasonic_.z_);
    variant_map_ultrasonic.insert ("r",data_ultrasonic_.r_);
    variant_map_ultrasonic.insert ("minDist",data_ultrasonic_.min_dist_);
    variant_map_ultrasonic.insert ("maxDist",data_ultrasonic_.max_dist_);
    variant_map_ultrasonic.insert ("range",data_ultrasonic_.range_);
    if(data_ultrasonic_.func_==1)
    {
        variant_map_ultrasonic.insert("maxSpeed",data_ultrasonic_.max_speed_);
    }

    return variant_map_ultrasonic;
}

ItemScanArea *ItemUltrasonic::getScanArea()
{
    return scanArea_;
}

void ItemUltrasonic::update_item()
{
    setPos (data_ultrasonic_.x_*100,data_ultrasonic_.y_*100);
    text_infor_=QString::number(data_ultrasonic_.id_);
    setID(data_ultrasonic_.id_);
    scanArea_->setID(data_ultrasonic_.id_);//扇形区域的id与父item 的id一致
    update();
}

void ItemUltrasonic::update_data()
{
    data_ultrasonic_.x_=get_scene_pos_m ().x ();
    data_ultrasonic_.y_=get_scene_pos_m ().y ();
}

void ItemUltrasonic::slot_angel_changed(qreal angel)
{
    this->set_d_r(angel);//重绘当前item
    scanArea_->setRotation(90+angel);//旋转扫描区域
    qreal k_radius = scanArea_->get_rotateRadius();
    scanArea_->setPos(k_radius*qCos(angel*M_PI/180.),k_radius*qSin(angel*M_PI/180.));//调整旋转区域位置

    if(is_ppt_ready (widType::WIDGET_ULTRASIONIC))
    {
        widget_setting_->set_r(angel);
    }
}

void ItemUltrasonic::slot_guidconfig_finished()
{
    this->data_ultrasonic_.func_ = ultrasoinSetting_->ultrasonicData_.func_;
    this->data_ultrasonic_.max_dist_ = ultrasoinSetting_->ultrasonicData_.max_dist_;
    this->data_ultrasonic_.min_dist_ = ultrasoinSetting_->ultrasonicData_.min_dist_;
    this->data_ultrasonic_.range_ = ultrasoinSetting_->ultrasonicData_.range_;
    this->scanArea_->setRange(this->data_ultrasonic_.range_);
    add_setting_widget();
    WidgetUltrasonicSetting* temp = dynamic_cast<WidgetUltrasonicSetting*>(widget_setting_);

    temp->set_func_byGuidConfigure(this->data_ultrasonic_.func_);
    temp->set_maxDist_byGuidConfigure(this->data_ultrasonic_.max_dist_);
    temp->set_minDist_byGuidConfigure(this->data_ultrasonic_.min_dist_);
    temp->set_range_byGuidConfigure(this->data_ultrasonic_.range_);
    temp->set_Type_byGuidConfigure(ultrasoinSetting_->ultrasonicType_);

    ultrasoinSetting_->close();
}

void ItemUltrasonic::mousePressEvent (QGraphicsSceneMouseEvent *event)
{
    model_scene()->setCurrentPressItem(this);

    ItemBase::mousePressEvent (event);
    if(event->button()==Qt::LeftButton)
    {
        setNewPos(event->pos());
    }
    if(count == 1) return;
    ultrasoinSetting_ = new  UltrasonicContentSetting;
    connect(ultrasoinSetting_,SIGNAL(sig_finished()),this,SLOT(slot_guidconfig_finished()));
    ultrasoinSetting_->setModal(true);
    ultrasoinSetting_->show();
    count++;
}
void ItemUltrasonic::mouseMoveEvent (QGraphicsSceneMouseEvent *event)
{

    ItemBase::mouseMoveEvent (event);
}

void ItemUltrasonic::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
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
        QList<QGraphicsItem*>* templist = model_scene()->get_selected_items();
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
