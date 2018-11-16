#include "WidgetRectRobotModelSetting.h"
#include "ui_WidgetRectRobotModelSetting.h"
#include "ItemRobotModel.h"
#include <QJsonDocument>
#include <QJsonArray>
WidgetRectRobotModelSetting::WidgetRectRobotModelSetting(ItemRobotModel *model, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetRectRobotModelSetting),
    robot_model_(model)
{
    ui->setupUi(this);
    m_unStack = UndoStack::InitStack();
    initUI();
    initWidgetData();
    this->setStyleSheet("QComboBox{min-height: 22px;}");
}

WidgetRectRobotModelSetting::~WidgetRectRobotModelSetting()
{
    delete ui;
}

void WidgetRectRobotModelSetting::initWidgetData()
{
    m_ori_modelData.shapType_ = RECTANGLE;
    //设置宽、头、尾、轮半径公共属性控件数值.

    ui->doubleSpinBox_width->blockSignals(true);
    ui->doubleSpinBox_width->setValue (robot_model_->robot_model_data_.width_/100.0);
    ui->doubleSpinBox_width->setDataType(TYPE_METER);
    ui->doubleSpinBox_width->blockSignals(false);
    ui->doubleSpinBox_width->setOriValue(robot_model_->robot_model_data_.width_/100.);
    m_ori_modelData.width_ = robot_model_->robot_model_data_.width_/100.;

    ui->doubleSpinBox_head->blockSignals(true);
    ui->doubleSpinBox_head->setValue (robot_model_->robot_model_data_.head_/100.0);
    ui->doubleSpinBox_head->setDataType(TYPE_METER);
    ui->doubleSpinBox_head->blockSignals(false);
    ui->doubleSpinBox_head->setOriValue(robot_model_->robot_model_data_.head_/100.0);
    m_ori_modelData.head_ = robot_model_->robot_model_data_.head_/100.0;


    ui->doubleSpinBox_tail->blockSignals(true);
    ui->doubleSpinBox_tail->setValue (robot_model_->robot_model_data_.tail_/100.0);
    ui->doubleSpinBox_tail->setDataType(TYPE_METER);
    ui->doubleSpinBox_tail->blockSignals(false);
    ui->doubleSpinBox_tail->setOriValue(robot_model_->robot_model_data_.tail_/100.0);
    m_ori_modelData.tail_ = robot_model_->robot_model_data_.tail_/100.;

    ui->doubleSpinBox_wheelRadius->blockSignals(true);
    ui->doubleSpinBox_wheelRadius->setDataType(TYPE_METER);
    ui->doubleSpinBox_wheelRadius->setValue(robot_model_->robot_model_data_.wheelRadius_/100.0);
    ui->doubleSpinBox_wheelRadius->blockSignals(false);
    ui->doubleSpinBox_wheelRadius->setOriValue(robot_model_->robot_model_data_.wheelRadius_/100.0);
    m_ori_modelData.wheelRadius_ = robot_model_->robot_model_data_.wheelRadius_/100.;
    MoveStyle mvtype = robot_model_->robot_model_data_.mvStyle_;

    switch (mvtype) {
    case DIFFERENTIAL://差动.
    {
        int index1 = ui->comboBox_moveType->findData(DIFFERENTIAL,Qt::UserRole);
        ui->comboBox_moveType->blockSignals(true);
        ui->comboBox_moveType->setCurrentIndex(index1);
        ui->comboBox_moveType->blockSignals(false);

        ui->comboBox_chassisType->blockSignals(true);
        ui->comboBox_chassisType->clear();
        ui->comboBox_chassisType->addItem(tr("Two wheel differential"),DIFFERENTIAL_TWOWHEEL);
        ui->comboBox_chassisType->addItem(tr("Four wheel differential"),DIFFERENTIAL_FOURWHEEL);
        ui->comboBox_chassisType->blockSignals(false);

        if(robot_model_->robot_model_data_.chasisType_==DIFFERENTIAL_TWOWHEEL)
        {
            ui->comboBox_chassisType->blockSignals(true);
            ui->comboBox_chassisType->setCurrentIndex(0);
            ui->comboBox_chassisType->blockSignals(false);
            set_Diff_two_wheel_argument(robot_model_->robot_model_data_.chasisModel.DiffTwoWheel.a_axis_distance);

            ui->doubleSpinBox_Aaxis->setOriValue(robot_model_->robot_model_data_.chasisModel.DiffTwoWheel.a_axis_distance/100.);
            //undo redo.
            m_ori_dataIndex.a_index = 0;
            m_ori_dataIndex.b_index = 0;
            //切换model，记录切换前数据.
            m_ori_modelData.chasisModel.DiffTwoWheel.a_axis_distance = robot_model_->robot_model_data_.chasisModel.DiffTwoWheel.a_axis_distance/100;
            setDriverInverAndIdDisplay(2);
        }
        else if(robot_model_->robot_model_data_.chasisType_ == DIFFERENTIAL_FOURWHEEL)
        {
            ui->comboBox_chassisType->blockSignals(true);
            ui->comboBox_chassisType->setCurrentIndex(1);
            ui->comboBox_chassisType->blockSignals(false);
            set_Diff_four_wheel_argument(robot_model_->robot_model_data_.chasisModel.DiffFourWheel.a_axis_distance,
                                         robot_model_->robot_model_data_.chasisModel.DiffFourWheel.b_axis_distance);
            //set orivalue for undo redo.
            ui->doubleSpinBox_Aaxis->setOriValue(robot_model_->robot_model_data_.chasisModel.DiffFourWheel.a_axis_distance/100.);
            ui->doubleSpinBox_Baxis->setOriValue(robot_model_->robot_model_data_.chasisModel.DiffFourWheel.b_axis_distance/100.);
            //undo redo.
            m_ori_dataIndex.a_index = 0;
            m_ori_dataIndex.b_index = 1;
            //切换model，记录切换前数据.
            m_ori_modelData.chasisModel.DiffFourWheel.a_axis_distance = robot_model_->robot_model_data_.chasisModel.DiffFourWheel.a_axis_distance/100;
            m_ori_modelData.chasisModel.DiffFourWheel.b_axis_distance = robot_model_->robot_model_data_.chasisModel.DiffFourWheel.b_axis_distance/100;
            setDriverInverAndIdDisplay(4);
        }
    }
        break;
    case ALLDIRECTION://全向.
    {
        int index = ui->comboBox_moveType->findData(ALLDIRECTION,Qt::UserRole);
        ui->comboBox_moveType->blockSignals(true);
        ui->comboBox_moveType->setCurrentIndex(index);
        ui->comboBox_moveType->blockSignals(false);

        ui->comboBox_chassisType->blockSignals(true);
        ui->comboBox_chassisType->clear();
        ui->comboBox_chassisType->addItem(tr("Three wheel omni"),ALLDIRECTION_THREEWHEEL);
        ui->comboBox_chassisType->blockSignals(false);

        if(robot_model_->robot_model_data_.chasisType_==ALLDIRECTION_THREEWHEEL)
        {
            ui->comboBox_chassisType->blockSignals(true);
            ui->comboBox_chassisType->setCurrentIndex(0);
            ui->comboBox_chassisType->blockSignals(false);
            set_AllDirection_three_wheel_argument(robot_model_->robot_model_data_.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance,
                                                  robot_model_->robot_model_data_.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle);
            //set orivalue for undo redo.
            ui->doubleSpinBox_corePoint_distance->setOriValue(robot_model_->robot_model_data_.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance/100.);
            ui->doubleSpinBox_firstWheel_delta_angle_toPostiveDirction->setOriValue(robot_model_->robot_model_data_.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle);
            m_ori_dataIndex.a_index = 1;
            m_ori_dataIndex.b_index = 0 ;
            //切换model，记录切换前数据.
            m_ori_modelData.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle = robot_model_->robot_model_data_.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle;
            m_ori_modelData.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance = robot_model_->robot_model_data_.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance/100;
            setDriverInverAndIdDisplay(3);
        }

    }
        break;
    case MECANUM://马克纳姆.
    {
        int index = ui->comboBox_moveType->findData(MECANUM,Qt::UserRole);
        ui->comboBox_moveType->blockSignals(true);
        ui->comboBox_moveType->setCurrentIndex(index);
        ui->comboBox_moveType->blockSignals(false);
        ui->comboBox_chassisType->blockSignals(true);
        ui->comboBox_chassisType->clear();
        ui->comboBox_chassisType->addItem(tr("Four wheel of Mecanum"),MECANUM_FOURWHEEL);
        ui->comboBox_chassisType->blockSignals(false);
        if(robot_model_->robot_model_data_.chasisType_==MECANUM_FOURWHEEL)
        {
            ui->comboBox_chassisType->blockSignals(true);
            ui->comboBox_chassisType->setCurrentIndex(0);
            ui->comboBox_chassisType->blockSignals(false);
            set_Mecanum_four_wheel_argument(robot_model_->robot_model_data_.chasisModel.MecanumFourWheel.a_axis_distance,
                                            robot_model_->robot_model_data_.chasisModel.MecanumFourWheel.b_axis_distance);
            //for undo redo.
            ui->doubleSpinBox_Aaxis->setOriValue(robot_model_->robot_model_data_.chasisModel.MecanumFourWheel.a_axis_distance/100.);
            ui->doubleSpinBox_Baxis->setOriValue(robot_model_->robot_model_data_.chasisModel.MecanumFourWheel.b_axis_distance/100.);
            m_ori_dataIndex.a_index = 2 ;
            m_ori_dataIndex.b_index = 0;

            //切换model，记录切换前数据.
            m_ori_modelData.chasisModel.MecanumFourWheel.a_axis_distance = robot_model_->robot_model_data_.chasisModel.MecanumFourWheel.a_axis_distance/100;
            m_ori_modelData.chasisModel.MecanumFourWheel.b_axis_distance = robot_model_->robot_model_data_.chasisModel.MecanumFourWheel.b_axis_distance/100;
            setDriverInverAndIdDisplay(4);
        }

    }
        break;
    case SINGLESTEERWHEEL: //单舵轮.
    {
        int index = ui->comboBox_moveType->findData(SINGLESTEERWHEEL,Qt::UserRole);
        ui->comboBox_moveType->blockSignals(true);
        ui->comboBox_moveType->setCurrentIndex(index);
        ui->comboBox_moveType->blockSignals(false);

        ui->comboBox_chassisType->blockSignals(true);
        ui->comboBox_chassisType->clear();
        ui->comboBox_chassisType->addItem(tr("Single rudder front drive and front encoder"),SINGLESTEERWHEEL_FRONTDRIVER_FRONT_CODERTYPE);
        ui->comboBox_chassisType->addItem(tr("Single rudder front drive and rear encoder"),SINGLESTEERWHEEL_FRONTDRIVER_BACK_CODERTYPE);
        ui->comboBox_chassisType->addItem(tr("Single rudder rear drive and front encoder"),SINGLESTEERWHEEL_BACKDRIVER_FRONT_CODERTYPE);
        ui->comboBox_chassisType->addItem(tr("Single rudder rear drive and rear encoder"),SINGLESTEERWHEEL_BACKDRIVER_BACK_CODERTYPE);
        ui->comboBox_chassisType->blockSignals(false);
        set_SingleWheelDriver_argument(robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.a_axis_distance,
                                       robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.b_axis_distance,
                                       robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.steer_angle_max,
                                       robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.steer_angle_min,
                                       robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.absEncoderRange_max,
                                       robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.absEncoderRange_min,
                                       robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.absEncoderType);

        if(robot_model_->robot_model_data_.chasisType_==SINGLESTEERWHEEL_FRONTDRIVER_FRONT_CODERTYPE)
        {
            ui->comboBox_chassisType->blockSignals(true);
            ui->comboBox_chassisType->setCurrentIndex(0);
            ui->comboBox_chassisType->blockSignals(false);
        }
        else if(robot_model_->robot_model_data_.chasisType_==SINGLESTEERWHEEL_FRONTDRIVER_BACK_CODERTYPE)
        {
            ui->comboBox_chassisType->blockSignals(true);
            ui->comboBox_chassisType->setCurrentIndex(1);
            ui->comboBox_chassisType->blockSignals(false);

        }
        else if(robot_model_->robot_model_data_.chasisType_==SINGLESTEERWHEEL_BACKDRIVER_FRONT_CODERTYPE)
        {
            ui->comboBox_chassisType->blockSignals(true);
            ui->comboBox_chassisType->setCurrentIndex(2);
            ui->comboBox_chassisType->blockSignals(false);
        }
        else if(robot_model_->robot_model_data_.chasisType_==SINGLESTEERWHEEL_BACKDRIVER_BACK_CODERTYPE)
        {
            ui->comboBox_chassisType->blockSignals(true);
            ui->comboBox_chassisType->setCurrentIndex(3);
            ui->comboBox_chassisType->blockSignals(false);

        }
        setDriverInverAndIdDisplay(1);
        //for undo redo.
        ui->doubleSpinBox_Aaxis->setOriValue(robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.a_axis_distance/100.);
        ui->doubleSpinBox_Baxis->setOriValue( robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.b_axis_distance/100.);
        ui->spinBox_absEncoderRange_max->setOriValue(robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.absEncoderRange_max);
        ui->spinBox_absEncoderRange_min->setOriValue(robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.absEncoderRange_min);
        ui->spinBox_steer_angle_max->setOriValue(robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.steer_angle_max);
        ui->spinBox_steer_angle_min->setOriValue(robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.steer_angle_min);
        m_ori_dataIndex.a_index = 3 ;
        m_ori_dataIndex.b_index = ui->comboBox_chassisType->currentIndex();

        ui->comboBox_absEncoderType->setOri_index(robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.absEncoderType);
        m_ori_modelData.chasisModel.SingleWheelDriver.a_axis_distance = robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.a_axis_distance/100;
        m_ori_modelData.chasisModel.SingleWheelDriver.b_axis_distance = robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.b_axis_distance/100;
        m_ori_modelData.chasisModel.SingleWheelDriver.steer_angle_max = robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.steer_angle_max;
        m_ori_modelData.chasisModel.SingleWheelDriver.steer_angle_min = robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.steer_angle_min;
        m_ori_modelData.chasisModel.SingleWheelDriver.absEncoderRange_max = robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.absEncoderRange_max;
        m_ori_modelData.chasisModel.SingleWheelDriver.absEncoderRange_min = robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.absEncoderRange_min;
        m_ori_modelData.chasisModel.SingleWheelDriver.absEncoderType = robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.absEncoderType;
    }
        break;
    default:
        break;
    }

    ui->comboBox_moveType->setOri_index(ui->comboBox_moveType->currentData().toInt());
    ui->comboBox_chassisType->setOri_index(ui->comboBox_chassisType->currentData().toInt());

}

void WidgetRectRobotModelSetting::initUI()
{
    ui->comboBox_moveType->blockSignals(true);
    ui->comboBox_moveType->addItem(tr("Differential"),DIFFERENTIAL);
    ui->comboBox_moveType->addItem(tr("Omnidirectional"),ALLDIRECTION);
    ui->comboBox_moveType->addItem(tr("Mecanum"),MECANUM);
    ui->comboBox_moveType->addItem(tr("Single rudder"),SINGLESTEERWHEEL);
    ui->comboBox_moveType->blockSignals(false);
    //记录变化前后的index用于撤销与前进（undo redo）.
    ui->comboBox_moveType->setOri_index(ui->comboBox_moveType->currentIndex());
    ui->comboBox_moveType->setNew_index(0);
    ui->comboBox_moveType->setIsClickedTrigger(true);//标志位用于undo redo 标记是否是鼠标点击.
    ui->comboBox_chassisType->setIsClickedTrigger(true);
    ui->doubleSpinBox_head->setSingleStep(0.01);//设置步长为0.01.
    ui->doubleSpinBox_head->setDecimals(3);
    ui->doubleSpinBox_head->setDataType(TYPE_METER);
    ui->doubleSpinBox_width->setSingleStep(0.01);
    ui->doubleSpinBox_width->setDecimals(3);
    ui->doubleSpinBox_width->setDataType(TYPE_METER);
    ui->doubleSpinBox_wheelRadius->setSingleStep(0.01);
    ui->doubleSpinBox_wheelRadius->setDecimals(4);
    ui->doubleSpinBox_wheelRadius->setDataType(TYPE_METER);
    ui->doubleSpinBox_tail->setSingleStep(0.01);
    ui->doubleSpinBox_tail->setDecimals(3);
    ui->doubleSpinBox_tail->setDataType(TYPE_METER);
    ui->doubleSpinBox_Aaxis->setDecimals(3);
    ui->doubleSpinBox_Aaxis->setDataType(TYPE_METER);
    ui->doubleSpinBox_Baxis->setDecimals(3);
    ui->doubleSpinBox_Aaxis->setSingleStep(0.01);
    ui->doubleSpinBox_Baxis->setDataType(TYPE_METER);
    ui->doubleSpinBox_Baxis->setSingleStep(0.01);
    ui->spinBox_steer_angle_min->setDataType(TYPE_DEGREE);
    ui->spinBox_steer_angle_min->setToolTip(tr("range[-90,maxinum]"));
    ui->spinBox_steer_angle_max->setToolTip(tr("range[mininum,90]"));
    ui->spinBox_steer_angle_max->setDataType(TYPE_DEGREE);
    ui->spinBox_absEncoderRange_min->setDataType(TYPE_NONE);
    ui->spinBox_absEncoderRange_min->setToolTip(tr("range[0,absEncoderMaxinum]"));
    ui->spinBox_absEncoderRange_max->setDataType(TYPE_NONE);
    ui->spinBox_absEncoderRange_max->setToolTip(tr("range[absEncoderMininum,1800]"));
    ui->doubleSpinBox_corePoint_distance->setDataType(TYPE_METER);
    ui->doubleSpinBox_corePoint_distance->setSingleStep(0.01);
    ui->doubleSpinBox_firstWheel_delta_angle_toPostiveDirction->setSingleStep(1);
    ui->doubleSpinBox_firstWheel_delta_angle_toPostiveDirction->setDataType(TYPE_DEGREE);
    ui->doubleSpinBox_firstWheel_delta_angle_toPostiveDirction->setDecimals(1);
    ui->doubleSpinBox_firstWheel_delta_angle_toPostiveDirction->setMaximum(120);
    ui->doubleSpinBox_firstWheel_delta_angle_toPostiveDirction->setMinimum(0);
    ui->doubleSpinBox_firstWheel_delta_angle_toPostiveDirction->setToolTip(tr("range[0,120]"));
    //读取absEncodertype配置文件.
    ConfigureData item =  readAbsEncoderTypeFile(CONFIGFILE_PATH);
    QList<ItemData>::iterator iter = item.dataList.begin();

    for(;iter != item.dataList.end();iter++)
    {
        ui->comboBox_absEncoderType->blockSignals(true);
        ui->comboBox_absEncoderType->addItem((*iter).describtion,(*iter).index);
        ui->comboBox_absEncoderType->blockSignals(false);
    }

    if(robot_model_->isCreatedByLoading_)
    {
        ui->comboBox_absEncoderType->blockSignals(true);
        int idx = ui->comboBox_absEncoderType->findData(robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.absEncoderType);
        ui->comboBox_absEncoderType->setCurrentIndex(idx);
        ui->comboBox_absEncoderType->blockSignals(false);
    }
    else
    {
        int defaultIdex = item.defaultidx;//当前显示默认值（角色值为配置文件中默认值）.
        int specifyIdex = ui->comboBox_absEncoderType->findData(defaultIdex);//查找角色值等于默认值项.
        ui->comboBox_absEncoderType->blockSignals(true);
        ui->comboBox_absEncoderType->setCurrentIndex(specifyIdex);
        ui->comboBox_absEncoderType->blockSignals(false);
        robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.absEncoderType = specifyIdex;//newmodel设置默认idex.
    }
    ui->comboBox_absEncoderType->setOri_index(ui->comboBox_absEncoderType->currentIndex());

    ui->spinBox_absEncoderRange_max->setMaximum(1800);
    ui->spinBox_absEncoderRange_max->setMinimum(0);
    ui->spinBox_absEncoderRange_min->setMaximum(1800);
    ui->spinBox_absEncoderRange_min->setMinimum(0);

    ui->spinBox_steer_angle_max->setMaximum(90);
    ui->spinBox_steer_angle_max->setMinimum(-90);
    ui->spinBox_steer_angle_min->setMaximum(90);
    ui->spinBox_steer_angle_min->setMinimum(-90);
    ui->comboBox_inverse1->blockSignals(true);
    ui->comboBox_inverse1->addItem(tr("Corotation"));
    ui->comboBox_inverse1->addItem(tr("Reversal"));
    ui->comboBox_inverse1->blockSignals(false);
    ui->comboBox_inverse2->blockSignals(true);
    ui->comboBox_inverse2->addItem(tr("Corotation"));
    ui->comboBox_inverse2->addItem(tr("Reversal"));
    ui->comboBox_inverse2->blockSignals(false);
    ui->comboBox_inverse3->blockSignals(true);
    ui->comboBox_inverse3->addItem(tr("Corotation"));
    ui->comboBox_inverse3->addItem(tr("Reversal"));
    ui->comboBox_inverse3->blockSignals(false);
    ui->comboBox_inverse4->blockSignals(true);
    ui->comboBox_inverse4->addItem(tr("Corotation"));
    ui->comboBox_inverse4->addItem(tr("Reversal"));
    ui->comboBox_inverse4->blockSignals(true);

    driverInverseList_.append(ui->comboBox_inverse1);
    driverInverseList_.append(ui->comboBox_inverse2);
    driverInverseList_.append(ui->comboBox_inverse3);
    driverInverseList_.append(ui->comboBox_inverse4);

    driverIDspinboxList_.append(ui->spinBox_ID1);
    driverIDspinboxList_.append(ui->spinBox_ID2);
    driverIDspinboxList_.append(ui->spinBox_ID3);
    driverIDspinboxList_.append(ui->spinBox_ID4);

    for(int i = 0; i<driverIDspinboxList_.size();i++)
    {
        driverIDspinboxList_.at(i)->setMinimum(0);
        driverIDspinboxList_.at(i)->setMaximum(255);
        driverIDspinboxList_.at(i)->setToolTip(tr("range[0,255]"));
    }
   //初始化编号
    ui->spinBox_ID1->blockSignals(true);
    ui->spinBox_ID1->setValue(id1);
    ui->spinBox_ID1->blockSignals(false);
    ui->spinBox_ID1->setOriValue(id1);
    ui->spinBox_ID2->blockSignals(true);
    ui->spinBox_ID2->setValue(id2);
    ui->spinBox_ID2->blockSignals(false);
    ui->spinBox_ID2->setOriValue(id2);
    ui->spinBox_ID3->blockSignals(true);
    ui->spinBox_ID3->setValue(id3);
    ui->spinBox_ID3->blockSignals(false);
    ui->spinBox_ID3->setOriValue(id3);
    ui->spinBox_ID4->blockSignals(true);
    ui->spinBox_ID4->setValue(id4);
    ui->spinBox_ID4->blockSignals(false);
    ui->spinBox_ID4->setOriValue(id4);
    getInverseAndIdFlag();
    //将doubleSpinbox 指针打包压栈用于undo redo.
    m_DoubleSpinBoxMap.insert(ID_DOUBLESPINBOX_WIDTH,ui->doubleSpinBox_width);
    m_DoubleSpinBoxMap.insert(ID_DOUBLESPINBOX_HEAD,ui->doubleSpinBox_head);
    m_DoubleSpinBoxMap.insert(ID_DOUBLESPINBOX_TAIL,ui->doubleSpinBox_tail);
    m_DoubleSpinBoxMap.insert(ID_DOUBLESPINBOX_RADIUS,ui->doubleSpinBox_wheelRadius);
    m_DoubleSpinBoxMap.insert(ID_DOUBLESPINBOX_A_AXIS, ui->doubleSpinBox_Aaxis);
    m_DoubleSpinBoxMap.insert(ID_DOUBLESPINBOX_B_AXIS, ui->doubleSpinBox_Baxis);
    m_DoubleSpinBoxMap.insert(ID_DOUBLESPINBOX_ALLDIRECION_AXIS,ui->doubleSpinBox_corePoint_distance);
    m_DoubleSpinBoxMap.insert(ID_DOUBLESPINBOX_ALLDIRECTION_ANGLE,ui->doubleSpinBox_firstWheel_delta_angle_toPostiveDirction);

    m_SpinBoxMap.insert(ID_DOUBLESPINBOX_STEER_ANGLE_MAX,ui->spinBox_steer_angle_max);
    m_SpinBoxMap.insert(ID_DOUBLESPINBOX_STEER_ANGLE_MIN,ui->spinBox_steer_angle_min);
    m_SpinBoxMap.insert(ID_DOUBLESPINBOX_ABSENCODER_RANGLE_MAX,ui->spinBox_absEncoderRange_max);
    m_SpinBoxMap.insert(ID_DOUBLESPINBOX_ABSENCODER_RANGLE_MIN,ui->spinBox_absEncoderRange_min);

    //将要访问的label打包压栈用于undo redo.
    m_LabelMap.insert(ID_LABEL_WIDTH,ui->label_width);
    m_LabelMap.insert(ID_LABEL_HEAD,ui->label_head);
    m_LabelMap.insert(ID_LABEL_TAIL,ui->label_tail);
    m_LabelMap.insert(ID_LABEL_RADIUS,ui->label_wheelRadius);
    m_LabelMap.insert(ID_LABEL_A_AXIS,ui->label_Aaxis);
    m_LabelMap.insert(ID_LABEL_B_AXIS,ui->label_Baxis);
    m_LabelMap.insert(ID_LABLE_ALLDIRECTION_A_AXIS,ui->label_wheel_corePoint_distance);
    m_LabelMap.insert(ID_LABEL_ALLDIRECTION_ANGLE,ui->label_firstWheel_delta_angle_toPostiveDirction);
    m_LabelMap.insert(ID_LABEL_STEER_ANGEL_MIN,ui->label_steer_angle_min);
    m_LabelMap.insert(ID_LABEL_STEER_ANGEL_MAX,ui->label_steer_angle_max);
    m_LabelMap.insert(ID_LABEL_ABSENCODER_RANGLE_MIN,ui->label_absEncoderRange_min);
    m_LabelMap.insert(ID_LABEL_ABSENCODER_RANGLE_MAX,ui->label_absEncoderRange_max);
    m_LabelMap.insert(ID_LABEL_ABSENCODER_TYPE,ui->label_absEncoderType);
}

ConfigureData WidgetRectRobotModelSetting::readAbsEncoderTypeFile(const QString &filePath)
{
    QString val;
    QFile file;
    file.setFileName(filePath);
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    val = file.readAll();
    file.close();

    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject obj = d.object();

    QJsonValue chassis_value = obj.value(QString("chassis"));

    QJsonObject chassis_obj = chassis_value.toObject();

    QJsonValue mode_param_value = chassis_obj["mode_param"];

    QJsonObject mode_param_obj = mode_param_value.toObject();

    QJsonValue absEncoder_value = mode_param_obj["absEncoder"];

    QJsonObject absEncoder_obj = absEncoder_value.toObject();

    QJsonValue defaultIdx = absEncoder_obj["defaultIndex"].toInt();

    QJsonArray typeList = absEncoder_obj["typeList"].toArray();

    ConfigureData configure;

    configure.defaultidx = defaultIdx.toInt();

    for(int i = 0; i < typeList.size();i++)
    {
        QJsonObject tempObj = typeList.at(i).toObject();
        QJsonValue tempIndex  =tempObj["index"].toInt() ;
        QJsonValue tempDesc  = tempObj["desc"].toString();

        ItemData tempData;
        tempData.index = tempIndex.toInt();
        tempData.describtion = tempDesc.toString();
        configure.dataList.push_back(tempData);
    }

    return configure;
}

void WidgetRectRobotModelSetting::set_Diff_two_wheel_argument(double adistance)
{
    //（robot_model_数据结构是联合体应先对相应参数进行赋值，否则读取的应是前一种类型的数据）.
    robot_model_->robot_model_data_.chasisModel.DiffFourWheel.a_axis_distance=adistance;

    //设置底盘选型.
    robot_model_->setChasiseType(DIFFERENTIAL_TWOWHEEL);
    robot_model_->robot_model_data_.chasisType_ = DIFFERENTIAL_TWOWHEEL;
    //设置编辑控件数据(在控件发送valuechanged消息时设置相应的属性数据).

    ui->doubleSpinBox_Aaxis->blockSignals(true);
    ui->doubleSpinBox_Aaxis->setValue(adistance/100);
    ui->doubleSpinBox_Aaxis->blockSignals(false);
    //a轴的原始值用于undo redo.
    ui->doubleSpinBox_Aaxis->setOriValue(adistance/100);

    ui->label_Aaxis->setVisible(true);//左右轮距.
    ui->label_Baxis->setVisible(false);//轴距.
    ui->label_wheel_corePoint_distance->setVisible(false);//轮到中心距离.
    ui->label_firstWheel_delta_angle_toPostiveDirction->setVisible(false);//0号车轮与正方向夹角.
    ui->label_steer_angle_min->setVisible(false);//编码器转角.
    ui->label_steer_angle_max->setVisible(false);
    ui->label_absEncoderRange_max->setVisible(false);
    ui->label_absEncoderRange_min->setVisible(false);
    ui->label_absEncoderType->setVisible(false);
    ui->doubleSpinBox_Aaxis->setVisible(true);
    ui->doubleSpinBox_Baxis->setVisible(false);
    ui->doubleSpinBox_corePoint_distance->setVisible(false);
    ui->doubleSpinBox_firstWheel_delta_angle_toPostiveDirction->setVisible(false);
    ui->spinBox_steer_angle_max->setVisible(false);
    ui->spinBox_steer_angle_min->setVisible(false);
    ui->spinBox_absEncoderRange_max->setVisible(false);
    ui->spinBox_absEncoderRange_min->setVisible(false);
    ui->comboBox_absEncoderType->setVisible(false);
}

void WidgetRectRobotModelSetting::set_Diff_four_wheel_argument(double adistance, double bdistance)
{
    robot_model_->setChasiseType(DIFFERENTIAL_FOURWHEEL);
    robot_model_->robot_model_data_.chasisType_ = DIFFERENTIAL_FOURWHEEL;
    ui->label_Aaxis->setVisible(true);//左右轮距.
    ui->label_Baxis->setVisible(true);//轴距.
    ui->label_wheel_corePoint_distance->setVisible(false);//轮到中心距离.
    ui->label_firstWheel_delta_angle_toPostiveDirction->setVisible(false);//0号车轮与正方向夹角.
    ui->label_steer_angle_min->setVisible(false);//编码器转角.
    ui->label_steer_angle_max->setVisible(false);
    ui->label_absEncoderType->setVisible(false);
    ui->doubleSpinBox_Aaxis->setVisible(true);
    ui->doubleSpinBox_Baxis->setVisible(true);
    ui->doubleSpinBox_corePoint_distance->setVisible(false);
    ui->doubleSpinBox_firstWheel_delta_angle_toPostiveDirction->setVisible(false);
    ui->label_absEncoderRange_min->setVisible(false);
    ui->label_absEncoderRange_max->setVisible(false);
    ui->spinBox_steer_angle_min->setVisible(false);
    ui->spinBox_steer_angle_max->setVisible(false);
    ui->spinBox_absEncoderRange_max->setVisible(false);
    ui->spinBox_absEncoderRange_min->setVisible(false);
    ui->comboBox_absEncoderType->setVisible(false);

    //（robot_model_数据结构是联合体应先对相应参数进行赋值，否则读取的应是前一种类型的数据）.
    robot_model_->robot_model_data_.chasisModel.DiffFourWheel.a_axis_distance=adistance;
    robot_model_->robot_model_data_.chasisModel.DiffFourWheel.b_axis_distance=bdistance;

    //设置编辑控件数据(在控件发送valuechanged消息时设置相应的属性数据).
    //A轴距.
    ui->doubleSpinBox_Aaxis->blockSignals(true);
    ui->doubleSpinBox_Aaxis->setValue(adistance/100);
    ui->doubleSpinBox_Aaxis->blockSignals(false);
    //B轴距.
    ui->doubleSpinBox_Baxis->blockSignals(true);
    ui->doubleSpinBox_Baxis->setValue(bdistance/100);
    ui->doubleSpinBox_Baxis->blockSignals(false);

    //a轴的原始值用于undo redo.
    ui->doubleSpinBox_Aaxis->setOriValue(adistance/100);
    //b轴的原始值用于undo redo.
    ui->doubleSpinBox_Baxis->setOriValue(bdistance/100);

}

void WidgetRectRobotModelSetting::set_Mecanum_four_wheel_argument(double adistance, double bdistance)
{
    //设置底盘选型.
    robot_model_->setChasiseType(MECANUM_FOURWHEEL);
    robot_model_->robot_model_data_.chasisType_ = MECANUM_FOURWHEEL;

    ui->label_Aaxis->setVisible(true);//左右轮距.
    ui->label_Baxis->setVisible(true);//轴距.
    ui->label_wheel_corePoint_distance->setVisible(false);//轮到中心距离.
    ui->label_firstWheel_delta_angle_toPostiveDirction->setVisible(false);//0号车轮与正方向夹角.
    ui->label_steer_angle_min->setVisible(false);//编码器转角.
    ui->label_steer_angle_max->setVisible(false);
    ui->label_absEncoderRange_min->setVisible(false);
    ui->label_absEncoderRange_max->setVisible(false);
    ui->label_absEncoderType->setVisible(false);
    ui->doubleSpinBox_Aaxis->setVisible(true);
    ui->doubleSpinBox_Baxis->setVisible(true);
    ui->doubleSpinBox_corePoint_distance->setVisible(false);
    ui->doubleSpinBox_firstWheel_delta_angle_toPostiveDirction->setVisible(false);
    ui->spinBox_steer_angle_min->setVisible(false);
    ui->spinBox_steer_angle_max->setVisible(false);
    ui->spinBox_absEncoderRange_max->setVisible(false);
    ui->spinBox_absEncoderRange_min->setVisible(false);
    ui->comboBox_absEncoderType->setVisible(false);
    //（robot_model_数据结构是联合体应先对相应参数进行赋值，否则读取的应是前一种类型的数据）.
    robot_model_->robot_model_data_.chasisModel.MecanumFourWheel.a_axis_distance=adistance;
    robot_model_->robot_model_data_.chasisModel.MecanumFourWheel.b_axis_distance= bdistance;
    //设置编辑控件数据(在控件发送valuechanged消息时设置相应的属性数据).
    ui->doubleSpinBox_Aaxis->blockSignals(true);
    ui->doubleSpinBox_Aaxis->setValue(adistance/100);
    ui->doubleSpinBox_Aaxis->blockSignals(false);
    ui->doubleSpinBox_Baxis->blockSignals(true);
    ui->doubleSpinBox_Baxis->setValue(bdistance/100);
    ui->doubleSpinBox_Baxis->blockSignals(false);

    //a轴的原始值用于undo redo.
    ui->doubleSpinBox_Aaxis->setOriValue(adistance/100);
    //b轴的原始值用于undo redo.
    ui->doubleSpinBox_Baxis->setOriValue(bdistance/100);

}

void WidgetRectRobotModelSetting::set_AllDirection_three_wheel_argument(double adistance, double angle)
{
    //设置底盘选型
    robot_model_->setChasiseType(ALLDIRECTION_THREEWHEEL);
    robot_model_->robot_model_data_.chasisType_ = ALLDIRECTION_THREEWHEEL;
    ui->label_Aaxis->setVisible(false);//左右轮距.
    ui->label_Baxis->setVisible(false);//轴距.
    ui->label_wheel_corePoint_distance->setVisible(true);//轮到中心距离.
    ui->label_firstWheel_delta_angle_toPostiveDirction->setVisible(true);//0号车轮与正方向夹角.
    ui->label_steer_angle_min->setVisible(false);//编码器转角.
    ui->label_steer_angle_max->setVisible(false);
    ui->label_absEncoderRange_min->setVisible(false);
    ui->label_absEncoderRange_max->setVisible(false);
    ui->label_absEncoderType->setVisible(false);
    ui->doubleSpinBox_Aaxis->setVisible(false);
    ui->doubleSpinBox_Baxis->setVisible(false);
    ui->doubleSpinBox_corePoint_distance->setVisible(true);
    ui->doubleSpinBox_firstWheel_delta_angle_toPostiveDirction->setVisible(true);
    ui->spinBox_steer_angle_min->setVisible(false);
    ui->spinBox_steer_angle_max->setVisible(false);
    ui->spinBox_absEncoderRange_max->setVisible(false);
    ui->spinBox_absEncoderRange_min->setVisible(false);
    ui->comboBox_absEncoderType->setVisible(false);
    //设置全向数据参数（robot_model_数据结构是联合体应先对相应参数进行赋值，否则读取的应是前一种类型的数据）.
    robot_model_->robot_model_data_.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle=angle;
    robot_model_->robot_model_data_.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance = adistance;
    //设置编辑控件数据(在控件发送valuechanged消息时设置相应的属性数据).
    ui->doubleSpinBox_firstWheel_delta_angle_toPostiveDirction->blockSignals(true);
    ui->doubleSpinBox_firstWheel_delta_angle_toPostiveDirction->setValue(angle);
    ui->doubleSpinBox_firstWheel_delta_angle_toPostiveDirction->blockSignals(false);

    ui->doubleSpinBox_corePoint_distance->blockSignals(true);
    ui->doubleSpinBox_corePoint_distance->setValue(adistance/100);
    ui->doubleSpinBox_corePoint_distance->blockSignals(false);

}

void WidgetRectRobotModelSetting::set_SingleWheelDriver_argument(double adistance, double bdistance, double steerAngleMax, double steerAngleMin, double absEncoderRangeMax, double absEncoderRangeMin, int encoderType)
{
    ui->label_Aaxis->setVisible(true);//左右轮距.
    ui->label_Baxis->setVisible(true);//轴距.
    ui->label_wheel_corePoint_distance->setVisible(false);//轮到中心距离.
    ui->label_firstWheel_delta_angle_toPostiveDirction->setVisible(false);//0号车轮与正方向夹角.
    ui->label_steer_angle_min->setVisible(true);//编码器转角.
    ui->label_steer_angle_max->setVisible(true);
    ui->doubleSpinBox_Aaxis->setVisible(true);
    ui->doubleSpinBox_Baxis->setVisible(true);
    ui->doubleSpinBox_corePoint_distance->setVisible(false);
    ui->doubleSpinBox_firstWheel_delta_angle_toPostiveDirction->setVisible(false);
    ui->label_absEncoderRange_min->setVisible(true);
    ui->label_absEncoderRange_max->setVisible(true);
    ui->spinBox_steer_angle_min->setVisible(true);
    ui->spinBox_steer_angle_max->setVisible(true);
    ui->spinBox_absEncoderRange_max->setVisible(true);
    ui->spinBox_absEncoderRange_min->setVisible(true);
    ui->label_absEncoderType->setVisible(true);
    ui->comboBox_absEncoderType->setVisible(true);
    //（robo_model_数据结构是联合体应先对相应参数进行赋值，否则读取的应是前一种类型的数据）.
    robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.a_axis_distance=adistance;//单位cm.
    robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.b_axis_distance=bdistance;
    robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.steer_angle_max= steerAngleMax;
    robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.steer_angle_min=steerAngleMin;
    robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.absEncoderRange_max= absEncoderRangeMax;
    robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.absEncoderRange_min= absEncoderRangeMin;
    robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.absEncoderType = encoderType;
    //由于absEncoderRange的范围可由steerAngle范围确定所以不用给其赋值.
    //设置编辑控件数据(在控件发送valuechanged消息时设置相应的属性数据).
    ui->spinBox_steer_angle_min->blockSignals(true);
    ui->spinBox_steer_angle_min->setValue(steerAngleMin);
    ui->spinBox_steer_angle_min->blockSignals(false);
    ui->spinBox_steer_angle_max->blockSignals(true);
    ui->spinBox_steer_angle_max->setValue(steerAngleMax);
    ui->spinBox_steer_angle_max->blockSignals(false);
    ui->doubleSpinBox_Baxis->blockSignals(true);
    ui->doubleSpinBox_Baxis->setValue(bdistance/100);
    ui->doubleSpinBox_Baxis->blockSignals(false);
    ui->doubleSpinBox_Aaxis->blockSignals(true);
    ui->doubleSpinBox_Aaxis->setValue(adistance/100);
    ui->doubleSpinBox_Aaxis->blockSignals(false);
    ui->spinBox_absEncoderRange_max->blockSignals(true);
    ui->spinBox_absEncoderRange_max->setValue(absEncoderRangeMax);
    ui->spinBox_absEncoderRange_max->blockSignals(false);
    ui->spinBox_absEncoderRange_min->blockSignals(true);
    ui->spinBox_absEncoderRange_min->setValue(absEncoderRangeMin);
    ui->spinBox_absEncoderRange_min->blockSignals(false);
    ui->comboBox_absEncoderType->blockSignals(true);
    ui->comboBox_absEncoderType->setCurrentIndex(encoderType);
    ui->comboBox_absEncoderType->blockSignals(false);
}

void WidgetRectRobotModelSetting::setDriverInverAndIdDisplay(int num)
{
    switch (num) {
    case 1:
    {
        ui->widget_driver1->show();
        ui->widget_driver2->hide();
        ui->widget_driver3->hide();
        ui->widget_driver4->hide();
    }
        break;
    case 2:
    {
        ui->widget_driver1->show();
        ui->widget_driver2->show();
        ui->widget_driver3->hide();
        ui->widget_driver4->hide();
    }
        break;
    case 3:
    {
        ui->widget_driver1->show();
        ui->widget_driver2->show();
        ui->widget_driver3->show();
        ui->widget_driver4->hide();
    }
        break;
    case 4:
    {
        ui->widget_driver1->show();
        ui->widget_driver2->show();
        ui->widget_driver3->show();
        ui->widget_driver4->show();
    }
        break;
    default:
        break;
    }

    getInverseAndIdFlag();

}

void WidgetRectRobotModelSetting::getInverseAndIdFlag()
{
    switch (robot_model_->robot_model_data_.chasisType_) {
    case DIFFERENTIAL_TWOWHEEL:
        m_Rect_inverseList.clear();
        m_Rect_inverseList.append(inverse1);
        m_Rect_inverseList.append(inverse2);
        m_Rect_IdList.clear();
        m_Rect_IdList.append(id1);
        m_Rect_IdList.append(id2);

        break;
    case DIFFERENTIAL_FOURWHEEL:
        m_Rect_inverseList.clear();
        m_Rect_inverseList.clear();
        m_Rect_inverseList.append(inverse1);
        m_Rect_inverseList.append(inverse2);
        m_Rect_inverseList.append(inverse3);
        m_Rect_inverseList.append(inverse4);
        m_Rect_IdList.clear();
        m_Rect_IdList.append(id1);
        m_Rect_IdList.append(id2);
        m_Rect_IdList.append(id3);
        m_Rect_IdList.append(id4);

        break;
    case ALLDIRECTION_THREEWHEEL:
        m_Rect_inverseList.clear();
        m_Rect_inverseList.append(inverse1);
        m_Rect_inverseList.append(inverse2);
        m_Rect_inverseList.append(inverse3);
        m_Rect_IdList.clear();
        m_Rect_IdList.append(id1);
        m_Rect_IdList.append(id2);
        m_Rect_IdList.append(id3);

        break;
    case MECANUM_FOURWHEEL:
        m_Rect_inverseList.clear();
        m_Rect_inverseList.append(inverse1);
        m_Rect_inverseList.append(inverse2);
        m_Rect_inverseList.append(inverse3);
        m_Rect_inverseList.append(inverse4);
        m_Rect_IdList.clear();
        m_Rect_IdList.append(id1);
        m_Rect_IdList.append(id2);
        m_Rect_IdList.append(id3);
        m_Rect_IdList.append(id4);
        break;
    case SINGLESTEERWHEEL_FRONTDRIVER_FRONT_CODERTYPE:
    case SINGLESTEERWHEEL_FRONTDRIVER_BACK_CODERTYPE:
    case SINGLESTEERWHEEL_BACKDRIVER_FRONT_CODERTYPE:
    case SINGLESTEERWHEEL_BACKDRIVER_BACK_CODERTYPE:
        m_Rect_inverseList.clear();
        m_Rect_inverseList.append(inverse1);
        m_Rect_IdList.clear();
        m_Rect_IdList.append(id1);

        break;
    default:
        break;
    }

    robot_model_->robot_model_data_.driverID.clear();
    robot_model_->robot_model_data_.driverID = m_Rect_IdList;

    robot_model_->robot_model_data_.inverse_.clear();
    robot_model_->robot_model_data_.inverse_ = m_Rect_inverseList;
}

WidgetSetting *WidgetRectRobotModelSetting::getParentWid()
{
    return parentWid_;
}

void WidgetRectRobotModelSetting::setParentWid(WidgetSetting *parent)
{
    parentWid_= parent;
}

customDoubleSpinBox *WidgetRectRobotModelSetting::get_doubleSpinbox_head()
{
   return ui->doubleSpinBox_head;
}

customDoubleSpinBox *WidgetRectRobotModelSetting::get_doubleSpinbox_width()
{
   return ui->doubleSpinBox_width;
}

customDoubleSpinBox *WidgetRectRobotModelSetting::get_doubleSpinbox_tail()
{
  return ui->doubleSpinBox_tail;
}

void WidgetRectRobotModelSetting::on_comboBox_moveType_currentIndexChanged(int index)
{
    //记录当前最新索引.
    ui->comboBox_moveType->setNew_index(ui->comboBox_moveType->currentData().toInt());
    MoveStyle mvstyle = (MoveStyle)ui->comboBox_moveType->currentData().toInt();
    robot_model_->setMoveStyle(mvstyle);
    robot_model_->robot_model_data_.mvStyle_=mvstyle;
    switch (mvstyle) {
    case DIFFERENTIAL://差动.
    {
        ui->comboBox_chassisType->blockSignals(true);
        ui->comboBox_chassisType->clear();
        ui->comboBox_chassisType->addItem(tr("Two wheel differential"),DIFFERENTIAL_TWOWHEEL);
        ui->comboBox_chassisType->addItem(tr("Four wheel differential"),DIFFERENTIAL_FOURWHEEL);
        ui->comboBox_chassisType->blockSignals(false);
    }
        break;
    case ALLDIRECTION://全向.
    {
        ui->comboBox_chassisType->blockSignals(true);
        ui->comboBox_chassisType->clear();
        ui->comboBox_chassisType->addItem(tr("Three wheel omni"),ALLDIRECTION_THREEWHEEL);
        ui->comboBox_chassisType->blockSignals(false);
        ui->doubleSpinBox_firstWheel_delta_angle_toPostiveDirction->setOriValue(60);
        ui->doubleSpinBox_corePoint_distance->setOriValue(15/100.);
    }
        break;
    case  MECANUM://马克纳姆.
    {
        ui->comboBox_chassisType->blockSignals(true);
        ui->comboBox_chassisType->clear();
        ui->comboBox_chassisType->addItem(tr("Four wheel Mecanum"),MECANUM_FOURWHEEL);
        ui->comboBox_chassisType->blockSignals(false);
    }
        break;
    case SINGLESTEERWHEEL://单舵轮.
    {
        ui->comboBox_chassisType->blockSignals(true);
        ui->comboBox_chassisType->clear();
        ui->comboBox_chassisType->addItem(tr("Single rudder front drive and front encoder"),SINGLESTEERWHEEL_FRONTDRIVER_FRONT_CODERTYPE);
        ui->comboBox_chassisType->addItem(tr("Single rudder front and rear encoder"),SINGLESTEERWHEEL_FRONTDRIVER_BACK_CODERTYPE);
        ui->comboBox_chassisType->addItem(tr("Single rudder rear drive and front encoder"),SINGLESTEERWHEEL_BACKDRIVER_FRONT_CODERTYPE);
        ui->comboBox_chassisType->addItem(tr("Single rudder rear drive and rear encoder"),SINGLESTEERWHEEL_BACKDRIVER_BACK_CODERTYPE);
        ui->comboBox_chassisType->blockSignals(false);
    }
        break;
    default:
        break;
    }
   on_comboBox_chassisType_currentIndexChanged(0);
}

void WidgetRectRobotModelSetting::on_comboBox_chassisType_currentIndexChanged(int index)
{
    //    //切换时将头，尾，宽，半径编辑框从新设置为初始值.
    ui->doubleSpinBox_head->setOriValue(0.28);
    ui->doubleSpinBox_tail->setOriValue(0.28);
    ui->doubleSpinBox_width->setOriValue(0.56);
    ui->doubleSpinBox_wheelRadius->setOriValue(0.10);//轮半径.
    //model切换记录属性参数.
    m_new_modelData.head_ = 0.28;
    m_new_modelData.tail_ = 0.28;
    m_new_modelData.width_ = 0.56;
    m_new_modelData.wheelRadius_ = 0.10;
    m_new_modelData.shapType_ = RECTANGLE;
    ChassisType type = (ChassisType)ui->comboBox_chassisType->itemData(ui->comboBox_chassisType->currentIndex(),Qt::UserRole).toInt();
    switch (type)
    {
    case DIFFERENTIAL_TWOWHEEL://差动两轮.
        ui->doubleSpinBox_Aaxis->setOriValue(26/100.);
        //model切换记录属性参数.
        m_new_modelData.chasisModel.DiffTwoWheel.a_axis_distance = 0.26;
        setDriverInverAndIdDisplay(2);
        break;
    case DIFFERENTIAL_FOURWHEEL://差动四轮.
        ui->doubleSpinBox_Aaxis->setOriValue(26/100.);
        ui->doubleSpinBox_Baxis->setOriValue(30/100.);
        //model切换记录属性参数.
        m_new_modelData.chasisModel.DiffFourWheel.a_axis_distance = 0.26;
        m_new_modelData.chasisModel.DiffFourWheel.b_axis_distance = 0.30;
        setDriverInverAndIdDisplay(4);
        break;
    case ALLDIRECTION_THREEWHEEL://全向三轮.
        ui->doubleSpinBox_firstWheel_delta_angle_toPostiveDirction->setOriValue(60);
        ui->doubleSpinBox_corePoint_distance->setOriValue(15/100.);
        //model切换记录属性参数.
        m_new_modelData.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle = 60;
        m_new_modelData.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance = 0.15;
        setDriverInverAndIdDisplay(3);
        break;
    case MECANUM_FOURWHEEL://四轮麦克纳姆.
        ui->doubleSpinBox_Aaxis->setOriValue(26/100.);
        ui->doubleSpinBox_Baxis->setOriValue(30/100.);
        //model切换记录属性参数.
        m_new_modelData.chasisModel.MecanumFourWheel.a_axis_distance = 0.26;
        m_new_modelData.chasisModel.MecanumFourWheel.b_axis_distance = 0.30;
        setDriverInverAndIdDisplay(4);
        break;
    case SINGLESTEERWHEEL_FRONTDRIVER_FRONT_CODERTYPE://前驱+前转动编码器
    case SINGLESTEERWHEEL_FRONTDRIVER_BACK_CODERTYPE://前驱+后转动编码器
    case SINGLESTEERWHEEL_BACKDRIVER_FRONT_CODERTYPE://后驱+前转动编码器
    case SINGLESTEERWHEEL_BACKDRIVER_BACK_CODERTYPE://后驱+后转动编码器
        //设置底盘选型.
        ui->doubleSpinBox_Aaxis->setOriValue(26/100.);
        ui->doubleSpinBox_Baxis->setOriValue(30/100.);
        ui->spinBox_steer_angle_max->setOriValue(90);
        ui->spinBox_steer_angle_min->setOriValue(-90);
        ui->spinBox_absEncoderRange_max->setOriValue(1800);
        ui->spinBox_absEncoderRange_min->setOriValue(0);
        //model切换记录属性参数.
        m_new_modelData.chasisModel.SingleWheelDriver.a_axis_distance = 0.26;
        m_new_modelData.chasisModel.SingleWheelDriver.b_axis_distance = 0.30;
        m_new_modelData.chasisModel.SingleWheelDriver.steer_angle_max = 90;
        m_new_modelData.chasisModel.SingleWheelDriver.steer_angle_min = -90;
        m_new_modelData.chasisModel.SingleWheelDriver.absEncoderRange_max = 1800;
        m_new_modelData.chasisModel.SingleWheelDriver.absEncoderRange_min = 0;
        setDriverInverAndIdDisplay(1);
        break;
    default:
        break;
    }
    m_new_dataIndex.a_index = ui->comboBox_moveType->currentIndex();
    m_new_dataIndex.b_index =ui->comboBox_chassisType->currentIndex();
    modelboxCommand* chassis_command = new modelboxCommand(robot_model_,
                                                           ui->comboBox_moveType,ui->comboBox_chassisType,
                                                           ui->comboBox_absEncoderType,
                                                           m_ori_dataIndex,m_new_dataIndex,
                                                           m_LabelMap,
                                                           m_DoubleSpinBoxMap,
                                                           m_SpinBoxMap,
                                                           m_ori_modelData,m_new_modelData,
                                                           getParentWid(),
                                                           NULL);
    m_unStack->push(chassis_command);

    m_ori_dataIndex.a_index = ui->comboBox_moveType->currentIndex();
    m_ori_dataIndex.b_index = ui->comboBox_chassisType->currentIndex();

    m_ori_modelData.head_ = m_new_modelData.head_;
    m_ori_modelData.width_ = m_new_modelData.width_;
    m_ori_modelData.tail_ = m_new_modelData.tail_;
    m_ori_modelData.wheelRadius_ = m_new_modelData.wheelRadius_;

    switch (type)
    {
    case DIFFERENTIAL_TWOWHEEL://差动两轮.
        //model切换记录属性参数.
        m_ori_modelData.chasisModel.DiffTwoWheel.a_axis_distance = m_new_modelData.chasisModel.DiffTwoWheel.a_axis_distance;
        break;
    case DIFFERENTIAL_FOURWHEEL://差动四轮.

        //model切换记录属性参数.
        m_ori_modelData.chasisModel.DiffFourWheel.a_axis_distance = m_new_modelData.chasisModel.DiffFourWheel.a_axis_distance;
        m_ori_modelData.chasisModel.DiffFourWheel.b_axis_distance = m_new_modelData.chasisModel.DiffFourWheel.b_axis_distance;
        break;
    case ALLDIRECTION_THREEWHEEL://全向三轮.

        //model切换记录属性参数.
        m_ori_modelData.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle = m_new_modelData.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle;
        m_ori_modelData.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance =  m_new_modelData.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance;
        break;
    case MECANUM_FOURWHEEL://四轮麦克纳姆.

        //model切换记录属性参数.
        m_ori_modelData.chasisModel.MecanumFourWheel.a_axis_distance = m_new_modelData.chasisModel.MecanumFourWheel.a_axis_distance;
        m_ori_modelData.chasisModel.MecanumFourWheel.b_axis_distance = m_new_modelData.chasisModel.MecanumFourWheel.b_axis_distance;

        break;
    case SINGLESTEERWHEEL_FRONTDRIVER_FRONT_CODERTYPE://前驱+前转动编码器.
    case SINGLESTEERWHEEL_FRONTDRIVER_BACK_CODERTYPE://前驱+后转动编码器.
    case SINGLESTEERWHEEL_BACKDRIVER_FRONT_CODERTYPE://后驱+前转动编码器.
    case SINGLESTEERWHEEL_BACKDRIVER_BACK_CODERTYPE://后驱+后转动编码器.

        //model切换记录属性参数.
        m_ori_modelData.chasisModel.SingleWheelDriver.a_axis_distance =m_new_modelData.chasisModel.SingleWheelDriver.a_axis_distance;
        m_ori_modelData.chasisModel.SingleWheelDriver.b_axis_distance =m_new_modelData.chasisModel.SingleWheelDriver.b_axis_distance;
        m_ori_modelData.chasisModel.SingleWheelDriver.steer_angle_max = m_new_modelData.chasisModel.SingleWheelDriver.steer_angle_max;
        m_ori_modelData.chasisModel.SingleWheelDriver.steer_angle_min = m_new_modelData.chasisModel.SingleWheelDriver.steer_angle_min;
        m_ori_modelData.chasisModel.SingleWheelDriver.absEncoderRange_max = m_new_modelData.chasisModel.SingleWheelDriver.absEncoderRange_max;
        m_ori_modelData.chasisModel.SingleWheelDriver.absEncoderRange_min = m_new_modelData.chasisModel.SingleWheelDriver.absEncoderRange_min;

        break;
    default:
        break;
    }
}

void WidgetRectRobotModelSetting::on_doubleSpinBox_width_valueChanged(double value)
{
    robot_model_->robot_model_data_.width_ = value*100;
    robot_model_->update_model ();

    if(m_unStack==NULL)
    {
        return;
    }
    if(ui->doubleSpinBox_width->getIsClickedTrigger())
    {
        ui->doubleSpinBox_width->setNewValue(value);
        doubleSpinBoxCommand *wid_command=
                new doubleSpinBoxCommand(
                    ui->doubleSpinBox_width,
                    robot_model_,
                    ui->doubleSpinBox_width->getOriValue(),
                    value,getParentWid(),
                    NULL);
        m_unStack->push(wid_command);
        ui->doubleSpinBox_width->setOriValue(value);
    }

    ui->doubleSpinBox_width->setIsClickedTrigger(true);

    //保存切换之前的数据.
    m_ori_modelData.width_ = value;
}

void WidgetRectRobotModelSetting::on_doubleSpinBox_head_valueChanged(double value)
{
    robot_model_->robot_model_data_.head_=value*100;
    robot_model_->update_model ();

    if(m_unStack==NULL)
    {
        return;
    }
    ui->doubleSpinBox_head->setNewValue(value);
    if(ui->doubleSpinBox_head->getIsClickedTrigger())
    {
        doubleSpinBoxCommand* head_command =
                new doubleSpinBoxCommand(
                    ui->doubleSpinBox_head,
                    robot_model_,
                    ui->doubleSpinBox_head->getOriValue(),
                    value,getParentWid(),
                    NULL);
        m_unStack->push(head_command);
    }

    ui->doubleSpinBox_head->setOriValue(value);
    ui->doubleSpinBox_head->setIsClickedTrigger(true);

    //保存切换之前的数据.
    m_ori_modelData.head_ = value ;
}

void WidgetRectRobotModelSetting::on_doubleSpinBox_tail_valueChanged(double value)
{
    //获取最新Tail数据.
    robot_model_->robot_model_data_.tail_=value*100;
    robot_model_->update_model ();

    if(m_unStack==NULL)
    {
        return;
    }
    ui->doubleSpinBox_tail->setNewValue(value);
    if(ui->doubleSpinBox_tail->getIsClickedTrigger())
    {
        doubleSpinBoxCommand* tail_command =
                new doubleSpinBoxCommand(
                    ui->doubleSpinBox_tail,
                    robot_model_,
                    ui->doubleSpinBox_tail->getOriValue(),
                    value,getParentWid(),
                    NULL);
        m_unStack->push(tail_command);
    }
    ui->doubleSpinBox_tail->setOriValue(value);

    ui->doubleSpinBox_tail->setIsClickedTrigger(true);

    //保存切换之前的数据.
    m_ori_modelData.tail_ = value;
}

void WidgetRectRobotModelSetting::on_doubleSpinBox_wheelRadius_valueChanged(double value)
{
    //获取最新radius数据.
    //    robot_model_->robot_model_data_.radius_ = value*100;.
    switch (robot_model_->robot_model_data_.chasisType_)
    {
    case DIFFERENTIAL_TWOWHEEL://两轮差动.
    {
        robot_model_->robot_model_data_.wheelRadius_ = value*100;//无限制.
    }
        break;
    case DIFFERENTIAL_FOURWHEEL://四轮差动.
    {
        robot_model_->robot_model_data_.wheelRadius_ = value*100;
    }
        break;
    case ALLDIRECTION_THREEWHEEL://全向三轮.
    {
        robot_model_->robot_model_data_.wheelRadius_ = value*100;
    }
        break;
    case MECANUM_FOURWHEEL://四轮麦克纳姆.
    {
        robot_model_->robot_model_data_.wheelRadius_ = value*100;
    }
        break;
    case SINGLESTEERWHEEL_FRONTDRIVER_FRONT_CODERTYPE://前驱+前转动编码器 单舵轮.
    case SINGLESTEERWHEEL_FRONTDRIVER_BACK_CODERTYPE://前驱+后转动编码器.
    case SINGLESTEERWHEEL_BACKDRIVER_FRONT_CODERTYPE://后驱+前转动编码器.
    case SINGLESTEERWHEEL_BACKDRIVER_BACK_CODERTYPE://后驱+后转动编码器.
    {
        ui->doubleSpinBox_wheelRadius->blockSignals(true);
        ui->doubleSpinBox_wheelRadius->setMaximum(robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.b_axis_distance/2);
        ui->doubleSpinBox_wheelRadius->blockSignals(false);
        robot_model_->robot_model_data_.wheelRadius_ = value*100;
    }
        break;
    default:
        break;
    }
    robot_model_->update_model();

    if(m_unStack==NULL)
    {
        return;
    }

    ui->doubleSpinBox_wheelRadius->setNewValue(value);
    if(ui->doubleSpinBox_wheelRadius->getIsClickedTrigger())
    {
        doubleSpinBoxCommand* radius_command = new doubleSpinBoxCommand(ui->doubleSpinBox_wheelRadius,robot_model_,
                                                                        ui->doubleSpinBox_wheelRadius->getOriValue(),
                                                                        value,getParentWid(),NULL);

        m_unStack->push(radius_command);
    }
    ui->doubleSpinBox_wheelRadius->setOriValue(value);
    ui->doubleSpinBox_wheelRadius->setIsClickedTrigger(true);


    //保存切换之前的数据.
    m_ori_modelData.wheelRadius_ = value;
}

void WidgetRectRobotModelSetting::on_doubleSpinBox_corePoint_distance_valueChanged(double value)
{
    robot_model_->robot_model_data_.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance = value*100;
    robot_model_->update_model();

    if(m_unStack==NULL)
    {
        return;
    }
    if(ui->doubleSpinBox_corePoint_distance->getIsClickedTrigger())
    {
        ui->doubleSpinBox_corePoint_distance->setNewValue(value);
        doubleSpinBoxCommand* angle_command =
                new doubleSpinBoxCommand(
                    ui->doubleSpinBox_corePoint_distance,
                    robot_model_,ui->doubleSpinBox_corePoint_distance->getOriValue(),
                    value,getParentWid(),NULL);
        m_unStack->push(angle_command);
        ui->doubleSpinBox_corePoint_distance->setOriValue(value);
    }
    ui->doubleSpinBox_corePoint_distance->setIsClickedTrigger(true);

    //model切换记录切换前数据.
    m_ori_modelData.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance  = value;
}

void WidgetRectRobotModelSetting::on_doubleSpinBox_Baxis_valueChanged(double value)
{
    switch (robot_model_->robot_model_data_.chasisType_)
    {
    case DIFFERENTIAL_TWOWHEEL://两轮差动.

        break;
    case DIFFERENTIAL_FOURWHEEL://四轮差动.
    {
        ui->doubleSpinBox_wheelRadius->setMaximum(value/2);//半径的最大值不能超过b轴的一半.

        robot_model_->robot_model_data_.chasisModel.DiffFourWheel.b_axis_distance = value*100;
        // 切换model记录属性数据.
        m_ori_modelData.chasisModel.DiffFourWheel.b_axis_distance = value;
    }
        break;
    case ALLDIRECTION_THREEWHEEL://全向三轮.
        break;
    case MECANUM_FOURWHEEL://四轮麦克纳姆.
    {
        ui->doubleSpinBox_wheelRadius->setMaximum(value/2);//半径的最大值不能超过b轴的一半.

        robot_model_->robot_model_data_.chasisModel.MecanumFourWheel.b_axis_distance = value*100;
        // 切换model记录属性数据.
        m_ori_modelData.chasisModel.MecanumFourWheel.b_axis_distance = value;
    }
        break;
    case SINGLESTEERWHEEL_FRONTDRIVER_FRONT_CODERTYPE://前驱+前转动编码器 单舵轮.
    case SINGLESTEERWHEEL_FRONTDRIVER_BACK_CODERTYPE://前驱+后转动编码器.
    case SINGLESTEERWHEEL_BACKDRIVER_FRONT_CODERTYPE://后驱+前转动编码器.
    case SINGLESTEERWHEEL_BACKDRIVER_BACK_CODERTYPE://后驱+后转动编码器.
        robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.b_axis_distance = value*100;
        // 切换model记录属性数据
        m_ori_modelData.chasisModel.SingleWheelDriver.b_axis_distance = value;
        break;
    default:
        break;
    }
    robot_model_->update_model();


    if(m_unStack==NULL)
    {
        return;
    }


    ui->doubleSpinBox_Baxis->setNewValue(value);
    if(ui->doubleSpinBox_Baxis->getIsClickedTrigger())
    {
        doubleSpinBoxCommand* b_axis_command =
                new doubleSpinBoxCommand(
                    ui->doubleSpinBox_Baxis,
                    robot_model_,ui->doubleSpinBox_Baxis->getOriValue(),
                    value,getParentWid(),NULL);
        m_unStack->push(b_axis_command);
    }
    ui->doubleSpinBox_Baxis->setOriValue(value);
    ui->doubleSpinBox_Baxis->setIsClickedTrigger(true);
}

void WidgetRectRobotModelSetting::on_spinBox_steer_angle_min_valueChanged(int value)
{
    ui->spinBox_steer_angle_min->setMaximum(ui->spinBox_steer_angle_max->value());
    robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.steer_angle_min = value;

    if(m_unStack==NULL)
    {
        return;
    }

    ui->spinBox_steer_angle_min->setNewValue(value);
    if(ui->spinBox_steer_angle_min->getIsClickedTrigger())
    {
        SpinBoxCommand* steer_minAngle_command =
                new SpinBoxCommand(
                    ui->spinBox_steer_angle_min,
                    ui->spinBox_steer_angle_min->getOriValue(),
                    value,getParentWid(),NULL);
        m_unStack->push(steer_minAngle_command);
    }
    ui->spinBox_steer_angle_min->setOriValue(value);
    ui->spinBox_steer_angle_min->setIsClickedTrigger(true);

    //切换model，记录切换前数据
    m_ori_modelData.chasisModel.SingleWheelDriver.steer_angle_min = value;
}

void WidgetRectRobotModelSetting::on_spinBox_steer_angle_max_valueChanged(int value)
{
    ui->spinBox_steer_angle_max->setMinimum(ui->spinBox_steer_angle_min->value());

    robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.steer_angle_max = value;

    if(m_unStack==NULL)
    {
        return;
    }

    ui->spinBox_steer_angle_max->setNewValue(value);
    if(ui->spinBox_steer_angle_max->getIsClickedTrigger())
    {
        SpinBoxCommand* steer_maxAngle_command =
                new SpinBoxCommand(
                    ui->spinBox_steer_angle_max,
                    ui->spinBox_steer_angle_max->getOriValue(),
                    value,getParentWid(),NULL);
        m_unStack->push(steer_maxAngle_command);
    }

    ui->spinBox_steer_angle_max->setOriValue(value);
    ui->spinBox_steer_angle_max->setIsClickedTrigger(true);
    //切换model，记录切换前数据
    m_ori_modelData.chasisModel.SingleWheelDriver.steer_angle_max = value;
}


void WidgetRectRobotModelSetting::on_comboBox_absEncoderType_currentIndexChanged(int index)
{
    int type = ui->comboBox_absEncoderType->currentData().toInt();
    robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.absEncoderType = type;

    if(m_unStack==NULL)
    {
        return ;
    }
    //将变化信息存入栈中用于undo 与redo 操作
    int newidx = ui->comboBox_absEncoderType->currentIndex();
    ui->comboBox_absEncoderType->setNew_index(newidx);
    if(ui->comboBox_absEncoderType->getIsClickedTrigger())//如果是鼠标点击触发的则压栈
    {
        customboxCommand* temp_Command = new customboxCommand(ui->comboBox_absEncoderType,
                                                              ui->comboBox_absEncoderType->getOri_index(),
                                                              ui->comboBox_absEncoderType->getNew_index(),getParentWid(),NULL);
        m_unStack->push(temp_Command);
    }
    ui->comboBox_absEncoderType->setOri_index(newidx);
    ui->comboBox_absEncoderType->setIsClickedTrigger(true);
}

void WidgetRectRobotModelSetting::on_spinBox_ID1_valueChanged(int value)
{
    if(m_unStack==NULL) m_unStack = UndoStack::InitStack();
    id1 = value;
    if(ui->spinBox_ID1->getIsClickedTrigger())
    {
        ui->spinBox_ID1->setNewValue(value);
        SpinBoxCommand* tempCmd =    new SpinBoxCommand(ui->spinBox_ID1,ui->spinBox_ID1->getOriValue(),
                                                        ui->spinBox_ID1->getNewValue(),getParentWid(),NULL);
        m_unStack->push(tempCmd);
        ui->spinBox_ID1->setOriValue(value);
    }
    ui->spinBox_ID1->setIsClickedTrigger(true);
    getInverseAndIdFlag();
}

void WidgetRectRobotModelSetting::on_spinBox_ID2_valueChanged(int value)
{
    if(m_unStack==NULL) m_unStack = UndoStack::InitStack();
    id2 = value;
    if(ui->spinBox_ID2->getIsClickedTrigger())
    {
        ui->spinBox_ID2->setNewValue(value);
        SpinBoxCommand* tempCmd =  new SpinBoxCommand(ui->spinBox_ID2,ui->spinBox_ID2->getOriValue(),
                                                      ui->spinBox_ID2->getNewValue(),getParentWid(),NULL);
        m_unStack->push(tempCmd);
        ui->spinBox_ID2->setOriValue(value);
    }
    ui->spinBox_ID2->setIsClickedTrigger(true);
    getInverseAndIdFlag();
}

void WidgetRectRobotModelSetting::on_spinBox_ID3_valueChanged(int value)
{
    if(m_unStack==NULL) m_unStack = UndoStack::InitStack();
    id3 = value;
    if(ui->spinBox_ID3->getIsClickedTrigger())
    {
        ui->spinBox_ID3->setNewValue(value);
        SpinBoxCommand* tempCmd =    new SpinBoxCommand(ui->spinBox_ID3,ui->spinBox_ID3->getOriValue(),
                                                        ui->spinBox_ID3->getNewValue(),getParentWid(),NULL);
        m_unStack->push(tempCmd);
        ui->spinBox_ID3->setOriValue(value);
    }
    ui->spinBox_ID3->setIsClickedTrigger(true);
    getInverseAndIdFlag();
}

void WidgetRectRobotModelSetting::on_spinBox_ID4_valueChanged(int value)
{
    if(m_unStack==NULL) m_unStack = UndoStack::InitStack();
    id4 = value;
    if(ui->spinBox_ID4->getIsClickedTrigger())
    {
        ui->spinBox_ID4->setNewValue(value);
        SpinBoxCommand* tempCmd =    new SpinBoxCommand(ui->spinBox_ID4,ui->spinBox_ID4->getOriValue(),
                                                        ui->spinBox_ID4->getNewValue(),getParentWid(),NULL);
        m_unStack->push(tempCmd);
        ui->spinBox_ID4->setOriValue(value);
    }
    ui->spinBox_ID4->setIsClickedTrigger(true);
    getInverseAndIdFlag();
}

void WidgetRectRobotModelSetting::on_comboBox_inverse1_currentIndexChanged(int index)
{
    if(0 == index)
    {
        inverse1 = false;
    }
    else
    {
        inverse1 = true;
    }
    getInverseAndIdFlag();

    if(m_unStack==NULL)
    {
        m_unStack = UndoStack::InitStack();
    }
    if(ui->comboBox_inverse1->getIsClickedTrigger())
    {
        ui->comboBox_inverse1->setNew_index(index);
        customboxCommand* tempCmd =new  customboxCommand(ui->comboBox_inverse1,ui->comboBox_inverse1->getOri_index(),ui->comboBox_inverse1->getNew_index(),getParentWid(),NULL);
        m_unStack->push(tempCmd);
        ui->comboBox_inverse1->setOri_index(index);
    }
    ui->comboBox_inverse1->setIsClickedTrigger(true);
}

void WidgetRectRobotModelSetting::on_comboBox_inverse2_currentIndexChanged(int index)
{
    if(0 == index)
    {
        inverse2 = false;
    }
    else
    {
        inverse2 = true;
    }
    getInverseAndIdFlag();
    if(ui->comboBox_inverse2->getIsClickedTrigger())
    {
        ui->comboBox_inverse2->setNew_index(index);
        customboxCommand* tempCmd =new customboxCommand(ui->comboBox_inverse2,ui->comboBox_inverse2->getOri_index(),ui->comboBox_inverse2->getNew_index(),getParentWid(),NULL);
        m_unStack->push(tempCmd);
        ui->comboBox_inverse2->setOri_index(index);
    }
    ui->comboBox_inverse2->setIsClickedTrigger(true);
}

void WidgetRectRobotModelSetting::on_comboBox_inverse3_currentIndexChanged(int index)
{
    if(0 == index)
    {
        inverse3 = false;
    }
    else
    {
        inverse3 = true ;
    }
    if(m_unStack==NULL)
    {
        m_unStack = UndoStack::InitStack();
    }

    getInverseAndIdFlag();
    if(ui->comboBox_inverse3->getIsClickedTrigger())
    {
        ui->comboBox_inverse3->setNew_index(index);
        customboxCommand* tempCmd =new customboxCommand(ui->comboBox_inverse3,ui->comboBox_inverse3->getOri_index(),ui->comboBox_inverse3->getNew_index(),getParentWid(),NULL);
        m_unStack->push(tempCmd);
        ui->comboBox_inverse3->setOri_index(index);
    }
    ui->comboBox_inverse3->setIsClickedTrigger(true);
}

void WidgetRectRobotModelSetting::on_comboBox_inverse4_currentIndexChanged(int index)
{
    if(0 == index)
    {
        inverse4 = false;
    }
    else
    {
        inverse4 = true;
    }
    if(m_unStack==NULL)
    {
        m_unStack = UndoStack::InitStack();
    }

    getInverseAndIdFlag();
    if(ui->comboBox_inverse4->getIsClickedTrigger())
    {
        ui->comboBox_inverse4->setNew_index(index);
        customboxCommand* tempCmd =new  customboxCommand(ui->comboBox_inverse4,ui->comboBox_inverse4->getOri_index(),
                                                         ui->comboBox_inverse4->getNew_index(),getParentWid(),NULL);
        m_unStack->push(tempCmd);
        ui->comboBox_inverse4->setOri_index(index);
    }
    ui->comboBox_inverse4->setIsClickedTrigger(true);
}

void WidgetRectRobotModelSetting::on_doubleSpinBox_Aaxis_valueChanged(double value)
{
    switch (robot_model_->robot_model_data_.chasisType_)
    {
    case DIFFERENTIAL_TWOWHEEL://两轮差动

        robot_model_->robot_model_data_.chasisModel.DiffTwoWheel.a_axis_distance = value*100;
        //保存model切换之前的数据
        m_ori_modelData.chasisModel.DiffTwoWheel.a_axis_distance = value;
        break;
    case DIFFERENTIAL_FOURWHEEL://四轮差动
        robot_model_->robot_model_data_.chasisModel.DiffFourWheel.a_axis_distance = value*100;
        //保存model切换之前的数据
        m_ori_modelData.chasisModel.DiffFourWheel.a_axis_distance = value;
        break;
    case ALLDIRECTION_THREEWHEEL://全向三轮
        break;
    case MECANUM_FOURWHEEL://四轮麦克纳姆
        robot_model_->robot_model_data_.chasisModel.MecanumFourWheel.a_axis_distance = value*100;
        //保存model切换之前的数据
        m_ori_modelData.chasisModel.MecanumFourWheel.a_axis_distance = value;
        break;
    case SINGLESTEERWHEEL_FRONTDRIVER_FRONT_CODERTYPE://前驱+前转动编码器 单舵轮
    case SINGLESTEERWHEEL_FRONTDRIVER_BACK_CODERTYPE://前驱+后转动编码器
    case SINGLESTEERWHEEL_BACKDRIVER_FRONT_CODERTYPE://后驱+前转动编码器
    case SINGLESTEERWHEEL_BACKDRIVER_BACK_CODERTYPE://后驱+后转动编码器
        robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.a_axis_distance = value*100;
        m_ori_modelData.chasisModel.SingleWheelDriver.a_axis_distance = value;
        break;
    default:
        break;
    }
    robot_model_->update_model();

    if(m_unStack==NULL)
    {
        m_unStack = UndoStack::InitStack();
    }

    ui->doubleSpinBox_Aaxis->setNewValue(value);
    if(ui->doubleSpinBox_Aaxis->getIsClickedTrigger())
    {
        doubleSpinBoxCommand* a_axis_command =
                new doubleSpinBoxCommand(
                    ui->doubleSpinBox_Aaxis,
                    robot_model_,ui->doubleSpinBox_Aaxis->getOriValue(),
                    value,getParentWid(),NULL);

        m_unStack->push(a_axis_command);
    }
    ui->doubleSpinBox_Aaxis->setOriValue(value);
    ui->doubleSpinBox_Aaxis->setIsClickedTrigger(true);
}


void WidgetRectRobotModelSetting::on_spinBox_absEncoderRange_min_valueChanged(int value)
{
    ui->spinBox_absEncoderRange_min->setMaximum(ui->spinBox_absEncoderRange_max->value());
    robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.absEncoderRange_min = value;

    if(m_unStack==NULL)
    {
        m_unStack = UndoStack::InitStack();
    }


    ui->spinBox_absEncoderRange_min->setNewValue(value);
    if(ui->spinBox_absEncoderRange_min->getIsClickedTrigger())
    {
        SpinBoxCommand* absEcoder_min_command =
                new SpinBoxCommand(
                    ui->spinBox_absEncoderRange_min,
                    ui->spinBox_absEncoderRange_min->getOriValue(),
                    value,getParentWid(),NULL);
        m_unStack->push(absEcoder_min_command);
    }
    ui->spinBox_absEncoderRange_min->setOriValue(value);
    ui->spinBox_absEncoderRange_min->setIsClickedTrigger(true);

    //切换model，记录切换前数据
    m_ori_modelData.chasisModel.SingleWheelDriver.absEncoderRange_min = value;
}

void WidgetRectRobotModelSetting::on_doubleSpinBox_firstWheel_delta_angle_toPostiveDirction_valueChanged(double value)
{
    robot_model_->robot_model_data_.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle = value;
    robot_model_->update_model();
    if(m_unStack==NULL)
    {
        m_unStack = UndoStack::InitStack();
    }

    ui->doubleSpinBox_firstWheel_delta_angle_toPostiveDirction->setNewValue(value);
    if(ui->doubleSpinBox_firstWheel_delta_angle_toPostiveDirction->getIsClickedTrigger())
    {
        doubleSpinBoxCommand* angle_command =
                new doubleSpinBoxCommand(
                    ui->doubleSpinBox_firstWheel_delta_angle_toPostiveDirction,
                    robot_model_,ui->doubleSpinBox_firstWheel_delta_angle_toPostiveDirction->getOriValue(),
                    value,getParentWid(),NULL);
        m_unStack->push(angle_command);
    }
    ui->doubleSpinBox_firstWheel_delta_angle_toPostiveDirction->setOriValue(value);
    ui->doubleSpinBox_firstWheel_delta_angle_toPostiveDirction->setIsClickedTrigger(true);

    //model切换记录切换前数据
    m_ori_modelData.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle = value;

}

void WidgetRectRobotModelSetting::on_spinBox_absEncoderRange_max_valueChanged(int value)
{
    ui->spinBox_absEncoderRange_max->setMinimum(ui->spinBox_absEncoderRange_min->value());
    robot_model_->robot_model_data_.chasisModel.SingleWheelDriver.absEncoderRange_max= value;

    if(m_unStack==NULL)
    {
        m_unStack = UndoStack::InitStack();
    }

    ui->spinBox_absEncoderRange_max->setNewValue(value);
    if(ui->spinBox_absEncoderRange_max->getIsClickedTrigger())
    {
        SpinBoxCommand* absEcoder_max_command =
                new SpinBoxCommand(
                    ui->spinBox_absEncoderRange_max,
                    ui->spinBox_absEncoderRange_max->getOriValue(),
                    value,getParentWid(),NULL);
        m_unStack->push(absEcoder_max_command);

    }
    ui->spinBox_absEncoderRange_max->setOriValue(value);
    ui->spinBox_absEncoderRange_max->setIsClickedTrigger(true);
    //切换model，记录切换前数据
    m_ori_modelData.chasisModel.SingleWheelDriver.absEncoderRange_max = value;
}

void WidgetRectRobotModelSetting::on_comboBox_currentIndexChanged(int index)
{

}
