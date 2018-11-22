#include "ItemDI.h"
#include "WidgetDISetting.h"
#include "ItemAngelCtroller.h"
#include "ModelGraphicsScene.h"
#include"ModelCustomCommand.h"
#include<QtMath>
#include"ItemScanArea.h"
#include"./GuidenceWidgets/DIContentSetting.h"
int ItemDI::count = 1;//0表示引导创建，1表示非引导创建
ItemDI::ItemDI(ModelGraphicsScene *scene,QJsonObject json):ItemBase(scene)
{
    setCursor(Qt::SizeAllCursor);
    brush_=QBrush(QColor(110,122,234));
    painter_path_.addRect(-k_radius_size_,-k_radius_size_,k_radius_size_*2,k_radius_size_*2);


    scanArea_ = new ItemScanArea(scene,this);

    scanArea_->set_rotateRadius(6);
    scanArea_->setPos(scanArea_->get_rotateRadius(),0);//将扫描区域item移至（6,0）位置，减少与DI重叠
    item_angel_ctroller_=new ItemAngelCtroller(scene,this);

    if(!json.isEmpty ())
    {
        data_di_.id_=json["id"].toInt ();
        data_di_.func_=json["func"].toInt ();

        int type = json["type"].toInt();
        data_di_.type_ = type;
        data_di_.x_=json["x"].toDouble ();
        data_di_.y_=json["y"].toDouble ();
        data_di_.z_=json["z"].toDouble ();
        data_di_.r_=json["r"].toDouble ();
        data_di_.rang_ = json["range"].toDouble();
        data_di_.inverse_=json["inverse"].toBool ();
        data_di_.max_speed_=json["maxSpeed"].toDouble();
        data_di_.minDist_ = json["minDist"].toDouble();
        data_di_.maxDist_ = json["maxDist"].toDouble();
        m_ori_pos = QPointF(data_di_.x_*100,data_di_.y_*100);
        scanArea_->setRotation(90+data_di_.r_);
        scanArea_->setRange(data_di_.rang_);
        scanArea_->setPos(scanArea_->get_rotateRadius()*qCos(data_di_.r_*M_PI/180.),scanArea_->get_rotateRadius()*qSin(data_di_.r_*M_PI/180.));
        item_angel_ctroller_->set_rotation(data_di_.r_);
        scanArea_->update();
        //加载模型文件创建
        isCreatedByLoading_ = true;
    }
    else
    {
        scanArea_->setRotation(90);
        while(model_scene()->get_id_list(0).contains(QString("%1").arg(data_di_.id_)))
        {
            data_di_.id_++;
        }
        //向场景中拖拽方式创建
        isCreatedByLoading_ = false;
    }
    scanArea_->setID(data_di_.id_);
    scanArea_->setToolTip(QString("DIScanArea_%1").arg(data_di_.id_));
    setToolTip(QString("ItemDI_%1").arg(data_di_.id_));
    update_item();
    update();



}

ItemDI::~ItemDI()
{

}

void ItemDI::add_setting_widget()
{
    if(widget_setting_==NULL)
    {
        widget_setting_=new WidgetDISetting(this);
    }
}

ItemScanArea *ItemDI::getScanArea()
{
    return scanArea_;
}

void ItemDI::updateWidgetValue()
{
    qobject_cast<WidgetDISetting *>(widget_setting_)->update_value ();
}

void ItemDI::position_changed()
{
    isPostion_changed = true;
    data_di_.x_=get_scene_pos_m ().x ();
    data_di_.y_=get_scene_pos_m ().y ();

    if(is_ppt_ready (widType::WIDGET_DI))
    {
        if(getIsBeingDraged())
        {
            qobject_cast<WidgetDISetting *>(widget_setting_)->update_value ();
        }
    }
}


void ItemDI::mousePressEvent (QGraphicsSceneMouseEvent *event)
{
    qDebug()<<"itemdi press.....";
    item_angel_ctroller_->setZValue(this->zValue()+1);
    scanArea_->setZValue(this->zValue());


    model_scene()->setCurrentPressItem(this);
    ItemBase::mousePressEvent (event);
    if(event->button()==Qt::LeftButton)
    {
        setNewPos(event->pos());
    }
    if(count==1) return;
    diContentSetting_= new DIContentSetting(I_di,data_di_.id_);
    //引导完成后更新当前item
    connect(diContentSetting_,SIGNAL(sig_finished()),this,SLOT(slot_guidconfig_finished()));
    diContentSetting_->show();
    count++;
}

void ItemDI::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    ItemBase::mouseMoveEvent (event);
}

void ItemDI::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
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
    moveItemCommand* mv_command = new moveItemCommand(m_ori_pos,m_new_pos,model_scene(),this,NULL);
    m_undoStack->push(mv_command);
    m_ori_pos = this->pos();

}

void ItemDI::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
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
    painter->resetMatrix();
}

QVariantMap ItemDI::get_di_data() const
{
    QVariantMap variant_map_di;
    variant_map_di.insert ("id",data_di_.id_);
    variant_map_di.insert ("func",data_di_.func_);
    quint8 type = data_di_.type_;
//    if(type>2)
//    {
//        type++;
//    }
    variant_map_di.insert ("type",type);
    variant_map_di.insert ("x",data_di_.x_);
    variant_map_di.insert ("y",data_di_.y_);
    variant_map_di.insert ("z",data_di_.z_);
    variant_map_di.insert ("r",data_di_.r_);
    variant_map_di.insert("range",data_di_.rang_);
    variant_map_di.insert ("inverse",data_di_.inverse_);
    variant_map_di.insert("minDist",data_di_.minDist_);
    variant_map_di.insert("maxDist",data_di_.maxDist_);

    if(data_di_.func_==1)
    {
        variant_map_di.insert("maxSpeed",data_di_.max_speed_);
    }

    return variant_map_di;
}

void ItemDI::update_data()
{
    data_di_.x_=get_scene_pos_m ().x ();
    data_di_.y_=get_scene_pos_m ().y ();
}

void ItemDI::update_item()
{
    setPos (data_di_.x_*100,data_di_.y_*100);//更新坐标
    set_d_r(data_di_.r_);//更新角度
    text_infor_=QString::number(data_di_.id_);
    setID(data_di_.id_);
    scanArea_->setID(data_di_.id_);//扇形区域的id与父item 的id一致
    this->update();
}

void ItemDI::slot_angel_changed(qreal angel)
{
    set_d_r(angel);//重绘DI
    scanArea_->setRotation(90+angel);//旋转扫描区域Item
    qreal k_radius = scanArea_->get_rotateRadius();
    scanArea_->setPos(k_radius*qCos(angel*M_PI/180.),k_radius*qSin(angel*M_PI/180.));
    if(is_ppt_ready (widType::WIDGET_DI))
    {
        widget_setting_->set_r(angel);
    }
}

void ItemDI::slot_guidconfig_finished()
{
    this->data_di_.func_ =  diContentSetting_->diData_.func_ ;
    this->data_di_.type_ =  diContentSetting_->diData_.type_ ;
    this->data_di_.maxDist_ =  diContentSetting_->diData_.maxDist_ ;
    this->data_di_.minDist_ =  diContentSetting_->diData_.minDist_ ;
    this->data_di_.inverse_ =  diContentSetting_->diData_.inverse_ ;
    this->data_di_.rang_ =  diContentSetting_->diData_.rang_ ;
    this->scanArea_->setRange(this->data_di_.rang_);
    add_setting_widget();
    WidgetDISetting* temp = dynamic_cast<WidgetDISetting*>(this->widget_setting_);
    temp->set_func_byConfigure(this->data_di_.func_);
    temp->set_type_byConfigure(this->data_di_.type_);
    temp->set_maxDist_byConfigure(this->data_di_.maxDist_);
    temp->set_minDist_byConfigure(this->data_di_.minDist_);
    temp->set_inverse_byConfigure(this->data_di_.inverse_);
    temp->set_rang_byConfigure(this->data_di_.rang_);
    diContentSetting_->close();
}
