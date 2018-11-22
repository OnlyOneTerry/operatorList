#include "ModelGraphicsScene.h"
#include "ModelGraphicsView.h"
#include "ItemLaser.h"
#include "ItemPolygon.h"
#include "ItemPolypos.h"
#include "ItemDI.h"
#include "ItemDO.h"
#include"ItemBase.h"
#include "ItemCrashDI.h"
#include "ItemMagneticSensor.h"
#include "ItemRobotModel.h"
#include "ItemUltrasonic.h"
#include <QApplication>
#include "ItemAngelCtroller.h"
#include "ModelHelper.h"
#include"ModelCommon.h"
#include"ModelCustomCommand.h"
#include"ItemScanArea.h"
#include"UndoStack.h"
#include"ModelBasicSettingWidget.h"
#include"ItemRFID.h"
#include"ItemCamera.h"
#include"WidgetRobotModelSetting.h"
#include"WidgetCircleRobotModelSetting.h"
#include"UiClass.h"

ModelGraphicsScene::ModelGraphicsScene(QObject *parent):
    SCBaseGraphicsScene(parent),
    robot_model_(new ItemRobotModel(this)),
    scanArea_(new ItemScanArea(this)),
    m_ori_activeItem(NULL),
    m_new_activeItem(NULL),
    m_add_ori_type(INVALIDE),//添加前设为无效值
    m_add_ori_point(0,0),
    m_del_ori_pos(0,0),
    m_del_ori_type(INVALIDE),//添加前设为无效值
    m_del_new_type(INVALIDE),
    m_del_oriItem(NULL),
    m_add_oriItem(NULL),
    m_isElapsedEnough(false),
    m_isPressed(false),
    m_isAllowAddItem(true)
{
    setItemIndexMethod(QGraphicsScene::NoIndex);

    setSceneRect(-800,-800,1600,1600);
    //添加网格坐标
    add_axis ();
    addItem (robot_model_);
    robot_model_->setZValue(-1);
    robot_model_->setToolTip("robotModel");

    //按压时启动定时器，若定期器超过200ms则允许拖动，以此来判断是否为误触
    timer  = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(slotPressElapsedEnough()));
    timer->setInterval(200);
    timer->setSingleShot(true);
    m_undoStack = UndoStack::InitStack();
}

ModelGraphicsScene::~ModelGraphicsScene()
{
    delete robot_model_;
    delete scanArea_;
}

void ModelGraphicsScene::delete_operation()
{
    if(m_undoStack==NULL)
    {
        m_undoStack = UndoStack::InitStack();
    }
    //删除框选的多个item 操作

    //1. 获取选中的所有item
    QList<QGraphicsItem*>* selectedItemlist  = get_selected_items();
    QList<ItemBase*>  itemsList;
    foreach (auto var, *selectedItemlist) {
        itemsList.append(dynamic_cast<ItemBase*>(var));
    }
    if(itemsList.size()>0)
    {
        //2. 删除所有框选item
        deleteItemsCommand* delItemsCmd = new deleteItemsCommand(this,itemsList,NULL);
        m_undoStack->push(delItemsCmd);
        get_selected_items()->clear();
    }
    else if(selectedItems().size()>1)
    {
        //2.ctrl多选
        QList<ItemBase*>  CtrlitemsList;
        foreach (auto var, selectedItems()) {
            CtrlitemsList.append(dynamic_cast<ItemBase*>(var));
        }

        deleteItemsCommand* delItemsCmd = new deleteItemsCommand(this,CtrlitemsList,NULL);
        m_undoStack->push(delItemsCmd);
        selectedItems().clear();
    }
    else
    {
        //删除单个点击选中的item
        qDebug()<<"m_del new type is "<<m_del_new_type;//2009
        if(m_del_new_type == INVALIDE || m_del_new_type == I_robot_model) return; //当为
        if(m_del_new_type == ItemCrashDIpos::Type)
        {//第一个点和最后一个点不删除
            if(m_del_newItem ==qgraphicsitem_cast<ItemCrashDIpos*>(m_del_newItem)->getParent()->getStartPos()||
                    m_del_newItem == qgraphicsitem_cast<ItemCrashDIpos*>(m_del_newItem)->getParent()->getEndPos())
            {
                qDebug()<<"-==--------------------invalid pos ---------";
                UiClass::init()->showDialog(tr("Endpoint does not allow deletion"),tr("Information"),0);
                return;
            }
        }

        deleteItemCommand* del_command = new deleteItemCommand(m_del_ori_type,
                                                               m_del_new_type,
                                                               m_del_ori_pos,
                                                               m_del_new_pos,this,
                                                               m_del_oriItem,m_del_newItem,
                                                               robot_model_,
                                                               NULL);
        m_undoStack->push(del_command);
        m_del_oriItem = m_del_newItem;
        m_del_ori_type = m_del_new_type;
        m_del_ori_pos = m_del_new_pos;
    }

}


QPointF ModelGraphicsScene::get_scene_mousepos() const
{
    return mouse_pos_;
}

ModelGraphicsView *ModelGraphicsScene::model_view() const
{
    return (ModelGraphicsView*)(views().first ());
}

QStringList ModelGraphicsScene::get_id_list(quint8 type) const
{
    QStringList id_list;

    switch (type) {
    case 0://di
    {
        foreach (auto var, di_list_)
        {
            id_list.append(QString("%1").arg( var->data_di_.id_));
        }
        foreach (auto var, crashDi_list_)
        {
            id_list.append(QString("%1").arg( var->data_crashDi_.id_));
        }
    }break;
    case 1://ultrasonic
    {
        foreach (auto var, ultrasonic_list_)
        {
            id_list.append(QString("%1").arg( var->data_ultrasonic_.id_));
        }
    }break;
    case 2://do
    {
        foreach (auto var, do_list_)
        {
            id_list.append(QString("%1").arg( var->data_do_.id_));
        }
    }break;
    case 3://magnetic
    {
        foreach (auto var, magneticSensor_list_)
        {
            id_list.append(QString("%1").arg(var->data_magnetic_.id_));
        }
    }break;
    case 4://laser
    {
        foreach (auto var, laser_list_)
        {
            id_list.append(QString("%1").arg(var->data_laser_.id_));
        }
    }break;
    case 5://polygon
    {
        foreach (auto var, polygon_list_)
        {
            id_list.append(QString("%1").arg(var->data_polygon_.id_));
        }
    }break;
    default:
    {
    }break;
    }

    return id_list;
}

void ModelGraphicsScene::init_new()
{
    qreal head=robot_model_->robot_model_data_.head_/100.0;
    qreal tail=robot_model_->robot_model_data_.tail_/100.0;
    qreal width=robot_model_->robot_model_data_.width_/100.0;


    QPointF pp1(head+1.5,width/2.0+0.4);
    QPointF pp2(head+1.5,-(width/2.0+0.4));
    QPointF pp3(-(tail+1.5),-(width/2.0+0.4));
    QPointF pp4(-(tail+1.5),(width/2.0+0.4));

    QList<QPointF> l_pos_list;
    l_pos_list<<pp1<<pp2<<pp3<<pp4;
    ItemPolygon *l_ipoly=new ItemPolygon(this,ItemPolygon::PolyType::LOWSPEED,l_pos_list);
    addItem (l_ipoly);
    l_ipoly->stackBefore(robot_model_);
    polygon_list_.append (l_ipoly);

    QPointF p1(head+0.3,width/2.0+0.3);
    QPointF p2(head+0.3,-(width/2.0+0.3));
    QPointF p3(-(tail+0.3),-(width/2.0+0.3));
    QPointF p4(-(tail+0.3),(width/2.0+0.3));
    QList<QPointF> s_pos_list;
    s_pos_list<<p1<<p2<<p3<<p4;
    ItemPolygon *s_ipoly=new ItemPolygon(this,ItemPolygon::PolyType::STOPOBJ,s_pos_list);
    addItem (s_ipoly);
    s_ipoly->stackBefore(robot_model_);
    polygon_list_.append (s_ipoly);
}

void ModelGraphicsScene::adjust_polygons(qreal del_head, qreal del_tail, qreal del_width)
{
    foreach (auto var_polygon, polygon_list_)
    {
        var_polygon->adjust_polygon(del_head,del_tail,del_width);
    }
}

void ModelGraphicsScene::clearFocusState()
{
    robot_model_->setPen(Qt::NoPen);
    foreach (auto var, laser_list_) {
        var->setPen(Qt::NoPen);
    }
    foreach (auto var, polygon_list_) {
        var->setPen(Qt::NoPen);
    }
    foreach (auto var, di_list_) {
        var->setPen(Qt::NoPen);
    }
    foreach (auto var, do_list_) {
        var->setPen(Qt::NoPen);
    }
    foreach (auto var, ultrasonic_list_) {
        var->setPen(Qt::NoPen);
    }
    foreach (auto var, magneticSensor_list_) {
        var->setPen(Qt::NoPen);
    }
}

bool ModelGraphicsScene::isAlldevsOnChassis()
{
    bool allOnchassisFlag = true;

    foreach (auto var, laser_list_) {
        if(!isWithInChassisArea(var->pos().x(),var->pos().y()))
        {
            allOnchassisFlag = false;
            return false;
        }

    }
    foreach (auto var, di_list_) {

        if(!isWithInChassisArea(var->pos().x(),var->pos().y()))
        {
            allOnchassisFlag = false;
            return false;
        }
    }
    foreach (auto var, crashDi_list_) {
        QList<ItemCrashDIpos*> poslist = *var->getLine_pos_list();
        foreach(auto v,poslist) {
            if(!isWithInChassisArea(dynamic_cast<ItemCrashDIpos*>(v)->scenePos().x(),
                                    dynamic_cast<ItemCrashDIpos*>(v)->scenePos().y(),true))
            {
                allOnchassisFlag = false;
                return false;
            }
        }

    }
    foreach (auto var, do_list_) {
        if(!isWithInChassisArea(var->pos().x(),var->pos().y()))
        {
            allOnchassisFlag = false;
            return false;
        }
    }
    foreach (auto var, ultrasonic_list_) {
        if(!isWithInChassisArea(var->pos().x(),var->pos().y()))
        {
            allOnchassisFlag = false;
            return false;
        }
    }
    foreach (auto var, magneticSensor_list_) {

        if(!isWithInChassisArea(var->pos().x(),var->pos().y()))
        {
            allOnchassisFlag = false;
            return false;
        }
    }
    foreach (auto var, RFID_list_) {
        if(!isWithInChassisArea(var->pos().x(),var->pos().y()))
        {
            allOnchassisFlag = false;
            return false;
        }
    }
    foreach (auto var, camera_list_) {
        if(!isWithInChassisArea(var->pos().x(),var->pos().y()))
        {
            allOnchassisFlag = false;
            return false;
        }
    }
    return allOnchassisFlag;
}

bool ModelGraphicsScene::isWithInChassisArea(qreal x, qreal y, bool isCrashDI)
{
    //保证所有的设备都落在底盘上：
    //区分底盘形状
    if(robot_model_->robot_model_data_.shapType_== RECTANGLE)
    {
        if(isCrashDI)
        {
            if(y>(robot_model_->robot_model_data_.width_/2+3) || y<(-robot_model_->robot_model_data_.width_/2-3)
                    ||x>(robot_model_->robot_model_data_.head_+3)||x<(-robot_model_->robot_model_data_.tail_-3))
            {
                return false;
            }
            else
            {
                return true;
            }
        }

        if(y>robot_model_->robot_model_data_.width_/2 || y<-robot_model_->robot_model_data_.width_/2
                ||x>robot_model_->robot_model_data_.head_||x<-robot_model_->robot_model_data_.tail_)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    else if(robot_model_->robot_model_data_.shapType_ == CIRCLE)//设备距离圆心<=半径
    {
        if(isCrashDI)
        {
            if((x*x+y*y)>robot_model_->robot_circle_model_data_.radius_*robot_model_->robot_circle_model_data_.radius_+9)
            {
                return false;
            }
            else
            {
                return true;
            }
        }
        if((x*x+y*y)>robot_model_->robot_circle_model_data_.radius_*robot_model_->robot_circle_model_data_.radius_)
        {
            return false;
        }
        else
        {
            return true;
        }
    }
}

bool ModelGraphicsScene::isGuidenceFinished()
{
    //二次引导配置的设备有，激光，di,do，超声，磁传感器
    bool finished = true;
    if(laser_list_.size()>0)
    {
        if(ItemLaser::count==1)
        {
            finished = true;
        }
        else
        {
            finished = false;
        }
    }
    if(di_list_.size()>0)
    {
        if(ItemDI::count==1)
        {
            finished = true;
        }
        else
        {
            finished = false;
        }
    }
    if(do_list_.size()>0)
    {
        if(ItemDO::count==1)
        {
            finished = true;
        }
        else
        {
            finished = false;
        }
    }
    if(ultrasonic_list_.size()>0)
    {
        if(ItemUltrasonic::count==1)
        {
            finished = true;
        }
        else
        {
            finished = false;
        }
    }
    if(crashDi_list_.size()>0)
    {
        if(ItemCrashDI::count==1)
        {
            finished = true;
        }
        else
        {
            finished = false;
        }
    }
    if(magneticSensor_list_.size()>0)
    {
        if(ItemMagneticSensor::count==1)
        {
            finished = true;
        }
        else
        {
            finished = false;
        }
    }
    return finished;
}

qreal ModelGraphicsScene::getDistanceBetweenTwoPoints(QPointF pos1, QPointF pos2)
{
    qreal distance = qSqrt((pos1.x()-pos2.x())*(pos1.x()-pos2.x())+(pos1.y()-pos2.y())*(pos1.y()-pos2.y()));
    return distance;
}

void ModelGraphicsScene::zoomIn()
{
    if(scaleFactor>15)
    {
        return;
    }
    zoom(1.2);
}

void ModelGraphicsScene::zoomOut()
{
    if(scaleFactor<2.5)
    {
        return;
    }
    zoom(0.8);
}

void ModelGraphicsScene::zoom(float scale)
{
    scaleFactor *=scale;
}

void ModelGraphicsScene::mousePressEvent (QGraphicsSceneMouseEvent *event)
{
    timer->start();
    m_isPressed = true;

    SCBaseGraphicsScene::mousePressEvent(event);
    int itemCount = selectedItems().size();

    /*框选*/
    {
        if(QApplication::keyboardModifiers ()!=Qt::AltModifier && event->buttons()==Qt::LeftButton)
        {
            QList<QGraphicsItem *> item_all = find_item_by_pos (event->scenePos ());

            if (item_all.size() < 1&&itemCount<1)
            {
                if(model_view()->get_viewport_cursor().shape()==Qt::ArrowCursor)
                {
                    if(!m_bIsTwoPoint)
                        add_mouse_moved_rect (event->scenePos ());
                }
            }
        }
    }

    //更新图层
    model_view ()->property_widget ()->update_layer (find_items (event->scenePos ()));

    if(itemCount==0)
    {
        qDebug()<<"selected_items.size()==0";
        clearFocusState();
    }
}
void ModelGraphicsScene::mouseMoveEvent (QGraphicsSceneMouseEvent *event)
{
    //按压了但时间没到 直接返回，若没有按压则执行else
    if(!m_isElapsedEnough && m_isPressed)
    {
        return;
    }

    mouse_pos_=event->scenePos ();
    SCBaseGraphicsScene::mouseMoveEvent (event);

    /*框选*/
    {
        adjust_mouse_moved_rect (event->scenePos ());
    }
}
void ModelGraphicsScene::mouseReleaseEvent (QGraphicsSceneMouseEvent *event)
{
    //松开鼠标后复位
    m_isPressed = false;
    timer->stop();
    m_isElapsedEnough = false;

    mouse_pos_=event->scenePos ();
    SCBaseGraphicsScene::mouseReleaseEvent (event);
    int numCoveredItem = 0;//框选item个数
    /*框选*/
    {
        numCoveredItem = set_items_selected_by_rect ();

        if(numCoveredItem>=1)
        {
            model_view ()->reset_property_widget ();
        }
        //记录拖拽移动前item 的状态
        m_oriItemMap.clear();
        QList<QGraphicsItem*> *templist = get_selected_items();
        foreach (auto var, *templist) {
            m_oriItemMap.insert(dynamic_cast<ItemBase*>(var),dynamic_cast<ItemBase*>(var)->getOriPos());
        }
    }
    //-----------------------
    if(numCoveredItem >0) return;


    //删除时 选中的item
    QList<QGraphicsItem* > selected_items=selectedItems();
    {
        //如果是多选
        if(selected_items.size()>1) return;
        //当前只处理单选
        foreach (auto var, selected_items) {
            if(var->type() == ItemAngelCtroller::Type)
            {

            }
            else if(var->type ()==ItemPolygon::Type)
            {
                m_del_new_type = ItemPolygon::Type;
                ItemPolygon* tempItem = qgraphicsitem_cast<ItemPolygon*>(var);
                m_del_new_pos = tempItem->pos();
                m_del_newItem = tempItem;
                break;
            }
            else if(var->type()==ItemRobotModel::Type)
            {
                m_del_new_type = ItemRobotModel::Type;
                ItemRobotModel* tempItem = qgraphicsitem_cast<ItemRobotModel*>(var);
                m_del_new_pos = tempItem->pos();
                m_del_newItem = tempItem;
                break;
            }
            else if(var->type()==ItemDO::Type)
            {
                m_del_new_type = ItemDO::Type;
                ItemDO* tempItem = qgraphicsitem_cast<ItemDO*>(var);
                m_del_new_pos = tempItem->pos();
                m_del_newItem = tempItem;
                break;
            }
            else if(var->type()==ItemDI::Type)
            {
                m_del_new_type = ItemDI::Type;
                ItemDI* tempItem = qgraphicsitem_cast<ItemDI*>(var);
                m_del_new_pos = tempItem->pos();
                m_del_newItem = tempItem;
                break;
            }
            else if(var->type()==ItemLaser::Type)
            {
                m_del_new_type = ItemLaser::Type;
                ItemLaser* tempItem = qgraphicsitem_cast<ItemLaser*>(var);
                m_del_new_pos = tempItem->pos();
                m_del_newItem = tempItem;
                break;
            }
            else if(var->type()==ItemPolypos::Type)
            {
                m_del_new_type = ItemPolypos::Type;
                ItemPolypos* tempItem = qgraphicsitem_cast<ItemPolypos*>(var);
                m_del_new_pos = tempItem->pos();
                m_del_newItem = tempItem;
                break;
            }
            else if(var->type()==ItemUltrasonic::Type)
            {
                m_del_new_type = ItemUltrasonic::Type;
                ItemUltrasonic* tempItem = qgraphicsitem_cast<ItemUltrasonic*>(var);
                m_del_new_pos = tempItem->pos();
                m_del_newItem = tempItem;
                break;

            }
            else if(var->type()==ItemMagneticSensor::Type)
            {
                m_del_new_type = ItemMagneticSensor::Type;
                ItemMagneticSensor* tempItem = qgraphicsitem_cast<ItemMagneticSensor*>(var);
                m_del_new_pos = tempItem->pos();
                m_del_newItem = tempItem;
                break;
            }
            else if(var->type()==ItemCrashDI::Type)
            {
                m_del_new_type = ItemCrashDI::Type;
                ItemCrashDI*  tempItem = qgraphicsitem_cast<ItemCrashDI*>(var);
                m_del_new_pos = tempItem->pos();
                m_del_newItem = tempItem;
                break;
            }
            else if(var->type()==ItemCrashDIpos::Type)
            {
                ItemCrashDIpos* tempItem = qgraphicsitem_cast<ItemCrashDIpos*>(var);
                m_del_new_type = ItemCrashDIpos::Type;
                m_del_new_pos = tempItem->pos();
                m_del_newItem = tempItem;
                break;
            }
            else if(var->type()==ItemRFIDSensor::Type)
            {
                ItemRFIDSensor* tempItem = qgraphicsitem_cast<ItemRFIDSensor*>(var);
                m_del_new_type = ItemRFIDSensor::Type;
                m_del_new_pos  = tempItem->pos();
                m_del_newItem = tempItem;
                break;
            }
            else if(var->type()==ItemCamera::Type)
            {
                ItemCamera* tempItem = qgraphicsitem_cast<ItemCamera*>(var);
                m_del_new_type =ItemCamera::Type;
                m_del_new_pos = tempItem->pos();
                m_del_newItem = tempItem;
                break;
            }

        }
    }

}

void ModelGraphicsScene::mouseDoubleClickEvent (QGraphicsSceneMouseEvent *event)
{
    SCBaseGraphicsScene::mouseDoubleClickEvent (event);
}

void ModelGraphicsScene::keyReleaseEvent(QKeyEvent *event)
{
    //ctrl多选
    if(QApplication::keyboardModifiers ()==Qt::ControlModifier)
    {
        //记录拖拽移动前item 的状态
        m_oriItemMap.clear();
        QList<QGraphicsItem*> templist = selectedItems();
        foreach (auto var, templist) {
            if(var->type()==I_robot_model||
                    var->type()==I_angle_controller||
                    var->type()==I_scanArea_model) continue;
            m_oriItemMap.insert(dynamic_cast<ItemBase*>(var),dynamic_cast<ItemBase*>(var)->getOriPos());
        }
    }
}

void ModelGraphicsScene::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    //先选中再右击删除
    QList<QGraphicsItem*> selectedItemlist  = selectedItems();
    //先判断是否选中的item为箭头或扫描区域
    if(selectedItemlist.size()==1&&(selectedItemlist.at(0)->type()==I_angle_controller||selectedItemlist.at(0)->type()==I_scanArea_model))
        return;
    //如果当前item被选中并且右击位置在item区域内部
    QTransform transform;
    QGraphicsItem* item = itemAt(event->scenePos(),transform);//获取鼠标右击位置最上层的item
    if(selectedItemlist.size()==1&&selectedItemlist.at(0)==item)
    {
        m_currentRightClickedItem = selectedItemlist.first();
        if(m_currentRightClickedItem->type()== I_robot_model ||m_currentRightClickedItem->type()== I_Axis
                || m_currentRightClickedItem->type()== I_scanArea_model )//底盘，坐标系，扫描区域不允许删除
        {
            return;
        }
        QMenu menu;
        QAction *deleteAct = menu.addAction(tr("delete"));
        bool a = connect(deleteAct, SIGNAL(triggered()),this, SLOT(slot_rightClickToDelete()));
        menu.exec(event->screenPos());
        QGraphicsScene::contextMenuEvent(event);
    }
}

bool ModelGraphicsScene::event(QEvent *event)
{
    static int count = 0;
    switch(event->type())
    {
    case QEvent::TouchBegin:
    {
        qDebug()<<"touch begin------------------";
    }
        break;
    case QEvent::TouchUpdate:
    {
        count++;
        QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
        QList<QTouchEvent::TouchPoint> points = touchEvent->touchPoints();
        int num = points.count();
        event->accept();
        if(num==2)
        {
            m_bIsTwoPoint = true;

            if(count==1)
            {
                old_Distance = getDistanceBetweenTwoPoints(points.at(0).startScreenPos(),
                                                           points.at(1).lastScreenPos());
            }
            else
            {
                new_Distance = getDistanceBetweenTwoPoints(points.at(0).startScreenPos(),
                                                           points.at(1).lastScreenPos());
                qDebug()<<"distance is -------"<<new_Distance-old_Distance;
                qDebug()<<"scale is -------"<<scaleFactor;
                if(new_Distance-old_Distance>4)
                {
                    zoomIn();
                    old_Distance = new_Distance;
                }
                else if(new_Distance-old_Distance<-4)
                {
                    zoomOut();
                    old_Distance = new_Distance;
                }
                model_view()->zoomInOut(scaleFactor);
            }
        }
        else
        {
            m_bIsTwoPoint = false;
        }
    }
        break;
    case QEvent::TouchEnd:
    {
        qDebug()<<"touch TouchUpdate------------------";
        count = 0;
    }
    default:
        break;
    }

    return SCBaseGraphicsScene::event(event);
}

void ModelGraphicsScene::add_axis()
{
    ItemAxis *item_axis=new ItemAxis;
    addItem (item_axis);
}

void ModelGraphicsScene::slot_add_polygon(QPoint pos,int type)
{
    //判断引导配置是否完成
    if(!isGuidenceFinished())
    {
        QStringList list;
        list<<tr("Cancel");
        UiClass::init()->showDialog(tr("Operation failed, please continue to click right graphics to finish guidence,RFID except,attention you just need to click one of them if there are several same items like three lasers!"),tr("Information"),0,list);
        return;
    }

    if(m_undoStack==NULL)
    {
        m_undoStack = UndoStack::InitStack();
    }

    m_add_new_type = type;
    m_add_new_point = pos;

    if(type<3)          //多边形
    {
        //针对没有有数量限制的设备设置允许添加标记
        m_isAllowAddItem = true;

        ItemPolygon *ipoly=new ItemPolygon(this,ItemPolygon::PolyType(type));
        ipoly->setOriPos(QPointF(pos.x(),pos.y()));
        m_add_newItem = ipoly;
        //addItem (ipoly);
        //ipoly->stackBefore(robot_model_);
        //ipoly->setPos (model_view ()->mapToScene (pos));

        ipoly->setZValue(getItemAcount());//设置新建item的层级
    }
    else if(type==10)//激光
    {
        //针对没有有数量限制的设备设置允许添加标记
        m_isAllowAddItem = true;
        ItemLaser::count = 1;//标记不调用引导窗
        ItemLaser *ilaser=new ItemLaser(this);
        ilaser->setOriPos(QPointF(pos.x(),pos.y()));
        m_add_newItem = ilaser;
        ilaser->setZValue(getItemAcount());//设置新建item的层级
    }
    else if(type==11)//ultrasonic
    {
        if(ultrasonic_list_.size()<model_view()->basic_widget()->model_base_data_.max_ultrasonic_num_)
        {
            //针对有数量限制的设备设置允许添加标记
            m_isAllowAddItem = true;
            ItemUltrasonic::count = 1;//标记不调用引导窗
            ItemUltrasonic *iul=new ItemUltrasonic(this);
            iul->setOriPos(QPointF(pos.x(),pos.y()));
            m_add_newItem = iul;

            iul->setZValue(getItemAcount());//设置新建item的层级
            iul->getScanArea()->setZValue(iul->zValue());
            iul->getAngelCtroller()->setZValue(iul->zValue()+1);
        }
        else
        {
            m_isAllowAddItem = false;
            QStringList list;
            list<<tr("Close");
            UiClass::init()->showDialog(tr("Ultrasound has reached the maximum number set, if you want to continue to add, please change the maximum number of ultrasound in the left property window"),tr("Information"),0,list);
        }
    }
    else if(type==12)//DI  di+crashDi<规定最大数量
    {
        if(di_list_.size()+crashDi_list_.size()<model_view()->basic_widget()->model_base_data_.max_di_num_)
        {
            //针对有数量限制的设备设置允许添加标记
            m_isAllowAddItem = true;
            ItemDI::count = 1;//标记不调用引导窗
            ItemDI *idi=new ItemDI(this);
            idi->setOriPos(QPointF(pos.x(),pos.y()));
            m_add_newItem = idi;
            idi->setZValue(getItemAcount());//设置新建item的层级
            idi->getScanArea()->setZValue(idi->zValue());
            idi->getAngelCtroller()->setZValue(idi->zValue()+1);
        }
        else
        {
            m_isAllowAddItem = false;
            QStringList list;
            list<<tr("Close");
            UiClass::init()->showDialog(tr("DI has reached the maximum number set. To continue to add, modify the maximum number of DI in the left property window"),tr("Information"),0,list);
        }
    }
    else if(type==13)//magneticScesor
    {
        //针对没有有数量限制的设备设置允许添加标记
        if(magneticSensor_list_.size()<11)
        {
            m_isAllowAddItem = true;
            ItemMagneticSensor::count = 1;//标记不调用引导窗
            ItemMagneticSensor *MagneticSensor=new ItemMagneticSensor(this);
            MagneticSensor->setOriPos(QPointF(pos.x(),pos.y()));
            m_add_newItem = MagneticSensor;

            MagneticSensor->setZValue(getItemAcount());//设置新建item的层级
        }
        else
        {
            m_isAllowAddItem = false;
            QStringList list;
            list<<tr("Close");
            UiClass::init()->showDialog(tr("The maximum number of magnetic sensors has been reached"),tr("Information"),0,list);
        }
    }
    else if(type==14)//DO
    {
        if(do_list_.size()<model_view()->basic_widget()->model_base_data_.max_do_num_)
        {
            //针对有数量限制的设备设置允许添加标记
            m_isAllowAddItem = true;
            ItemDO::count = 1;//标记不调用引导窗
            ItemDO *ido=new ItemDO(this);
            ido->setOriPos(QPointF(pos.x(),pos.y()));
            m_add_newItem = ido;

            ido->setZValue(getItemAcount());//设置新建item的层级

        }
        else
        {
            m_isAllowAddItem = false;
            QStringList list;
            list<<tr("Close");
            UiClass::init()->showDialog(tr("DO has reached the maximum number set. To continue to add, modify the maximum number of DO in the left property window"),tr("Information"),0,list);
        }
    }
    else if(type == 15) //碰撞DI
    {
        if(di_list_.size()+crashDi_list_.size()<model_view()->basic_widget()->model_base_data_.max_di_num_)
        {
            //针对有数量限制的设备设置允许添加标记
            m_isAllowAddItem = true;
            ItemCrashDI::count  = 1;//标记不调用引导窗
            ItemCrashDI* crashDi = new ItemCrashDI(this);
            crashDi->setOriPos(QPointF(pos.x(),pos.y()));
            m_add_newItem = crashDi;
            crashDi->setZValue(getItemAcount());

            for(int i = 0; i<crashDi->getLine_pos_list()->size();i++)
            {
                crashDi->getLine_pos_list()->at(i)->setOriPos(
                            crashDi->getLine_pos_list()->at(i)->scenePos());
            }

        }
        else
        {
            m_isAllowAddItem = false;
            QStringList list;
            list<<tr("Close");
            UiClass::init()->showDialog(tr("DI has reached the maximum number set. To continue to add, modify the maximum number of DI in the left property window"),tr("Information"),0,list);
        }
    }
    else if(type == 16)//RFID
    {
        if(RFID_list_.size()==0)
        {
            m_isAllowAddItem = true;
            ItemRFIDSensor*  RFID = new ItemRFIDSensor(this);
            RFID_list_.append(RFID);
            RFID->setOriPos(QPointF(pos.x(),pos.y()));
            m_add_newItem = RFID;
            RFID->setZValue(getItemAcount());
        }
        else
        {
            m_isAllowAddItem = false;
            QStringList list;
            list<<tr("Close");
            UiClass::init()->showDialog(tr("RFID has reached its maximum number and is not allowed to add"),tr("Information"),0,list);
        }
    }
    else if(type == 17)//camera
    {
        if(camera_list_.size()==0)
        {
            m_isAllowAddItem = true;
            ItemCamera* camera = new ItemCamera(this);
            camera_list_.append(camera);
            camera->setOriPos(QPointF(pos.x(),pos.y()));
            m_add_newItem = camera;
            camera->setZValue(getItemAcount());
        }
        else
        {
            m_isAllowAddItem = false;
            QStringList list;
            list<<tr("Close");
            UiClass::init()->showDialog(tr("camera has reached its maxinum number set and is not allowed to add"),tr("Information"),0,list);
        }
    }

    if(m_isAllowAddItem)
    {
        addItemCommand* add_command = new addItemCommand(m_add_ori_type,
                                                         m_add_new_type,
                                                         m_add_ori_point,
                                                         m_add_new_point,
                                                         m_add_oriItem,m_add_newItem,
                                                         this,NULL);

        m_undoStack->push(add_command);
        m_add_ori_type = type;
        m_add_ori_point = pos;
        update ();
    }
}

int ModelGraphicsScene::getItemAcount()
{
    int statistic;
    statistic = polygon_list_.size()+laser_list_.size()+di_list_.size()+do_list_.size()+ultrasonic_list_.size()
            +magneticSensor_list_.size()+crashDi_list_.size()+camera_list_.size();
    return statistic;
}

void ModelGraphicsScene::slot_rightClickToDelete()
{

    if(m_currentRightClickedItem->type()==ItemRobotModel::Type)
    {
        m_del_new_type = ItemRobotModel::Type;
        ItemRobotModel* tempItem = qgraphicsitem_cast<ItemRobotModel*>(m_currentRightClickedItem);
        m_del_new_pos = tempItem->pos();
        m_del_newItem = tempItem;
    }
    else if(m_currentRightClickedItem->type()==ItemDO::Type)
    {
        m_del_new_type = ItemDO::Type;
        ItemDO* tempItem = qgraphicsitem_cast<ItemDO*>(m_currentRightClickedItem);
        m_del_new_pos = tempItem->pos();
        m_del_newItem = tempItem;
    }
    else if(m_currentRightClickedItem->type()==ItemDI::Type)
    {
        m_del_new_type = ItemDI::Type;
        ItemDI* tempItem = qgraphicsitem_cast<ItemDI*>(m_currentRightClickedItem);
        m_del_new_pos = tempItem->pos();
        m_del_newItem = tempItem;
    }
    else if(m_currentRightClickedItem->type()==ItemLaser::Type)
    {
        m_del_new_type = ItemLaser::Type;
        ItemLaser* tempItem = qgraphicsitem_cast<ItemLaser*>(m_currentRightClickedItem);
        m_del_new_pos = tempItem->pos();
        m_del_newItem = tempItem;
    }
    else if(m_currentRightClickedItem->type()==ItemPolypos::Type)
    {
        m_del_new_type = ItemPolypos::Type;
        ItemPolypos* tempItem = qgraphicsitem_cast<ItemPolypos*>(m_currentRightClickedItem);
        m_del_new_pos = tempItem->pos();
        m_del_newItem = tempItem;
    }
    else if(m_currentRightClickedItem->type()==ItemUltrasonic::Type)
    {
        m_del_new_type = ItemUltrasonic::Type;
        ItemUltrasonic* tempItem = qgraphicsitem_cast<ItemUltrasonic*>(m_currentRightClickedItem);
        m_del_new_pos = tempItem->pos();
        m_del_newItem = tempItem;

    }
    else if(m_currentRightClickedItem->type()==ItemMagneticSensor::Type)
    {
        m_del_new_type = ItemMagneticSensor::Type;
        ItemMagneticSensor* tempItem = qgraphicsitem_cast<ItemMagneticSensor*>(m_currentRightClickedItem);
        m_del_new_pos = tempItem->pos();
        m_del_newItem = tempItem;
    }
    else if(m_currentRightClickedItem->type()==ItemCrashDI::Type)
    {
        m_del_new_type = ItemCrashDI::Type;
        ItemCrashDI*  tempItem = qgraphicsitem_cast<ItemCrashDI*>(m_currentRightClickedItem);
        m_del_new_pos = tempItem->pos();
        m_del_newItem = tempItem;
    }
    else if(m_currentRightClickedItem->type()==ItemCrashDIpos::Type)
    {
        ItemCrashDIpos* tempItem = qgraphicsitem_cast<ItemCrashDIpos*>(m_currentRightClickedItem);
        m_del_new_type = ItemCrashDIpos::Type;
        m_del_new_pos = tempItem->pos();
        m_del_newItem = tempItem;
    }
    else if(m_currentRightClickedItem->type()==ItemRFIDSensor::Type)
    {
        ItemRFIDSensor* tempItem = qgraphicsitem_cast<ItemRFIDSensor*>(m_currentRightClickedItem);
        m_del_new_type = ItemRFIDSensor::Type;
        m_del_new_pos  = tempItem->pos();
        m_del_newItem = tempItem;
    }
    else if(m_currentRightClickedItem->type()==ItemCamera::Type)
    {
        ItemCamera* tempItem = qgraphicsitem_cast<ItemCamera*>(m_currentRightClickedItem);
        m_del_new_type =ItemCamera::Type;
        m_del_new_pos = tempItem->pos();
        m_del_newItem = tempItem;
    }

    //右击删除tem
    qDebug()<<"m_del new type is "<<m_del_new_type;//2009
    if(m_del_new_type == INVALIDE || m_del_new_type == I_robot_model) return; //当为
    if(m_del_new_type == ItemCrashDIpos::Type)
    {//第一个点和最后一个点不删除
        if(m_del_newItem ==qgraphicsitem_cast<ItemCrashDIpos*>(m_del_newItem)->getParent()->getStartPos()||
                m_del_newItem == qgraphicsitem_cast<ItemCrashDIpos*>(m_del_newItem)->getParent()->getEndPos())
        {
            qDebug()<<"-==--------------------invalid pos ---------";
            UiClass::init()->showDialog(tr("Endpoint does not allow deletion"),tr("Information"),0);
            return;
        }
    }

    deleteItemCommand* del_command = new deleteItemCommand(m_del_ori_type,
                                                           m_del_new_type,
                                                           m_del_ori_pos,
                                                           m_del_new_pos,this,
                                                           m_del_oriItem,m_del_newItem,
                                                           robot_model_,
                                                           NULL);
    m_undoStack->push(del_command);
    m_del_oriItem = m_del_newItem;
    m_del_ori_type = m_del_new_type;
    m_del_ori_pos = m_del_new_pos;
}

void ModelGraphicsScene::slotPressElapsedEnough()
{
    m_isElapsedEnough = true;
}

RobotModelData ModelGraphicsScene::get_robot_model_data()
{
    //RobotModelData data;
    if(robot_model_->robot_model_data_.shapType_==RECTANGLE)
    {
        return robot_model_->robot_model_data_;
    }
    else if(robot_model_->robot_model_data_.shapType_==CIRCLE)
    {
        return robot_model_->robot_circle_model_data_;
    }
}

QGraphicsItem *ModelGraphicsScene::item_robot_model() const
{
    return robot_model_;
}

ItemRobotModel *ModelGraphicsScene::getItem_robot_model() const
{
    return robot_model_ ;
}
//读取模型文件
void ModelGraphicsScene::parse_robot_data(const RobotModelData &robot_model_data)
{
    robot_model_->setChasisShapeType(robot_model_data.shapType_);//设置模型外轮廓形状
    if(robot_model_data.shapType_==RECTANGLE)
    {
        robot_model_->robot_model_data_.head_=robot_model_data.head_;
        robot_model_->robot_model_data_.tail_=robot_model_data.tail_;
        robot_model_->robot_model_data_.width_=robot_model_data.width_;
        robot_model_->setMOriHead(robot_model_data.head_);
        robot_model_->setMOriWidth(robot_model_data.width_);
        robot_model_->setMOriTail(robot_model_data.tail_);

        robot_model_->robot_model_data_.wheelRadius_= robot_model_data.wheelRadius_;
        robot_model_->robot_model_data_.chasisType_=robot_model_data.chasisType_;
        robot_model_->robot_model_data_.mvStyle_ = robot_model_data.mvStyle_;

        switch (robot_model_->robot_model_data_.chasisType_) {
        case DIFFERENTIAL_TWOWHEEL://差动两轮
            robot_model_->robot_model_data_.chasisModel.DiffTwoWheel.a_axis_distance=robot_model_data.chasisModel.DiffTwoWheel.a_axis_distance;
            break;
        case DIFFERENTIAL_FOURWHEEL://差动四轮
            robot_model_->robot_model_data_.chasisModel.DiffFourWheel.a_axis_distance= robot_model_data.chasisModel.DiffFourWheel.a_axis_distance;
            robot_model_->robot_model_data_.chasisModel.DiffFourWheel.b_axis_distance= robot_model_data.chasisModel.DiffFourWheel.b_axis_distance;
            break;
        case ALLDIRECTION_THREEWHEEL://全向三轮
            robot_model_->robot_model_data_.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance=robot_model_data.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance;
            robot_model_->robot_model_data_.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle = robot_model_data.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle;
            break;
        case MECANUM_FOURWHEEL://四轮麦克纳姆
            robot_model_->robot_model_data_.chasisModel.MecanumFourWheel.a_axis_distance=robot_model_data.chasisModel.MecanumFourWheel.a_axis_distance;
            robot_model_->robot_model_data_.chasisModel.MecanumFourWheel.b_axis_distance=robot_model_data.chasisModel.MecanumFourWheel.b_axis_distance;
            break;
        case SINGLESTEERWHEEL_FRONTDRIVER_FRONT_CODERTYPE://前驱+前转动编码器
        case SINGLESTEERWHEEL_FRONTDRIVER_BACK_CODERTYPE://前驱+后转动编码器
        case SINGLESTEERWHEEL_BACKDRIVER_FRONT_CODERTYPE://后驱+前转动编码器
        case SINGLESTEERWHEEL_BACKDRIVER_BACK_CODERTYPE://后驱+后转动编码器
            robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.a_axis_distance=robot_model_data.chasisModel.SingleWheelDriver.a_axis_distance;
            robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.b_axis_distance=robot_model_data.chasisModel.SingleWheelDriver.b_axis_distance;
            robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.absEncoderType=robot_model_data.chasisModel.SingleWheelDriver.absEncoderType;
            robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.steer_angle_min=robot_model_data.chasisModel.SingleWheelDriver.steer_angle_min;
            robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.steer_angle_max=robot_model_data.chasisModel.SingleWheelDriver.steer_angle_max;
            robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.absEncoderRange_min=robot_model_data.chasisModel.SingleWheelDriver.absEncoderRange_min;
            robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.absEncoderRange_max=robot_model_data.chasisModel.SingleWheelDriver.absEncoderRange_max;
            robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.absEncoderType = robot_model_data.chasisModel.SingleWheelDriver.absEncoderType;
            break;
        default:
            break;
        }
    }
    else if(robot_model_data.shapType_==CIRCLE)
    {
        robot_model_->robot_circle_model_data_.radius_ = robot_model_data.radius_;
        robot_model_->robot_circle_model_data_.wheelRadius_= robot_model_data.wheelRadius_;
        robot_model_->robot_circle_model_data_.chasisType_=robot_model_data.chasisType_;
        robot_model_->robot_circle_model_data_.mvStyle_ = robot_model_data.mvStyle_;
        robot_model_->setMOriRadius(robot_model_->robot_circle_model_data_.radius_);//设置初始值用于undo redo
        switch (robot_model_->robot_circle_model_data_.chasisType_) {
        case DIFFERENTIAL_TWOWHEEL://差动两轮
            robot_model_->robot_circle_model_data_.chasisModel.DiffTwoWheel.a_axis_distance=robot_model_data.chasisModel.DiffTwoWheel.a_axis_distance;
            break;
        case DIFFERENTIAL_FOURWHEEL://差动四轮
            robot_model_->robot_circle_model_data_.chasisModel.DiffFourWheel.a_axis_distance= robot_model_data.chasisModel.DiffFourWheel.a_axis_distance;
            robot_model_->robot_circle_model_data_.chasisModel.DiffFourWheel.b_axis_distance= robot_model_data.chasisModel.DiffFourWheel.b_axis_distance;
            break;
        case ALLDIRECTION_THREEWHEEL://全向三轮
            robot_model_->robot_circle_model_data_.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance=robot_model_data.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance;
            robot_model_->robot_circle_model_data_.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle = robot_model_data.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle;
            break;
        default:
            break;
        }
    }
    robot_model_->update_model ();
}

void ModelGraphicsScene::parse_json_polygon(const QJsonObject &json_object_polygon, const quint8 &poly_type)
{
    QJsonArray json_array_polygon=json_object_polygon["index"].toArray ();
    int c_size=qMin(json_array_polygon.size(),json_object_polygon["num"].toInt());

    for(int ti=0;ti<c_size;ti++)
    {
        QJsonObject j_object=json_array_polygon[ti].toObject();
        ItemPolygon *ipoly=new ItemPolygon(this,ItemPolygon::PolyType(poly_type),j_object);
        addItem (ipoly);
        ipoly->stackBefore(robot_model_);
        polygon_list_.append (ipoly);

        ipoly->setZValue(getItemAcount());//设置新建item的层级
    }
}

void ModelGraphicsScene::parse_json_laser(const QJsonObject &json_object_laser)
{
    QJsonArray json_array_laser=json_object_laser["index"].toArray ();

    int c_size=qMin(json_array_laser.size(),json_object_laser["num"].toInt());

    for(int ti=0;ti<c_size;ti++)
    {
        QJsonObject j_object=json_array_laser[ti].toObject ();
        ItemLaser::count = 1;//标记不是引导创建
        ItemLaser *ilaser=new ItemLaser(this,j_object);
        addItem (ilaser);
        laser_list_.append (ilaser);

        ilaser->setZValue(getItemAcount());//设置新建item的层级
    }
}

void ModelGraphicsScene::parse_json_ultrasonic(const QJsonObject &json_object_ultrasonic)
{

    model_view()->basic_widget()->model_base_data_.max_ultrasonic_num_ = json_object_ultrasonic["maxUltrasonicNum"].toInt();
    model_view()->basic_widget()->model_base_data_.ultrasonic_ = json_object_ultrasonic["type"].toInt();

    QJsonArray json_array_ultrasonic=json_object_ultrasonic["index"].toArray ();

    int c_size=qMin(json_array_ultrasonic.size(),json_object_ultrasonic["num"].toInt());
    model_view()->basic_widget()->set_ultrasonic_minNum(c_size);
    for(int ti=0;ti<c_size;ti++)
    {
        QJsonObject j_object=json_array_ultrasonic[ti].toObject ();
        ItemUltrasonic::count = 1;//标记不是引导创建
        ItemUltrasonic *iul=new ItemUltrasonic(this,j_object);
        addItem (iul);
        ultrasonic_list_.append (iul);

        iul->setZValue(getItemAcount());//设置新建item的层级
    }
}

void ModelGraphicsScene::parse_json_di(const QJsonObject &json_object_di)
{
    model_view()->basic_widget()->model_base_data_.max_di_num_ = json_object_di["maxDINum"].toInt();

    QJsonArray json_array_di=json_object_di["index"].toArray ();
    int c_size=qMin(json_object_di["num"].toInt(),json_array_di.size());
    model_view()->basic_widget()->set_di_minNum(c_size);
    for(int ti=0;ti<c_size;ti++)
    {
        QJsonObject j_object=json_array_di[ti].toObject ();

        //分类型：防撞型DI 和 非防撞型DI
        QJsonValue typeValue = j_object["type"];

        if(typeValue.toInt() == 3)
        {
            ItemCrashDI::count = 1;//标记不是引导创建
            ItemCrashDI*  crashDi = new ItemCrashDI(this,j_object);
            addItem(crashDi);
            crashDi_list_.append(crashDi);
            crashDi->setZValue(getItemAcount());
        }
        else
        {
            ItemDI::count = 1;//标记不是引导创建
            ItemDI *idi=new ItemDI(this,j_object);
            addItem (idi);
            di_list_.append (idi);
            idi->setZValue(getItemAcount());//设置新建item的层级
        }
    }
}

void ModelGraphicsScene::parse_json_do(const QJsonObject &json_object_do)
{
    model_view()->basic_widget()->model_base_data_.max_do_num_ = json_object_do["maxDONum"].toInt();
    QJsonArray json_array_do=json_object_do["index"].toArray ();

    int c_size=qMin(json_object_do["num"].toInt(),json_array_do.size());
    model_view()->basic_widget()->set_do_minNum(c_size);
    for(int ti=0;ti<c_size;ti++)
    {
        QJsonObject j_object=json_array_do[ti].toObject ();
        ItemDO::count = 1;//标记不是引导创建
        ItemDO *ido=new ItemDO(this,j_object);
        addItem (ido);
        do_list_.append (ido);
        ido->setZValue(getItemAcount());//设置新建item的层级
    }

    QList<quint8> dil;
    QJsonArray json_array_do_info=json_object_do["defaultValue"].toArray();
    foreach (auto var_j, json_array_do_info) {
        dil.append(var_j.toInt());
    }
    model_view()->basic_widget()->set_do_info_list(dil);
}

void ModelGraphicsScene::parse_json_magnetic(const QJsonObject &json_magnetic)
{
    model_view()->basic_widget()->model_base_data_.magnetic_sensor_ = json_magnetic["type"].toInt();

    QJsonArray json_array_magnetic=json_magnetic["index"].toArray ();

    int c_size=qMin(json_magnetic["num"].toInt(),json_array_magnetic.size());

    for(int ti=0;ti<c_size;ti++)
    {
        QJsonObject j_object=json_array_magnetic[ti].toObject ();
        ItemMagneticSensor::count = 1;//标记不是引导创建
        ItemMagneticSensor *ims=new ItemMagneticSensor(this,j_object);
        addItem (ims);
        magneticSensor_list_.append (ims);
        ims->setZValue(getItemAcount());//设置新建item的层级
    }
}

void ModelGraphicsScene::parse_json_RFID(const QJsonObject &json_RFID)
{
    model_view()->basic_widget()->model_base_data_.RFID_ = json_RFID["type"].toInt();

    QJsonArray json_array_RFID=json_RFID["index"].toArray ();

    int c_size=qMin(json_RFID["num"].toInt(),json_array_RFID.size());
    for(int ti=0;ti<c_size;ti++)
    {
        QJsonObject json_RFID = json_array_RFID[ti].toObject();
        ItemRFIDSensor* RFID = new ItemRFIDSensor(this,json_RFID);
        addItem(RFID);
        RFID_list_.append(RFID);
        RFID->setZValue(getItemAcount());
    }

}

void ModelGraphicsScene::parse_json_Cameral(const QJsonObject &json_Cameral)
{
    QJsonArray json_array = json_Cameral["index"].toArray();

    int c_size = qMin(json_Cameral["num"].toInt(),json_array.size());
    for(int ti = 0;ti<c_size;ti++)
    {
        QJsonObject json_Camera = json_array[ti].toObject();
        ItemCamera* camera = new ItemCamera(this,json_Camera);
        addItem(camera);
        camera_list_.append(camera);
        camera->setZValue(getItemAcount());
    }
}

void ModelGraphicsScene::parse_json_Inverse(QList<bool> list)
{
    robot_model_->inverse_list_  = list;
    robot_model_->robot_circle_model_data_.inverse_ = list;
    robot_model_->robot_model_data_.inverse_ = list;
}

void ModelGraphicsScene::parse_json_Inverse(QJsonArray &array)
{
    robot_model_->inverse_list_.clear();
    robot_model_->robot_circle_model_data_.inverse_.clear();
    robot_model_->robot_model_data_.inverse_.clear();
    for(int i = 0; i<array.size();i++)
    {
        robot_model_->inverse_list_.append(array.at(i).toBool());
        robot_model_->robot_model_data_.inverse_.append(array.at(i).toBool());
        robot_model_->robot_circle_model_data_.inverse_.append(array.at(i).toBool());
    }
}

void ModelGraphicsScene::parse_json_driverID(QJsonArray &array)
{
    robot_model_->driverId_list_.clear();
    robot_model_->robot_model_data_.driverID.clear();
    robot_model_->robot_circle_model_data_.driverID.clear();

    for(int i = 0; i<array.size();i++)
    {
        robot_model_->driverId_list_.append(array.at(i).toInt());
        robot_model_->robot_model_data_.driverID.append(array.at(i).toInt());
        robot_model_->robot_circle_model_data_.driverID.append(array.at(i).toInt());
    }
}

QVariantMap ModelGraphicsScene::get_polygon_data(const quint8 &type)
{
    QVariantMap variant_polygon_map;

    QVariantList variant_list_polygon;
    foreach (auto var, polygon_list_)
    {
        if(int(var->poly_type_)==type)
        {
            variant_list_polygon.append (var->get_polygon_data ());
        }
    }
    variant_polygon_map.insert ("num",variant_list_polygon.size ());
    variant_polygon_map.insert ("index",variant_list_polygon);

    return variant_polygon_map;
}

QVariantMap ModelGraphicsScene::get_laser_data()
{
    //将用于定位的激光放在第一个
    for(int i = 0; i<laser_list_.size();i++)
    {
        ItemLaser * laser = NULL;
        if(laser_list_.at(i)->data_laser_.use_for_localization_)
        {
            laser = laser_list_.at(i);
            laser_list_.removeAt(i);
            laser_list_.push_front(laser);
            break;
        }
    }

    QVariantMap variant_map_laser;

    QVariantList variant_list_laser;
    foreach (auto var, laser_list_)
    {
        variant_list_laser.push_back(var->get_laser_data ());
    }

    variant_map_laser.insert ("num",variant_list_laser.size ());
    variant_map_laser.insert ("index",variant_list_laser);

    return variant_map_laser;
}

QVariantMap ModelGraphicsScene::get_ultrasonic_data()
{
    QVariantMap variant_map_ultrasonic;

    QVariantList variant_list_ultrasonic;
    foreach (auto var, ultrasonic_list_)
    {
        variant_list_ultrasonic.append (var->get_ultrasonic_data ());
    }
    variant_map_ultrasonic.insert("type",model_view()->basic_widget()->model_base_data_.ultrasonic_);
    variant_map_ultrasonic.insert("maxUltrasonicNum",model_view()->basic_widget()->model_base_data_.max_ultrasonic_num_);
    variant_map_ultrasonic.insert ("num",variant_list_ultrasonic.size ());
    variant_map_ultrasonic.insert ("index",variant_list_ultrasonic);

    return variant_map_ultrasonic;
}

QVariantMap ModelGraphicsScene::get_di_data()
{
    QVariantMap variant_map_di;

    QVariantList variant_list_di;
    foreach (auto var, di_list_)
    {
        variant_list_di.append (var->get_di_data ());
    }

    foreach (auto var, crashDi_list_) {

        variant_list_di.append(var->getCrashDi_data());
    }

    variant_map_di.insert("maxDINum",model_view()->basic_widget()->model_base_data_.max_di_num_);
    variant_map_di.insert ("num",variant_list_di.size ());
    variant_map_di.insert ("index",variant_list_di);
    return variant_map_di;
}

QVariantMap ModelGraphicsScene::get_magnetic_data()
{
    QVariantMap variant_map_magnetic;

    QVariantList variant_list_magnetic;
    foreach (auto var, magneticSensor_list_)
    {
        variant_list_magnetic.append (var->get_magnetic_data ());
    }
    variant_map_magnetic.insert("type",model_view()->basic_widget()->model_base_data_.magnetic_sensor_);
    variant_map_magnetic.insert ("num",variant_list_magnetic.size ());
    variant_map_magnetic.insert ("index",variant_list_magnetic);

    return variant_map_magnetic;
}

QVariantMap ModelGraphicsScene::get_do_data()
{
    QVariantMap variant_map_do;

    QVariantList variant_list_do;
    foreach (auto var, do_list_)
    {
        variant_list_do.append (var->get_do_data ());
    }

    variant_map_do.insert("maxDONum",model_view()->basic_widget()->model_base_data_.max_do_num_);
    variant_map_do.insert ("num",variant_list_do.size ());
    variant_map_do.insert ("index",variant_list_do);

    QVariantList do_info_list;
    for(int ti=0;ti<model_view()->basic_widget()->get_do_info_list().size();ti++)
    {
        do_info_list.append(model_view()->basic_widget()->get_do_info_list().at(ti));
    }
    variant_map_do.insert("defaultValue",do_info_list);

    return variant_map_do;
}

QVariantMap ModelGraphicsScene::get_RFID_data()
{
    QVariantMap variant_map_RFID;
    QVariantList variant_list_RFID;
    variant_map_RFID.insert("type",model_view()->basic_widget()->model_base_data_.RFID_);
    foreach ( auto var, RFID_list_) {
        QVariantMap variant_location_map;
        variant_location_map.insert("x",var->data_RFID_.x_);
        variant_location_map.insert("y",var->data_RFID_.y_);
        variant_location_map.insert("z",var->data_RFID_.z_);
        variant_list_RFID.append(variant_location_map);
    }
    variant_map_RFID.insert ("num",RFID_list_.size ());
    variant_map_RFID.insert ("index",variant_list_RFID);

    return variant_map_RFID;
}

QVariantMap ModelGraphicsScene::get_Camera_data()
{
    QVariantMap variant_map_Camera;
    QVariantList variant_list_camera;
    foreach (auto var, camera_list_) {
        QVariantMap variant_location_map;
        variant_location_map.insert("type",var->data_camera_.type_);
        variant_location_map.insert("x",var->data_camera_.x_);
        variant_location_map.insert("y",var->data_camera_.y_);
        variant_location_map.insert("z",var->data_camera_.z_);
        variant_location_map.insert("roll",var->data_camera_.roll_);
        variant_location_map.insert("pitch",var->data_camera_.pitch_);
        variant_location_map.insert("yaw",var->data_camera_.yaw_);
        variant_list_camera.append(variant_location_map);
    }
    variant_map_Camera.insert("num",camera_list_.size());
    variant_map_Camera.insert("index",variant_list_camera);

    return variant_map_Camera;
}

QVariantList ModelGraphicsScene::getRobotInverse_data()
{
    QVariantList inverseList;

    if(robot_model_->robot_model_data_.shapType_==RECTANGLE)
    {

        QList<bool> list =  robot_model_->robot_model_data_.inverse_;
        foreach ( auto var, list) {

            inverseList.append(var);
        }

    }
    else if(robot_model_->robot_model_data_.shapType_==CIRCLE)
    {

        QList<bool> list =  robot_model_->robot_circle_model_data_.inverse_;

        foreach ( auto var, list) {
            inverseList.append(var);
        }
    }

    return inverseList;
}

QVariantList ModelGraphicsScene::getRobotDriverId_data()
{
    QVariantList driverList;

    if(robot_model_->robot_model_data_.shapType_ == RECTANGLE)
    {
        QList<int> list1 = robot_model_->robot_model_data_.driverID;
        foreach (auto var, list1) {
            driverList.append(var);
        }
    }
    else if(robot_model_->robot_model_data_.shapType_==CIRCLE)
    {
        QList<int> list1 = robot_model_->robot_circle_model_data_.driverID;
        foreach (auto var, list1) {
            driverList.append(var);
        }
    }
    return driverList;
}

QList<ItemBase *> ModelGraphicsScene::find_items(const QPointF &pos)
{
    if(pos.isNull())
    {
        items_under_pos_list_.clear ();
        return items_under_pos_list_;
    }

    QRectF rectf(pos.x() - 1, pos.y() - 1, 2, 2);
    items_under_pos_list_.clear ();
    foreach (auto var,items(rectf))
    {
        //        if(var->type ()==6001)
        //            continue;
        if(var->type ()>=2000)
            items_under_pos_list_.append (qgraphicsitem_cast<ItemBase *>(var));
    }
    return items_under_pos_list_;
}

QList<ItemPolygon *>* ModelGraphicsScene::get_PolygonList()
{
    return &polygon_list_;
}

QList<ItemLaser *>* ModelGraphicsScene::get_Laserlist()
{
    return &laser_list_;
}

QList<ItemDI *>* ModelGraphicsScene::get_Di_list()
{
    return &di_list_;
}

QList<ItemDO *>* ModelGraphicsScene::get_Do_list()
{
    return &do_list_;
}

QList<ItemUltrasonic *>* ModelGraphicsScene::get_ultrasonic_list()
{
    return &ultrasonic_list_;
}


QList<ItemMagneticSensor *>* ModelGraphicsScene::get_magneticSensor_list()
{
    return &magneticSensor_list_;
}

QList<ItemCrashDI *> *ModelGraphicsScene::get_crashDi_list()
{
    return & crashDi_list_;
}

QList<ItemRFIDSensor *> *ModelGraphicsScene::get_RFID_list()
{
    return &RFID_list_;
}

QList<ItemCamera *> *ModelGraphicsScene::get_Camera_list()
{
    return &camera_list_;
}

int ModelGraphicsScene::laserForLocationNum()
{
    int count = 0;
    for(int i = 0; i<laser_list_.size();i++)
    {
        if(laser_list_.at(i)->data_laser_.use_for_localization_==true)
        {
            count++;
        }
    }
    return count;
}

void ModelGraphicsScene::setCurrentPressItem(ItemBase *curItem)
{
    m_currentPressItem = curItem;
}

ItemBase *ModelGraphicsScene::getCurrentPressItem()
{
    return m_currentPressItem;
}



