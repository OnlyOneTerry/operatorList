#include "ItemLaser.h"
#include "WidgetLaserSetting.h"
#include "ModelGraphicsView.h"
#include "ItemAngelCtroller.h"
#include"./GuidenceWidgets/LaserContentSetting.h"

int ItemLaser::count = 1;//0表示引导创建，1表示非引导创建
ItemLaser::ItemLaser(ModelGraphicsScene *scene,QJsonObject json):
    ItemBase(scene),
    laser_distance_(15.0)
{
    setCursor(Qt::SizeAllCursor);

    brush_=QBrush(QColor(32,178,170));
    item_angel_ctroller_=new ItemAngelCtroller(scene,this);
    if(!json.isEmpty ())
    {
        data_laser_.model_=json["model"].toString ();
        data_laser_.x_=json["x"].toDouble ();
        data_laser_.y_=json["y"].toDouble ();
        data_laser_.z_=json["z"].toDouble ();
        data_laser_.r_=json["r"].toDouble ();
        data_laser_.step_=json["step"].toDouble();
        data_laser_.min_angle_=json["minAngle"].toDouble ();
        data_laser_.max_angle_=json["maxAngle"].toDouble ();
        data_laser_.upside_=json["upside"].toBool ();
        data_laser_.ip_=json["ip"].toString ();
        data_laser_.port_=json["port"].toInt ();
        data_laser_.use_for_localization_=json["useForLocalization"].toBool ();
        m_ori_pos = QPointF(data_laser_.x_*100,data_laser_.y_*100);
        update_pos ();
        update_angle ();
        //加载模型文件创建
        isCreatedByLoading_ = true;
    }
    else
    {
        //向场景中拖拽方式创建
        isCreatedByLoading_ = false;
    }

    while(model_scene()->get_id_list(4).contains(QString("%1").arg(data_laser_.id_)))
    {
        data_laser_.id_++;
    }

    setToolTip(tr("Laser")+QString("_%1").arg(data_laser_.id_));
    setID(data_laser_.id_);
}

ItemLaser::~ItemLaser()
{

}

QPainterPath ItemLaser::shape() const
{
    QPainterPath path;
    path.addEllipse (-15,-15,15*2,15*2);
    return path;
}

QRectF ItemLaser::boundingRect() const
{
    return QRectF(-laser_distance_,-laser_distance_,laser_distance_*2,laser_distance_*2);
}

void ItemLaser::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->rotate(d_r_);
    painter->setPen (pen_);
    painter->setBrush (brush_);
    painter->drawPie (QRect(-15,-15,15*2,15*2),-90*16,180*16);
    painter->rotate(-d_r_);
    painter->setPen(Qt::white);
    painter->setBrush(Qt::NoBrush);
    QTransform tran(QMatrix(1, 0, 0, -1, 0, 0.));
    tran.rotate(90);
    tran.rotate(-data_laser_.r_);
    painter->setTransform(tran,true);
    painter->setPen(Qt::white);
    painter->setFont( QFont("Times",6));//QRectF(-15,-18,30,30)
    //    painter->drawText(QRectF(-14,-18,28,28),Qt::AlignCenter,data_laser_.model_+QString("%1").arg(getId()));
    painter->drawText(QRectF(-15,-22,30,30),Qt::AlignCenter,QString("%1").arg(getId()));
    painter->resetMatrix();
    ItemBase::paint(painter,option,widget);
}

void ItemLaser::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    model_scene()->setCurrentPressItem(this);
    ItemBase::mousePressEvent (event);
    if(count == 1) return;
    laserContentsetting_ = new LaserContentSetting(data_laser_.id_);
    connect(laserContentsetting_,SIGNAL(sig_finished()),this,SLOT(slot_guidconfig_finished()));
    laserContentsetting_->show();
    count++;
}

void ItemLaser::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    ItemBase::mouseMoveEvent (event);
}

void ItemLaser::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
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

void ItemLaser::add_setting_widget()
{
    if(widget_setting_==NULL)
    {
        widget_setting_=new WidgetLaserSetting(this);
    }
}

void ItemLaser::updateWidgetValue()
{
    qobject_cast<WidgetLaserSetting *>(widget_setting_)->update_value ();
}

void ItemLaser::position_changed()
{
    isPostion_changed = true;
    data_laser_.x_=get_scene_pos_m ().x ();
    data_laser_.y_=get_scene_pos_m ().y ();

    if(is_ppt_ready (widType::WIDGET_LASER))
    {
        if(getIsBeingDraged())
        {
            qobject_cast<WidgetLaserSetting *>(widget_setting_)->update_value ();
        }
    }

}

void ItemLaser::item_selected (const bool &is_selected)
{
    if(is_selected)
    {

    }
    else
    {

    }

    ItemBase::item_selected (is_selected);
}

QVariantMap ItemLaser::get_laser_data() const
{
    QVariantMap variant_map_laser_;
    variant_map_laser_.insert ("model",data_laser_.model_);
    variant_map_laser_.insert ("x",data_laser_.x_);
    variant_map_laser_.insert ("y",data_laser_.y_);
    variant_map_laser_.insert ("z",data_laser_.z_);
    variant_map_laser_.insert ("r",data_laser_.r_);
    variant_map_laser_.insert ("step",data_laser_.step_);
    variant_map_laser_.insert ("minAngle",data_laser_.min_angle_);
    variant_map_laser_.insert ("maxAngle",data_laser_.max_angle_);
    variant_map_laser_.insert ("upside",data_laser_.upside_);
    variant_map_laser_.insert ("ip",data_laser_.ip_);
    variant_map_laser_.insert ("port",data_laser_.port_);
    variant_map_laser_.insert ("useForLocalization",data_laser_.use_for_localization_);
    return variant_map_laser_;
}

void ItemLaser::update_pos()
{
    setPos (data_laser_.x_*100,data_laser_.y_*100);
}

void ItemLaser::update_angle()
{
    //旋转箭头
    getAngelCtroller()->setPos(15*qCos(data_laser_.r_*M_PI/180.),15*qSin(data_laser_.r_*M_PI/180.));
    getAngelCtroller()->setRotation(data_laser_.r_);
    set_d_r(data_laser_.r_);
}

void ItemLaser::update_data()
{
    data_laser_.x_=get_scene_pos_m ().x ();
    data_laser_.y_=get_scene_pos_m ().y ();
    data_laser_.r_=rotation ();
}

void ItemLaser::slot_angel_changed(qreal angel)
{
    set_d_r(angel);//重绘di

    if(is_ppt_ready (widType::WIDGET_LASER))
    {
        widget_setting_->set_r(angel);
    }
}

void ItemLaser::slot_guidconfig_finished()
{
    this->data_laser_.ip_ =  laserContentsetting_->laserData_.ip_;
    this->data_laser_.upside_ = laserContentsetting_->laserData_.upside_;
    this->data_laser_.max_angle_ = laserContentsetting_->laserData_.max_angle_;
    this->data_laser_.min_angle_  = laserContentsetting_->laserData_.min_angle_;
    this->data_laser_.port_ = laserContentsetting_->laserData_.port_;
    this->data_laser_.step_ = laserContentsetting_->laserData_.step_;
    this->data_laser_.model_ = laserContentsetting_->laserData_.model_;
    this->data_laser_.use_for_localization_ = laserContentsetting_->laserData_.use_for_localization_;
    add_setting_widget();
    WidgetLaserSetting* temp  = dynamic_cast<WidgetLaserSetting*>(widget_setting_);
    temp->set_forLocation_byGuidConfigure(this->data_laser_.use_for_localization_);
    temp->set_upside_byGuidConfigure(this->data_laser_.upside_);
    temp->set_Ip_byGuidConfigure(this->data_laser_.ip_);
    temp->set_maxAngle_byGuidConfigure(this->data_laser_.max_angle_);
    temp->set_minAngle_byGuidConfigure(this->data_laser_.min_angle_);
    temp->set_port_byGuidConfigure(this->data_laser_.port_);
    temp->set_step_byGuidConfigure(this->data_laser_.step_);
    temp->set_model_byGuidConfigure(this->data_laser_.model_);

    laserContentsetting_->close();
}
