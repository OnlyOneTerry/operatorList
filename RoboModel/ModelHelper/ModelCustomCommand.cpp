#include "ModelCustomCommand.h"
#include "ItemRobotModel.h"
#include"ItemAngelCtroller.h"
#include"ItemPolygon.h"
#include"ItemPolypos.h"
#include"ItemDO.h"
#include"ItemDI.h"
#include"ItemLaser.h"
#include"ItemUltrasonic.h"
#include"ModelGraphicsView.h"
#include<QGraphicsScene>
#include<QCoreApplication>
#include"ItemMagneticSensor.h"
#include"WidgetDISetting.h"
#include"WidgetCrashDISetting.h"
#include"WidgetModelSetting.h"
#include"WidgetDOSetting.h"
#include"WidgetLaserSetting.h"
#include"WidgetPosSetting.h"
#include"WidgetRobotModelSetting.h"
#include"WidgetCircleRobotModelSetting.h"
#include"WidgetCrashDiPosSetting.h"
#include"WidgetUltrasonicSetting.h"
#include"WidgetMagneticSensorSetting.h"
#include"WidgetPlygonSetting.h"
#include"WidgetRFIDsetting.h"
#include"ItemPolygon.h"
#include"ItemDI.h"
#include"ItemDO.h"
#include"ItemPolypos.h"
#include"ItemPolygon.h"
#include"ItemCrashDI.h"
#include"ItemCrashDIpos.h"
#include"ItemCamera.h"
#include "RoLog.h"
#include"GlobalVariant.h"
#include"ItemRFID.h"
#include"ItemScanArea.h"
#include"ModelCommon.h"
customboxCommand::customboxCommand(customComboBox *box1, int ori_index, int new_index, PropertyBasicWidget *parentWid, QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_combox1 = box1;
    m_parentWid = parentWid;
    m_oriIndex = ori_index;
    m_newIndex = new_index;
}

void customboxCommand::undo()
{
    SCDebug<<"customboxCommand undo---ori index is "<<m_oriIndex ;
    m_combox1->setIsClickedTrigger(false);
    m_combox1->setCurrentIndex(m_oriIndex);
    undoToShowRelativeWidget(m_parentWid);
}

void customboxCommand::redo()
{
    qDebug()<<"redo ----------------------1---------------";
    m_combox1->setIsClickedTrigger(false);
    m_combox1->setCurrentIndex(m_newIndex);
    undoToShowRelativeWidget(m_parentWid);
}

lineEditCommand::lineEditCommand(QLineEdit *lineEdit, QString ori_text, QString new_text, QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_lineEdit = lineEdit;
    m_oritext = ori_text;
    m_newtext = new_text;
}

void lineEditCommand::undo()
{
    m_lineEdit->blockSignals(true);
    m_lineEdit->setText(m_oritext);
    m_lineEdit->blockSignals(false);
}

void lineEditCommand::redo()
{
    qDebug()<<"redo ----------------------4---------------";
    SCDebug<<"lineEditcommand redo ";
    m_lineEdit->blockSignals(true);
    m_lineEdit->setText(m_newtext);
    m_lineEdit->blockSignals(false);

}

checkCommand::checkCommand(QCheckBox *checkbox, bool ori_check, bool new_check, PropertyBasicWidget *parentWid, QUndoCommand *parent)
{
    m_checkBox = checkbox;
    m_oriCheck = ori_check;
    m_newCheck = new_check;
    m_parentWid = parentWid;
}

void checkCommand::undo()
{
    m_checkBox->blockSignals(true);
    m_checkBox->setChecked(m_oriCheck);
    m_checkBox->blockSignals(false);
    undoToShowRelativeWidget(m_parentWid);
}

void checkCommand::redo()
{
    qDebug()<<"redo ----------------------6---------------";
    m_checkBox->blockSignals(true);
    m_checkBox->setChecked(m_newCheck);
    m_checkBox->blockSignals(false);
    undoToShowRelativeWidget(m_parentWid);
}

doubleSpinBoxCommand::doubleSpinBoxCommand(customDoubleSpinBox *spinBox,ItemBase *item, double ori_value, double new_value,PropertyBasicWidget* parentWid, QUndoCommand *parent):
    QUndoCommand(parent),
    m_item(item),
    m_doubleSpinBox(spinBox),
    m_oriValue(ori_value),
    m_newValue(new_value),
    m_parentWid(parentWid)
{

}

void doubleSpinBoxCommand::undo()
{
    if(m_item)
    {

        m_doubleSpinBox->setIsClickedTrigger(false);

        m_doubleSpinBox->setValue(m_oriValue);

        if(m_item->type()==ItemDO::Type)
        {
            ItemDO *temp_item=qgraphicsitem_cast<ItemDO *>(m_item);
            temp_item->update_item();
        }
        else if(m_item->type()==ItemMagneticSensor::Type)
        {
            ItemMagneticSensor *temp_item=qgraphicsitem_cast<ItemMagneticSensor *>(m_item);
            temp_item->update_item();
        }
        m_item->model_scene()->model_view()->update();
    }
    else
    {
        m_doubleSpinBox->setIsClickedTrigger(false);

        m_doubleSpinBox->setValue(m_oriValue);
    }
    undoToShowRelativeWidget(m_parentWid);
    qDebug()<<"current operation is -----"<<GlobalVariant::getInst()->isRedo;
}

void doubleSpinBoxCommand::redo()
{
    qDebug()<<"redo ----------------------7---------------";
    if(m_item)
    {

        m_doubleSpinBox->setIsClickedTrigger(false);

        if(GlobalVariant::getInst()->isRedo)
        {
            m_doubleSpinBox->setValue(m_newValue);
            undoToShowRelativeWidget(m_parentWid);// ******此处调用否则导致属性窗口刷新控件数字自动补0
        }

        if(m_item->type()==ItemDO::Type)
        {
            ItemDO *temp_item=qgraphicsitem_cast<ItemDO *>(m_item);
            temp_item->update_item();
        }
        else if(m_item->type()==ItemMagneticSensor::Type)
        {
            ItemMagneticSensor *temp_item=qgraphicsitem_cast<ItemMagneticSensor *>(m_item);
            temp_item->update_item();
        }
        m_item->model_scene()->model_view()->update();
    }
    else
    {
        SCDebug<<"mdoublspinbox is "<<m_doubleSpinBox;
        m_doubleSpinBox->setIsClickedTrigger(false);
        if(GlobalVariant::getInst()->isRedo)
        {
            m_doubleSpinBox->setValue(m_newValue);
            undoToShowRelativeWidget(m_parentWid);// ******此处调用否则导致属性窗口刷新控件数字自动补0
        }
    }

}

SpinBoxCommand::SpinBoxCommand(customSpinBox *spinBox, int ori_value, int new_value,PropertyBasicWidget *parentWid, QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_spinBox = spinBox;
    m_oriValue = ori_value;
    m_newValue = new_value;
    m_parentWid = parentWid;
}

void SpinBoxCommand::undo()
{
    m_spinBox->setIsClickedTrigger(false);
    m_spinBox->setValue(m_oriValue);
    undoToShowRelativeWidget(m_parentWid);// ******此处只在undo时调用，否则导致属性窗口刷新控件数字自动补0
}


void SpinBoxCommand::redo()
{
    qDebug()<<"redo ----------------------9---------------";
    SCDebug<<"spinbox command redo ...";
    m_spinBox->setIsClickedTrigger(false);
    qDebug()<<"current operation is -----"<<GlobalVariant::getInst()->isRedo;
    m_spinBox->setValue(m_newValue);
}


comboxCommand::comboxCommand(QComboBox *box, int ori_indx, int new_index,   QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_combox = box;
    m_oriIndex = ori_indx;
    m_newIndex = new_index;
}

void comboxCommand::undo()
{
    m_combox->blockSignals(true);
    m_combox->setCurrentIndex(m_oriIndex);
    m_combox->blockSignals(false);
}

void comboxCommand::redo()
{
    qDebug()<<"redo ----------------------3---------------";
    m_combox->blockSignals(true);
    m_combox->setCurrentIndex(m_newIndex);
    m_combox->blockSignals(false);

}

modelboxCommand::modelboxCommand(ItemBase *item,
                                 customComboBox *box1, customComboBox *box2, customComboBox* box3,
                                 DataIndex oriIndex , DataIndex newIndex ,
                                 QMap<WidgetType,QLabel*> labelMap, QMap<WidgetType, QDoubleSpinBox *> doubleSpinBoxMap,
                                 QMap<WidgetType, QSpinBox *> spinBoxMap,
                                 RobotModelData oriData, RobotModelData newData,
                                 PropertyBasicWidget* parentWid ,
                                 QUndoCommand *parent)
    :QUndoCommand(parent)
{
    m_oriIndex = oriIndex;
    m_newIndex = newIndex;
    moveTypeCombox = box1;
    chasisTypeComBox = box2;
    absEncoderTypeComBox = box3;
    m_item = item;
    m_DoubleSpinBoxMap = doubleSpinBoxMap;
    m_SpinBoxMap = spinBoxMap;
    m_LabelMap = labelMap;
    m_oriModelData = oriData;
    m_newModelData = newData;
    m_parentWid = parentWid;
}

void modelboxCommand::undo()
{

    SCDebug<<"undo ori index is "<<m_oriIndex.a_index<<"\t"<<m_oriIndex.b_index;
    //设置当前运动方式
    moveTypeCombox->blockSignals(true);
    moveTypeCombox->setCurrentIndex(m_oriIndex.a_index);
    moveTypeCombox->blockSignals(false);
    ItemRobotModel* robot_model_ = qgraphicsitem_cast<ItemRobotModel*>(m_item);
    //判断当前运动方式
    MoveStyle mvstyle = (MoveStyle)moveTypeCombox->currentData().toInt();
    robot_model_->setMoveStyle(mvstyle);

    if(robot_model_->getChasisShapeType()==RECTANGLE)
    {
        robot_model_->robot_model_data_.mvStyle_=mvstyle;

        switch (mvstyle) {
        case DIFFERENTIAL://差动
        {
            chasisTypeComBox->blockSignals(true);
            chasisTypeComBox->clear();
            chasisTypeComBox->addItem(QObject::tr("Two wheel differential"),DIFFERENTIAL_TWOWHEEL);
            chasisTypeComBox->addItem(QObject::tr("Four wheel differential"),DIFFERENTIAL_FOURWHEEL);
            chasisTypeComBox->blockSignals(false);
        }
            break;
        case ALLDIRECTION://全向
        {
            chasisTypeComBox->blockSignals(true);
            chasisTypeComBox->clear();
            chasisTypeComBox->addItem(QObject::tr("Three wheel omni"),ALLDIRECTION_THREEWHEEL);
            chasisTypeComBox->blockSignals(false);
        }
            break;
        case  MECANUM://马克纳姆
        {
            chasisTypeComBox->blockSignals(true);
            chasisTypeComBox->clear();
            chasisTypeComBox->addItem(QObject::tr("Four wheel Mecanum"),MECANUM_FOURWHEEL);
            chasisTypeComBox->blockSignals(false);
        }
            break;
        case SINGLESTEERWHEEL://单舵轮
        {
            chasisTypeComBox->blockSignals(true);
            chasisTypeComBox->clear();
            chasisTypeComBox->addItem(QObject::tr("Single rudder front drive and front encoder"),SINGLESTEERWHEEL_FRONTDRIVER_FRONT_CODERTYPE);
            chasisTypeComBox->addItem(QObject::tr("Single rudder front drive and rear encoder"),SINGLESTEERWHEEL_FRONTDRIVER_BACK_CODERTYPE);
            chasisTypeComBox->addItem(QObject::tr("Single rudder rear drive and rear encoder"),SINGLESTEERWHEEL_BACKDRIVER_FRONT_CODERTYPE);
            chasisTypeComBox->addItem(QObject::tr("Single rudder rear drive and rear encoder"),SINGLESTEERWHEEL_BACKDRIVER_BACK_CODERTYPE);
            chasisTypeComBox->blockSignals(false);
        }
            break;
        default:
            break;
        }
    }
    else if(robot_model_->getChasisShapeType()==CIRCLE)
    {
        robot_model_->robot_circle_model_data_.mvStyle_=mvstyle;
        switch (mvstyle) {
        case DIFFERENTIAL://差动
        {
            chasisTypeComBox->blockSignals(true);
            chasisTypeComBox->clear();
            chasisTypeComBox->addItem(QObject::tr("Two wheel differential"),DIFFERENTIAL_TWOWHEEL);
            chasisTypeComBox->addItem(QObject::tr("Four wheel differential"),DIFFERENTIAL_FOURWHEEL);
            chasisTypeComBox->blockSignals(false);

        }
            break;
        case ALLDIRECTION://全向
        {
            chasisTypeComBox->blockSignals(true);
            chasisTypeComBox->clear();
            chasisTypeComBox->addItem(QObject::tr("Three wheel omni"),ALLDIRECTION_THREEWHEEL);
            chasisTypeComBox->blockSignals(false);
        }
            break;
        default:
            break;
        }
    }

    chasisTypeComBox->blockSignals(true);
    chasisTypeComBox->setCurrentIndex(m_oriIndex.b_index);
    chasisTypeComBox->blockSignals(false);

    //底盘选型
    chasisTypeComboxchange(m_oriModelData);
    robot_model_->update_model ();

    //显示当前属性窗口
    WidgetRobotModelSetting* tempWid = dynamic_cast<WidgetRobotModelSetting*>(m_parentWid);
    ItemRobotModel*  temp_item = qgraphicsitem_cast<ItemRobotModel*>(tempWid->getRelateItem());
    temp_item->add_setting_widget();
    temp_item->model_scene()->model_view()->property_widget()->update_property_widget(temp_item->getWidgetSetting());
    setCurrentItemStatus(temp_item->model_scene(),m_parentWid);
    temp_item->update();
}

void modelboxCommand::redo()
{
    qDebug()<<"redo ----------------------2---------------";
    //设置当前运动方式
    moveTypeCombox->blockSignals(true);
    moveTypeCombox->setCurrentIndex(m_newIndex.a_index);
    moveTypeCombox->blockSignals(false);
    ItemRobotModel* robot_model_ = qgraphicsitem_cast<ItemRobotModel*>(m_item);
    //判断当前运动方式类型
    MoveStyle mvstyle = (MoveStyle)moveTypeCombox->currentData().toInt();
    robot_model_->setMoveStyle(mvstyle);

    if(robot_model_->getChasisShapeType()==RECTANGLE)
    {
        robot_model_->robot_model_data_.mvStyle_=mvstyle;

        switch (mvstyle) {
        case DIFFERENTIAL://差动
        {
            chasisTypeComBox->blockSignals(true);
            chasisTypeComBox->clear();
            chasisTypeComBox->addItem(QObject::tr("Two wheel differential"),DIFFERENTIAL_TWOWHEEL);
            chasisTypeComBox->addItem(QObject::tr("Four wheel differential"),DIFFERENTIAL_FOURWHEEL);
            chasisTypeComBox->blockSignals(false);

        }
            break;
        case ALLDIRECTION://全向
        {
            SCDebug<<"alldirection is invorked...";
            chasisTypeComBox->blockSignals(true);
            chasisTypeComBox->clear();
            chasisTypeComBox->addItem(QObject::tr("Three wheel omni"),ALLDIRECTION_THREEWHEEL);
            chasisTypeComBox->blockSignals(false);
        }
            break;
        case  MECANUM://马克纳姆
        {
            SCDebug<<"马克纳姆 is invorked...";
            chasisTypeComBox->blockSignals(true);
            chasisTypeComBox->clear();
            chasisTypeComBox->addItem(QObject::tr("Four wheel Mecanum"),MECANUM_FOURWHEEL);
            chasisTypeComBox->blockSignals(false);
        }
            break;
        case SINGLESTEERWHEEL://单舵轮
        {
            SCDebug<<"单舵轮 is invorked...";
            chasisTypeComBox->blockSignals(true);
            chasisTypeComBox->clear();
            chasisTypeComBox->addItem(QObject::tr("Single rudder front drive and front encoder"),SINGLESTEERWHEEL_FRONTDRIVER_FRONT_CODERTYPE);
            chasisTypeComBox->addItem(QObject::tr("Single rudder front drive and rear encoder"),SINGLESTEERWHEEL_FRONTDRIVER_BACK_CODERTYPE);
            chasisTypeComBox->addItem(QObject::tr("Single rudder rear drive and front encoder"),SINGLESTEERWHEEL_BACKDRIVER_FRONT_CODERTYPE);
            chasisTypeComBox->addItem(QObject::tr("Single rudder rear drive and rear encoder"),SINGLESTEERWHEEL_BACKDRIVER_BACK_CODERTYPE);
            chasisTypeComBox->blockSignals(false);
        }
            break;
        default:
            break;
        }

    }
    else if(robot_model_->getChasisShapeType()==CIRCLE)
    {
        robot_model_->robot_circle_model_data_.mvStyle_=mvstyle;
        switch (mvstyle) {
        case DIFFERENTIAL://差动
        {
            chasisTypeComBox->blockSignals(true);
            chasisTypeComBox->clear();
            chasisTypeComBox->addItem(QObject::tr("Two wheel differential"),DIFFERENTIAL_TWOWHEEL);
            chasisTypeComBox->addItem(QObject::tr("Four wheel differential"),DIFFERENTIAL_FOURWHEEL);
            chasisTypeComBox->blockSignals(false);

        }
            break;
        case ALLDIRECTION://全向
        {
            SCDebug<<"alldirection is invorked...";
            chasisTypeComBox->blockSignals(true);
            chasisTypeComBox->clear();
            chasisTypeComBox->addItem(QObject::tr("Three wheel omni"),ALLDIRECTION_THREEWHEEL);
            chasisTypeComBox->blockSignals(false);
        }
            break;
        default:
            break;
        }
    }
    chasisTypeComBox->blockSignals(true);
    chasisTypeComBox->setCurrentIndex(m_newIndex.b_index);
    chasisTypeComBox->blockSignals(false);
    //底盘选型
    chasisTypeComboxchange(m_newModelData);
    robot_model_->slot_recover_polygons();//鼠标点击切换到最新模型为底盘为初始状态

    //显示当前属性窗口
    WidgetRobotModelSetting* tempWid = dynamic_cast<WidgetRobotModelSetting*>(m_parentWid);
    ItemRobotModel*  temp_item = qgraphicsitem_cast<ItemRobotModel*>(tempWid->getRelateItem());
    temp_item->add_setting_widget();
    temp_item->model_scene()->model_view()->property_widget()->update_property_widget(temp_item->getWidgetSetting());
    setCurrentItemStatus(temp_item->model_scene(),m_parentWid);
    temp_item->update();
}

void modelboxCommand::chasisTypeComboxchange(RobotModelData& modelData)
{
    ItemRobotModel* robot_model_ = qgraphicsitem_cast<ItemRobotModel*>(m_item);
    //    //切换时将头，尾，宽，半径编辑框从新设置为初始值
    if(modelData.shapType_==RECTANGLE)
    {
        SCDebug<<"keep track of the error 2=======";
        QMap<WidgetType,QDoubleSpinBox*>::iterator iter =  m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_HEAD);
        iter.value()->blockSignals(true);
        iter.value()->setValue(modelData.head_);
        iter.value()->blockSignals(false);

        iter =  m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_TAIL);
        iter.value()->blockSignals(true);
        iter.value()->setValue(modelData.tail_);
        iter.value()->blockSignals(false);

        iter =  m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_WIDTH);
        iter.value()->blockSignals(true);
        iter.value()->setValue(modelData.width_);
        iter.value()->blockSignals(false);
        iter =  m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_RADIUS);
        iter.value()->blockSignals(true);
        iter.value()->setValue(modelData.wheelRadius_);
        iter.value()->blockSignals(false);

        robot_model_->robot_model_data_.head_ = modelData.head_*100;
        robot_model_->robot_model_data_.tail_ = modelData.tail_*100;
        robot_model_->robot_model_data_.width_ = modelData.width_*100;
        robot_model_->robot_model_data_.wheelRadius_ = modelData.wheelRadius_*100;
        SCDebug<<"keep track of the error 5=======";
    }
    else if(modelData.shapType_==CIRCLE)
    {
        SCDebug<<"keep track of the error 3=======";
        QMap<WidgetType,QDoubleSpinBox*>::iterator iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_OUTLINE_RADIUS);
        SCDebug<<"modelData.radius is "<<modelData.radius_;
        iter.value()->blockSignals(true);
        iter.value()->setValue(modelData.radius_);
        iter.value()->blockSignals(false);
        SCDebug<<"keep track of the error 4=======";

        iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_RADIUS);
        iter.value()->blockSignals(true);
        iter.value()->setValue(modelData.wheelRadius_);
        iter.value()->blockSignals(false);

        robot_model_->robot_circle_model_data_.radius_ = modelData.radius_*100;
        robot_model_->robot_circle_model_data_.wheelRadius_ = modelData.wheelRadius_*100;
    }

    robot_model_->update_model ();

    ChassisType shape = (ChassisType)chasisTypeComBox->itemData(chasisTypeComBox->currentIndex(),Qt::UserRole).toInt();
    SCDebug<<"keep track of the error 6=======";
    switch (shape)
    {
    case DIFFERENTIAL_TWOWHEEL://差动两轮
        Diff_two_wheel_selected(modelData);
        SCDebug<<"shape is DIFFERENTIAL_TWOWHEEL";
        if(modelData.shapType_==CIRCLE)
        {
            dynamic_cast<WidgetRobotModelSetting*>(robot_model_->getWidgetSetting())->getCirSetting()->setDriverInverAndIdDisplay(2);
        }
        else if(modelData.shapType_==RECTANGLE)
        {
            //根据对应的底盘选型显示相应编号设置框
            dynamic_cast<WidgetRobotModelSetting*>(robot_model_->getWidgetSetting())->getRectSetting()->setDriverInverAndIdDisplay(2);
        }
        break;
    case DIFFERENTIAL_FOURWHEEL://差动四轮
        Diff_four_wheel_selected(modelData);
        SCDebug<<"shape is DIFFERENTIAL_FOURWHEEL";
        if(modelData.shapType_==CIRCLE)
        {
            dynamic_cast<WidgetRobotModelSetting*>(robot_model_->getWidgetSetting())->getCirSetting()->setDriverInverAndIdDisplay(4);
        }
        else if(modelData.shapType_==RECTANGLE)
        {
            //根据对应的底盘选型显示相应编号设置框
            dynamic_cast<WidgetRobotModelSetting*>(robot_model_->getWidgetSetting())->getRectSetting()->setDriverInverAndIdDisplay(4);
        }
        break;
    case ALLDIRECTION_THREEWHEEL://全向三轮
        AllDirection_three_wheel_selected(modelData);
        SCDebug<<"shape is ALLDIRECTION_THREEWHEEL";
        if(modelData.shapType_==CIRCLE)
        {
            dynamic_cast<WidgetRobotModelSetting*>(robot_model_->getWidgetSetting())->getCirSetting()->setDriverInverAndIdDisplay(3);
        }
        else if(modelData.shapType_==RECTANGLE)
        {
            //根据对应的底盘选型显示相应编号设置框
            dynamic_cast<WidgetRobotModelSetting*>(robot_model_->getWidgetSetting())->getRectSetting()->setDriverInverAndIdDisplay(3);
        }
        break;
    case MECANUM_FOURWHEEL://四轮麦克纳姆
        Mecanum_four_wheel_selected(modelData);
        if(modelData.shapType_==CIRCLE)
        {

        }
        else if(modelData.shapType_==RECTANGLE)
        {
            //根据对应的底盘选型显示相应编号设置框
            dynamic_cast<WidgetRobotModelSetting*>(robot_model_->getWidgetSetting())->getRectSetting()->setDriverInverAndIdDisplay(4);
        }
        break;
    case SINGLESTEERWHEEL_FRONTDRIVER_FRONT_CODERTYPE://前驱+前转动编码器
    {
        //设置底盘选型
        robot_model_->setChasiseType(SINGLESTEERWHEEL_FRONTDRIVER_FRONT_CODERTYPE);
        robot_model_->robot_model_data_.chasisType_ = SINGLESTEERWHEEL_FRONTDRIVER_FRONT_CODERTYPE;
        SingleWheelDriver_selected(modelData);

        if(modelData.shapType_==CIRCLE)
        {

        }
        else if(modelData.shapType_==RECTANGLE)
        {
            //根据对应的底盘选型显示相应编号设置框
            dynamic_cast<WidgetRobotModelSetting*>(robot_model_->getWidgetSetting())->getRectSetting()->setDriverInverAndIdDisplay(1);
        }
    }
        break;
    case SINGLESTEERWHEEL_FRONTDRIVER_BACK_CODERTYPE://前驱+后转动编码器
    {
        //设置底盘选型
        robot_model_->setChasiseType(SINGLESTEERWHEEL_FRONTDRIVER_BACK_CODERTYPE);
        robot_model_->robot_model_data_.chasisType_ = SINGLESTEERWHEEL_FRONTDRIVER_BACK_CODERTYPE;
        SingleWheelDriver_selected(modelData);
        //单舵轮只有矩形底盘会有，为了以后拓展加上圆形的判断条件
        if(modelData.shapType_==CIRCLE)
        {

        }
        else if(modelData.shapType_==RECTANGLE)
        {
            //根据对应的底盘选型显示相应编号设置框
            dynamic_cast<WidgetRobotModelSetting*>(robot_model_->getWidgetSetting())->getRectSetting()->setDriverInverAndIdDisplay(1);
        }
    }
        break;
    case SINGLESTEERWHEEL_BACKDRIVER_FRONT_CODERTYPE://后驱+前转动编码器
    {
        //设置底盘选型
        robot_model_->setChasiseType(SINGLESTEERWHEEL_BACKDRIVER_FRONT_CODERTYPE);
        robot_model_->robot_model_data_.chasisType_ = SINGLESTEERWHEEL_BACKDRIVER_FRONT_CODERTYPE;
        SingleWheelDriver_selected(modelData);
        if(modelData.shapType_==CIRCLE)
        {

        }
        else if(modelData.shapType_==RECTANGLE)
        {
            dynamic_cast<WidgetRobotModelSetting*>(robot_model_->getWidgetSetting())->getRectSetting()->setDriverInverAndIdDisplay(1);
        }
    }
        break;
    case SINGLESTEERWHEEL_BACKDRIVER_BACK_CODERTYPE://后驱+后转动编码器
    {
        //设置底盘选型
        robot_model_->setChasiseType(SINGLESTEERWHEEL_BACKDRIVER_BACK_CODERTYPE);
        robot_model_->robot_model_data_.chasisType_ = SINGLESTEERWHEEL_BACKDRIVER_BACK_CODERTYPE;
        SingleWheelDriver_selected(modelData);
        if(modelData.shapType_==CIRCLE)
        {

        }
        else if(modelData.shapType_==RECTANGLE)
        {
            dynamic_cast<WidgetRobotModelSetting*>(robot_model_->getWidgetSetting())->getRectSetting()->setDriverInverAndIdDisplay(1);
        }
    }
        break;
    default:
        break;
    }

}

void modelboxCommand::Diff_two_wheel_selected(RobotModelData& modelData)
{
    ItemRobotModel* robot_model_ = qgraphicsitem_cast<ItemRobotModel*>(m_item);

    //设置底盘选型
    robot_model_->setChasiseType(DIFFERENTIAL_TWOWHEEL);
    if(robot_model_->getChasisShapeType()==RECTANGLE)
    {
        robot_model_->robot_model_data_.chasisType_ = DIFFERENTIAL_TWOWHEEL;
        //（robot_model_数据结构是联合体应先对相应参数进行赋值，否则读取的应是前一种类型的数据）
        robot_model_->robot_model_data_.chasisModel.DiffFourWheel.a_axis_distance
                =modelData.chasisModel.DiffTwoWheel.a_axis_distance*100;

        //设置编辑控件数据(在控件发送valuechanged消息时设置相应的属性数据)
        QMap<WidgetType,QDoubleSpinBox*>::iterator iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_A_AXIS);
        iter.value()->blockSignals(true);
        //    iter.value()->setValue(26/100.0);
        SCDebug<<"difftwo a axis is "<<modelData.chasisModel.DiffTwoWheel.a_axis_distance;
        iter.value()->setValue(modelData.chasisModel.DiffTwoWheel.a_axis_distance);
        iter.value()->blockSignals(false);
        iter.value()->setVisible(true);

        QMap<WidgetType,QLabel*>::iterator iterLabel = m_LabelMap.find(ID_LABEL_A_AXIS);
        iterLabel.value()->setVisible(true);

        iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_B_AXIS);
        iter.value()->setVisible(false);

        iterLabel = m_LabelMap.find(ID_LABEL_B_AXIS);
        iterLabel.value()->setVisible(false);

        iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_ALLDIRECION_AXIS);
        iter.value()->setVisible(false);

        iterLabel = m_LabelMap.find(ID_LABLE_ALLDIRECTION_A_AXIS);
        iterLabel.value()->setVisible(false);

        iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_ALLDIRECTION_ANGLE);
        iter.value()->setVisible(false);

        iterLabel = m_LabelMap.find(ID_LABEL_ALLDIRECTION_ANGLE);
        iterLabel.value()->setVisible(false);
        QMap<WidgetType,QSpinBox*>::iterator iterSpinbox = m_SpinBoxMap.find(ID_DOUBLESPINBOX_ABSENCODER_RANGLE_MAX);
        iterSpinbox = m_SpinBoxMap.find(ID_DOUBLESPINBOX_ABSENCODER_RANGLE_MAX);
        iterSpinbox.value()->setVisible(false);
        iterSpinbox = m_SpinBoxMap.find(ID_DOUBLESPINBOX_ABSENCODER_RANGLE_MIN);
        iterSpinbox.value()->setVisible(false);

        iterLabel = m_LabelMap.find(ID_LABEL_ABSENCODER_RANGLE_MIN);
        iterLabel.value()->setVisible(false);

        iterLabel = m_LabelMap.find(ID_LABEL_ABSENCODER_RANGLE_MAX);
        iterLabel.value()->setVisible(false);

        iterSpinbox = m_SpinBoxMap.find(ID_DOUBLESPINBOX_STEER_ANGLE_MAX);
        iterSpinbox.value()->setVisible(false);
        iterSpinbox = m_SpinBoxMap.find(ID_DOUBLESPINBOX_STEER_ANGLE_MIN);
        iterSpinbox.value()->setVisible(false);

        iterLabel = m_LabelMap.find(ID_LABEL_STEER_ANGEL_MIN);
        iterLabel.value()->setVisible(false);

        iterLabel = m_LabelMap.find(ID_LABEL_STEER_ANGEL_MAX);
        iterLabel.value()->setVisible(false);

        iterLabel = m_LabelMap.find(ID_LABEL_ABSENCODER_TYPE);
        iterLabel.value()->setVisible(false);

        absEncoderTypeComBox->setVisible(false);
    }
    else if(robot_model_->getChasisShapeType()==CIRCLE)
    {
        robot_model_->robot_circle_model_data_.chasisType_ = DIFFERENTIAL_TWOWHEEL;
        //（robot_model_数据结构是联合体应先对相应参数进行赋值，否则读取的应是前一种类型的数据）
        robot_model_->robot_circle_model_data_.chasisModel.DiffFourWheel.a_axis_distance
                =modelData.chasisModel.DiffTwoWheel.a_axis_distance*100;

        //设置编辑控件数据(在控件发送valuechanged消息时设置相应的属性数据)
        QMap<WidgetType,QDoubleSpinBox*>::iterator iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_A_AXIS);
        iter.value()->blockSignals(true);
        //    iter.value()->setValue(26/100.0);
        SCDebug<<"difftwo a axis is "<<modelData.chasisModel.DiffTwoWheel.a_axis_distance;
        iter.value()->setValue(modelData.chasisModel.DiffTwoWheel.a_axis_distance);
        iter.value()->blockSignals(false);
        iter.value()->setVisible(true);

        QMap<WidgetType,QLabel*>::iterator iterLabel = m_LabelMap.find(ID_LABEL_A_AXIS);
        iterLabel.value()->setVisible(true);

        iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_B_AXIS);
        iter.value()->setVisible(false);

        iterLabel = m_LabelMap.find(ID_LABEL_B_AXIS);
        iterLabel.value()->setVisible(false);

        iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_ALLDIRECION_AXIS);
        iter.value()->setVisible(false);

        iterLabel = m_LabelMap.find(ID_LABLE_ALLDIRECTION_A_AXIS);
        iterLabel.value()->setVisible(false);

        iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_ALLDIRECTION_ANGLE);
        iter.value()->setVisible(false);

        iterLabel = m_LabelMap.find(ID_LABEL_ALLDIRECTION_ANGLE);
        iterLabel.value()->setVisible(false);
    }
}

void modelboxCommand::Diff_four_wheel_selected(RobotModelData& modelData)
{
    ItemRobotModel* robot_model_ = qgraphicsitem_cast<ItemRobotModel*>(m_item);
    robot_model_->setChasiseType(DIFFERENTIAL_FOURWHEEL);
    if(robot_model_->getChasisShapeType()==RECTANGLE)
    {
        //（robot_model_数据结构是联合体应先对相应参数进行赋值，否则读取的应是前一种类型的数据）
        robot_model_->robot_model_data_.chasisModel.DiffFourWheel.a_axis_distance=modelData.chasisModel.DiffFourWheel.a_axis_distance*100;
        robot_model_->robot_model_data_.chasisModel.DiffFourWheel.b_axis_distance=modelData.chasisModel.DiffFourWheel.b_axis_distance*100;
        robot_model_->robot_model_data_.chasisType_ = DIFFERENTIAL_FOURWHEEL;

        //设置编辑控件数据(在控件发送valuechanged消息时设置相应的属性数据)
        QMap<WidgetType,QDoubleSpinBox*>::iterator iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_A_AXIS);
        iter.value()->blockSignals(true);
        iter.value()->setValue(modelData.chasisModel.DiffFourWheel.a_axis_distance);
        iter.value()->blockSignals(false);
        iter.value()->setVisible(true);

        QMap<WidgetType,QLabel*>::iterator iterLabel = m_LabelMap.find(ID_LABEL_A_AXIS);
        iterLabel.value()->setVisible(true);

        iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_B_AXIS);
        iter.value()->blockSignals(true);
        iter.value()->setValue(modelData.chasisModel.DiffFourWheel.b_axis_distance);
        iter.value()->blockSignals(false);
        iter.value()->setVisible(true);

        iterLabel = m_LabelMap.find(ID_LABEL_B_AXIS);
        iterLabel.value()->setVisible(true);

        iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_ALLDIRECION_AXIS);
        iter.value()->setVisible(false);

        iterLabel = m_LabelMap.find(ID_LABLE_ALLDIRECTION_A_AXIS);
        iterLabel.value()->setVisible(false);

        iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_ALLDIRECTION_ANGLE);
        iter.value()->setVisible(false);

        iterLabel = m_LabelMap.find(ID_LABEL_ALLDIRECTION_ANGLE);
        iterLabel.value()->setVisible(false);
        QMap<WidgetType,QSpinBox*>::iterator iterSpinbox = m_SpinBoxMap.find(ID_DOUBLESPINBOX_ABSENCODER_RANGLE_MAX);
        iterSpinbox = m_SpinBoxMap.find(ID_DOUBLESPINBOX_ABSENCODER_RANGLE_MAX);
        iterSpinbox.value()->setVisible(false);
        iterSpinbox = m_SpinBoxMap.find(ID_DOUBLESPINBOX_ABSENCODER_RANGLE_MIN);
        iterSpinbox.value()->setVisible(false);

        iterLabel = m_LabelMap.find(ID_LABEL_ABSENCODER_RANGLE_MIN);
        iterLabel.value()->setVisible(false);
        iterLabel = m_LabelMap.find(ID_LABEL_ABSENCODER_RANGLE_MAX);
        iterLabel.value()->setVisible(false);

        iterSpinbox = m_SpinBoxMap.find(ID_DOUBLESPINBOX_STEER_ANGLE_MAX);
        iterSpinbox.value()->setVisible(false);
        iterSpinbox = m_SpinBoxMap.find(ID_DOUBLESPINBOX_STEER_ANGLE_MIN);
        iterSpinbox.value()->setVisible(false);

        iterLabel = m_LabelMap.find(ID_LABEL_STEER_ANGEL_MIN);
        iterLabel.value()->setVisible(false);

        iterLabel = m_LabelMap.find(ID_LABEL_STEER_ANGEL_MAX);
        iterLabel.value()->setVisible(false);

        iterLabel = m_LabelMap.find(ID_LABEL_ABSENCODER_TYPE);
        iterLabel.value()->setVisible(false);

        absEncoderTypeComBox->setVisible(false);
    }
    else if(robot_model_->getChasisShapeType()==CIRCLE)
    {
        //（robot_model_数据结构是联合体应先对相应参数进行赋值，否则读取的应是前一种类型的数据）
        robot_model_->robot_circle_model_data_.chasisModel.DiffFourWheel.a_axis_distance=modelData.chasisModel.DiffFourWheel.a_axis_distance*100;
        robot_model_->robot_circle_model_data_.chasisModel.DiffFourWheel.b_axis_distance=modelData.chasisModel.DiffFourWheel.b_axis_distance*100;
        robot_model_->robot_circle_model_data_.chasisType_ = DIFFERENTIAL_FOURWHEEL;


        //设置编辑控件数据(在控件发送valuechanged消息时设置相应的属性数据)
        QMap<WidgetType,QDoubleSpinBox*>::iterator iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_A_AXIS);
        iter.value()->blockSignals(true);
        iter.value()->setValue(modelData.chasisModel.DiffFourWheel.a_axis_distance);
        iter.value()->blockSignals(false);
        iter.value()->setVisible(true);

        QMap<WidgetType,QLabel*>::iterator iterLabel = m_LabelMap.find(ID_LABEL_A_AXIS);
        iterLabel.value()->setVisible(true);

        iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_B_AXIS);
        iter.value()->blockSignals(true);
        iter.value()->setValue(modelData.chasisModel.DiffFourWheel.b_axis_distance);
        iter.value()->blockSignals(false);
        iter.value()->setVisible(true);

        iterLabel = m_LabelMap.find(ID_LABEL_B_AXIS);
        iterLabel.value()->setVisible(true);

        iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_ALLDIRECION_AXIS);
        iter.value()->setVisible(false);

        iterLabel = m_LabelMap.find(ID_LABLE_ALLDIRECTION_A_AXIS);
        iterLabel.value()->setVisible(false);

        iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_ALLDIRECTION_ANGLE);
        iter.value()->setVisible(false);

        iterLabel = m_LabelMap.find(ID_LABEL_ALLDIRECTION_ANGLE);
        iterLabel.value()->setVisible(false);
    }

}

void modelboxCommand::AllDirection_three_wheel_selected(RobotModelData& modelData)
{
    ItemRobotModel* robot_model_ = qgraphicsitem_cast<ItemRobotModel*>(m_item);
    //设置底盘选型
    robot_model_->setChasiseType(ALLDIRECTION_THREEWHEEL);

    if(robot_model_->getChasisShapeType()==RECTANGLE)
    {
        robot_model_->robot_model_data_.chasisType_ = ALLDIRECTION_THREEWHEEL;
        //设置全向数据参数（robot_model_数据结构是联合体应先对相应参数进行赋值，否则读取的应是前一种类型的数据）
        robot_model_->robot_model_data_.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle=modelData.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle;
        robot_model_->robot_model_data_.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance = modelData.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance*100;

        //设置编辑控件数据(在控件发送valuechanged消息时设置相应的属性数据)
        QMap<WidgetType,QDoubleSpinBox*>::iterator iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_A_AXIS);
        iter.value()->setVisible(false);

        QMap<WidgetType,QLabel*>::iterator iterLabel = m_LabelMap.find(ID_LABEL_A_AXIS);
        iterLabel.value()->setVisible(false);

        iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_B_AXIS);
        iter.value()->setVisible(false);

        iterLabel = m_LabelMap.find(ID_LABEL_B_AXIS);
        iterLabel.value()->setVisible(false);

        iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_ALLDIRECION_AXIS);
        iter.value()->blockSignals(true);
        iter.value()->setValue(modelData.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance);
        iter.value()->blockSignals(false);
        iter.value()->setVisible(true);

        iterLabel = m_LabelMap.find(ID_LABLE_ALLDIRECTION_A_AXIS);
        iterLabel.value()->setVisible(true);


        iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_ALLDIRECTION_ANGLE);
        iter.value()->blockSignals(true);
        iter.value()->setValue(modelData.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle);
        iter.value()->blockSignals(false);
        iter.value()->setVisible(true);

        iterLabel = m_LabelMap.find(ID_LABEL_ALLDIRECTION_ANGLE);
        iterLabel.value()->setVisible(true);

        QMap<WidgetType,QSpinBox*>::iterator iterSpinbox = m_SpinBoxMap.find(ID_DOUBLESPINBOX_ABSENCODER_RANGLE_MAX);
        iterSpinbox = m_SpinBoxMap.find(ID_DOUBLESPINBOX_ABSENCODER_RANGLE_MAX);
        iterSpinbox.value()->setVisible(false);
        iterSpinbox = m_SpinBoxMap.find(ID_DOUBLESPINBOX_ABSENCODER_RANGLE_MIN);
        iterSpinbox.value()->setVisible(false);

        iterLabel = m_LabelMap.find(ID_LABEL_ABSENCODER_RANGLE_MIN);
        iterLabel.value()->setVisible(false);
        iterLabel = m_LabelMap.find(ID_LABEL_ABSENCODER_RANGLE_MAX);
        iterLabel.value()->setVisible(false);

        iterSpinbox = m_SpinBoxMap.find(ID_DOUBLESPINBOX_STEER_ANGLE_MAX);
        iterSpinbox.value()->setVisible(false);
        iterSpinbox = m_SpinBoxMap.find(ID_DOUBLESPINBOX_STEER_ANGLE_MIN);
        iterSpinbox.value()->setVisible(false);

        iterLabel = m_LabelMap.find(ID_LABEL_STEER_ANGEL_MIN);
        iterLabel.value()->setVisible(false);
        iterLabel = m_LabelMap.find(ID_LABEL_STEER_ANGEL_MAX);
        iterLabel.value()->setVisible(false);

        iterLabel = m_LabelMap.find(ID_LABEL_ABSENCODER_TYPE);
        iterLabel.value()->setVisible(false);

        absEncoderTypeComBox->setVisible(false);

    }
    else if(robot_model_->getChasisShapeType()==CIRCLE)
    {
        robot_model_->robot_circle_model_data_.chasisType_ = ALLDIRECTION_THREEWHEEL;
        robot_model_->robot_circle_model_data_.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle= modelData.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle;
        robot_model_->robot_circle_model_data_.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance = modelData.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance*100;
        //设置编辑控件数据(在控件发送valuechanged消息时设置相应的属性数据)
        QMap<WidgetType,QDoubleSpinBox*>::iterator iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_A_AXIS);
        iter.value()->setVisible(false);

        QMap<WidgetType,QLabel*>::iterator iterLabel = m_LabelMap.find(ID_LABEL_A_AXIS);
        iterLabel.value()->setVisible(false);

        iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_B_AXIS);
        iter.value()->setVisible(false);

        iterLabel = m_LabelMap.find(ID_LABEL_B_AXIS);
        iterLabel.value()->setVisible(false);

        iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_ALLDIRECION_AXIS);
        iter.value()->blockSignals(true);
        iter.value()->setValue(modelData.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance);
        iter.value()->blockSignals(false);
        iter.value()->setVisible(true);

        iterLabel = m_LabelMap.find(ID_LABLE_ALLDIRECTION_A_AXIS);
        iterLabel.value()->setVisible(true);


        iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_ALLDIRECTION_ANGLE);
        iter.value()->blockSignals(true);
        iter.value()->setValue(modelData.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle);
        iter.value()->blockSignals(false);
        iter.value()->setVisible(true);

        iterLabel = m_LabelMap.find(ID_LABEL_ALLDIRECTION_ANGLE);
        iterLabel.value()->setVisible(true);
    }
}

void modelboxCommand::Mecanum_four_wheel_selected(RobotModelData& modelData)
{
    ItemRobotModel* robot_model_ = qgraphicsitem_cast<ItemRobotModel*>(m_item);
    //设置底盘选型
    robot_model_->setChasiseType(MECANUM_FOURWHEEL);
    robot_model_->robot_model_data_.chasisType_ = MECANUM_FOURWHEEL;

    //（robot_model_数据结构是联合体应先对相应参数进行赋值，否则读取的应是前一种类型的数据）
    robot_model_->robot_model_data_.chasisModel.MecanumFourWheel.a_axis_distance=modelData.chasisModel.MecanumFourWheel.a_axis_distance*100;
    robot_model_->robot_model_data_.chasisModel.MecanumFourWheel.b_axis_distance= modelData.chasisModel.MecanumFourWheel.b_axis_distance*100;

    //设置编辑控件数据(在控件发送valuechanged消息时设置相应的属性数据)
    QMap<WidgetType,QDoubleSpinBox*>::iterator iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_A_AXIS);
    iter.value()->blockSignals(true);
    iter.value()->setValue(modelData.chasisModel.MecanumFourWheel.a_axis_distance);
    iter.value()->blockSignals(false);
    iter.value()->setVisible(true);

    QMap<WidgetType,QLabel*>::iterator iterLabel = m_LabelMap.find(ID_LABEL_A_AXIS);
    iterLabel.value()->setVisible(true);

    iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_B_AXIS);
    iter.value()->blockSignals(true);
    iter.value()->setValue(modelData.chasisModel.MecanumFourWheel.b_axis_distance);
    iter.value()->blockSignals(false);
    iter.value()->setVisible(true);

    iterLabel = m_LabelMap.find(ID_LABEL_B_AXIS);
    iterLabel.value()->setVisible(true);

    iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_ALLDIRECION_AXIS);
    iter.value()->setVisible(false);

    iterLabel = m_LabelMap.find(ID_LABLE_ALLDIRECTION_A_AXIS);
    iterLabel.value()->setVisible(false);

    iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_ALLDIRECTION_ANGLE);
    iter.value()->setVisible(false);

    iterLabel = m_LabelMap.find(ID_LABEL_ALLDIRECTION_ANGLE);
    iterLabel.value()->setVisible(false);

    QMap<WidgetType,QSpinBox*>::iterator iterSpinbox = m_SpinBoxMap.find(ID_DOUBLESPINBOX_ABSENCODER_RANGLE_MAX);

    iterSpinbox = m_SpinBoxMap.find(ID_DOUBLESPINBOX_ABSENCODER_RANGLE_MAX);
    iterSpinbox.value()->setVisible(false);
    iterSpinbox = m_SpinBoxMap.find(ID_DOUBLESPINBOX_ABSENCODER_RANGLE_MIN);
    iterSpinbox.value()->setVisible(false);

    iterLabel = m_LabelMap.find(ID_LABEL_ABSENCODER_RANGLE_MIN);
    iterLabel.value()->setVisible(false);
    iterLabel = m_LabelMap.find(ID_LABEL_ABSENCODER_RANGLE_MAX);
    iterLabel.value()->setVisible(false);

    iterSpinbox = m_SpinBoxMap.find(ID_DOUBLESPINBOX_STEER_ANGLE_MAX);
    iterSpinbox.value()->setVisible(false);
    iterSpinbox = m_SpinBoxMap.find(ID_DOUBLESPINBOX_STEER_ANGLE_MIN);
    iterSpinbox.value()->setVisible(false);

    iterLabel = m_LabelMap.find(ID_LABEL_STEER_ANGEL_MIN);
    iterLabel.value()->setVisible(false);

    iterLabel = m_LabelMap.find(ID_LABEL_STEER_ANGEL_MAX);
    iterLabel.value()->setVisible(false);

    iterLabel = m_LabelMap.find(ID_LABEL_ABSENCODER_TYPE);
    iterLabel.value()->setVisible(false);

    absEncoderTypeComBox->setVisible(false);

}

void modelboxCommand::SingleWheelDriver_selected(RobotModelData& modelData)
{
    ItemRobotModel* robot_model_ = qgraphicsitem_cast<ItemRobotModel*>(m_item);
    //（robo_model_数据结构是联合体应先对相应参数进行赋值，否则读取的应是前一种类型的数据）
    robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.a_axis_distance=modelData.chasisModel.SingleWheelDriver.a_axis_distance*100;//单位cm
    robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.b_axis_distance=modelData.chasisModel.SingleWheelDriver.b_axis_distance*100;
    robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.steer_angle_max= modelData.chasisModel.SingleWheelDriver.steer_angle_max ;
    robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.steer_angle_min= modelData.chasisModel.SingleWheelDriver.steer_angle_min ;
    robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.absEncoderRange_max= modelData.chasisModel.SingleWheelDriver.absEncoderRange_max ;
    robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.absEncoderRange_min= modelData.chasisModel.SingleWheelDriver.absEncoderRange_min ;

    QMap<WidgetType,QDoubleSpinBox*>::iterator iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_A_AXIS);
    iter.value()->blockSignals(true);
    iter.value()->setValue(modelData.chasisModel.SingleWheelDriver.a_axis_distance);
    iter.value()->blockSignals(false);
    iter.value()->setVisible(true);


    QMap<WidgetType,QLabel*>::iterator iterLabel = m_LabelMap.find(ID_LABEL_A_AXIS);
    iterLabel.value()->setVisible(true);

    iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_B_AXIS);
    iter.value()->blockSignals(true);
    iter.value()->setValue(modelData.chasisModel.SingleWheelDriver.b_axis_distance);
    iter.value()->blockSignals(false);
    iter.value()->setVisible(true);

    iterLabel = m_LabelMap.find(ID_LABEL_B_AXIS);
    iterLabel.value()->setVisible(true);

    iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_ALLDIRECION_AXIS);
    iter.value()->setVisible(false);

    iterLabel = m_LabelMap.find(ID_LABLE_ALLDIRECTION_A_AXIS);
    iterLabel.value()->setVisible(false);

    iter = m_DoubleSpinBoxMap.find(ID_DOUBLESPINBOX_ALLDIRECTION_ANGLE);
    iter.value()->setVisible(false);

    iterLabel = m_LabelMap.find(ID_LABEL_ALLDIRECTION_ANGLE);
    iterLabel.value()->setVisible(false);

    QMap<WidgetType,QSpinBox*>::iterator iterSpinbox = m_SpinBoxMap.find(ID_DOUBLESPINBOX_ABSENCODER_RANGLE_MAX);

    iterSpinbox.value()->blockSignals(true);
    iterSpinbox.value()->setValue(modelData.chasisModel.SingleWheelDriver.absEncoderRange_max);
    iterSpinbox.value()->blockSignals(false);
    iterSpinbox.value()->setVisible(true);
    iterSpinbox = m_SpinBoxMap.find(ID_DOUBLESPINBOX_ABSENCODER_RANGLE_MIN);
    iterSpinbox.value()->blockSignals(true);
    iterSpinbox.value()->setValue(modelData.chasisModel.SingleWheelDriver.absEncoderRange_min);
    iterSpinbox.value()->blockSignals(false);
    iterSpinbox.value()->setVisible(true);

    iterLabel = m_LabelMap.find(ID_LABEL_ABSENCODER_RANGLE_MIN);
    iterLabel.value()->setVisible(true);
    iterLabel = m_LabelMap.find(ID_LABEL_ABSENCODER_RANGLE_MAX);
    iterLabel.value()->setVisible(true);

    iterSpinbox = m_SpinBoxMap.find(ID_DOUBLESPINBOX_STEER_ANGLE_MAX);
    iterSpinbox.value()->blockSignals(true);
    iterSpinbox.value()->setValue( modelData.chasisModel.SingleWheelDriver.steer_angle_max);
    iterSpinbox.value()->blockSignals(false);
    iterSpinbox.value()->setVisible(true);
    iterSpinbox = m_SpinBoxMap.find(ID_DOUBLESPINBOX_STEER_ANGLE_MIN);
    iterSpinbox.value()->blockSignals(true);
    iterSpinbox.value()->setValue( modelData.chasisModel.SingleWheelDriver.steer_angle_min);
    iterSpinbox.value()->blockSignals(false);
    iterSpinbox.value()->setVisible(true);

    iterLabel = m_LabelMap.find(ID_LABEL_STEER_ANGEL_MIN);
    iterLabel.value()->setVisible(true);

    iterLabel = m_LabelMap.find(ID_LABEL_STEER_ANGEL_MAX);
    iterLabel.value()->setVisible(true);

    iterLabel = m_LabelMap.find(ID_LABEL_ABSENCODER_TYPE);
    iterLabel.value()->setVisible(true);

    absEncoderTypeComBox->setVisible(true);

}

DragChassisCommand::DragChassisCommand(ItemBase *item,double ori_headValue, double new_headValue,
                                       double ori_tailValue, double new_tailValue,
                                       double ori_widValue, double new_widValue,
                                       customDoubleSpinBox* headbox,
                                       customDoubleSpinBox* tailbox,
                                       customDoubleSpinBox* widbox,
                                       ModelGraphicsScene* scene,
                                       QUndoCommand *parent)
    :QUndoCommand(parent)
    ,m_item(item)
    ,m_headbox(headbox)
    ,m_tailbox(tailbox)
    ,m_widbox(widbox)
    ,m_ori_headValue(ori_headValue)
    , m_new_headValue(new_headValue)
    , m_ori_tailValue(ori_tailValue)
    , m_new_tailValue(new_tailValue)
    , m_ori_widValue(ori_widValue)
    , m_new_widValue(new_widValue)
    ,m_scene(scene)
{

}

DragChassisCommand::DragChassisCommand(ItemBase *item, double ori_radius,
                                       double new_radius, customDoubleSpinBox *radiusbox,
                                       ModelGraphicsScene *scene, QUndoCommand *parent)
    :QUndoCommand(parent)
    ,m_item(item)
    ,m_radiusBox(radiusbox)
    ,m_ori_radius(ori_radius)
    ,m_new_radius(new_radius)
    ,m_scene(scene)
{

}

void DragChassisCommand::undo()
{
    if(m_item==NULL)
    {
        SCDebug<<"robot item is null";
    }
    ItemRobotModel *rm=qgraphicsitem_cast<ItemRobotModel *>(m_item);

    if(rm->getChasisShapeType()==RECTANGLE)
    {
        rm->robot_model_data_.width_=m_ori_widValue;
        rm->robot_model_data_.head_=m_ori_headValue;
        rm->robot_model_data_.tail_=m_ori_tailValue;

        m_headbox->blockSignals(true);
        m_headbox->setValue(m_ori_headValue/100.);
        m_headbox->blockSignals(false);

        m_tailbox->blockSignals(true);
        m_tailbox->setValue(m_ori_tailValue/100.);
        m_tailbox->blockSignals(false);

        m_widbox->blockSignals(true);
        m_widbox->setValue(m_ori_widValue/100.);
        m_widbox->blockSignals(false);

    }
    else if(rm->getChasisShapeType()==CIRCLE)
    {
        rm->robot_circle_model_data_.radius_ = m_ori_radius;
        m_radiusBox->blockSignals(true);
        m_radiusBox->setValue(m_ori_radius/100.);
        m_radiusBox->blockSignals(false);
    }
    rm->update_model ();
    m_scene->model_view()->update();
    undoToShowRelativeWidget(dynamic_cast<WidgetRobotModelSetting*>(rm->getWidgetSetting()));
}

void DragChassisCommand::redo()
{
    qDebug()<<"redo ----------------------10---------------";

    if(m_item==NULL)
    {
        SCDebug<<"robot item is null";
    }

    ItemRobotModel *rm=qgraphicsitem_cast<ItemRobotModel *>(m_item);

    if(rm->getChasisShapeType()==RECTANGLE)
    {
        rm->robot_model_data_.width_=m_new_widValue;
        rm->robot_model_data_.head_=m_new_headValue;
        rm->robot_model_data_.tail_=m_new_tailValue;

        m_headbox->blockSignals(true);
        m_headbox->setValue(m_new_headValue/100.);
        m_headbox->blockSignals(false);

        m_tailbox->blockSignals(true);
        m_tailbox->setValue(m_new_tailValue/100.);
        m_tailbox->blockSignals(false);

        m_widbox->blockSignals(true);
        m_widbox->setValue(m_new_widValue/100.);
        m_widbox->blockSignals(false);
    }
    else if(rm->getChasisShapeType()==CIRCLE)
    {
        rm->robot_circle_model_data_.radius_ = m_new_radius;
        m_radiusBox->blockSignals(true);
        m_radiusBox->setValue(m_new_radius/100.);
        m_radiusBox->blockSignals(false);
    }

    rm->update_model ();
    m_scene->model_view()->update();

}

moveItemCommand::moveItemCommand(QPointF ori_pos,QPointF new_pos,ModelGraphicsScene* scene,ItemBase *item, QUndoCommand *parent)
    :QUndoCommand(parent)
    ,m_ori_pos(ori_pos)
    ,m_new_pos(new_pos)
    ,m_item(item)
    ,m_scene(scene)
{

}

void moveItemCommand::undo()
{
    if(m_item->type()==ItemPolygon::Type)
    {
        ItemPolygon* temp_item = qgraphicsitem_cast<ItemPolygon*>(m_item);
        temp_item->setPos(m_ori_pos);
        temp_item->setOriPos(m_ori_pos);//
    }
    else if(m_item->type()==ItemRobotModel::Type)
    {
        ItemRobotModel*  temp_item = qgraphicsitem_cast<ItemRobotModel*>(m_item);
        temp_item->setPos(m_ori_pos);
        temp_item->setOriPos(m_ori_pos);//
    }
    else if(m_item->type() == ItemAngelCtroller::Type)
    {
        ItemAngelCtroller*  temp_item = qgraphicsitem_cast<ItemAngelCtroller*>(m_item);
        temp_item->setPos(m_ori_pos);
        temp_item->setOriPos(m_ori_pos);//

    }
    else if(m_item->type()==ItemDO::Type)
    {
        ItemDO*  temp_item = qgraphicsitem_cast<ItemDO*>(m_item);

        temp_item->setPos(m_ori_pos);
        temp_item->setOriPos(m_ori_pos);//
        temp_item->updateWidgetValue();
    }
    else if(m_item->type()==ItemDI::Type)
    {
        ItemDI*  temp_item = qgraphicsitem_cast<ItemDI*>(m_item);
        temp_item->setPos(m_ori_pos);
        temp_item->setOriPos(m_ori_pos);//
        temp_item->updateWidgetValue();
    }
    else if(m_item->type()==ItemLaser::Type)
    {
        ItemLaser*  temp_item = qgraphicsitem_cast<ItemLaser*>(m_item);
        temp_item->setPos(m_ori_pos);
        temp_item->updateWidgetValue();
        temp_item->setOriPos(m_ori_pos);//
    }
    else if(m_item->type()==ItemPolypos::Type)
    {
        ItemPolypos*  temp_item = qgraphicsitem_cast<ItemPolypos*>(m_item);

        temp_item->setPos(m_ori_pos);
        temp_item->setOriPos(m_ori_pos);//
    }
    else if(m_item->type()==ItemUltrasonic::Type)
    {
        ItemUltrasonic*  temp_item = qgraphicsitem_cast<ItemUltrasonic*>(m_item);
        temp_item->setPos(m_ori_pos);
        temp_item->setOriPos(m_ori_pos);//
        temp_item->updateWidgetValue();
    }
    else if(m_item->type()==ItemMagneticSensor::Type)
    {
        ItemMagneticSensor*  temp_item = qgraphicsitem_cast<ItemMagneticSensor*>(m_item);
        temp_item->setPos(m_ori_pos);
        temp_item->updateWidgetValue();
        temp_item->setOriPos(m_ori_pos);//
    }
    else if(m_item->type()==ItemCrashDI::Type)
    {
        ItemCrashDI* temp_item = qgraphicsitem_cast<ItemCrashDI*>(m_item);
        temp_item->setPos(m_ori_pos);
        temp_item->setOriPos(m_ori_pos);//
    }
    else if(m_item->type()==ItemCrashDIpos::Type)
    {
        ItemCrashDIpos* temp_item = qgraphicsitem_cast<ItemCrashDIpos*>(m_item);
        temp_item->setPos(m_ori_pos);
        temp_item->updatePosWidget();
        temp_item->setOriPos(m_ori_pos);//
    }
    else if(m_item->type()==ItemRFIDSensor::Type)
    {
        ItemRFIDSensor* temp_item = qgraphicsitem_cast<ItemRFIDSensor*>(m_item);
        temp_item->setPos(m_ori_pos);
        temp_item->updateWidgetValue();
        temp_item->setOriPos(m_ori_pos);//

    }
    else if(m_item->type()==ItemCamera::Type)
    {
        ItemCamera* temp_item = qgraphicsitem_cast<ItemCamera*>(m_item);
        temp_item->setPos(m_ori_pos);
        temp_item->updateWidgetValue();
        temp_item->setOriPos(m_ori_pos);
    }
    m_scene->model_view()->update();
    setCurrentItemStatus(m_item->model_scene(),m_item->getWidgetSetting());
}

void moveItemCommand::redo()
{
    qDebug()<<"redo ----------------------15---------------";
    if(m_item->type()==ItemPolygon::Type)
    {
        ItemPolygon* temp_item = qgraphicsitem_cast<ItemPolygon*>(m_item);
        temp_item->setPos(m_new_pos);
    }
    else if(m_item->type()==ItemRobotModel::Type)
    {
        ItemRobotModel*  temp_item = qgraphicsitem_cast<ItemRobotModel*>(m_item);
        temp_item->setPos(m_new_pos);
    }
    else if(m_item->type() == ItemAngelCtroller::Type)
    {
        ItemAngelCtroller*  temp_item = qgraphicsitem_cast<ItemAngelCtroller*>(m_item);
        temp_item->setPos(m_new_pos);

    }
    else if(m_item->type()==ItemDO::Type)
    {
        ItemDO*  temp_item = qgraphicsitem_cast<ItemDO*>(m_item);

        temp_item->setPos(m_new_pos);
        temp_item->updateWidgetValue();
    }
    else if(m_item->type()==ItemDI::Type)
    {
        ItemDI*  temp_item = qgraphicsitem_cast<ItemDI*>(m_item);
        temp_item->setPos(m_new_pos);
        temp_item->updateWidgetValue();
    }
    else if(m_item->type()==ItemLaser::Type)
    {
        ItemLaser*  temp_item = qgraphicsitem_cast<ItemLaser*>(m_item);
        temp_item->setPos(m_new_pos);
        temp_item->updateWidgetValue();
    }
    else if(m_item->type()==ItemPolypos::Type)
    {
        ItemPolypos*  temp_item = qgraphicsitem_cast<ItemPolypos*>(m_item);

        temp_item->setPos(m_new_pos);
    }
    else if(m_item->type()==ItemUltrasonic::Type)
    {
        ItemUltrasonic*  temp_item = qgraphicsitem_cast<ItemUltrasonic*>(m_item);
        temp_item->setPos(m_new_pos);
        temp_item->updateWidgetValue();
    }
    else if(m_item->type()==ItemMagneticSensor::Type)
    {
        ItemMagneticSensor*  temp_item = qgraphicsitem_cast<ItemMagneticSensor*>(m_item);
        temp_item->setPos(m_new_pos);
        temp_item->updateWidgetValue();
    }
    else if(m_item->type()==ItemCrashDI::Type)
    {
        ItemCrashDI* temp_item = qgraphicsitem_cast<ItemCrashDI*>(m_item);
        temp_item->setPos(m_new_pos);
        for(int i = 0; i< temp_item->getLine_pos_list()->size();i++)
        {
            temp_item->getLine_pos_list()->at(i)->setOriPos(temp_item->getLine_pos_list()->at(i)->pos());
        }
    }
    else if(m_item->type()==ItemCrashDIpos::Type)
    {
        ItemCrashDIpos* temp_item = qgraphicsitem_cast<ItemCrashDIpos*>(m_item);
        temp_item->setPos(m_new_pos);
        temp_item->updatePosWidget();
    }
    else if(m_item->type()==ItemRFIDSensor::Type)
    {
        ItemRFIDSensor* temp_item = qgraphicsitem_cast<ItemRFIDSensor*>(m_item);
        temp_item->setPos(m_new_pos);
        temp_item->updateWidgetValue();
    }
    else if(m_item->type()==ItemCamera::Type)
    {
        ItemCamera* temp_item = qgraphicsitem_cast<ItemCamera*>(m_item);
        temp_item->setPos(m_new_pos);
        temp_item->updateWidgetValue();
    }
    setCurrentItemStatus(m_item->model_scene(),m_item->getWidgetSetting());
    m_scene->model_view()->update();
}

rotateItemCommand::rotateItemCommand(qreal ori_angel, qreal new_angel, ModelGraphicsScene *scene, ItemBase *item, QUndoCommand *parent)
    :QUndoCommand(parent)
    ,m_ori_angel(ori_angel)
    ,m_new_angel(new_angel)
    ,m_scene(scene)
    ,m_item(item)
{

}

void rotateItemCommand::undo()
{
    ItemAngelCtroller* AngelItem = qgraphicsitem_cast<ItemAngelCtroller*>(m_item);
    AngelItem->setRotation(m_ori_angel);
    qreal k_radius = AngelItem->getRoationRadius();
    AngelItem->setPos(k_radius*qCos(m_ori_angel*M_PI/180.),k_radius*qSin(m_ori_angel*M_PI/180.));//
    AngelItem->sentAngelChange(m_ori_angel);
}

void rotateItemCommand::redo()
{
    qDebug()<<"redo ----------------------16---------------";
    ItemAngelCtroller* AngelItem = qgraphicsitem_cast<ItemAngelCtroller*>(m_item);
    AngelItem->setRotation(m_new_angel);
    qreal k_radius = AngelItem->getRoationRadius();
    AngelItem->setPos(k_radius*qCos(m_new_angel*M_PI/180.),k_radius*qSin(m_new_angel*M_PI/180.));//
    AngelItem->sentAngelChange(m_new_angel);
}

addItemCommand::addItemCommand(int oritype,int newtype,QPoint oriPos,QPoint newPos,
                               QGraphicsItem* ori_item,QGraphicsItem* new_item,
                               ModelGraphicsScene* scene, QUndoCommand *parent)
    :QUndoCommand(parent)
    ,m_ori_Type(oritype)
    ,m_new_Type(newtype)
    ,m_ori_pos(oriPos)
    ,m_new_pos(newPos)
    ,m_scene(scene)
    ,m_ori_item(ori_item)
    ,m_new_item(new_item)
{

}

void addItemCommand::undo()
{
    if(m_new_Type<3)          //多边形
    {
        ItemPolygon *ipoly = qgraphicsitem_cast<ItemPolygon *>(m_new_item);
        m_scene->get_PolygonList()->removeOne(ipoly);//从列表中移除
        m_scene->removeItem(ipoly);//从场景中移除
    }
    else if(m_new_Type==10)//激光
    {
        ItemLaser *ilaser = qgraphicsitem_cast<ItemLaser *>(m_new_item);
        m_scene->get_Laserlist()->removeOne(ilaser);//从列表中移除
        m_scene->removeItem(ilaser);//从场景中移除
    }
    else if(m_new_Type==11)//ultrasonic
    {
        ItemUltrasonic *iul = qgraphicsitem_cast<ItemUltrasonic *>(m_new_item);
        m_scene->get_ultrasonic_list()->removeOne(iul);
        m_scene->removeItem(iul);
    }
    else if(m_new_Type==12)//DI
    {
        ItemDI *idi = qgraphicsitem_cast<ItemDI *>(m_new_item);
        m_scene->get_Di_list()->removeOne(idi);
        m_scene->removeItem(idi);
    }
    else if(m_new_Type==13)//magneticScesor
    {
        ItemMagneticSensor *MagneticSensor = qgraphicsitem_cast<ItemMagneticSensor *>(m_new_item);
        m_scene->get_magneticSensor_list()->removeOne(MagneticSensor);
        m_scene->removeItem(MagneticSensor);
    }
    else if(m_new_Type==14)//DO
    {
        ItemDO *ido = qgraphicsitem_cast<ItemDO *>(m_new_item);
        m_scene->get_Do_list()->removeOne(ido);
        m_scene->removeItem(ido);
    }
    else if(m_new_Type==15)//防撞Di
    {
        ItemCrashDI* crashDi = qgraphicsitem_cast<ItemCrashDI*>(m_new_item);
        m_scene->get_crashDi_list()->removeOne(crashDi);
        m_scene->removeItem(crashDi);
    }
    else if(m_new_Type == 16)
    {
        ItemRFIDSensor* RFID = qgraphicsitem_cast<ItemRFIDSensor*>(m_new_item);
        m_scene->get_RFID_list()->removeOne(RFID);
        m_scene->removeItem(RFID);
    }
    else if(m_new_Type == 17)
    {
        ItemCamera* camera = qgraphicsitem_cast<ItemCamera*>(m_new_item);
        m_scene->get_Camera_list()->removeOne(camera);
        m_scene->removeItem(camera);
    }
    m_scene->update();
    //undo删除添加的item时清空右侧属性窗口
    m_scene->model_view ()->property_widget()->update_layer (QList<ItemBase *>());
}

void addItemCommand::redo()
{
    qDebug()<<"redo ----------------------17---------------";
    if(m_new_Type<3)          //多边形
    {
        ItemPolygon* ipoly = qgraphicsitem_cast<ItemPolygon*>(m_new_item);
        m_scene->get_PolygonList()->append(ipoly);
        m_scene->addItem(ipoly);
        ipoly->stackBefore( m_scene->getItem_robot_model() );
        ipoly->setPos ( m_scene->model_view ()->mapToScene(m_new_pos));//将试图坐标系点映射到场景坐标系点， item为场景最上层，其坐标为场景坐标系下得坐标
        ipoly->setOriPos(m_scene->model_view ()->mapToScene(m_new_pos));
        ipoly->add_setting_widget();
        m_scene->model_view ()->property_widget ()->update_property_widget (ipoly->getWidgetSetting());
        setCurrentItemStatus(m_scene,ipoly->getWidgetSetting());
    }
    else if(m_new_Type==10)//激光
    {
        ItemLaser *ilaser = qgraphicsitem_cast<ItemLaser*>(m_new_item);
        m_scene->get_Laserlist()->append(ilaser);
        m_scene->addItem (ilaser);//添加到场景
        //设置在场景中的位置
        ilaser->setPos( m_scene->model_view ()->mapToScene (m_new_pos));
        //记录初始位置
        ilaser->setOriPos(m_scene->model_view ()->mapToScene(m_new_pos));
        //更新属性值
        ilaser->update_data ();
        //创建对应的属性窗口
        ilaser->add_setting_widget();
        //更新属性窗口显示当前item对应的窗口
        m_scene->model_view ()->property_widget ()->update_property_widget (ilaser->getWidgetSetting());
        //设置当前item处于选中状态
        setCurrentItemStatus(m_scene,ilaser->getWidgetSetting());
    }
    else if(m_new_Type==11)//ultrasonic
    {
        ItemUltrasonic *iul= qgraphicsitem_cast<ItemUltrasonic*>(m_new_item);
        m_scene->get_ultrasonic_list()->append(iul);
        m_scene->addItem (iul);
        iul->setPos ( m_scene->model_view ()->mapToScene (m_new_pos));
        iul->setOriPos(m_scene->model_view ()->mapToScene(m_new_pos));
        iul->update_data ();
        iul->add_setting_widget();
        m_scene->model_view ()->property_widget ()->update_property_widget (iul->getWidgetSetting());
        setCurrentItemStatus(m_scene,iul->getWidgetSetting());
    }
    else if(m_new_Type==12)//DI
    {
        ItemDI *idi = qgraphicsitem_cast<ItemDI*>(m_new_item);
        m_scene->get_Di_list()->append(idi);
        m_scene->addItem (idi);
        idi->setPos ( m_scene->model_view ()->mapToScene (m_new_pos));
        idi->setOriPos(m_scene->model_view ()->mapToScene(m_new_pos));
        idi->update_data ();
        idi->add_setting_widget();
        m_scene->model_view ()->property_widget ()->update_property_widget (idi->getWidgetSetting());
        setCurrentItemStatus(m_scene,idi->getWidgetSetting());
    }
    else if(m_new_Type==13)//magneticScesor
    {
        ItemMagneticSensor *MagneticSensor = qgraphicsitem_cast<ItemMagneticSensor*>(m_new_item);
        m_scene->get_magneticSensor_list()->append(MagneticSensor);
        m_scene->addItem (MagneticSensor);
        MagneticSensor->setPos ( m_scene->model_view ()->mapToScene (m_new_pos));
        MagneticSensor->setOriPos(m_scene->model_view ()->mapToScene(m_new_pos));
        MagneticSensor->update_data ();
        MagneticSensor->add_setting_widget();
        m_scene->model_view ()->property_widget ()->update_property_widget (MagneticSensor->getWidgetSetting());
        setCurrentItemStatus(m_scene,MagneticSensor->getWidgetSetting());
    }
    else if(m_new_Type==14)//DO
    {
        ItemDO *ido = qgraphicsitem_cast<ItemDO*>(m_new_item);
        m_scene->get_Do_list()->append(ido);
        m_scene->addItem (ido);
        ido->setPos ( m_scene->model_view ()->mapToScene (m_new_pos));
        ido->setOriPos( m_scene->model_view ()->mapToScene(m_new_pos));
        ido->update_data ();
        ido->add_setting_widget();
        m_scene->model_view ()->property_widget ()->update_property_widget (ido->getWidgetSetting());
        setCurrentItemStatus(m_scene,ido->getWidgetSetting());
    }
    else if(m_new_Type==15)//防撞DI
    {
        ItemCrashDI* crashDi = qgraphicsitem_cast<ItemCrashDI*>(m_new_item);
        m_scene->get_crashDi_list()->append(crashDi);
        m_scene->addItem(crashDi);
        crashDi->setPos(m_scene->model_view()->mapToScene(m_new_pos));
        crashDi->setOriPos(m_scene->model_view()->mapToScene(m_new_pos));
        crashDi->add_setting_widget();
        m_scene->model_view ()->property_widget ()->update_property_widget (crashDi->getWidgetSetting());
        setCurrentItemStatus(m_scene,crashDi->getWidgetSetting());
    }
    else if(m_new_Type == 16)//RFID
    {
        ItemRFIDSensor*  RFID = qgraphicsitem_cast<ItemRFIDSensor*>(m_new_item);
        m_scene->addItem(RFID);
        RFID->setPos(m_scene->model_view()->mapToScene(m_new_pos));
        RFID->setOriPos(m_scene->model_view()->mapToScene(m_new_pos));
        RFID->update_data();
        RFID->add_setting_widget();
        m_scene->model_view ()->property_widget ()->update_property_widget (RFID->getWidgetSetting());
        setCurrentItemStatus(m_scene,RFID->getWidgetSetting());
    }
    else if(m_new_Type == 17)
    {
        ItemCamera* camera = qgraphicsitem_cast<ItemCamera*>(m_new_item);
        m_scene->addItem(camera);
        camera->setPos(m_scene->model_view()->mapToScene(m_new_pos));
        camera->setOriPos(m_scene->model_view()->mapToScene(m_new_pos));
        camera->update_data();
        camera->add_setting_widget();
        m_scene->model_view ()->property_widget ()->update_property_widget (camera->getWidgetSetting());
        setCurrentItemStatus(m_scene,camera->getWidgetSetting());
    }
    m_scene->update();
}

deleteItemCommand::deleteItemCommand(int oritype, int newtype, QPointF oriPos,
                                     QPointF newPos, ModelGraphicsScene *scene,
                                     QGraphicsItem* oriItem,QGraphicsItem* newItem,
                                     QGraphicsItem * roboItem,
                                     QUndoCommand *parent)
    :QUndoCommand(parent)
    ,m_ori_Type(oritype)
    ,m_new_Type(newtype)
    ,m_ori_pos(oriPos)
    ,m_new_pos(newPos)
    ,m_scene(scene)
    ,m_oriItem(oriItem)
    ,m_newItem(newItem)
    ,m_roboItem(roboItem)
{

}

void deleteItemCommand::undo()
{
    SCDebug<<"m_new_type is "<<m_new_Type;

    if(m_new_Type==ItemPolygon::Type)          //多边形
    {
        ItemPolygon * ipoly = qgraphicsitem_cast<ItemPolygon*>(m_newItem);
        m_scene->addItem(ipoly);
        m_scene->get_PolygonList()->append(ipoly);
        ipoly->add_setting_widget();
        ipoly->setPos(m_new_pos.x(),m_new_pos.y());
        ItemRobotModel* roboItem  = qgraphicsitem_cast<ItemRobotModel*>(m_roboItem);
        ipoly->stackBefore(roboItem);
    }
    else if(m_new_Type== ItemPolypos::Type)
    {
        ItemPolypos* ipos = qgraphicsitem_cast<ItemPolypos*>(m_newItem);
        SCDebug<<"ipos index in polygon is "<<ipos->getIndexInPolygon();
        ipos->getparentPolygon()->get_polygon_pos_list()->insert(ipos->getIndexInPolygon(),ipos);
        ipos->getparentPolygon()->update();
        ipos->show();
        ipos->add_setting_widget();
        ipos->setPos(m_new_pos.x(),m_new_pos.y());
    }
    else if(m_new_Type==ItemLaser::Type)//激光
    {
        ItemLaser * ilaser = qgraphicsitem_cast<ItemLaser*>(m_newItem);
        m_scene->addItem (ilaser);
        m_scene->get_Laserlist()->append(ilaser);
        ilaser->add_setting_widget();
        ilaser->setPos(m_new_pos.x(),m_new_pos.y());
        ilaser->update_data ();
    }
    else if(m_new_Type==ItemUltrasonic::Type)//ultrasonic
    {
        if(m_scene->get_ultrasonic_list()->size()< m_scene->model_view()->basic_widget()->model_base_data_.max_ultrasonic_num_)
        {
            ItemUltrasonic * iul = qgraphicsitem_cast<ItemUltrasonic*>(m_newItem);
            m_scene->addItem (iul);
            m_scene->get_ultrasonic_list()->append(iul);
            iul->add_setting_widget();
            iul->setPos(m_new_pos.x(),m_new_pos.y());
            iul->update_data ();

        }
    }
    else if(m_new_Type==ItemDI::Type)//DI
    {
        if(m_scene->get_Di_list()->size()< m_scene->model_view()->basic_widget()->model_base_data_.max_di_num_)
        {
            ItemDI * idi = qgraphicsitem_cast<ItemDI*>(m_newItem);
            m_scene->addItem (idi);
            m_scene->get_Di_list()->append(idi);
            idi->add_setting_widget();
            idi->setPos(m_new_pos.x(),m_new_pos.y());
            idi->update_data ();
        }
    }
    else if(m_new_Type==ItemMagneticSensor::Type)//magneticScesor
    {
        ItemMagneticSensor * MagneticSensor = qgraphicsitem_cast<ItemMagneticSensor*>(m_newItem);
        m_scene->addItem (MagneticSensor);
        m_scene->get_magneticSensor_list()->append(MagneticSensor);
        MagneticSensor->add_setting_widget();
        MagneticSensor->setPos(m_new_pos.x(),m_new_pos.y());
        MagneticSensor->update_data ();
    }
    else if(m_new_Type==ItemDO::Type)//DO
    {
        if(m_scene->get_Do_list()->size()< m_scene->model_view()->basic_widget()->model_base_data_.max_do_num_)
        {
            ItemDO * ido = qgraphicsitem_cast<ItemDO*>(m_newItem);
            m_scene->addItem (ido);
            m_scene->get_Do_list()->append(ido);
            ido->add_setting_widget();
            ido->update_data ();
            ido->setPos(m_new_pos.x(),m_new_pos.y());
        }
    }
    else if(m_new_Type == ItemCrashDI::Type)
    {
        ItemCrashDI* crashDi = qgraphicsitem_cast<ItemCrashDI*>(m_newItem);
        m_scene->addItem(crashDi);
        m_scene->get_crashDi_list()->append(crashDi);
        crashDi->add_setting_widget();
        crashDi->setPos(m_new_pos.x(),m_new_pos.y());
    }
    else if(m_new_Type == ItemCrashDIpos::Type)
    {
        ItemCrashDIpos* crashDipos = qgraphicsitem_cast<ItemCrashDIpos*>(m_newItem);
        crashDipos->add_setting_widget();
        crashDipos->getParent()->getLine_pos_list()->insert(crashDipos->getIndexfromLine(),crashDipos);
        crashDipos->getParent()->update();
        crashDipos->show();
        crashDipos->setPos(m_new_pos.x(),m_new_pos.y());
    }
    else if(m_new_Type == ItemRFIDSensor::Type)
    {
        ItemRFIDSensor* RFID = qgraphicsitem_cast<ItemRFIDSensor*>(m_newItem);
        m_scene->addItem(RFID);
        RFID->add_setting_widget();
        m_scene->get_RFID_list()->append(RFID);
        RFID->setPos(m_new_pos.x(),m_new_pos.y());
    }
    else if(m_new_Type == ItemCamera::Type)
    {
        ItemCamera* camera = qgraphicsitem_cast<ItemCamera*>(m_newItem);
        m_scene->addItem(camera);
        camera->add_setting_widget();
        m_scene->get_Camera_list()->append(camera);
        camera->setPos(m_new_pos.x(),m_new_pos.y());
    }

    m_scene->update();
    setCurrentItemStatus(dynamic_cast<ItemBase*>(m_newItem)->model_scene(),dynamic_cast<ItemBase*>(m_newItem)->getWidgetSetting());
    m_scene->model_view()->update();
}

void deleteItemCommand::redo()
{
    qDebug()<<"redo ----------------------18---------------";

    if(m_new_Type ==ItemPolypos::Type)
    {
        ItemPolypos* tempItem = qgraphicsitem_cast<ItemPolypos*>(m_newItem);
        tempItem->getparentPolygon()->get_polygon_pos_list()->removeOne(tempItem);
        tempItem->getparentPolygon()->update();
        tempItem->hide();
    }

    else if(m_new_Type ==ItemPolygon::Type)
    {
        ItemPolygon* tempItem = qgraphicsitem_cast<ItemPolygon*>(m_newItem);
        m_scene->get_PolygonList()->removeOne (tempItem);
        m_scene->removeItem (tempItem);
    }

    else if(m_new_Type ==ItemLaser::Type)
    {
        ItemLaser* tempItem = qgraphicsitem_cast<ItemLaser*>(m_newItem);
        m_scene->get_Laserlist()->removeOne(tempItem);
        m_scene->removeItem (tempItem);
    }
    else if(m_new_Type ==ItemDI::Type)
    {
        ItemDI* tempItem = qgraphicsitem_cast<ItemDI*>(m_newItem);
        m_scene->get_Di_list()->removeOne (tempItem);
        m_scene->removeItem (tempItem);
    }
    else if(m_new_Type ==ItemMagneticSensor::Type)
    {
        ItemMagneticSensor* tempItem = qgraphicsitem_cast<ItemMagneticSensor*>(m_newItem);
        m_scene->get_magneticSensor_list()->removeOne (tempItem);
        m_scene->removeItem (tempItem);
    }

    else if(m_new_Type ==ItemUltrasonic::Type)
    {
        ItemUltrasonic* tempItem = qgraphicsitem_cast<ItemUltrasonic*>(m_newItem);
        m_scene->get_ultrasonic_list()->removeOne (tempItem);
        m_scene->removeItem (tempItem);
    }

    else if(m_new_Type==ItemDO::Type)
    {
        ItemDO* tempItem = qgraphicsitem_cast<ItemDO*>(m_newItem);
        m_scene->get_Do_list()->removeOne(tempItem);
        m_scene->removeItem(tempItem);
    }
    else if(m_new_Type==ItemCrashDI::Type )
    {
        SCDebug<<"remove ItemCrashDI ";
        ItemCrashDI* tempItem = qgraphicsitem_cast<ItemCrashDI*>(m_newItem);
        m_scene->get_crashDi_list()->removeOne(tempItem);
        m_scene->removeItem(tempItem);
    }
    else if(m_new_Type == ItemCrashDIpos::Type)
    {
        SCDebug<<"remove itemCrashDi pos";
        ItemCrashDIpos* tempItem = qgraphicsitem_cast<ItemCrashDIpos*>(m_newItem);
        SCDebug<<"before removing pos size is "<< tempItem->getParent()->getLine_pos_list()->size();
        tempItem->getParent()->getLine_pos_list()->removeOne(tempItem);
        SCDebug<<"after removing pos size is "<< tempItem->getParent()->getLine_pos_list()->size();
        tempItem->hide();
        tempItem->getParent()->update();
    }
    else if(m_new_Type == ItemRFIDSensor::Type)
    {
        ItemRFIDSensor* tempItem = qgraphicsitem_cast<ItemRFIDSensor*>(m_newItem);
        m_scene->get_RFID_list()->removeOne(tempItem);
        m_scene->removeItem(tempItem);
    }
    else if(m_new_Type == ItemCamera::Type)
    {
        ItemCamera* tempItem = qgraphicsitem_cast<ItemCamera*>(m_newItem);
        m_scene->get_Camera_list()->removeOne(tempItem);
        m_scene->removeItem(tempItem);
    }
    m_scene->model_view ()->property_widget()->update_layer (QList<ItemBase *>());
    m_scene->update();
}


ipEditCommand::ipEditCommand(QIpAddressEdit *ipEdit, QString ori_ip, QString new_ip, PropertyBasicWidget* parentWid,QUndoCommand *parent)
    :QUndoCommand(parent)
    ,m_ori_ip(ori_ip)
    ,m_new_ip(new_ip)
    ,m_ipEdit(ipEdit)
    ,m_parentWid(parentWid)
{

}

void ipEditCommand::undo()
{
    m_ipEdit->blockSignals(true);
    m_ipEdit->setText(m_ori_ip);
    m_ipEdit->blockSignals(false);
    undoToShowRelativeWidget(m_parentWid);
}

void ipEditCommand::redo()
{
    qDebug()<<"redo ---------------------5---------------";
    m_ipEdit->blockSignals(true);
    m_ipEdit->setText(m_new_ip);
    m_ipEdit->blockSignals(false);
    undoToShowRelativeWidget(m_parentWid);

}

layerCommand::layerCommand(SCDragDropListWidget* listWiget,QList<ItemBase*>selected_item_list, QList<QListWidgetItem *> ori_list, QList<QListWidgetItem *> new_list, QUndoCommand *parent)
    :QUndoCommand(parent)
    ,m_ori_itemlist(ori_list)
    ,m_new_itemlist(new_list)
    ,m_selected_item_list(selected_item_list)
    ,m_listWiget(listWiget)
{

}

void layerCommand::undo()
{
    SCDebug<<"layer undo ..";

    templist.clear();

    SCDebug<<"morilist size is "<<m_ori_itemlist.size();
    for(int i = 0; i< m_ori_itemlist.count();i++)
    {

        QListWidgetItem* tempItem = new QListWidgetItem;
        tempItem->setText(m_ori_itemlist.at(i)->text());
        tempItem->setData(Qt::UserRole,m_ori_itemlist.at(i)->data(Qt::UserRole));
        templist.push_back(tempItem);
        SCDebug<<"tempItem  is "<<tempItem->text();
    }

    m_listWiget->clear();

    for(int r= 0; r <templist.size();r++)
    {
        m_listWiget->addItem(templist.at(r));
    }

    SCDebug<<"undo  first listwidget item is "<<m_listWiget->item(0)->text();

    int tepIndex= 100;
    for(int ti=0;ti < m_listWiget->count(); ti++)
    {
        int z= m_listWiget->item (ti)->data (Qt::UserRole).toInt ();
        if(m_selected_item_list.at(z)->type()==I_scanArea_model)
        {
            if(dynamic_cast<ItemScanArea*>(m_selected_item_list.at(z))->getParent()->type()==I_di)
            {
                dynamic_cast<ItemScanArea*>(m_selected_item_list.at(z))->getParent()->setZValue(tepIndex);
                tepIndex--;
            }
            else if(dynamic_cast<ItemScanArea*>(m_selected_item_list.at(z))->getParent()->type()==I_ultrasonic)
            {
                dynamic_cast<ItemScanArea*>(m_selected_item_list.at(z))->getParent()->setZValue(tepIndex);
                tepIndex--;
            }

        }
        else
        {
            m_selected_item_list.at(z)->setZValue(tepIndex);
            tepIndex--;
        }

    }

    m_selected_item_list.last ()->model_scene ()->update ();
}

void layerCommand::redo()
{
    qDebug()<<"redo ----------------------11---------------";

    templist.clear();
    for(int j = 0; j< m_new_itemlist.count();j++)
    {
        QListWidgetItem* tempItem = new QListWidgetItem;
        tempItem->setText(m_new_itemlist.at(j)->text());
        tempItem->setData(Qt::UserRole,m_new_itemlist.at(j)->data(Qt::UserRole).toInt()) ;
        templist.push_back(tempItem);
        SCDebug<<"tempItem  is "<<tempItem->text()<<"item data is "<<m_new_itemlist.at(j)->data(Qt::UserRole).toInt();
    }

    m_listWiget->clear();

    for(int r= 0; r <templist.size();r++)
    {
        m_listWiget->addItem(templist.at(r));
    }

    SCDebug<<"redo first listwidget item is "<<m_listWiget->item(0)->text();
    int tepIndex= 100;
    for(int ti=0;ti < m_listWiget->count(); ti++)
    {
        int z= m_listWiget->item(ti)->data (Qt::UserRole).toInt ();
        SCDebug<<"z is "<<z;
        //设置zValue来管理层级关系时，若itemA有子subitem时若想设置itemB与subItem的层级关系要设置itemA与itemB的zValue的关系
        //zValue scene先画层级低的item， subitem属于itemA，要通过设置itemA的zValue来达到控制subitem的层级
        if(m_selected_item_list.at(z)->type()==I_scanArea_model)
        {
            if(dynamic_cast<ItemScanArea*>(m_selected_item_list.at(z))->getParent()->type()==I_di)
            {
                dynamic_cast<ItemScanArea*>(m_selected_item_list.at(z))->getParent()->setZValue(tepIndex);
                tepIndex--;
            }
            else if(dynamic_cast<ItemScanArea*>(m_selected_item_list.at(z))->getParent()->type()==I_ultrasonic)
            {
                dynamic_cast<ItemScanArea*>(m_selected_item_list.at(z))->getParent()->setZValue(tepIndex);
                tepIndex--;
            }

        }
        else
        {
            m_selected_item_list.at(z)->setZValue(tepIndex);
            tepIndex--;
        }
    }

    m_selected_item_list.last ()->model_scene ()->update ();
}

zoomInOutCommand::zoomInOutCommand(ModelGraphicsScene* scene, int oriScale, int newScale, QUndoCommand *parent)
    :QUndoCommand(parent)
    ,m_ori_scale(oriScale)
    ,m_new_scale(newScale)
    ,m_scene(scene)

{

}

void zoomInOutCommand::undo()
{
    m_scene->model_view()->resetTransform();
    QMatrix view_matrix(1,0,0 ,-1,0,0);
    view_matrix.rotate (90);
    m_scene->model_view()->setMatrix(view_matrix,true);
    SCDebug<<"undo scale is "<<m_ori_scale;
    //    m_scene->model_view()->set_scale_size(m_ori_scale);
    m_scene->model_view()->scale(m_ori_scale,m_ori_scale);
}

void zoomInOutCommand::redo()
{
    qDebug()<<"redo ----------------------13---------------";
    m_scene->model_view()->resetTransform();
    QMatrix view_matrix(1,0,0 ,-1,0,0);
    view_matrix.rotate (90);
    m_scene->model_view()->setMatrix(view_matrix,true);
    SCDebug<<"redo scale is "<<m_new_scale;
    //    m_scene->model_view()->set_scale_size(m_new_scale);
    m_scene->model_view()->scale(m_new_scale,m_new_scale);
}



DoStateCommand::DoStateCommand(customPushBtn* doBtn,int oriState,int newState,QUndoCommand *parent)
    :QUndoCommand(parent)
    ,m_oriState(oriState)
    ,m_newState(newState)
    ,m_doBtn(doBtn)
{

}

void DoStateCommand::undo()
{
    if(m_oriState)
    {
        m_doBtn->blockSignals(true);
        m_doBtn->setStyleSheet(QString("QPushButton{background-color:green;border-radius:14px;}"));
        m_doBtn->setChecked(true);//为防止在undo 时进行压栈，阻塞状态变化消息
        m_doBtn->blockSignals(false);
    }
    else
    {
        m_doBtn->blockSignals(true);
        m_doBtn->setChecked(false);
        m_doBtn->setStyleSheet(QString("QPushButton{background-color:red;border-radius:14px;}"));
        m_doBtn->blockSignals(false);
    }
}

void DoStateCommand::redo()
{
    qDebug()<<"redo ----------------------20---------------";
    if(m_newState)
    {
        m_doBtn->blockSignals(true);
        m_doBtn->setStyleSheet(QString("QPushButton{background-color:green;border-radius:14px;}"));
        m_doBtn->setChecked(true);
        m_doBtn->blockSignals(false);
    }
    else
    {
        m_doBtn->blockSignals(true);
        m_doBtn->setStyleSheet(QString("QPushButton{background-color:red;border-radius:14px;}"));
        m_doBtn->setChecked(false);
        m_doBtn->blockSignals(false);
    }

}

PosDoubleSpinBoxCommand::PosDoubleSpinBoxCommand(customDoubleSpinBox *x_spinBox,
                                                 customDoubleSpinBox *y_spinBox,
                                                 QGraphicsItem *item,
                                                 double x_ori_value,
                                                 double x_new_value,
                                                 double y_ori_value,
                                                 double y_new_value, PropertyBasicWidget *parentWid,
                                                 QUndoCommand *parent)
    :QUndoCommand(parent)
    ,m_x_oriValue(x_ori_value)
    ,m_x_newValue(x_new_value)
    ,m_y_oriValue(y_ori_value)
    ,m_y_newValue(y_new_value)
    ,m_item(item)
    ,m_x_doubleSpinBox(x_spinBox)
    ,m_y_doubleSpinBox(y_spinBox)
    ,m_parentWid(parentWid)
{

}

void PosDoubleSpinBoxCommand::undo()
{

    m_x_doubleSpinBox->setIsClickedTrigger(false);
    m_y_doubleSpinBox->setIsClickedTrigger(false);

    m_x_doubleSpinBox->setValue(m_x_oriValue);
    m_y_doubleSpinBox->setValue(m_y_oriValue);
    if(m_item->type()==ItemPolypos::Type)
    {
        ItemPolypos* temp_item = qgraphicsitem_cast<ItemPolypos*>(m_item);
        temp_item->setPos(QPointF(m_x_oriValue*100,m_y_oriValue*100)
                          - temp_item->getparentPolygon()->scenePos());
        temp_item->model_scene()->update();
    }
    else if(m_item->type()==ItemCrashDIpos::Type)
    {
        SCDebug<<"dipos undo ";
        ItemCrashDIpos* temp_item = qgraphicsitem_cast<ItemCrashDIpos*>(m_item);
        temp_item->setPos(QPointF(m_x_oriValue*100,m_y_oriValue*100)-temp_item->getCrashDi()->scenePos());
        temp_item->model_scene()->update();
        undoToShowRelativeWidget(m_parentWid);
    }

}

void PosDoubleSpinBoxCommand::redo()
{
    qDebug()<<"redo ----------------------8--------------";
    m_x_doubleSpinBox->setIsClickedTrigger(false);
    m_y_doubleSpinBox->setIsClickedTrigger(false);
    if(m_item->type()==ItemPolypos::Type)
    {
        ItemPolypos* temp_item = qgraphicsitem_cast<ItemPolypos*>(m_item);
        temp_item->setPos(QPointF(m_x_newValue*100,m_y_newValue*100)
                          - temp_item->getparentPolygon()->scenePos());
        temp_item->model_scene()->update();
    }
    else if(m_item->type()==ItemCrashDIpos::Type)
    {
        SCDebug<<"dipos redo ";
        ItemCrashDIpos* temp_item = qgraphicsitem_cast<ItemCrashDIpos*>(m_item);
        temp_item->setPos(QPointF(m_x_newValue*100,m_y_newValue*100)-temp_item->getCrashDi()->scenePos());
        temp_item->model_scene()->update();
        if(GlobalVariant::getInst()->isRedo)
        {
            m_x_doubleSpinBox->setValue(m_x_newValue);
            m_y_doubleSpinBox->setValue(m_y_newValue);
            undoToShowRelativeWidget(m_parentWid);
        }
    }

}

addPosCommand::addPosCommand(ItemBase *item, int index, QGraphicsItem *oriItem, QGraphicsItem *newItem, ModelGraphicsScene *scene, QUndoCommand *parent)
    :QUndoCommand(parent)
    ,m_oriItem(oriItem)
    ,m_newItem(newItem)
    ,m_scene(scene)
    ,m_Item(item)
    ,m_index(index)

{

}

void addPosCommand::undo()
{
    if(m_Item->type()==ItemCrashDI::Type)
    {
        SCDebug<<"addPosCommand::undo()";
        ItemCrashDI* temp_item = qgraphicsitem_cast<ItemCrashDI*>(m_Item);
        ItemCrashDIpos* item_pos = qgraphicsitem_cast<ItemCrashDIpos*>(m_newItem);
        temp_item->getLine_pos_list()->removeOne(item_pos);//从列表中删除结点
        item_pos->hide();//隐藏结点
        temp_item->update();
    }
    else if(m_Item->type()==ItemPolygon::Type)
    {
        ItemPolygon* temp_item = qgraphicsitem_cast<ItemPolygon*>(m_Item);
        ItemPolypos* item_polypos = qgraphicsitem_cast<ItemPolypos*>(m_newItem);
        temp_item->get_polygon_pos_list()->removeOne(item_polypos);//从列表中删除结点
        item_polypos->hide();//隐藏结点
        temp_item->update();
        m_scene->update();
    }
    m_scene->update();
}

void addPosCommand::redo()
{
    qDebug()<<"redo ----------------------19---------------";
    if(m_Item->type()==ItemCrashDI::Type)
    {
        ItemCrashDI* temp_item = qgraphicsitem_cast<ItemCrashDI*>(m_Item);
        ItemCrashDIpos* item_linepos=qgraphicsitem_cast<ItemCrashDIpos*>(m_newItem);
        item_linepos->show();
        temp_item->getLine_pos_list()->insert(m_index,item_linepos);
        temp_item->update();
    }
    else if(m_Item->type()==ItemPolygon::Type)
    {
        SCDebug<<"addPolgonPosCommand redo ...";
        ItemPolygon* temp_item=qgraphicsitem_cast<ItemPolygon*>(m_Item);
        ItemPolypos* item_polyPos = qgraphicsitem_cast<ItemPolypos*>(m_newItem);
        temp_item->get_polygon_pos_list()->insert(m_index,item_polyPos);
        item_polyPos->show();
        temp_item->update();
        m_scene->update();
    }
    m_scene->update();
}

deleteItemsCommand::deleteItemsCommand(ModelGraphicsScene *scene, QList<ItemBase *> items, QUndoCommand *parent):
    QUndoCommand(parent),
    m_itemlist(items),
    m_scene(scene)
{

}

void deleteItemsCommand::undo()
{

    foreach (auto var, m_itemlist) {
        m_scene->addItem(var);
        var->setPos(var->getOriPos());
        if(var->type() ==ItemPolygon::Type)
        {
            m_scene->get_PolygonList()->append(dynamic_cast<ItemPolygon*>(var));
        }
        else if(var->type() ==ItemLaser::Type)
        {
            m_scene->get_Laserlist()->append(dynamic_cast<ItemLaser*>(var));
        }
        else if(var->type() ==ItemDI::Type)
        {
            m_scene->get_Di_list()->append(dynamic_cast<ItemDI*>(var));
        }
        else if(var->type() ==ItemMagneticSensor::Type)
        {
            m_scene->get_magneticSensor_list()->append(dynamic_cast<ItemMagneticSensor*>(var));
        }

        else if(var->type() ==ItemUltrasonic::Type)
        {
            m_scene->get_ultrasonic_list()->append(dynamic_cast<ItemUltrasonic*>(var));
        }

        else if(var->type()==ItemDO::Type)
        {
            m_scene->get_Do_list()->append(dynamic_cast<ItemDO*>(var));
        }
        else if(var->type()==ItemCrashDI::Type )
        {
            m_scene->get_crashDi_list()->append(dynamic_cast<ItemCrashDI*>(var));
        }
        else if(var->type() == ItemCrashDIpos::Type)
        {
            qgraphicsitem_cast<ItemCrashDIpos*>(var)->getParent()->getLine_pos_list()->append(dynamic_cast<ItemCrashDIpos*>(var));
        }
        else if(var->type() == ItemRFIDSensor::Type)
        {
            m_scene->get_RFID_list()->append(dynamic_cast<ItemRFIDSensor*>(var));
        }
        else if(var->type()==ItemCamera::Type)
        {
            m_scene->get_Camera_list()->append(dynamic_cast<ItemCamera*>(var));
        }

        var->setSelected(false);
    }
}

void deleteItemsCommand::redo()
{
    qDebug()<<"redo ----------------------18---------------";
    foreach (auto var, m_itemlist) {

        if(var->type() ==ItemPolygon::Type)
        {
            m_scene->get_PolygonList()->removeOne(dynamic_cast<ItemPolygon*>(var));
            m_scene->removeItem(var);
        }
        else if(var->type() ==ItemLaser::Type)
        {
            m_scene->get_Laserlist()->removeOne(dynamic_cast<ItemLaser*>(var));
            m_scene->removeItem(var);
        }
        else if(var->type() ==ItemDI::Type)
        {
            m_scene->get_Di_list()->removeOne(dynamic_cast<ItemDI*>(var));
            m_scene->removeItem(var);
        }
        else if(var->type() ==ItemMagneticSensor::Type)
        {
            m_scene->get_magneticSensor_list()->removeOne(dynamic_cast<ItemMagneticSensor*>(var));
            m_scene->removeItem(var);
        }

        else if(var->type() ==ItemUltrasonic::Type)
        {
            m_scene->get_ultrasonic_list()->removeOne(dynamic_cast<ItemUltrasonic*>(var));
            m_scene->removeItem(var);
        }
        else if(var->type()==ItemDO::Type)
        {
            m_scene->get_Do_list()->removeOne(dynamic_cast<ItemDO*>(var));
            m_scene->removeItem(var);
        }
        else if(var->type()==ItemCrashDI::Type )
        {
            m_scene->get_crashDi_list()->removeOne(dynamic_cast<ItemCrashDI*>(var));
            m_scene->removeItem(var);
        }
        else if(var->type() == ItemCrashDIpos::Type)
        {
            //如果当点的parent也被选中
            //            if(!m_itemlist.contains(dynamic_cast<ItemCrashDIpos*>(var)->getParent()))
            //            {
            //                if(dynamic_cast<ItemCrashDIpos*>(var) ==qgraphicsitem_cast<ItemCrashDIpos*>(var)->getParent()->getStartPos()||
            //                        dynamic_cast<ItemCrashDIpos*>(var) == qgraphicsitem_cast<ItemCrashDIpos*>(var)->getParent()->getEndPos())
            //                {
            //                    SCDebug<<"-==--------------------invalid pos ---------";
            //                }
            //                else
            //                {
            //                    SCDebug<<"-==--------------------delete valid pos ---------";
            //                    qgraphicsitem_cast<ItemCrashDIpos*>(var)->getParent()->getLine_pos_list()->removeOne(dynamic_cast<ItemCrashDIpos*>(var));
            //                    m_scene->removeItem(var);
            //                }
            //            }
        }
        else if(var->type() == ItemRFIDSensor::Type)
        {
            m_scene->get_RFID_list()->removeOne(dynamic_cast<ItemRFIDSensor*>(var));
            m_scene->removeItem(var);
        }
        else if(var->type()==ItemCamera::Type)
        {
            m_scene->get_Camera_list()->removeOne(dynamic_cast<ItemCamera*>(var));
            m_scene->removeItem(var);
        }

        m_scene->update();
    }
}

moveItemsCommand::moveItemsCommand(ModelGraphicsScene *scene, QMap<ItemBase*,QPointF> oriItems, QMap<ItemBase*,QPointF> newItems, QUndoCommand *parent):
    QUndoCommand(parent),
    m_oriItemMap(oriItems),
    m_newItemMap(newItems),
    m_scene(scene)
{

}

void moveItemsCommand::undo()
{
    QMap<ItemBase*,QPointF>::iterator iter = m_oriItemMap.begin();
    for(;iter != m_oriItemMap.end();iter++)
    {
        iter.key()->setPos(iter.value());
        iter.key()->setOriPos(iter.value());
    }
}

void moveItemsCommand::redo()
{
    qDebug()<<"redo ----------------------14---------------";
    QMap<ItemBase*,QPointF>::iterator iter = m_newItemMap.begin();
    for(;iter != m_newItemMap.end();iter++)
    {
        iter.key()->setPos(iter.value());
        iter.key()->setOriPos(iter.value());
    }
}

void undoToShowRelativeWidget(PropertyBasicWidget *parentWid)
{
    switch (parentWid->getType())
    {
    case WIDGET_DO:
    {
        WidgetDOSetting* tempWid = dynamic_cast<WidgetDOSetting*>(parentWid);
        ItemDO*  temp_item = qgraphicsitem_cast<ItemDO*>(tempWid->getRelateItem());
        temp_item->add_setting_widget();
        temp_item->model_scene()->model_view()->property_widget()->update_property_widget(temp_item->getWidgetSetting());
        temp_item->update();
        setCurrentItemStatus(temp_item->model_scene(),parentWid);
        break;
    }
    case WIDGET_DI:
    {
        WidgetDISetting* tempWid = dynamic_cast<WidgetDISetting*>(parentWid);
        ItemDI*  temp_item = qgraphicsitem_cast<ItemDI*>(tempWid->getRelateItem());
        temp_item->add_setting_widget();
        temp_item->model_scene()->model_view()->property_widget()->update_property_widget(temp_item->getWidgetSetting());
        setCurrentItemStatus(temp_item->model_scene(),parentWid);
        temp_item->update();
        break;
    }
        break;
    case WIDGET_LASER:
    {
        WidgetLaserSetting* tempWid = dynamic_cast<WidgetLaserSetting*>(parentWid);
        ItemLaser*  temp_item = qgraphicsitem_cast<ItemLaser*>(tempWid->getRelateItem());
        temp_item->add_setting_widget();
        if(!temp_item->getWidgetSetting()->isHidden())return;
        temp_item->model_scene()->model_view()->property_widget()->update_property_widget(temp_item->getWidgetSetting());
        setCurrentItemStatus(temp_item->model_scene(),parentWid);
        temp_item->update();
    }
        break;
    case WIDGET_ROBOTMODEL:
    {
        WidgetRobotModelSetting* tempWid = dynamic_cast<WidgetRobotModelSetting*>(parentWid);
        ItemRobotModel*  temp_item = qgraphicsitem_cast<ItemRobotModel*>(tempWid->getRelateItem());
        temp_item->add_setting_widget();
        temp_item->model_scene()->model_view()->property_widget()->update_property_widget(temp_item->getWidgetSetting());
        setCurrentItemStatus(temp_item->model_scene(),parentWid);
        temp_item->update();
    }
        break;
    case WIDGET_ULTRASIONIC:
    {
        WidgetUltrasonicSetting* tempWid = dynamic_cast<WidgetUltrasonicSetting*>(parentWid);
        ItemUltrasonic*  temp_item = qgraphicsitem_cast<ItemUltrasonic*>(tempWid->getRelateItem());
        temp_item->add_setting_widget();
        temp_item->model_scene()->model_view()->property_widget()->update_property_widget(temp_item->getWidgetSetting());
        setCurrentItemStatus(temp_item->model_scene(),parentWid);
        temp_item->update();
    }
        break;
    case WIDGET_MAGENETIC:
    {
        WidgetMagneticSensorSetting* tempWid = dynamic_cast<WidgetMagneticSensorSetting*>(parentWid);
        ItemMagneticSensor*  temp_item = qgraphicsitem_cast<ItemMagneticSensor*>(tempWid->getRelateItem());
        temp_item->add_setting_widget();
        temp_item->model_scene()->model_view()->property_widget()->update_property_widget(temp_item->getWidgetSetting());
        setCurrentItemStatus(temp_item->model_scene(),parentWid);
        temp_item->update();
    }
        break;
    case WIDGET_RFID:
    {
        WidgetRFIDsetting* tempWid = dynamic_cast<WidgetRFIDsetting*>(parentWid);
        ItemRFIDSensor*  temp_item = qgraphicsitem_cast<ItemRFIDSensor*>(tempWid->getRelateItem());
        temp_item->add_setting_widget();
        temp_item->model_scene()->model_view()->property_widget()->update_property_widget(temp_item->getWidgetSetting());
        setCurrentItemStatus(temp_item->model_scene(),parentWid);
        temp_item->update();
    }
        break;
    case WIDGET_CRASHDI:
    {
        WidgetCrashDISetting* tempWid = dynamic_cast<WidgetCrashDISetting*>(parentWid);
        ItemCrashDI*  temp_item = qgraphicsitem_cast<ItemCrashDI*>(tempWid->getRelateItem());
        temp_item->add_setting_widget();
        temp_item->model_scene()->model_view()->property_widget()->update_property_widget(temp_item->getWidgetSetting());
        setCurrentItemStatus(temp_item->model_scene(),parentWid);
        temp_item->update();
    }
        break;
    case WIDGET_CRASHDIPOS:
    {
        qDebug()<<"crashdi pos-------------";
        WidgetCrashDiPosSetting* tempWid = dynamic_cast<WidgetCrashDiPosSetting*>(parentWid);
        ItemCrashDIpos*  temp_item = qgraphicsitem_cast<ItemCrashDIpos*>(tempWid->getRelateItem());
        temp_item->add_setting_widget();
        temp_item->model_scene()->model_view()->property_widget()->update_property_widget(temp_item->getWidgetSetting());
        setCurrentItemStatus(temp_item->model_scene(),parentWid);
        temp_item->update();
    }
        break;
    case WIDGET_VISION:
    {
        WidgetCameraSetting* tempWid = dynamic_cast<WidgetCameraSetting*>(parentWid);
        ItemCamera* temp_item = qgraphicsitem_cast<ItemCamera*>(tempWid->getRelateItem());
        temp_item->add_setting_widget();
        temp_item->model_scene()->model_view()->property_widget()->update_property_widget(temp_item->getWidgetSetting());
        setCurrentItemStatus(temp_item->model_scene(),parentWid);
        temp_item->update();
    }
        break;
    default:
        break;
    }
}

void setCurrentItemStatus(ModelGraphicsScene *scene, PropertyBasicWidget *parentWid)
{

    //先清除所有item选中状态
    removeAllItemFocusStatus(scene);
    switch (parentWid->getType())
    {
    case WIDGET_DO:
    {
        //当前item设置选中状态
        WidgetDOSetting* tempWid = dynamic_cast<WidgetDOSetting*>(parentWid);
        ItemDO*  temp_item = qgraphicsitem_cast<ItemDO*>(tempWid->getRelateItem());
        temp_item->setSelected(true);
        temp_item->update();
    }
        break;
    case WIDGET_DI:
    {
        //当前item设置选中状态
        WidgetDISetting* tempWid = dynamic_cast<WidgetDISetting*>(parentWid);
        ItemDI*  temp_item = qgraphicsitem_cast<ItemDI*>(tempWid->getRelateItem());
        temp_item->setSelected(true);
        temp_item->update();
    }
        break;
    case WIDGET_LASER:
    {
        WidgetLaserSetting* tempWid = dynamic_cast<WidgetLaserSetting*>(parentWid);
        ItemLaser*  temp_item = qgraphicsitem_cast<ItemLaser*>(tempWid->getRelateItem());
        temp_item->setSelected(true);
        temp_item->update();
    }
        break;
    case WIDGET_ROBOTMODEL:
    {
        WidgetRobotModelSetting* tempWid = dynamic_cast<WidgetRobotModelSetting*>(parentWid);
        ItemRobotModel*  temp_item = qgraphicsitem_cast<ItemRobotModel*>(tempWid->getRelateItem());
        temp_item->setSelected(true);
        temp_item->update();
    }
        break;
    case WIDGET_ULTRASIONIC:
    {
        WidgetUltrasonicSetting* tempWid = dynamic_cast<WidgetUltrasonicSetting*>(parentWid);
        ItemUltrasonic*  temp_item = qgraphicsitem_cast<ItemUltrasonic*>(tempWid->getRelateItem());
        temp_item->setSelected(true);
        temp_item->update();
    }
        break;
    case WIDGET_MAGENETIC:
    {
        //当前item设置选中状态
        WidgetMagneticSensorSetting* tempWid = dynamic_cast<WidgetMagneticSensorSetting*>(parentWid);
        ItemMagneticSensor*  temp_item = qgraphicsitem_cast<ItemMagneticSensor*>(tempWid->getRelateItem());
        temp_item->setSelected(true);
        temp_item->update();
    }
        break;
    case WIDGET_RFID:
    {
        //当前item设置选中状态
        WidgetRFIDsetting* tempWid = dynamic_cast<WidgetRFIDsetting*>(parentWid);
        ItemRFIDSensor*  temp_item = qgraphicsitem_cast<ItemRFIDSensor*>(tempWid->getRelateItem());
        temp_item->setSelected(true);
        temp_item->update();
    }
        break;
    case WIDGET_CRASHDI:
    {
        WidgetCrashDISetting* tempWid = dynamic_cast<WidgetCrashDISetting*>(parentWid);
        ItemCrashDI*  temp_item = qgraphicsitem_cast<ItemCrashDI*>(tempWid->getRelateItem());
        temp_item->setSelected(true);
        temp_item->update();
    }
        break;
    case WIDGET_CRASHDIPOS:
    {
        WidgetCrashDiPosSetting* tempWid = dynamic_cast<WidgetCrashDiPosSetting*>(parentWid);
        ItemCrashDIpos*  temp_item = qgraphicsitem_cast<ItemCrashDIpos*>(tempWid->getRelateItem());
        temp_item->setSelected(true);
        temp_item->update();
    }
        break;
    case WIDGET_VISION:
    {
        WidgetCameraSetting* tempWid = dynamic_cast<WidgetCameraSetting*>(parentWid);
        ItemCamera* temp_item = qgraphicsitem_cast<ItemCamera*>(tempWid->getRelateItem());
        temp_item->setSelected(true);
        temp_item->update();
    }
    default:
        break;
    }
}

void removeDIFocusStatus(ModelGraphicsScene *scene)
{
    for(int i = 0; i<scene->get_Di_list()->size();i++)
    {
        scene->get_Di_list()->at(i)->setSelected(false);
    }
    scene->update();
}

void removeDOFocusStatus(ModelGraphicsScene *scene)
{
    for(int i = 0; i<scene->get_Do_list()->size();i++)
    {
        scene->get_Do_list()->at(i)->setSelected(false);
    }
    scene->update();
}

void removeLaserFocusStatus(ModelGraphicsScene *scene)
{
    for(int i = 0; i<scene->get_Laserlist() ->size();i++)
    {
        scene->get_Laserlist()->at(i)->setSelected(false);
    }
    scene->update();
}

void removeMagneticFocusStatus(ModelGraphicsScene *scene)
{
    for(int i = 0; i<scene->get_magneticSensor_list()->size();i++)
    {
        scene->get_magneticSensor_list()->at(i)->setSelected(false);
    }
    scene->update();
}

void removeRFIDFocusStatus(ModelGraphicsScene *scene)
{
    for(int i = 0; i<scene->get_RFID_list()->size();i++)
    {
        scene->get_RFID_list()->at(i)->setSelected(false);
    }
    scene->update();
}

void removeUltrasonicFocusStatus(ModelGraphicsScene *scene)
{
    for(int i = 0; i<scene->get_ultrasonic_list()->size();i++)
    {
        scene->get_ultrasonic_list()->at(i)->setSelected(false);
    }
    scene->update();
}

void removeRobotModelFocusStatus(ModelGraphicsScene *scene)
{
    scene->getItem_robot_model()->setSelected(false);
    scene->update();
}

void removeCrashDIFocusStatus(ModelGraphicsScene *scene)
{
    for(int i = 0; i<scene->get_crashDi_list()->size();i++)
    {
        scene->get_crashDi_list()->at(i)->setSelected(false);
    }
    scene->update();
}

void removeCrashDIPosFocusStatus(ModelGraphicsScene *scene)
{
    for(int i = 0; i<scene->get_crashDi_list()->size();i++)
    {
        for(int j = 0; j < scene->get_crashDi_list()->at(i)->getLine_pos_list()->size();j++)
        {
            scene->get_crashDi_list()->at(i)->getLine_pos_list()->at(j)->setSelected(false);
        }
    }
    scene->update();
}

void removeCameraFocusStatus(ModelGraphicsScene *scene)
{
    for(int i = 0; i<scene->get_Camera_list()->size();i++)
    {
        scene->get_Camera_list()->at(i)->setSelected(false);
    }
    scene->update();
}

void removeAllItemFocusStatus(ModelGraphicsScene *scene)
{
    removeDIFocusStatus(scene);
    removeDOFocusStatus(scene);
    removeLaserFocusStatus(scene);
    removeMagneticFocusStatus(scene);
    removeRFIDFocusStatus(scene);
    removeRobotModelFocusStatus(scene);
    removeUltrasonicFocusStatus(scene);
    removeCrashDIFocusStatus(scene);
    removeCrashDIPosFocusStatus(scene);
    removeCameraFocusStatus(scene);
}


checkBrakeCommand::checkBrakeCommand(QCheckBox *checkBrakebox, QCheckBox *checkAutoBrake, bool ori_check, bool new_check, PropertyBasicWidget *parentWid, QUndoCommand *parent)
{
    m_checkBrakeBox = checkBrakebox;
    m_checkAutoBox= checkAutoBrake;
    m_oriCheck = ori_check;
    m_newCheck = new_check;
    m_parentWid = parentWid;
}

void checkBrakeCommand::undo()
{
    m_checkBrakeBox->blockSignals(true);
    m_checkBrakeBox->setChecked(m_oriCheck);
    m_checkBrakeBox->blockSignals(false);
    if(!m_oriCheck) m_checkAutoBox->setChecked(false);
    m_oriCheck?m_checkAutoBox->setEnabled(true):m_checkAutoBox->setEnabled(false);
    undoToShowRelativeWidget(m_parentWid);
}

void checkBrakeCommand::redo()
{
    qDebug()<<"redo ----------------------66---------------";
    m_checkBrakeBox->blockSignals(true);
    m_checkBrakeBox->setChecked(m_newCheck);
    m_checkBrakeBox->blockSignals(false);
    if(!m_newCheck) m_checkAutoBox->setChecked(false);
    m_newCheck?m_checkAutoBox->setEnabled(true):m_checkAutoBox->setEnabled(false);
    undoToShowRelativeWidget(m_parentWid);
}

checkForkCommand::checkForkCommand(QCheckBox *checkbox, QWidget *forkWidget,bool ori_check, bool new_check, PropertyBasicWidget *parentWid, QUndoCommand *parent):
    QUndoCommand(parent),
    m_checkBox(checkbox),
    m_forkWidget(forkWidget),
    m_oriCheck(ori_check),
    m_newCheck(new_check)
//    m_parentWid(parentWid)
{

}

void checkForkCommand::undo()
{
    m_checkBox->blockSignals(true);
    m_checkBox->setChecked(m_oriCheck);
    dynamic_cast<WidgetForkSetting*>(m_forkWidget)->setIsConfigureFork(m_oriCheck);
    if(m_oriCheck)
    {
        m_forkWidget->show();
    }
    else
    {
        m_forkWidget->hide();
    }
    m_checkBox->blockSignals(false);
}

void checkForkCommand::redo()
{
    m_checkBox->blockSignals(true);
    m_checkBox->setChecked(m_newCheck);
    dynamic_cast<WidgetForkSetting*>(m_forkWidget)->setIsConfigureFork(m_newCheck);
    if(m_newCheck)
    {
        m_forkWidget->show();
    }
    else
    {
        m_forkWidget->hide();
    }
    m_checkBox->blockSignals(false);
}
