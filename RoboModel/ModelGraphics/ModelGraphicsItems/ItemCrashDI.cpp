#include "ItemCrashDI.h"
#include<QLine>
#include<QPolygonF>
#include "ModelGraphicsScene.h"
#include"../ModelHelper/ModelCustomCommand.h"
#include"WidgetCrashDISetting.h"
#include<QJsonArray>
#include<QJsonObject>
#include"./GuidenceWidgets/CrashDiContentSetting.h"

int ItemCrashDI::count = 1;//0表示引导创建，1表示非引导创建
ItemCrashDI::ItemCrashDI(ModelGraphicsScene *scene, QJsonObject json):ItemBase(scene)
  ,m_addPos_oriItem(NULL)
{
    setFlag (ItemIsMovable);
    setFlag (ItemIsSelectable);
    setFlag (ItemSendsGeometryChanges);

    if(json.isEmpty())
    {
        //手动添加
        //create  manually
        ItemCrashDIpos* posStart1 = new ItemCrashDIpos(this);
        line_pos_list_.append(posStart1);
        posStart1->setDrawFlag(true);//允许画编号使编号跟随第一个点
        posStart1->setIndexInLine(line_pos_list_.size()-1);
        data_crashDi_.posList.append(posStart1->pos());
        posStart1->setRadius(3);
        posStart1->setPos(0,-50);
        posStart1->update();
        ItemCrashDIpos* posEnd1 = new ItemCrashDIpos(this);
        line_pos_list_.append(posEnd1);
        posEnd1->setPos(0,50);
        posEnd1->setRadius(3);
        posEnd1->update();
        posEnd1->setIndexInLine(line_pos_list_.size()-1);
        data_crashDi_.posList.append(posEnd1->pos());
        startPos_ = posStart1;//记录开始点
        endPos_   = posEnd1;//记录结束点
        while(model_scene()->get_id_list(0).contains(QString("%1").arg(data_crashDi_.id_)))
        {
            data_crashDi_.id_++;
        }
    }
    else
    {
        //读取模型文件创建
        data_crashDi_.id_ = json["id"].toInt();
        data_crashDi_.func_= json["func"].toInt();
        data_crashDi_.type_ = json["type"].toInt();
        data_crashDi_.inverse_  = json["inverse"].toBool();

        QJsonArray  posList = json["vertex"].toArray();

        for(int i = 0; i<posList.size();i++)
        {
            QJsonObject tempObj = posList.at(i).toObject();
            QPointF  tempPos(tempObj["x"].toDouble()*100,tempObj["y"].toDouble()*100);
            data_crashDi_.posList.append(tempPos);

            ItemCrashDIpos* tempItemPos = new ItemCrashDIpos(this);
            QPointF scenePos = tempItemPos->mapToScene(tempPos);//转换到场景坐标系
            tempItemPos->setRadius(3);
            tempItemPos->setPos(scenePos);
            tempItemPos->setOriPos(scenePos);
            line_pos_list_.append(tempItemPos);
            tempItemPos->setIndexInLine(line_pos_list_.size()-1);

        }
        if(line_pos_list_.size()>0)
        {
            startPos_ = line_pos_list_.first();
            startPos_->setDrawFlag(true);//允许画编号使编号跟随第一个点
            endPos_   = line_pos_list_.last();
        }
    }
    setToolTip(QString("ItemDI_%1").arg(data_crashDi_.id_));
    updata_item();
}

ItemCrashDI::~ItemCrashDI()
{

}

void ItemCrashDI::add_setting_widget()
{
    if(widget_setting_==NULL)
    {
        widget_setting_ = new WidgetCrashDISetting(this);
    }
}

QPainterPath ItemCrashDI::shape() const
{
    return lineShape(3.);
}

QRectF ItemCrashDI::boundingRect() const
{
    return lineShape(2.).boundingRect().adjusted(-5,-5,10,10);
}

void ItemCrashDI::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(pen_);
    QBrush brush(QColor(246,148,50,128));
    painter->setBrush(brush);
    painter->drawPath(lineShape(2.));
    middle_pos_list_.clear();

    int ns = line_pos_list_.size();

    for(int i = 0; i<ns-1;i++)
    {
        QColor color;
        color.setRgb(0,217,255);
        painter->setPen(color);
        QPointF midPos((line_pos_list_.at(i)->pos()+line_pos_list_.at(i+1)->pos())/2.0);
        painter->drawEllipse(midPos,2,2);
//        painter->drawText(midPos.x(),QString("%1").arg(i));
        middle_pos_list_.append(midPos);
    }

}

void ItemCrashDI::updata_item()
{
    setID(data_crashDi_.id_);
}

QList<ItemCrashDIpos *> *ItemCrashDI::getLine_pos_list()
{
    return &line_pos_list_;
}

QList<QPointF> *ItemCrashDI::get_Middle_pos_list()
{
    return &middle_pos_list_;
}

QVariantMap ItemCrashDI::getCrashDi_data()
{

    QVariantMap crashDiMap;
    crashDiMap.insert("id",data_crashDi_.id_);
    crashDiMap.insert("func",data_crashDi_.func_);
    crashDiMap.insert("type",3);
    crashDiMap.insert("inverse",data_crashDi_.inverse_);
    QVariantList poslist;
    foreach (auto var, line_pos_list_) {
        QVariantMap variant_line_pos;
        variant_line_pos.insert("x",var->scenePos().x()/100.);//不能保存QPoinF类型，要拆开为x, y保存到map中
        variant_line_pos.insert("y",var->scenePos().y()/100.);

        poslist.append(variant_line_pos);
    }
    crashDiMap.insert("vertex",poslist);
    return crashDiMap;
}

int ItemCrashDI::getPosSize()
{
    return line_pos_list_.size();
}

void ItemCrashDI::setID(int newid)
{
    data_crashDi_.id_ = newid;
    m_id = newid;
}

ItemCrashDIpos *ItemCrashDI::getStartPos()
{
    return startPos_;
}

ItemCrashDIpos *ItemCrashDI::getEndPos()
{
    return endPos_;
}

void ItemCrashDI::position_changed()
{
    isPostion_changed = true;
}

void ItemCrashDI::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    model_scene()->setCurrentPressItem(this);
    ItemBase::mousePressEvent(event);

    if(cursor().shape()==Qt::PointingHandCursor)
    {
        return;
    }

    //-----------------------引导设置窗
    if(1 == count) return;
    crashDicontentSetting_ = new CrashDiContentSetting(data_crashDi_.id_);
    connect(crashDicontentSetting_,SIGNAL(sig_finished()),SLOT(slot_guidconfig_finished()));
    crashDicontentSetting_->show();
    count++;
}

void ItemCrashDI::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    //若点击线段中点
    QPointF midpos = mapFromScene(model_scene()->get_scene_mousepos());//场景中的点坐标映射到item点坐标
    int num = middle_pos_list_.size();
    for(int i = 0; i < num;i++)
    {
        double value = qPow(midpos.x()-middle_pos_list_.at(i).x(),2)+qPow(midpos.y()-middle_pos_list_.at(i).y(),2);
        if(value<36)
        {
            ItemCrashDIpos*  mpos = new ItemCrashDIpos(this);
            mpos->setPos(middle_pos_list_.at(i));
            mpos->setIndexInLine(i+1);
            // line_pos_list_.insert(i+1,mpos);//插入到指定位置
            m_addPos_newItem = mpos;
            addPosCommand* addPos_cmd = new addPosCommand(this,i+1,m_addPos_oriItem,m_addPos_newItem,model_scene(),NULL);
            m_undoStack->push(addPos_cmd);
            m_addPos_oriItem = mpos;
        }
    }
}

void ItemCrashDI::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    ItemBase::mouseMoveEvent(event);
}

void ItemCrashDI::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    ItemBase::mouseReleaseEvent(event);

    //同时拖动多个item undo redo
    if(!isPostion_changed)
    {
        return;
    }

    if(m_undoStack == NULL) m_undoStack = UndoStack::InitStack();
    //框选多个item
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
    moveItemCommand* mv_command = new moveItemCommand(m_ori_pos,m_new_pos,model_scene(),this);
    m_undoStack->push(mv_command);
    m_ori_pos = this->pos();
}

void ItemCrashDI::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if(!isSelected())
    {
        pen_ = QPen(Qt::yellow);
    }
    QPointF midpos= mapFromScene(model_scene()->get_scene_mousepos()); //转换到当前item坐标系

    int num = middle_pos_list_.size();
    for(int i = 0; i < num; i++)
    {
        double value = qPow(midpos.x()-middle_pos_list_.at(i).x(),2)+qPow(midpos.y()-middle_pos_list_.at(i).y(),2);
        if(value <36)
        {
            //中间空心点
            setCursor(Qt::PointingHandCursor);
            setToolTip(tr("double click to add control point"));
            break;
        }
        else
        {
            setCursor(Qt::SizeAllCursor);
            setToolTip(QString("ItemDI_%1").arg(data_crashDi_.id_));
        }
    }

}

QPainterPath ItemCrashDI::lineShape(const qreal &stroke_width) const
{
    QPainterPath line_path;
    if(line_pos_list_.size()) line_path.moveTo(line_pos_list_.first()->pos());

    foreach (auto line_pos_item, line_pos_list_)
    {
        line_path.lineTo(line_pos_item->pos());
    }

    QPainterPathStroker ps;
    ps.setWidth(stroke_width);
    return ps.createStroke(line_path);

}

void ItemCrashDI::slot_guidconfig_finished()
{
    this->data_crashDi_.func_ = crashDicontentSetting_->data_crashDi_.func_;
    this->data_crashDi_.type_ = crashDicontentSetting_->data_crashDi_.type_;
    this->data_crashDi_.inverse_ = crashDicontentSetting_->data_crashDi_.inverse_;
    add_setting_widget();
    WidgetCrashDISetting* temp = dynamic_cast<WidgetCrashDISetting*>(widget_setting_);
    temp->set_func_byGuidConfigure(this->data_crashDi_.func_);
    temp->set_inverse_byGuidConfigure(this->data_crashDi_.inverse_);
    temp->set_type_byGuidConfigure(this->data_crashDi_.type_);
    crashDicontentSetting_->close();
}

