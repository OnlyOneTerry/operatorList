#include "WidgetCircleRobotModelSetting.h"
#include "ui_WidgetCircleRobotModelSetting.h"
#include"ItemRobotModel.h"
#include"ModelCommon.h"
#include"ModelCustomCommand.h"
#include"UndoStack.h"

WidgetCircleRobotModelSetting::WidgetCircleRobotModelSetting(ItemRobotModel* model, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetCircleRobotModelSetting),
    robotModel_(model),
    m_unStack(NULL)
{
    ui->setupUi(this);
    m_unStack = UndoStack::InitStack();
    this->setStyleSheet("QComboBox{min-height: 22px;}");
    initUI();
    initEditingData();
    connect(ui->moveTypeCombox,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_mvTypeComboxChange(int)));
    connect(ui->chasisTypeComBox,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_chasisShapeComboxChange(int)));
    connect(ui->doubleSpinBox_angle,SIGNAL(valueChanged(double)),this,SLOT(slot_angle_Change(double)));
    connect(ui->doubleSpinBox_b_axis_distance,SIGNAL(valueChanged(double)),this,SLOT(slot_b_axis_Change(double)));
    connect(ui->doubleSpinBox_a_axis_distance,SIGNAL(valueChanged(double)),this,SLOT(slot_a_axis_Change(double)));
    connect(ui->doubleSpinBox_outLine_radius,SIGNAL(valueChanged(double)),this,SLOT(slot_Radius_Change(double)));
    connect(ui->doubleSpinBox_wheel_radius,SIGNAL(valueChanged(double)),this,SLOT(slot_wheel_radius_Change(double)));
    connect(ui->doubleSpinBox_wheel_CenterPoint_distance,SIGNAL(valueChanged(double)),this,SLOT(slot_Wheel_toCenter_distance_Change(double)));


    connect(ui->spinBox_1,SIGNAL(valueChanged(int)),this,SLOT(slot_spinbox_1_currentValueChanged(int)));
    connect(ui->spinBox_2,SIGNAL(valueChanged(int)),this,SLOT(slot_spinbox_2_currentValueChanged(int)));
    connect(ui->spinBox_3,SIGNAL(valueChanged(int)),this,SLOT(slot_spinbox_3_currentValueChanged(int)));
    connect(ui->spinBox_4,SIGNAL(valueChanged(int)),this,SLOT(slot_spinbox_4_currentValueChanged(int)));


}

WidgetCircleRobotModelSetting::~WidgetCircleRobotModelSetting()
{
    delete ui;
}

void WidgetCircleRobotModelSetting::initUI()
{
    ui->moveTypeCombox->blockSignals(true);
    ui->moveTypeCombox->addItem(tr("Differential"),DIFFERENTIAL);
    ui->moveTypeCombox->addItem(tr("Omnidirectional"),ALLDIRECTION);
    ui->moveTypeCombox->blockSignals(false);
    ui->doubleSpinBox_angle->setSingleStep(1);
    ui->doubleSpinBox_a_axis_distance->setSingleStep(0.01);
    ui->doubleSpinBox_b_axis_distance->setSingleStep(0.01);
    ui->doubleSpinBox_outLine_radius->setSingleStep(0.01);
    ui->doubleSpinBox_wheel_radius->setSingleStep(0.01);
    ui->doubleSpinBox_wheel_CenterPoint_distance->setSingleStep(0.01);

    //    //将doubleSpinbox 指针打包压栈
    m_DoubleSpinBoxMap.insert(ID_DOUBLESPINBOX_A_AXIS,ui->doubleSpinBox_a_axis_distance);
    m_DoubleSpinBoxMap.insert(ID_DOUBLESPINBOX_B_AXIS,ui->doubleSpinBox_b_axis_distance);
    m_DoubleSpinBoxMap.insert(ID_DOUBLESPINBOX_OUTLINE_RADIUS,ui->doubleSpinBox_outLine_radius);
    m_DoubleSpinBoxMap.insert(ID_DOUBLESPINBOX_RADIUS,ui->doubleSpinBox_wheel_radius);
    m_DoubleSpinBoxMap.insert(ID_DOUBLESPINBOX_ALLDIRECION_AXIS,ui->doubleSpinBox_wheel_CenterPoint_distance);
    m_DoubleSpinBoxMap.insert(ID_DOUBLESPINBOX_ALLDIRECTION_ANGLE,ui->doubleSpinBox_angle);

    //将要访问的label打包压栈
    m_LabelMap.insert(ID_LABEL_RADIUS,ui->label_wheel_radius);
    m_LabelMap.insert(ID_LABEL_OUTLINE_RADIUS ,ui->label_wheel_radius);
    m_LabelMap.insert(ID_LABEL_A_AXIS,ui->label_a_axis);
    m_LabelMap.insert(ID_LABEL_B_AXIS,ui->label_b_axis);
    m_LabelMap.insert(ID_LABLE_ALLDIRECTION_A_AXIS,ui->label_wheel_toCenterPoint);
    m_LabelMap.insert(ID_LABEL_ALLDIRECTION_ANGLE,ui->label_angle);

    driverInverseList_.append(ui->comboBox_1);
    driverInverseList_.append(ui->comboBox_2);
    driverInverseList_.append(ui->comboBox_3);
    driverInverseList_.append(ui->comboBox_4);


    driverIDspinboxList_.append(ui->spinBox_1);
    driverIDspinboxList_.append(ui->spinBox_2);
    driverIDspinboxList_.append(ui->spinBox_3);
    driverIDspinboxList_.append(ui->spinBox_4);

    for(int i = 0; i<driverIDspinboxList_.size();i++)
    {
        driverIDspinboxList_.at(i)->setMinimum(0);
        driverIDspinboxList_.at(i)->setMaximum(255);
        driverIDspinboxList_.at(i)->setToolTip(tr("range[0,255]"));
    }
   //初始化编号
    ui->spinBox_1->blockSignals(true);
    ui->spinBox_1->setValue(id1);
    ui->spinBox_1->blockSignals(false);
    ui->spinBox_1->setOriValue(id1);
    ui->spinBox_2->blockSignals(true);
    ui->spinBox_2->setValue(id2);
    ui->spinBox_2->setOriValue(id2);
    ui->spinBox_2->blockSignals(false);
    ui->spinBox_3->blockSignals(true);
    ui->spinBox_3->setValue(id3);
    ui->spinBox_3->setOriValue(id3);
    ui->spinBox_3->blockSignals(false);
    ui->spinBox_4->blockSignals(true);
    ui->spinBox_4->setValue(id4);
    ui->spinBox_4->setOriValue(id4);
    ui->spinBox_4->blockSignals(false);
    getInverseAndIdFlag();
}

void WidgetCircleRobotModelSetting::initEditingData()
{
    //数据从robotModel 传入
    m_ori_modelData.shapType_ = CIRCLE;
    ui->doubleSpinBox_wheel_radius->blockSignals(true);
    ui->doubleSpinBox_wheel_radius->setValue(robotModel_->robot_circle_model_data_.wheelRadius_/100.);
    ui->doubleSpinBox_wheel_radius->blockSignals(false);
    ui->doubleSpinBox_wheel_radius->setOriValue(robotModel_->robot_circle_model_data_.wheelRadius_/100.);
    m_ori_modelData.wheelRadius_= robotModel_->robot_circle_model_data_.wheelRadius_/100.;

    ui->doubleSpinBox_outLine_radius->blockSignals(true);
    ui->doubleSpinBox_outLine_radius->setValue(robotModel_->robot_circle_model_data_.radius_/100.);
    ui->doubleSpinBox_outLine_radius->blockSignals(false);
    ui->doubleSpinBox_outLine_radius->setOriValue(robotModel_->robot_circle_model_data_.radius_/100.);
    m_ori_modelData.radius_ = robotModel_->robot_circle_model_data_.radius_/100.;

    MoveStyle type = robotModel_->robot_circle_model_data_.mvStyle_;
    switch (type) {
    case DIFFERENTIAL:
        ui->chasisTypeComBox->blockSignals(true);
        ui->chasisTypeComBox->clear();
        ui->chasisTypeComBox->addItem(tr("Two wheel differential"),DIFFERENTIAL_TWOWHEEL);
        ui->chasisTypeComBox->addItem(tr("Four wheel differential"),DIFFERENTIAL_FOURWHEEL);
        ui->chasisTypeComBox->blockSignals(false);
        break;
    case ALLDIRECTION:
        ui->chasisTypeComBox->blockSignals(true);
        ui->chasisTypeComBox->clear();
        ui->chasisTypeComBox->addItem(tr("Three wheel omni"),ALLDIRECTION_THREEWHEEL);
        ui->chasisTypeComBox->blockSignals(false);
        break;
    default:
        break;
    }

    ChassisType chsType = robotModel_->robot_circle_model_data_.chasisType_;

    switch (chsType) {
    case DIFFERENTIAL_TWOWHEEL:
        ui->label_a_axis->setVisible(true);
        ui->doubleSpinBox_a_axis_distance->setVisible(true);
        ui->label_outline_radius->setVisible(true);
        ui->doubleSpinBox_outLine_radius->setVisible(true);
        ui->label_b_axis->setVisible(false);
        ui->doubleSpinBox_b_axis_distance->setVisible(false);
        ui->label_angle->setVisible(false);
        ui->doubleSpinBox_angle->setVisible(false);
        ui->label_wheel_toCenterPoint->setVisible(false);
        ui->doubleSpinBox_wheel_CenterPoint_distance->setVisible(false);

        //默认两轮差动
        ui->doubleSpinBox_a_axis_distance->blockSignals(true);
        ui->doubleSpinBox_a_axis_distance->setValue(robotModel_->robot_circle_model_data_.chasisModel.DiffTwoWheel.a_axis_distance/100.);
        ui->doubleSpinBox_a_axis_distance->blockSignals(false);
        ui->doubleSpinBox_a_axis_distance->setOriValue(robotModel_->robot_circle_model_data_.chasisModel.DiffTwoWheel.a_axis_distance/100.);
        m_ori_modelData.chasisModel.DiffTwoWheel.a_axis_distance=robotModel_->robot_circle_model_data_.chasisModel.DiffTwoWheel.a_axis_distance/100.;

        //undo redo
        m_ori_dataIndex.a_index = 0;
        m_ori_dataIndex.b_index = 0;
        setDriverInverAndIdDisplay(2);
        break;
    case DIFFERENTIAL_FOURWHEEL:
        ui->label_a_axis->setVisible(true);
        ui->doubleSpinBox_a_axis_distance->setVisible(true);
        ui->label_outline_radius->setVisible(true);
        ui->doubleSpinBox_outLine_radius->setVisible(true);
        ui->label_b_axis->setVisible(true);
        ui->doubleSpinBox_b_axis_distance->setVisible(true);
        ui->label_angle->setVisible(false);
        ui->doubleSpinBox_angle->setVisible(false);
        ui->label_wheel_toCenterPoint->setVisible(false);
        ui->doubleSpinBox_wheel_CenterPoint_distance->setVisible(false);

        ui->doubleSpinBox_a_axis_distance->blockSignals(true);
        ui->doubleSpinBox_a_axis_distance->setValue(robotModel_->robot_circle_model_data_.chasisModel.DiffFourWheel.a_axis_distance/100.);
        ui->doubleSpinBox_a_axis_distance->blockSignals(false);
        ui->doubleSpinBox_a_axis_distance->setOriValue(robotModel_->robot_circle_model_data_.chasisModel.DiffFourWheel.a_axis_distance/100.);
        m_ori_modelData.chasisModel.DiffFourWheel.a_axis_distance=robotModel_->robot_circle_model_data_.chasisModel.DiffFourWheel.a_axis_distance/100.;

        ui->doubleSpinBox_b_axis_distance->blockSignals(true);
        ui->doubleSpinBox_b_axis_distance->setValue(robotModel_->robot_circle_model_data_.chasisModel.DiffFourWheel.a_axis_distance/100.);
        ui->doubleSpinBox_b_axis_distance->blockSignals(false);
        ui->doubleSpinBox_b_axis_distance->setOriValue(robotModel_->robot_circle_model_data_.chasisModel.DiffFourWheel.a_axis_distance/100.);
        m_ori_modelData.chasisModel.DiffFourWheel.b_axis_distance=robotModel_->robot_circle_model_data_.chasisModel.DiffFourWheel.b_axis_distance/100.;

        //undo redo
        m_ori_dataIndex.a_index = 0;
        m_ori_dataIndex.b_index = 1;
        setDriverInverAndIdDisplay(4);
        break;
    case ALLDIRECTION_THREEWHEEL:
        ui->label_a_axis->setVisible(false);
        ui->doubleSpinBox_a_axis_distance->setVisible(false);
        ui->label_outline_radius->setVisible(true);
        ui->doubleSpinBox_outLine_radius->setVisible(true);
        ui->label_b_axis->setVisible(false);
        ui->doubleSpinBox_b_axis_distance->setVisible(false);
        ui->label_angle->setVisible(true);
        ui->doubleSpinBox_angle->setVisible(true);
        ui->label_wheel_toCenterPoint->setVisible(true);
        ui->doubleSpinBox_wheel_CenterPoint_distance->setVisible(true);

        ui->doubleSpinBox_wheel_CenterPoint_distance->blockSignals(true);
        ui->doubleSpinBox_wheel_CenterPoint_distance->setValue(robotModel_->robot_circle_model_data_.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance/100.);
        ui->doubleSpinBox_wheel_CenterPoint_distance->blockSignals(false);
        ui->doubleSpinBox_wheel_CenterPoint_distance->setOriValue(robotModel_->robot_circle_model_data_.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance/100.);
        m_ori_modelData.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance =robotModel_->robot_circle_model_data_.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance/100.;

        ui->doubleSpinBox_angle->blockSignals(true);
        ui->doubleSpinBox_angle->setValue(robotModel_->robot_circle_model_data_.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle);
        ui->doubleSpinBox_angle->blockSignals(false);
        ui->doubleSpinBox_angle->setOriValue(robotModel_->robot_circle_model_data_.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle);
        m_ori_modelData.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle =robotModel_->robot_circle_model_data_.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle;
        //undo redo
        m_ori_dataIndex.a_index = 1;//mvTypeCombox current index
        m_ori_dataIndex.b_index = 0;//chasisTypeCombox current index
        setDriverInverAndIdDisplay(3);
        break;
    default:
        break;
    }
    robotModel_->update_model();
}

customDoubleSpinBox *WidgetCircleRobotModelSetting::get_outLine_radiusSpinbox()
{
    return ui->doubleSpinBox_outLine_radius;
}

void WidgetCircleRobotModelSetting::setUndoStack(QUndoStack *stack)
{
    m_unStack = stack;
}

customComboBox *WidgetCircleRobotModelSetting::getMovCombx()
{
    return ui->moveTypeCombox;
}

customComboBox *WidgetCircleRobotModelSetting::getChassisCombx()
{
    return ui->chasisTypeComBox;
}


void WidgetCircleRobotModelSetting::setDriverInverAndIdDisplay(int num)
{
    switch (num) {
    case 1:
    {
        ui->widget_1->show();
        ui->widget_2->hide();
        ui->widget_3->hide();
        ui->widget_4->hide();
    }
        break;
    case 2:
    {
        ui->widget_1->show();
        ui->widget_2->show();
        ui->widget_3->hide();
        ui->widget_4->hide();
    }
        break;
    case 3:
    {
        ui->widget_1->show();
        ui->widget_2->show();
        ui->widget_3->show();
        ui->widget_4->hide();
    }
        break;
    case 4:
    {
        ui->widget_1->show();
        ui->widget_2->show();
        ui->widget_3->show();
        ui->widget_4->show();
    }
        break;
    default:
        break;
    }

    getInverseAndIdFlag();
}


void WidgetCircleRobotModelSetting::getInverseAndIdFlag()
{
    switch (robotModel_->robot_circle_model_data_.chasisType_) {
    case DIFFERENTIAL_TWOWHEEL:
        m_Cir_inverseList.clear();
        m_Cir_inverseList.append(inverse1);
        m_Cir_inverseList.append(inverse2);
        m_Cir_IdList.clear();
        m_Cir_IdList.append(id1);
        m_Cir_IdList.append(id2);
        break;
    case DIFFERENTIAL_FOURWHEEL:
        m_Cir_inverseList.clear();
        m_Cir_inverseList.append(inverse1);
        m_Cir_inverseList.append(inverse2);
        m_Cir_inverseList.append(inverse3);
        m_Cir_inverseList.append(inverse4);
        m_Cir_IdList.clear();
        m_Cir_IdList.append(id1);
        m_Cir_IdList.append(id2);
        m_Cir_IdList.append(id3);
        m_Cir_IdList.append(id4);

        break;
    case ALLDIRECTION_THREEWHEEL:
        m_Cir_inverseList.clear();
        m_Cir_inverseList.append(inverse1);
        m_Cir_inverseList.append(inverse2);
        m_Cir_inverseList.append(inverse3);
        m_Cir_IdList.clear();
        m_Cir_IdList.append(id1);
        m_Cir_IdList.append(id2);
        m_Cir_IdList.append(id3);

        break;
    default:
        break;
    }
    robotModel_->robot_circle_model_data_.driverID.clear();
    robotModel_->robot_circle_model_data_.driverID = m_Cir_IdList;

    robotModel_->robot_circle_model_data_.inverse_.clear();
    robotModel_->robot_circle_model_data_.inverse_ = m_Cir_inverseList;
}

void WidgetCircleRobotModelSetting::setParentWid(WidgetSetting *parent)
{
    parentWid_ = parent;
}

WidgetSetting *WidgetCircleRobotModelSetting::getParentWid()
{
    return parentWid_;
}

void WidgetCircleRobotModelSetting::slot_chasisShapeComboxChange(int idex)
{

    ui->doubleSpinBox_wheel_radius->setOriValue(0.10);//轮半径
    ui->doubleSpinBox_outLine_radius->setOriValue(0.28);//轮廓半径
    //model切换记录属性参数
    m_new_modelData.wheelRadius_ = 0.10;
    m_new_modelData.radius_ = 0.28;
    m_new_modelData.shapType_ = CIRCLE;
    ChassisType type = (ChassisType)ui->chasisTypeComBox->currentData(Qt::UserRole).toInt();
    robotModel_->robot_circle_model_data_.chasisType_ = type;
    switch (type) {
    case DIFFERENTIAL_TWOWHEEL:
        m_new_modelData.chasisModel.DiffTwoWheel.a_axis_distance = 0.26;
        ui->doubleSpinBox_a_axis_distance->setOriValue(0.26);//切换底盘选型时，此时记录当前属性的初始值，为此属性做undo redo
        ui->doubleSpinBox_a_axis_distance->setValue(0.26);
        break;
    case DIFFERENTIAL_FOURWHEEL:
        m_new_modelData.chasisModel.DiffFourWheel.a_axis_distance = 0.26;
        m_new_modelData.chasisModel.DiffFourWheel.b_axis_distance = 0.20;
        ui->doubleSpinBox_a_axis_distance->setOriValue(0.26);
        ui->doubleSpinBox_b_axis_distance->setOriValue(0.20);
        ui->doubleSpinBox_a_axis_distance->setValue(0.26);
        ui->doubleSpinBox_b_axis_distance->setValue(0.20);
        break;
    case ALLDIRECTION_THREEWHEEL:
        m_new_modelData.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle = 60;
        m_new_modelData.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance = 0.15;
        ui->doubleSpinBox_wheel_CenterPoint_distance->setOriValue(0.15);
        ui->doubleSpinBox_angle->setOriValue(60);
        ui->doubleSpinBox_wheel_CenterPoint_distance->setValue(0.15);
        ui->doubleSpinBox_angle->setValue(60);
        break;
    default:
        break;
    }

    if(m_unStack==NULL)
    {
        return;
    }
    m_new_dataIndex.a_index = ui->moveTypeCombox->currentIndex();
    m_new_dataIndex.b_index = ui->chasisTypeComBox->currentIndex();
    //将要变化的模型数据，数据参数对应的属性控件等压栈，实现数据变化及窗口切换时能够回退
    modelboxCommand* chassis_command = new modelboxCommand(robotModel_,
                                                           ui->moveTypeCombox,ui->chasisTypeComBox,
                                                           NULL,
                                                           m_ori_dataIndex,m_new_dataIndex,
                                                           m_LabelMap,
                                                           m_DoubleSpinBoxMap,
                                                           m_SpinBoxMap,
                                                           m_ori_modelData,m_new_modelData,
                                                           parentWid_,
                                                           NULL);
    m_unStack->push(chassis_command);
    m_ori_dataIndex.a_index = ui->moveTypeCombox->currentIndex();
    m_ori_dataIndex.b_index = ui->chasisTypeComBox->currentIndex();

    m_ori_modelData.wheelRadius_ = m_new_modelData.wheelRadius_;
    m_ori_modelData.radius_ = m_new_modelData.radius_;
    switch (type)
    {
    case DIFFERENTIAL_TWOWHEEL://差动两轮
        //model切换记录属性参数
        m_ori_modelData.chasisModel.DiffTwoWheel.a_axis_distance = m_new_modelData.chasisModel.DiffTwoWheel.a_axis_distance;
        break;
    case DIFFERENTIAL_FOURWHEEL://差动四轮
        //model切换记录属性参数
        m_ori_modelData.chasisModel.DiffFourWheel.a_axis_distance = m_new_modelData.chasisModel.DiffFourWheel.a_axis_distance;
        m_ori_modelData.chasisModel.DiffFourWheel.b_axis_distance = m_new_modelData.chasisModel.DiffFourWheel.b_axis_distance;
        break;
    case ALLDIRECTION_THREEWHEEL://全向三轮
        //model切换记录属性参数
        m_ori_modelData.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle = m_new_modelData.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle;
        m_ori_modelData.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance =  m_new_modelData.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance;
        break;
    }
    robotModel_->update_model();
}

void WidgetCircleRobotModelSetting::slot_Radius_Change(double reg)
{
    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_outLine_radius->getIsClickedTrigger())
    {
        ui->doubleSpinBox_outLine_radius->setNewValue(reg);
        doubleSpinBoxCommand* dbspinx_cmd = new doubleSpinBoxCommand(ui->doubleSpinBox_outLine_radius,
                                                                     robotModel_,
                                                                     ui->doubleSpinBox_outLine_radius->getOriValue(),
                                                                     ui->doubleSpinBox_outLine_radius->getNewValue(),
                                                                     parentWid_,NULL);
        m_unStack->push(dbspinx_cmd);
        ui->doubleSpinBox_outLine_radius->setOriValue(reg);
    }
    ui->doubleSpinBox_outLine_radius->setIsClickedTrigger(true);
    robotModel_->robot_circle_model_data_.radius_ = reg*100;
    robotModel_->update_model();
    //保存底盘切换前的数据（用于切换底盘时undo）
    m_ori_modelData.radius_ = reg;
}

void WidgetCircleRobotModelSetting::slot_a_axis_Change(double reg)
{
    if(m_unStack == NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_a_axis_distance->getIsClickedTrigger())
    {
        ui->doubleSpinBox_a_axis_distance->setNewValue(reg);
        doubleSpinBoxCommand* dbspinbx_cmd = new doubleSpinBoxCommand(ui->doubleSpinBox_a_axis_distance,
                                                                      robotModel_,
                                                                      ui->doubleSpinBox_a_axis_distance->getOriValue(),
                                                                      ui->doubleSpinBox_a_axis_distance->getNewValue(),
                                                                      parentWid_,NULL);
        m_unStack->push(dbspinbx_cmd);
        ui->doubleSpinBox_a_axis_distance->setOriValue(reg);
    }
    ui->doubleSpinBox_a_axis_distance->setIsClickedTrigger(true);

    ChassisType type = (ChassisType)ui->chasisTypeComBox->currentData(Qt::UserRole).toInt();
    switch (type) {
    case DIFFERENTIAL_TWOWHEEL:
        robotModel_->robot_circle_model_data_.chasisModel.DiffTwoWheel.a_axis_distance = reg*100;
        //保存底盘切换前的数据（用于切换底盘时undo）
        m_ori_modelData.chasisModel.DiffTwoWheel.a_axis_distance = reg;
        break;
    case DIFFERENTIAL_FOURWHEEL:
        robotModel_->robot_circle_model_data_.chasisModel.DiffFourWheel.a_axis_distance = reg*100;
        //保存底盘切换前的数据（用于切换底盘时undo）
        m_ori_modelData.chasisModel.DiffFourWheel.a_axis_distance = reg;
        break;
    case ALLDIRECTION_THREEWHEEL:
        break;
    default:
        break;
    }
    robotModel_->update_model();
}

void WidgetCircleRobotModelSetting::slot_b_axis_Change(double reg)
{

    if(m_unStack == NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_b_axis_distance->getIsClickedTrigger())
    {
        ui->doubleSpinBox_b_axis_distance->setNewValue(reg);
        doubleSpinBoxCommand* dbspinbx_cmd = new doubleSpinBoxCommand(ui->doubleSpinBox_b_axis_distance,
                                                                      robotModel_,
                                                                      ui->doubleSpinBox_b_axis_distance->getOriValue(),
                                                                      ui->doubleSpinBox_b_axis_distance->getNewValue(),
                                                                      parentWid_,NULL);
        m_unStack->push(dbspinbx_cmd);
        ui->doubleSpinBox_b_axis_distance->setOriValue(reg);
    }
    ui->doubleSpinBox_b_axis_distance->setIsClickedTrigger(true);

    ChassisType type = (ChassisType)ui->chasisTypeComBox->currentData(Qt::UserRole).toInt();
    switch (type) {
    case DIFFERENTIAL_TWOWHEEL:
        break;
    case DIFFERENTIAL_FOURWHEEL:
        robotModel_->robot_circle_model_data_.chasisModel.DiffFourWheel.b_axis_distance = reg*100;
        //保存底盘切换前的数据（用于切换底盘时undo）
        m_ori_modelData.chasisModel.DiffFourWheel.b_axis_distance = reg;
        break;
    default:
        break;
    }
    robotModel_->update_model();
}

void WidgetCircleRobotModelSetting::slot_angle_Change(double reg)
{
    if(m_unStack == NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_angle->getIsClickedTrigger())
    {
        ui->doubleSpinBox_angle->setNewValue(reg);
        doubleSpinBoxCommand* dbspinbx_cmd = new doubleSpinBoxCommand(ui->doubleSpinBox_angle,
                                                                      robotModel_,
                                                                      ui->doubleSpinBox_angle->getOriValue(),
                                                                      ui->doubleSpinBox_angle->getNewValue(),
                                                                      parentWid_,NULL);
        m_unStack->push(dbspinbx_cmd);
        ui->doubleSpinBox_angle->setOriValue(reg);
    }
    ui->doubleSpinBox_angle->setIsClickedTrigger(true);

    robotModel_->robot_circle_model_data_.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle = reg;
    robotModel_->update_model();

    //保存底盘切换前的数据（用于切换底盘时undo）
    m_ori_modelData.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle = reg;
}

void WidgetCircleRobotModelSetting::slot_Wheel_toCenter_distance_Change(double reg)
{
    if(m_unStack == NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_wheel_CenterPoint_distance->getIsClickedTrigger())
    {
        ui->doubleSpinBox_wheel_CenterPoint_distance->setNewValue(reg);
        doubleSpinBoxCommand* dbspinbx_cmd = new doubleSpinBoxCommand(ui->doubleSpinBox_wheel_CenterPoint_distance,
                                                                      robotModel_,
                                                                      ui->doubleSpinBox_wheel_CenterPoint_distance->getOriValue(),
                                                                      ui->doubleSpinBox_wheel_CenterPoint_distance->getNewValue(),
                                                                      parentWid_,NULL);
        m_unStack->push(dbspinbx_cmd);
        ui->doubleSpinBox_wheel_CenterPoint_distance->setOriValue(reg);
    }
    ui->doubleSpinBox_wheel_CenterPoint_distance->setIsClickedTrigger(true);

    robotModel_->robot_circle_model_data_.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance = reg*100;
    robotModel_->update_model();

    //保存底盘切换前的数据（用于切换底盘时undo）
    m_ori_modelData.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance = reg;
}

void WidgetCircleRobotModelSetting::slot_wheel_radius_Change(double reg)
{

    if(m_unStack == NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_wheel_radius->getIsClickedTrigger())
    {
        ui->doubleSpinBox_wheel_radius->setNewValue(reg);
        doubleSpinBoxCommand* dbspinbx_cmd = new doubleSpinBoxCommand(ui->doubleSpinBox_wheel_radius,
                                                                      robotModel_,
                                                                      ui->doubleSpinBox_wheel_radius->getOriValue(),
                                                                      ui->doubleSpinBox_wheel_radius->getNewValue(),
                                                                      parentWid_,NULL);
        m_unStack->push(dbspinbx_cmd);
        ui->doubleSpinBox_wheel_radius->setOriValue(reg);
    }
    ui->doubleSpinBox_wheel_radius->setIsClickedTrigger(true);

    robotModel_->robot_circle_model_data_.wheelRadius_ = reg*100;
    robotModel_->update_model();

    //保存底盘切换前的数据（用于切换底盘时undo）
    m_ori_modelData.wheelRadius_ = reg;
}

void WidgetCircleRobotModelSetting::slot_mvTypeComboxChange(int idex)
{
    ui->moveTypeCombox->setNew_index(idex);
    MoveStyle type = (MoveStyle)ui->moveTypeCombox->itemData(idex,Qt::UserRole).toInt();
    robotModel_->robot_circle_model_data_.mvStyle_ = type;
    switch (type) {
    case DIFFERENTIAL:
        ui->chasisTypeComBox->blockSignals(true);
        ui->chasisTypeComBox->clear();
        ui->chasisTypeComBox->addItem(tr("Two wheel differential"),DIFFERENTIAL_TWOWHEEL);
        ui->chasisTypeComBox->addItem(tr("Four wheel differential"),DIFFERENTIAL_FOURWHEEL);
        ui->chasisTypeComBox->blockSignals(false);

        break;
    case ALLDIRECTION:
        ui->chasisTypeComBox->blockSignals(true);
        ui->chasisTypeComBox->clear();
        ui->chasisTypeComBox->addItem(tr("Three wheel omni"),ALLDIRECTION_THREEWHEEL);
        ui->chasisTypeComBox->blockSignals(false);
        break;
    default:
        break;
    }

    slot_chasisShapeComboxChange(0);

}

void WidgetCircleRobotModelSetting::on_comboBox_1_currentIndexChanged(int index)
{
    if(0 == index)
    {
        inverse1 = false;
    }
    else if(1 == index )
    {
        inverse1 = true;
    }
    getInverseAndIdFlag();

    if(m_unStack==NULL) return;
    if(ui->comboBox_1->getIsClickedTrigger())
    {
        ui->comboBox_1->setNew_index(index);
        customboxCommand* tempCmd = new customboxCommand(ui->comboBox_1,ui->comboBox_1->getOri_index(),
                                                         ui->comboBox_1->getNew_index(),parentWid_,NULL);
        m_unStack->push(tempCmd);
        ui->comboBox_1->setOri_index(index);
    }
    ui->comboBox_1->setIsClickedTrigger(true);
}

void WidgetCircleRobotModelSetting::on_comboBox_2_currentIndexChanged(int index)
{
    if(0 == index)
    {
        inverse2 = false;
    }
    else if(1 == index )
    {
        inverse2 = true;
    }
    getInverseAndIdFlag();
    if(ui->comboBox_2->getIsClickedTrigger())
    {
        ui->comboBox_2->setNew_index(index);
        customboxCommand* tempCmd = new customboxCommand(ui->comboBox_2,ui->comboBox_2->getOri_index(),
                                                         ui->comboBox_2->getNew_index(),parentWid_,NULL);
        m_unStack->push(tempCmd);
        ui->comboBox_2->setOri_index(index);
    }
    ui->comboBox_2->setIsClickedTrigger(true);
}

void WidgetCircleRobotModelSetting::on_comboBox_3_currentIndexChanged(int index)
{
    if(0 == index)
    {
        inverse3 = false;
    }
    else if(1 == index )
    {
        inverse3 = true;
    }
    getInverseAndIdFlag();
    if(ui->comboBox_3->getIsClickedTrigger())
    {
        ui->comboBox_3->setNew_index(index);
        customboxCommand* tempCmd = new customboxCommand(ui->comboBox_3,ui->comboBox_3->getOri_index(),
                                                         ui->comboBox_3->getNew_index(),parentWid_,NULL);
        m_unStack->push(tempCmd);
        ui->comboBox_3->setOri_index(index);
    }
    ui->comboBox_3->setIsClickedTrigger(true);
}

void WidgetCircleRobotModelSetting::on_comboBox_4_currentIndexChanged(int index)
{
    if(0 == index)
    {
        inverse4 = false;
    }
    else if(1 == index )
    {
        inverse4 = true;
    }
    getInverseAndIdFlag();
    if(ui->comboBox_4->getIsClickedTrigger())
    {
        ui->comboBox_4->setNew_index(index);
        customboxCommand* tempCmd = new customboxCommand(ui->comboBox_4,ui->comboBox_4->getOri_index(),
                                                         ui->comboBox_4->getNew_index(),parentWid_,NULL);
        m_unStack->push(tempCmd);
        ui->comboBox_4->setOri_index(index);
    }
    ui->comboBox_4->setIsClickedTrigger(true);
}

void WidgetCircleRobotModelSetting::slot_spinbox_1_currentValueChanged(int val)
{
    if(m_unStack==NULL) return;
    id1 = val;
    if(ui->spinBox_1->getIsClickedTrigger())
    {
        ui->spinBox_1->setNewValue(val);
        SpinBoxCommand* tempCmd =    new SpinBoxCommand(ui->spinBox_1,ui->spinBox_1->getOriValue(),
                                                        ui->spinBox_1->getNewValue(),parentWid_,NULL);
        m_unStack->push(tempCmd);
        ui->spinBox_1->setOriValue(val);
    }
    ui->spinBox_1->setIsClickedTrigger(true);
    getInverseAndIdFlag();
}

void WidgetCircleRobotModelSetting::slot_spinbox_2_currentValueChanged(int val)
{

    if(m_unStack==NULL) return;
    id2 = val;
    if(ui->spinBox_2->getIsClickedTrigger())
    {
        ui->spinBox_2->setNewValue(val);
        SpinBoxCommand* tempCmd =    new SpinBoxCommand(ui->spinBox_2,ui->spinBox_2->getOriValue(),
                                                        ui->spinBox_2->getNewValue(),parentWid_,NULL);
        m_unStack->push(tempCmd);
        ui->spinBox_2->setOriValue(val);
    }
    ui->spinBox_2->setIsClickedTrigger(true);
    getInverseAndIdFlag();
}

void WidgetCircleRobotModelSetting::slot_spinbox_3_currentValueChanged(int val)
{

    if(m_unStack==NULL) return;
    id3 = val;
    if(ui->spinBox_3->getIsClickedTrigger())
    {
        ui->spinBox_3->setNewValue(val);
        SpinBoxCommand* tempCmd =    new SpinBoxCommand(ui->spinBox_3,ui->spinBox_3->getOriValue(),
                                                        ui->spinBox_3->getNewValue(),parentWid_,NULL);
        m_unStack->push(tempCmd);
        ui->spinBox_3->setOriValue(val);
    }
    ui->spinBox_3->setIsClickedTrigger(true);
    getInverseAndIdFlag();
}

void WidgetCircleRobotModelSetting::slot_spinbox_4_currentValueChanged(int val)
{
    if(m_unStack==NULL) return;
    id4 = val;
    if(ui->spinBox_4->getIsClickedTrigger())
    {
        ui->spinBox_4->setNewValue(val);
        SpinBoxCommand* tempCmd =    new SpinBoxCommand(ui->spinBox_4,ui->spinBox_4->getOriValue(),
                                                        ui->spinBox_4->getNewValue(),parentWid_,NULL);
        m_unStack->push(tempCmd);
        ui->spinBox_4->setOriValue(val);
    }
    ui->spinBox_4->setIsClickedTrigger(true);
    getInverseAndIdFlag();
}
