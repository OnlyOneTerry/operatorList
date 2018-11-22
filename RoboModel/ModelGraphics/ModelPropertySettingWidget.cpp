#include "ModelPropertySettingWidget.h"
#include "ui_ModelPropertySettingWidget.h"
#include <QDebug>
#include "ItemBase.h"
#include "WidgetSetting.h"
#include "ModelGraphicsView.h"
#include"ModelCustomCommand.h"
#include"UndoStack.h"
#include"ItemScanArea.h"
#include"ItemDI.h"
#include"ItemUltrasonic.h"
#include"ItemDO.h"
#include"ItemLaser.h"
#include"ItemMagneticSensor.h"
#include"ItemRobotModel.h"
#include"ItemCamera.h"
#include"ItemRFID.h"
#include"ItemCrashDI.h"
#include<QScrollBar>
ModelPropertySettingWidget::ModelPropertySettingWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ModelPropertySettingWidget),
    item_infor_widget_(new WidgetModelSetting()),
    m_lastMousePos(QPoint(0,0))
{
    ui->setupUi(this);
    ui->hline->setStyleSheet ("background-color: rgb(61,61,61);");
    connect (ui->listWidget_layer,SIGNAL(sigListWidgetChanged()),this,SLOT(slot_current_row_changed()));
    m_unstack = UndoStack::InitStack();
    connect(ui->listWidget_layer,SIGNAL(itemClicked(QListWidgetItem*)),this,SLOT(slot_moveToTop(QListWidgetItem*)));
    setAttribute(Qt::WA_AcceptTouchEvents);
}

ModelPropertySettingWidget::~ModelPropertySettingWidget()
{
    QLayoutItem* item = NULL;
    while(NULL!=(item=ui->vlayout_widget->takeAt(0)))
    {
        item->widget ()->close ();
    }
    delete ui;
}

void ModelPropertySettingWidget::update_property_widget(WidgetSetting *wid)
{
    QLayoutItem* item = NULL;
    while(NULL!=(item=ui->vlayout_widget->takeAt(0)))
    {
        qobject_cast<WidgetSetting *>(item->widget ())->close();
    }

    if(wid)
    {
        ui->vlayout_widget->addWidget (wid);
        if(wid->isHidden())
        {
            wid->show();
        }
        else
        {

        }
    }
    else
    {
        while(NULL!=(item=ui->vlayout_widget->takeAt(0)))
        {
            qobject_cast<WidgetSetting *>(item->widget ())->hide();
        }
    }
}

WidgetSetting *ModelPropertySettingWidget::get_current_property_widget() const
{
    WidgetSetting *ws=NULL;

    for(int ti=0;ti<ui->vlayout_widget->count ();ti++)
    {
        if(!ui->vlayout_widget->itemAt (ti)->isEmpty ())
        {
            ws=qobject_cast<WidgetSetting *>(ui->vlayout_widget->itemAt (ti)->widget ());
            break;
        }
    }
    return ws;
}

WidgetModelSetting *ModelPropertySettingWidget::getWidgetModelSetting()
{
    return item_infor_widget_;
}

void ModelPropertySettingWidget::setUndostack(QUndoStack* stack)
{
    m_unstack = stack;
}

QWidget *ModelPropertySettingWidget::getScrollcontentWidget()
{
    return ui->scrollAreaWidgetContents;
}

QUndoStack *ModelPropertySettingWidget::getUndoStack()
{
    return m_unstack;
}

void ModelPropertySettingWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(!m_bIsOnePoint) return;
    QPointF mouseDelta = event->pos() - m_lastMousePos;
    int deltay = mouseDelta.y();//竖直方向
    int pos = ui->scrollArea->verticalScrollBar()->value();
    if(abs(pos-deltay)>10)
        ui->scrollArea->verticalScrollBar()->setValue(pos-deltay*2);
    m_lastMousePos = event->pos();
    QWidget::mouseMoveEvent(event);
}

void ModelPropertySettingWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        m_lastMousePos = event->pos();
}

bool ModelPropertySettingWidget::event(QEvent *event)
{
    switch(event->type())
    {
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:
    {
        QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
        QList<QTouchEvent::TouchPoint> points = touchEvent->touchPoints();
        int num = points.count();
        qDebug()<<"points size is ----------------"<<num;
        event->accept();
        if(num==1)
        {
            m_bIsOnePoint = true;
        }
        else
        {
            m_bIsOnePoint = false;
        }
        break;
    }
    default:
        break;
    }
    return QWidget::event(event);
}

void ModelPropertySettingWidget::showTopItemWid(ItemBase *item)
{
    qDebug()<<"item type is ---------"<<item->type();
    foreach(auto var,selected_item_list_)
    {
        var->setSelected(false);
    }
    switch (item->type()) {
    case I_di:
        qgraphicsitem_cast<ItemDI*>(item)->add_setting_widget ();
        qgraphicsitem_cast<ItemDI*>(item)->setSelected(true);
        break;
    case I_do:
        qgraphicsitem_cast<ItemDO*>(item)->add_setting_widget ();
        qgraphicsitem_cast<ItemDO*>(item)->setSelected(true);

        break;
    case I_laser:
        qgraphicsitem_cast<ItemLaser*>(item)->add_setting_widget ();
        qgraphicsitem_cast<ItemLaser*>(item)->setSelected(true);
        break;
    case I_magnetic_sensor:
        qgraphicsitem_cast<ItemMagneticSensor*>(item)->add_setting_widget ();
        qgraphicsitem_cast<ItemMagneticSensor*>(item)->setSelected(true);
        break;
    case I_ultrasonic:
        qgraphicsitem_cast<ItemUltrasonic*>(item)->add_setting_widget ();
        qgraphicsitem_cast<ItemUltrasonic*>(item)->setSelected(true);
        break;
    case I_robot_model:
        qgraphicsitem_cast<ItemRobotModel*>(item)->add_setting_widget ();
        qgraphicsitem_cast<ItemRobotModel*>(item)->setSelected(true);
        break;
    case I_crashDi:
        qgraphicsitem_cast<ItemCrashDI*>(item)->add_setting_widget ();
        qgraphicsitem_cast<ItemCrashDI*>(item)->setSelected(true);
        break;
    case I_Camera:
        qgraphicsitem_cast<ItemCamera*>(item)->add_setting_widget ();
        qgraphicsitem_cast<ItemCamera*>(item)->setSelected(true);
        break;
    case I_RFID_sensor:
        qgraphicsitem_cast<ItemRFIDSensor*>(item)->add_setting_widget ();
        qgraphicsitem_cast<ItemRFIDSensor*>(item)->setSelected(true);
        break;
    default:
        break;
    }
    update_property_widget(item->getWidgetSetting());
}

void ModelPropertySettingWidget::update_layer(QList<ItemBase *> selected_item_list)
{
    selected_item_list_=selected_item_list;
    QListWidgetItem *lw=NULL;
    while (NULL!=(lw=ui->listWidget_layer->takeItem (0)))
    {
        delete lw;
    }
    if(selected_item_list.count()==0)
    {
        update_property_widget(dynamic_cast<WidgetSetting *>(item_infor_widget_));
        return;
    }
    int temp_count = 0;
    foreach (auto var, selected_item_list_)
    {
        if(var->type()==I_angle_controller||var->type()==I_robot_model) continue;

        QListWidgetItem *item = new QListWidgetItem(ui->listWidget_layer);
        var->layer_= temp_count;
        if(QString::fromLocal8Bit(var->metaObject ()->className())=="ItemCrashDI")//统一crashDI 与其他DI名称为ItemDI
        {
            item->setText (QString("ItemDI_%1").arg(var->getId()));
        }
        else if(QString::fromLocal8Bit(var->metaObject ()->className())=="ItemUltrasonic")
        {
            item->setText (tr("Ultrasonic")+QString("_%1").arg(var->getId()));
        }
        else if(QString::fromLocal8Bit(var->metaObject ()->className())=="ItemLaser")
        {
            item->setText (tr("Laser")+QString("_%1").arg(var->getId()));
        }
        else if(QString::fromLocal8Bit(var->metaObject ()->className())=="ItemCamera")
        {
            item->setText (tr("Camera")+QString("_%1").arg(var->getId()));
        }
        else if(QString::fromLocal8Bit(var->metaObject ()->className())=="ItemMagneticSensor")
        {
            item->setText (tr("MagneticSensor")+QString("_%1").arg(var->getId()));
        }
        else if(QString::fromLocal8Bit(var->metaObject ()->className())=="ItemScanArea")//扫描区域Item
        {
            if(qgraphicsitem_cast<ItemScanArea*>(var)->getParent()->type()== I_di)
            {
                item->setText(QString("DIScanArea_%1").arg(var->getId()));
            }
            else if(qgraphicsitem_cast<ItemScanArea*>(var)->getParent()->type() ==I_ultrasonic)
            {
                item->setText(QString("UltrasonicScanArea_%1").arg(var->getId()));
            }
        }
        else
        {
            item->setText (QString("%1").arg (var->metaObject ()->className ())+QString("_%1").arg(var->getId()));
        }

        item->setData (Qt::UserRole,var->layer_);
        item->setToolTip(tr("click to top"));
        ui->listWidget_layer->addItem (item);
        temp_count++;
    }

    temp_count = 0;
    m_ori_itemlist.clear();
    foreach (auto var, selected_item_list_)
    {
        QListWidgetItem *item = new QListWidgetItem;
        var->layer_= temp_count;
        if(QString::fromLocal8Bit(var->metaObject ()->className())=="ItemUltrasonic")
        {
            item->setText (tr("Ultrasonic")+QString("_%1").arg(var->getId()));
        }
        else if(QString::fromLocal8Bit(var->metaObject ()->className())=="ItemLaser")
        {
            item->setText (tr("Laser")+QString("_%1").arg(var->getId()));
        }
        else if(QString::fromLocal8Bit(var->metaObject ()->className())=="ItemCamera")
        {
            item->setText (tr("Camera")+QString("_%1").arg(var->getId()));
        }
        else if(QString::fromLocal8Bit(var->metaObject ()->className())=="ItemMagneticSensor")
        {
            item->setText (tr("MagneticSensor")+QString("_%1").arg(var->getId()));
        }
        else
        {
            item->setText (QString("%1").arg (var->metaObject()->className()+QString("_%1").arg(var->getId())));
        }
        item->setData (Qt::UserRole,var->layer_);
        m_ori_itemlist.push_back(item);
        temp_count++;
    }
}

void ModelPropertySettingWidget::slot_current_row_changed()
{
    if(m_unstack == NULL)
    {
        return;
    }

    m_new_itemlist.clear();

    for(int i=0; i< ui->listWidget_layer->count();i++)
    {
        QListWidgetItem *item = new QListWidgetItem;
        item->setText (ui->listWidget_layer->item(i)->text());
        item->setData (Qt::UserRole,ui->listWidget_layer->item(i)->data(Qt::UserRole));
        m_new_itemlist.push_back(item);
    }

    layerCommand* ly_command = new layerCommand(ui->listWidget_layer,selected_item_list_,m_ori_itemlist,m_new_itemlist,NULL);

    m_unstack->push(ly_command);

    m_ori_itemlist = m_new_itemlist;

}

void ModelPropertySettingWidget::slot_moveToTop(QListWidgetItem *item)
{
    //当前列表项置顶
    if(ui->listWidget_layer->currentRow()==0) return;
    QListWidgetItem* temp = ui->listWidget_layer->takeItem(ui->listWidget_layer->currentRow());
    ui->listWidget_layer->insertItem(0,temp);
    ui->listWidget_layer->scrollToTop();
    //对应的场景item置顶
    int tepIndex= 3;
    for(int ti=0;ti < ui->listWidget_layer->count(); ti++)
    {
        int z= ui->listWidget_layer->item(ti)->data (Qt::UserRole).toInt ();
        //设置zValue来管理层级关系时，若itemA有子subitem时若想设置itemB与subItem的层级关系要设置itemA与itemB的zValue的关系
        //zValue scene先画层级低的item， subitem属于itemA，要通过设置itemA的zValue来达到控制subitem的层级
        if(selected_item_list_.at(z)->type()==I_scanArea_model)
        {
            if(dynamic_cast<ItemScanArea*>(selected_item_list_.at(z))->getParent()->type()==I_di)
            {
                dynamic_cast<ItemScanArea*>(selected_item_list_.at(z))->getParent()->setZValue(tepIndex);
                tepIndex--;
            }
            else if(dynamic_cast<ItemScanArea*>(selected_item_list_.at(z))->getParent()->type()==I_ultrasonic)
            {
                dynamic_cast<ItemScanArea*>(selected_item_list_.at(z))->getParent()->setZValue(tepIndex);
                tepIndex--;
            }

        }
        else
        {
            selected_item_list_.at(z)->setZValue(tepIndex);
            tepIndex--;
        }
    }
    int idx = ui->listWidget_layer->item(0)->data (Qt::UserRole).toInt ();
    showTopItemWid(selected_item_list_.at(idx));
    selected_item_list_.last ()->model_scene ()->update ();
    ui->listWidget_layer->setCurrentRow(0);//焦点设为首行
}
