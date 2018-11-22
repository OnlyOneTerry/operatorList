#include "ItemDO.h"
#include "ModelGraphicsScene.h"
#include "ItemAngelCtroller.h"
#include "WidgetDOSetting.h"
#include"ModelCustomCommand.h"
#include"./GuidenceWidgets/DOContentSetting.h"

int ItemDO::count = 1;//0表示引导创建，1表示非引导创建
ItemDO::ItemDO(ModelGraphicsScene *scene, QJsonObject json):
    ItemBase(scene)
{
    m_ori_pos=QPointF(0,0);
    m_new_pos=QPointF(0,0);
    setCursor(Qt::SizeAllCursor);
    brush_=QBrush(QColor(222,122,234));
    painter_path_.addRect(-k_radius_size_,-k_radius_size_,k_radius_size_*2,k_radius_size_*2);

    item_angel_ctroller_=new ItemAngelCtroller(scene,this);
    if(!json.isEmpty ())
    {
        data_do_.id_=json["id"].toInt ();
        data_do_.func_=json["func"].toInt ();
        data_do_.type_=json["type"].toInt ();
        data_do_.x_=json["x"].toDouble ();
        data_do_.y_=json["y"].toDouble ();
        data_do_.z_=json["z"].toDouble ();
        data_do_.r_=json["r"].toDouble ();
        data_do_.inverse_=json["inverse"].toBool ();
        m_ori_pos = QPointF(data_do_.x_*100,data_do_.y_*100);
        update_item ();
        //加载模型文件创建
        isCreatedByLoading_ = true;
    }
    else
    {
        while(model_scene()->get_id_list(2).contains(QString("%1").arg(data_do_.id_)))
        {
            data_do_.id_++;
        }
        //向场景中拖拽方式创建
        isCreatedByLoading_ = false;
    }
    setToolTip(QString("ItemDO_%1").arg(data_do_.id_));
    setID(data_do_.id_);
    update_item();
    update();
}

ItemDO::~ItemDO()
{

}

void ItemDO::add_setting_widget()
{
    if(widget_setting_==NULL)
    {
        widget_setting_=new WidgetDOSetting(this);
    }
}

void ItemDO::updateWidgetValue()
{
    qobject_cast<WidgetDOSetting *>(widget_setting_)->update_value ();
}


void ItemDO::position_changed()
{
    isPostion_changed = true;

    data_do_.x_=get_scene_pos_m ().x ();
    data_do_.y_=get_scene_pos_m ().y ();

    if(is_ppt_ready (widType::WIDGET_DO))
    {

        if(getIsBeingDraged())
        {
            qobject_cast<WidgetDOSetting *>(widget_setting_)->update_value ();
        }
    }

}

void ItemDO::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    model_scene()->setCurrentPressItem(this);
    ItemBase::mousePressEvent(event);
    if(event->button()==Qt::LeftButton)
    {
        setNewPos(event->pos());
    }

    if(count==1) return;
    doContentSetting_ = new DOContentSetting(data_do_.id_);
    connect(doContentSetting_,SIGNAL(sig_finished()),this,SLOT(slot_guidconfig_finished()));
    doContentSetting_->show();
    count++;
}

void ItemDO::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    ItemBase::mouseMoveEvent(event);
}

void ItemDO::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
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

QVariantMap ItemDO::get_do_data() const
{
    QVariantMap variant_map_do;

    variant_map_do.insert ("id",data_do_.id_);
    variant_map_do.insert ("func",data_do_.func_);
    variant_map_do.insert ("type",data_do_.type_);
    variant_map_do.insert ("x",data_do_.x_);
    variant_map_do.insert ("y",data_do_.y_);
    variant_map_do.insert ("z",data_do_.z_);
    variant_map_do.insert ("r",data_do_.r_);
    variant_map_do.insert ("inverse",data_do_.inverse_);

    return variant_map_do;
}

void ItemDO::update_data()
{
    data_do_.x_=get_scene_pos_m ().x ();
    data_do_.y_=get_scene_pos_m ().y ();
}

void ItemDO::update_item()
{
    setPos (data_do_.x_*100,data_do_.y_*100);
    //旋转角标
    qreal k_radius  = item_angel_ctroller_->getRoationRadius();
    item_angel_ctroller_->setPos(k_radius*qCos(data_do_.r_*M_PI/180.),k_radius*qSin(data_do_.r_*M_PI/180.));
    item_angel_ctroller_->setRotation(data_do_.r_);
    set_d_r(data_do_.r_);
    text_infor_=QString::number(data_do_.id_);
    setID(data_do_.id_);
    update();
    model_scene()->update();
}

void ItemDO::slot_angel_changed(qreal angle)
{
    set_d_r(angle);

    if(is_ppt_ready (widType::WIDGET_DO))
    {
        widget_setting_->set_r(angle);
    }
}

void ItemDO::slot_guidconfig_finished()
{
    this->data_do_.func_ = doContentSetting_->doData_.func_;
    this->data_do_.type_ = doContentSetting_->doData_.type_;
    this->data_do_.inverse_ = doContentSetting_->doData_.inverse_;
    add_setting_widget();
    WidgetDOSetting* temp  = dynamic_cast<WidgetDOSetting*>(widget_setting_);
    temp->set_func_byConfigure(this->data_do_.func_);
    temp->set_type_byConfigure(this->data_do_.type_);
    temp->set_inverse_byConfigure(this->data_do_.inverse_);
    doContentSetting_->close();
}
