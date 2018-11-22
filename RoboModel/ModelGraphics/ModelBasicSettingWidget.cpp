#include "ModelBasicSettingWidget.h"
#include "ui_ModelBasicSettingWidget.h"
#include "ModelGraphicsView.h"
#include <QDebug>
#include <QPaintEvent>
#include "WidgetDOState.h"
#include "ModelHelper.h"
#include "ModelCommon.h"
#include <QVariantMap>
#include <QJsonDocument>
#include <QJsonArray>
#include "PublicClass.h"
#include"ModelCustomCommand.h"
#include"UndoStack.h"
#include"EditRobotModelWidget.h"
#include"GlobalVariant.h"
#include<QScrollBar>
#include<QGesture>
ModelBasicSettingWidget::ModelBasicSettingWidget(ModelGraphicsView *parent) :
    PropertyBasicWidget(parent),
    ui(new Ui::ModelBasicSettingWidget),
    model_view_(parent),
    forkWid_(new WidgetForkSetting(this)),
    m_undoStack(NULL),
    m_lastMousePos(QPoint(0,0))
{

    ui->setupUi(this);
    initBasicSettingWidgetInfo();
    ui->pushButton_off->setFixedSize(18,18);
    ui->pushButton_on->setFixedSize(18,18);
    ui->pushButton_off->setStyleSheet(QString("QPushButton{border-width: 1px;border-style: solid;background-color: rgb(255, 0, 0);border-radius:9px;}"));
    ui->pushButton_on->setStyleSheet(QString("QPushButton{border-width: 1px;border-style: solid;background-color: rgb(0, 128, 0);border-radius:9px;}"));
    m_undoStack  = UndoStack::InitStack();

    connect(ui->spinBox_DI,SIGNAL(sig_show_tooltips()),this,SLOT(slot_set_SpinboxtoolTips()));
    //    connect(ui->ComboBox_driver_type,SIGNAL(sig_indexChange(int)),this,SLOT(slot_driverType_change()));
    //驱动器类型隐藏
    ui->groupBox_driverType->hide();
    //添加叉车编辑窗
    ui->verticalLayout_fork->addWidget(forkWid_);
    forkWid_->hide();
    forkWid_->setParentWid(this);
    ui->checkBox_forkConfigure->setOriValue(false);
    //手势捕捉
    //    grabGesture(Qt::PanGesture);
    //    grabGesture(Qt::PinchGesture);
    //    grabGesture(Qt::SwipeGesture);
    setAttribute(Qt::WA_AcceptTouchEvents);
}

ModelBasicSettingWidget::~ModelBasicSettingWidget()
{
    delete ui;
}

void ModelBasicSettingWidget::initBasicSettingWidgetInfo()
{
    //    initDrvierTypeCombox();
    initDrvierBrandCombox();
    initGyroTypeCombox();
    initBatteryTypeCombox();
    initRFIDTypeCombox();
    initMagneticSensorTypeCombox();
    initUltrasonicTypeCombox();
    ui->lineEdit_model_name->blockSignals(true);
    ui->lineEdit_model_name->setText (model_base_data_.model_name_);
    ui->lineEdit_model_name->blockSignals(false);
    ui->DoubleSpinBox_reductionRatio->blockSignals(true);
    ui->DoubleSpinBox_reductionRatio->setValue (model_base_data_.reduction_ratio_);
    ui->DoubleSpinBox_reductionRatio->blockSignals(false);
    ui->SpinBox_encoderLine->blockSignals(true);
    ui->SpinBox_encoderLine->setValue (model_base_data_.encoder_line_);
    ui->SpinBox_encoderLine->blockSignals(false);
    ui->SpinBox_maxMotorRpm->blockSignals(true);
    ui->SpinBox_maxMotorRpm->setValue (model_base_data_.max_motor_rpm_);
    ui->SpinBox_maxMotorRpm->blockSignals(false);
    ui->checkBox_brake->blockSignals(true);
    ui->checkBox_brake->setChecked (model_base_data_.brake_);
    ui->checkBox_brake->blockSignals(false);
    ui->checkBox_autoBrake->blockSignals(true);
    ui->checkBox_autoBrake->setChecked (model_base_data_.auto_brake_);
    ui->checkBox_autoBrake->blockSignals(false);
    model_base_data_.brake_? ui->checkBox_autoBrake->setEnabled(true):ui->checkBox_autoBrake->setEnabled(false);
    ui->checkBox_LED->blockSignals(true);
    ui->checkBox_LED->setChecked (model_base_data_.LED_);
    ui->checkBox_LED->blockSignals(false);
    ui->spinBox_DI->blockSignals(true);
    ui->spinBox_DI->setValue (model_base_data_.max_di_num_);
    ui->spinBox_DI->blockSignals(false);
    ui->spinBox_DO->blockSignals(true);
    ui->spinBox_DO->setValue (model_base_data_.max_do_num_);
    ui->spinBox_DO->blockSignals(false);
    ui->spinBox_ultrasonic->blockSignals(true);
    ui->spinBox_ultrasonic->setValue (model_base_data_.max_ultrasonic_num_);
    ui->spinBox_ultrasonic->blockSignals(false);
    ui->comboBox_ultrasonic_type->blockSignals(true);
    ui->comboBox_ultrasonic_type->setCurrentIndex (model_base_data_.ultrasonic_);
    ui->comboBox_ultrasonic_type->blockSignals(false);
    update_do_size(model_base_data_.max_do_num_,widget_do_list_.size());

    //ori model name
    ui->lineEdit_model_name->setOriValue(model_base_data_.model_name_);
    ui->ComboBox_driver_type->setOri_index(ui->ComboBox_driver_type->currentIndex());
    ui->comboBox_gyro->setOri_index(ui->comboBox_gyro->currentIndex());
    ui->comboBox_batteryInfo_type->setOri_index(ui->comboBox_batteryInfo_type->currentIndex());
    ui->combox_RFID_type->setOri_index(ui->combox_RFID_type->currentIndex());
    ui->comboBox_magneticSensor_type->setOri_index(ui->comboBox_magneticSensor_type->currentIndex());
    ui->comboBox_ultrasonic_type->setOri_index(ui->comboBox_ultrasonic_type->currentIndex());

    //checkbox
    ui->checkBox_brake->setOriValue(model_base_data_.brake_);
    ui->checkBox_autoBrake->setOriValue(model_base_data_.auto_brake_);
    ui->checkBox_LED->setOriValue(model_base_data_.LED_);
    ui->checkBox_autoGyroCal->setOriValue(model_base_data_.auto_gyro_cal_);

    //spinbox
    ui->SpinBox_encoderLine->setOriValue(model_base_data_.encoder_line_);
    ui->SpinBox_encoderLine->setIsClickedTrigger(true);
    ui->SpinBox_maxMotorRpm->setOriValue(model_base_data_.max_motor_rpm_);
    ui->SpinBox_maxMotorRpm->setIsClickedTrigger(true);
    ui->spinBox_DI->setOriValue(model_base_data_.max_di_num_);
    ui->spinBox_DI->setIsClickedTrigger(true);
    ui->spinBox_DO->setOriValue(model_base_data_.max_do_num_);
    ui->spinBox_DO->setIsClickedTrigger(true);
    ui->spinBox_ultrasonic->setOriValue(model_base_data_.max_ultrasonic_num_);
    ui->spinBox_ultrasonic->setIsClickedTrigger(true);
    ui->DoubleSpinBox_reductionRatio->setOriValue(model_base_data_.reduction_ratio_);
    ui->DoubleSpinBox_reductionRatio->setIsClickedTrigger(true);

}

void ModelBasicSettingWidget::initForkWidget()
{

}

void ModelBasicSettingWidget::initDrvierTypeCombox()
{
    initCombox(ui->ComboBox_driver_type);
}

void ModelBasicSettingWidget::initDrvierBrandCombox()
{
    initCombox(ui->comboBox_driverBrand);
}

void ModelBasicSettingWidget::initGyroTypeCombox()
{
    initCombox(ui->comboBox_gyro);
}

void ModelBasicSettingWidget::initBatteryTypeCombox()
{
    initCombox(ui->comboBox_batteryInfo_type);
}

void ModelBasicSettingWidget::initRFIDTypeCombox()
{
    initCombox(ui->combox_RFID_type);
}

void ModelBasicSettingWidget::initMagneticSensorTypeCombox()
{
    initCombox(ui->comboBox_magneticSensor_type);
}

void ModelBasicSettingWidget::initUltrasonicTypeCombox()
{
    initCombox(ui->comboBox_ultrasonic_type);
}


void ModelBasicSettingWidget::initCombox(customComboBox *box)//适用于所有可配置的combox
{
    QString filePath = CONFIGFILE_PATH;
    ConfigureData  configInfo;

    if(box == ui->ComboBox_driver_type)
    {
        configInfo = readConfigure_drvierType(filePath);
    }
    else if(box == ui->comboBox_driverBrand)
    {
        configInfo = readConfigure_drvierBrand(filePath);
    }
    else if(box == ui->comboBox_batteryInfo_type)
    {
        configInfo = readConfigure_BatteryType(filePath);
    }
    else if(box == ui->comboBox_gyro)
    {
        configInfo = readConfigure_gyroType(filePath);
    }
    else if(box == ui->combox_RFID_type)
    {
        configInfo = readConfigure_RFIDType(filePath);
    }
    else if(box == ui->comboBox_magneticSensor_type)
    {
        configInfo = readConfigure_MagneticSensorType(filePath);
    }
    else if(box == ui->comboBox_ultrasonic_type)
    {
        configInfo = readConfigure_UltrasonicType(filePath);
    }

    int DefIndex = configInfo.defaultidx;

    QList<ItemData>::iterator iter = configInfo.dataList.begin();
    box->blockSignals(true);
    for(;iter!= configInfo.dataList.end();iter++)
    {
        box->addItem((*iter).describtion,(*iter).index);
    }
    box->blockSignals(false);

    int idex = box->findData(DefIndex,Qt::UserRole);

    bool flag = GlobalVariant::getInst()->flag;

    if(!flag)//newModel
    {
        box->blockSignals(true);
        box->setCurrentIndex(idex);
        box->blockSignals(false);
    }
    else
    {
        //加载模型创建时设置当前项为模型文件中保存的数据
        if(box == ui->ComboBox_driver_type)
        {
            box->blockSignals(true);
            int idx = box->findData(model_base_data_.driver_);
            box->setCurrentIndex(idx);
            box->blockSignals(false);
        }
        else if(box == ui->comboBox_driverBrand)
        {
            box->blockSignals(true);
            int idx = box->findText(model_base_data_.driverBrand_);
            box->setCurrentIndex(idx);
            box->blockSignals(false);
        }
        else if(box == ui->comboBox_batteryInfo_type)
        {
            box->blockSignals(true);
            int idx = box->findData(model_base_data_.battery_info_);
            box->setCurrentIndex(idx);
            box->blockSignals(false);
        }
        else if(box == ui->comboBox_gyro)
        {
            box->blockSignals(true);
            int idx = box->findData(model_base_data_.gyro_type_);
            box->setCurrentIndex(idx);
            box->blockSignals(false);
        }
        else if(box == ui->combox_RFID_type)
        {
            box->blockSignals(true);
            int idx = box->findData(model_base_data_.RFID_);
            box->setCurrentIndex(idx);
            box->blockSignals(false);
        }
        else if(box == ui->comboBox_magneticSensor_type)
        {
            box->blockSignals(true);
            int idx = box->findData(model_base_data_.magnetic_sensor_);
            box->setCurrentIndex(idx);
            box->blockSignals(false);
        }
        else if(box == ui->comboBox_ultrasonic_type)
        {
            box->blockSignals(true);
            int idx = box->findData(model_base_data_.ultrasonic_);
            box->setCurrentIndex(idx);
            box->blockSignals(false);
        }
    }

    box->setOri_index(box->currentIndex());
}
//仅适用于与底盘相关配置文件结构一致的设备（不通用）
ConfigureData ModelBasicSettingWidget::readMechineConfigureData(const QString& mechineConfigName,const QString& filePath)
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

    QJsonValue gyro_value = chassis_obj.value(mechineConfigName);
    QJsonObject gyro_obj = gyro_value.toObject();

    QJsonValue defindx = gyro_obj["defaultIndex"];


    QJsonArray typeList = gyro_obj["typeList"].toArray();

    ConfigureData  configData ;

    configData.defaultidx = defindx.toInt();


    for(int i = 0; i < typeList.size();i++)
    {

        QJsonObject tempObj = typeList.at(i).toObject();

        QJsonValue tempIdex = tempObj["index"];
        QJsonValue tempDesc = tempObj["desc"];

        ItemData  tempData;
        tempData.index = tempIdex.toInt();
        tempData.describtion = tempDesc.toString();
        configData.dataList.push_back(tempData);
    }

    return configData;

}

ConfigureData ModelBasicSettingWidget::readConfigure_drvierBrand(const QString &filePath)
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

    QJsonValue  mode_param_value= chassis_obj["mode_param"];
    QJsonObject mode_param_obj = mode_param_value.toObject();

    QJsonValue  driver_value = mode_param_obj["driverBrand"];
    QJsonObject driver_obj = driver_value.toObject();

    ConfigureData  driverBrandData;
    QJsonValue defaultIdex = driver_obj["defaultIndex"].toInt();
    driverBrandData.defaultidx = defaultIdex.toInt();
    QJsonArray typeList = driver_obj["typeList"].toArray();

    for(int i = 0; i < typeList.size();i++)
    {
        QJsonObject  tempObj = typeList.at(i).toObject();
        QJsonValue  tempIndex = tempObj["index"].toInt();
        QJsonValue  tempDesc = tempObj["desc"].toString();
        ItemData  tempdata;
        tempdata.index = tempIndex.toInt();
        tempdata.describtion = tempDesc.toString();
        driverBrandData.dataList.push_back(tempdata);
    }

    return driverBrandData;
}

ConfigureData ModelBasicSettingWidget::readConfigure_AMBdrvierBrand(const QString &filePath)
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

    QJsonValue  mode_param_value= chassis_obj["mode_param"];
    QJsonObject mode_param_obj = mode_param_value.toObject();

    QJsonValue  driver_value = mode_param_obj["AMBdriverBrand"];
    QJsonObject driver_obj = driver_value.toObject();

    ConfigureData  driverBrandData;
    QJsonValue defaultIdex = driver_obj["defaultIndex"].toInt();
    driverBrandData.defaultidx = defaultIdex.toInt();
    QJsonArray typeList = driver_obj["typeList"].toArray();

    for(int i = 0; i < typeList.size();i++)
    {
        QJsonObject  tempObj = typeList.at(i).toObject();
        QJsonValue  tempIndex = tempObj["index"].toInt();
        QJsonValue  tempDesc = tempObj["desc"].toString();
        ItemData  tempdata;
        tempdata.index = tempIndex.toInt();
        tempdata.describtion = tempDesc.toString();
        driverBrandData.dataList.push_back(tempdata);
    }

    return driverBrandData;
}


ConfigureData ModelBasicSettingWidget::readConfigure_drvierType(const QString &filePath)
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

    QJsonValue  mode_param_value= chassis_obj["mode_param"];
    QJsonObject mode_param_obj = mode_param_value.toObject();

    QJsonValue  driver_value = mode_param_obj["driver"];
    QJsonObject driver_obj = driver_value.toObject();

    ConfigureData  driverData;
    QJsonValue defaultIdex = driver_obj["defaultIndex"].toInt();
    driverData.defaultidx = defaultIdex.toInt();
    QJsonArray typeList = driver_obj["typeList"].toArray();

    for(int i = 0; i < typeList.size();i++)
    {
        QJsonObject  tempObj = typeList.at(i).toObject();
        QJsonValue  tempIndex = tempObj["index"].toInt();
        QJsonValue  tempDesc = tempObj["desc"].toString();
        ItemData  tempdata;
        tempdata.index = tempIndex.toInt();
        tempdata.describtion = tempDesc.toString();
        driverData.dataList.push_back(tempdata);
    }

    return driverData;
}

ConfigureData ModelBasicSettingWidget::readConfigure_gyroType(const QString &filePath)
{
    return readMechineConfigureData("gyro",filePath);
}

ConfigureData ModelBasicSettingWidget::readConfigure_BatteryType(const QString &filePath)
{
    return readMechineConfigureData("batteryInfo",filePath);
}

ConfigureData ModelBasicSettingWidget::readConfigure_RFIDType(const QString &filePath)
{
    return readMechineConfigureData("RFID",filePath);
}

ConfigureData ModelBasicSettingWidget::readConfigure_MagneticSensorType(const QString &filePath)
{
    return readMechineConfigureData("magneticSensor",filePath);
}

ConfigureData ModelBasicSettingWidget::readConfigure_CameralType(const QString &filePath)
{
    return readMechineConfigureData("cameral",filePath);
}

ConfigureData ModelBasicSettingWidget::readConfigure_UltrasonicType(const QString &filePath)
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

    QJsonValue ultrasonic_value = chassis_obj.value("ultrasonic");
    QJsonObject ultrasonic_obj = ultrasonic_value.toObject();

    QJsonValue type_value = ultrasonic_obj["type"];
    QJsonObject type_obj = type_value.toObject();

    QJsonValue defindx = type_obj["defaultIndex"];

    QJsonArray typeList = type_obj["typeList"].toArray();

    ConfigureData  configData ;

    configData.defaultidx = defindx.toInt();

    for(int i = 0; i < typeList.size();i++)
    {

        QJsonObject tempObj = typeList.at(i).toObject();

        QJsonValue tempIdex = tempObj["index"];
        QJsonValue tempDesc = tempObj["desc"];

        ItemData  tempData;
        tempData.index = tempIdex.toInt();
        tempData.describtion = tempDesc.toString();
        configData.dataList.push_back(tempData);
    }

    return configData;
}

void ModelBasicSettingWidget::slot_driverType_change()
{
    //当驱动器类型为普通类型时，驱动器品牌不可选
    if(ui->ComboBox_driver_type->currentText()!="GeneralChassis")
    {
        ui->comboBox_driverBrand->setEnabled(false);
        model_base_data_.driver_ = 20;
    }
    else
    {
        ui->comboBox_driverBrand->setEnabled(true);
    }
}

void ModelBasicSettingWidget::slot_set_SpinboxtoolTips()
{
    this->getDINumSpinbox()->setToolTip(QString(tr("range:[%1,16][The number of DI already exists in the scenario,maxinum]")).arg(model_view_->model_scene()->get_Di_list()->size()+model_view_->model_scene()->get_crashDi_list()->size()));
    this->getUltrasonicNumSpinbox()->setToolTip(QString(tr("range:[%1,24][The number of Ulstronic already existes in the scenario,maxinum]")).arg(model_view_->model_scene()->get_ultrasonic_list()->size()));
    this->getDONumSpinbox()->setToolTip(QString(tr("range:[%1,16][The number of DO altready existes in the scenario,maxinum]")).arg(model_view_->model_scene()->get_Do_list()->size()));
}

void ModelBasicSettingWidget::slot_updateScrollBar()
{
    ui->scrollArea->verticalScrollBar()->setValue(ui->scrollArea->verticalScrollBar()->maximum());
}

void ModelBasicSettingWidget::on_lineEdit_model_name_textChanged(const QString &arg1)
{
    model_base_data_.model_name_=arg1;

    if(m_undoStack==NULL)//如果为空则直接返回此时m_unStack还没有有效值
    {
        return;
    }
    //undo redo
    ui->lineEdit_model_name->setNewValue(arg1);
    lineEditCommand* text_comand = new lineEditCommand(ui->lineEdit_model_name,ui->lineEdit_model_name->getOriValue(),
                                                       ui->lineEdit_model_name->getNewValue());
    m_undoStack->push(text_comand);
    ui->lineEdit_model_name->setOriValue(arg1);
}

void ModelBasicSettingWidget::on_DoubleSpinBox_reductionRatio_valueChanged(double arg1)
{
    model_base_data_.reduction_ratio_=arg1;
    if(m_undoStack==NULL)
    {
        return;
    }
    if(ui->DoubleSpinBox_reductionRatio->getIsClickedTrigger())
    {
        ui->DoubleSpinBox_reductionRatio->setNewValue(arg1);
        doubleSpinBoxCommand* reduceRate_command = new doubleSpinBoxCommand(ui->DoubleSpinBox_reductionRatio,NULL,
                                                                            ui->DoubleSpinBox_reductionRatio->getOriValue(),
                                                                            ui->DoubleSpinBox_reductionRatio->getNewValue(),this,NULL);
        m_undoStack->push(reduceRate_command);
        ui->DoubleSpinBox_reductionRatio->setOriValue(arg1);
    }

    ui->DoubleSpinBox_reductionRatio->setIsClickedTrigger(true);
}

void ModelBasicSettingWidget::on_SpinBox_encoderLine_valueChanged(int arg1)
{
    model_base_data_.encoder_line_=arg1;

    if(m_undoStack==NULL)
    {
        return;
    }
    //undo redo
    ui->SpinBox_encoderLine->setNewValue(arg1);
    if(ui->SpinBox_encoderLine->getIsClickedTrigger())
    {
        SpinBoxCommand* encoderLine_command = new SpinBoxCommand(ui->SpinBox_encoderLine,ui->SpinBox_encoderLine->getOriValue(),
                                                                 ui->SpinBox_encoderLine->getNewValue(),this,NULL);
        m_undoStack->push(encoderLine_command);
    }
    ui->SpinBox_encoderLine->setOriValue(arg1);
    ui->SpinBox_encoderLine->setIsClickedTrigger(true);
}

void ModelBasicSettingWidget::on_SpinBox_maxMotorRpm_valueChanged(int arg1)
{
    model_base_data_.max_motor_rpm_=arg1;

    if(m_undoStack==NULL)
    {
        return;
    }

    //undo redo

    ui->SpinBox_maxMotorRpm->setNewValue(arg1);
    if(ui->SpinBox_maxMotorRpm->getIsClickedTrigger())
    {
        SpinBoxCommand* motorRpm_command = new SpinBoxCommand(ui->SpinBox_maxMotorRpm,ui->SpinBox_maxMotorRpm->getOriValue(),
                                                              ui->SpinBox_maxMotorRpm->getNewValue(),this,NULL);
        m_undoStack->push(motorRpm_command);
    }
    ui->SpinBox_maxMotorRpm->setOriValue(arg1);
    ui->SpinBox_maxMotorRpm->setIsClickedTrigger(true);
}

void ModelBasicSettingWidget::on_checkBox_autoGyroCal_toggled(bool checked)
{
    model_base_data_.auto_gyro_cal_=checked;

    if(m_undoStack==NULL)//如果为空则直接返回此时m_unStack还没有有效值
    {
        return;
    }
    //undo redo
    ui->checkBox_autoGyroCal->setNewValue(checked);
    checkCommand* autoGyroCal_command = new checkCommand(ui->checkBox_autoGyroCal,
                                                         ui->checkBox_autoGyroCal->getOriValue(),
                                                         ui->checkBox_autoGyroCal->getNewValue(),this,NULL);
    m_undoStack->push(autoGyroCal_command);
    ui->checkBox_autoGyroCal->setOriValue(checked);
}

void ModelBasicSettingWidget::on_checkBox_brake_toggled(bool checked)
{
    model_base_data_.brake_=checked;
    if(m_undoStack==NULL)//如果为空则直接返回此时m_unStack还没有有效值
    {
        return;
    }
    //undo redo
    ui->checkBox_brake->setNewValue(checked);
    checkBrakeCommand* brake_command = new checkBrakeCommand(ui->checkBox_brake,ui->checkBox_autoBrake,
                                                             ui->checkBox_brake->getOriValue(),
                                                             ui->checkBox_brake->getNewValue(),this,NULL);
    m_undoStack->push(brake_command);
    ui->checkBox_brake->setOriValue(checked);
}

void ModelBasicSettingWidget::on_checkBox_autoBrake_toggled(bool checked)
{
    model_base_data_.auto_brake_=checked;

    if(m_undoStack==NULL)//如果为空则直接返回此时m_unStack还没有有效值
    {
        return;
    }
    //undo redo
    ui->checkBox_autoBrake->setNewValue(checked);
    checkCommand* autobrake_command = new checkCommand(ui->checkBox_autoBrake,ui->checkBox_autoBrake->getOriValue()
                                                       ,ui->checkBox_autoBrake->getNewValue(),this,NULL);
    m_undoStack->push(autobrake_command);
    ui->checkBox_autoBrake->setOriValue(checked);
}

void ModelBasicSettingWidget::on_checkBox_LED_toggled(bool checked)
{
    model_base_data_.LED_=checked;


    if(m_undoStack==NULL)//如果为空则直接返回此时m_unStack还没有有效值
    {
        return;
    }
    //undo redo
    ui->checkBox_LED->setNewValue(checked);
    checkCommand* led_command = new checkCommand(ui->checkBox_LED,ui->checkBox_LED->getOriValue(),ui->checkBox_LED->getNewValue(),this,NULL);
    m_undoStack->push(led_command);
    ui->checkBox_LED->setOriValue(checked);
}



void ModelBasicSettingWidget::on_combox_RFID_type_currentIndexChanged(int index)
{
    model_base_data_.RFID_=index;

    if(m_undoStack==NULL)//如果为空则直接返回此时m_unStack还没有有效值
    {
        return;
    }
    //undo redo
    ui->combox_RFID_type->setNew_index(index);
    comboxCommand* RFID_command = new comboxCommand(ui->combox_RFID_type,ui->combox_RFID_type->getOri_index(),
                                                    ui->combox_RFID_type->getNew_index(),NULL);
    m_undoStack->push(RFID_command);
    ui->combox_RFID_type->setOri_index(index);
}

void ModelBasicSettingWidget::on_spinBox_DI_valueChanged(int arg1)
{

    model_base_data_.max_di_num_=arg1;
    emit sig_max_num_changed(0,arg1);
    //undo redo
    if(m_undoStack==NULL)
    {
        return;
    }

    if(ui->spinBox_DI->getIsClickedTrigger())//当时鼠标触发使控件值发生变化时进行压栈
    {
        ui->spinBox_DI->setNewValue(arg1);
        SpinBoxCommand* di_command = new SpinBoxCommand(ui->spinBox_DI,
                                                        ui->spinBox_DI->getOriValue(),
                                                        ui->spinBox_DI->getNewValue(),this,NULL);
        m_undoStack->push(di_command);
        ui->spinBox_DI->setOriValue(arg1);
    }
    ui->spinBox_DI->setIsClickedTrigger(true);
    //判断场景中DI数量若已达到当前最大数量，只能增加不能减少
    int diNum = model_view_->model_scene()->get_Di_list()->size()+model_view_->model_scene()->get_crashDi_list()->size();
    ui->spinBox_DI->setMinimum(diNum);
}

void ModelBasicSettingWidget::on_spinBox_DO_valueChanged(int arg1)
{

    model_base_data_.max_do_num_=arg1;
    emit sig_max_num_changed(3,arg1);
    update_do_size(arg1,widget_do_list_.size());

    if(m_undoStack==NULL)
    {
        return;
    }
    //undo redo

    ui->spinBox_DO->setNewValue(arg1);
    if(ui->spinBox_DO->getIsClickedTrigger())
    {
        SpinBoxCommand* do_command = new SpinBoxCommand(ui->spinBox_DO,ui->spinBox_DO->getOriValue(),ui->spinBox_DO->getNewValue(),this,NULL);
        m_undoStack->push(do_command);
    }
    ui->spinBox_DO->setOriValue(arg1);
    ui->spinBox_DO->setIsClickedTrigger(true);
    //判断场景中DI数量若已达到当前最大数量，只能增加不能减少
    int DoNum = model_view_->model_scene()->get_Do_list()->size();
    ui->spinBox_DO->setMinimum(DoNum);

    timer.setInterval(100);
    timer.setSingleShot(true);
    connect(&timer,SIGNAL(timeout()),this,SLOT(slot_updateScrollBar()));
    timer.start();

}

void ModelBasicSettingWidget::on_spinBox_ultrasonic_valueChanged(int arg1)
{

    model_base_data_.max_ultrasonic_num_=arg1;
    emit sig_max_num_changed(1,arg1);

    if(m_undoStack==NULL)
    {
        return;
    }
    //undo redo

    ui->spinBox_ultrasonic->setNewValue(arg1);
    if(ui->spinBox_ultrasonic->getIsClickedTrigger())
    {
        SpinBoxCommand* ultrasonicMaxNum_command = new SpinBoxCommand(ui->spinBox_ultrasonic,ui->spinBox_ultrasonic->getOriValue(),
                                                                      ui->spinBox_ultrasonic->getNewValue(),this,NULL);
        m_undoStack->push(ultrasonicMaxNum_command);
    }
    ui->spinBox_ultrasonic->setOriValue(arg1);
    ui->spinBox_ultrasonic->setIsClickedTrigger(true);

    //判断场景中DI数量若已达到当前最大数量，只能增加不能减少

    int ultrasionicNum = model_view_->model_scene()->get_ultrasonic_list()->size();
    ui->spinBox_ultrasonic->setMinimum(ultrasionicNum);

}

void ModelBasicSettingWidget::on_comboBox_ultrasonic_type_currentIndexChanged(int index)
{
    model_base_data_.ultrasonic_=index;

    if(m_undoStack==NULL)//如果为空则直接返回此时m_unStack还没有有效值
    {
        return;
    }
    //undo redo
    ui->comboBox_ultrasonic_type->setNew_index(ui->comboBox_ultrasonic_type->currentIndex());
    comboxCommand* ultrasonic_command = new comboxCommand(ui->comboBox_ultrasonic_type,ui->comboBox_ultrasonic_type->getOri_index()
                                                          ,ui->comboBox_ultrasonic_type->getNew_index(),NULL);
    m_undoStack->push(ultrasonic_command);
    ui->comboBox_ultrasonic_type->setOri_index(ui->comboBox_ultrasonic_type->currentIndex());
}

void ModelBasicSettingWidget::update_panel_infor()
{
    if(getForkWidget()->getIsConfigureFork())
    {
        ui->checkBox_forkConfigure->blockSignals(true);
        ui->checkBox_forkConfigure->setChecked(true);
        ui->checkBox_forkConfigure->setOriValue(true);
        forkWid_->show();
        ui->checkBox_forkConfigure->blockSignals(false);
        forkWid_->initWidgetData();
    }
    else
    {
        ui->checkBox_forkConfigure->blockSignals(true);
        ui->checkBox_forkConfigure->setChecked(false);
        ui->checkBox_forkConfigure->setOriValue(false);
        forkWid_->hide();
        ui->checkBox_forkConfigure->blockSignals(false);
    }

    ui->lineEdit_model_name->blockSignals(true);
    ui->lineEdit_model_name->setText (model_base_data_.model_name_);
    ui->lineEdit_model_name->setOriValue(model_base_data_.model_name_);
    ui->lineEdit_model_name->blockSignals(false);
    ui->DoubleSpinBox_reductionRatio->blockSignals(true);
    ui->DoubleSpinBox_reductionRatio->setValue (model_base_data_.reduction_ratio_);
    ui->DoubleSpinBox_reductionRatio->setOriValue(model_base_data_.reduction_ratio_);
    ui->DoubleSpinBox_reductionRatio->blockSignals(false);
    ui->SpinBox_encoderLine->blockSignals(true);
    ui->SpinBox_encoderLine->setValue (model_base_data_.encoder_line_);
    ui->SpinBox_encoderLine->setOriValue(model_base_data_.encoder_line_);
    ui->SpinBox_encoderLine->blockSignals(false);
    ui->SpinBox_maxMotorRpm->blockSignals(true);
    ui->SpinBox_maxMotorRpm->setValue (model_base_data_.max_motor_rpm_);
    ui->SpinBox_maxMotorRpm->setOriValue (model_base_data_.max_motor_rpm_);
    ui->SpinBox_maxMotorRpm->blockSignals(false);
    adjust_combox_items(model_base_data_.driver_,ui->ComboBox_driver_type);//
    adjust_combox_items(model_base_data_.driverBrand_,ui->comboBox_driverBrand);
    adjust_combox_items(model_base_data_.gyro_type_,ui->comboBox_gyro);//
    ui->checkBox_autoGyroCal->blockSignals(true);
    ui->checkBox_autoGyroCal->setChecked (model_base_data_.auto_gyro_cal_);
    ui->checkBox_autoGyroCal->setOriValue(model_base_data_.auto_gyro_cal_);
    ui->checkBox_autoGyroCal->blockSignals(false);
    ui->checkBox_brake->blockSignals(true);
    ui->checkBox_brake->setChecked (model_base_data_.brake_);
    ui->checkBox_brake->setOriValue(model_base_data_.brake_);
    ui->checkBox_brake->blockSignals(false);
    ui->checkBox_autoBrake->blockSignals(true);
    ui->checkBox_autoBrake->setChecked (model_base_data_.auto_brake_);
    ui->checkBox_autoBrake->setOriValue (model_base_data_.auto_brake_);
    ui->checkBox_autoBrake->blockSignals(false);
    model_base_data_.brake_?ui->checkBox_autoBrake->setEnabled(true):ui->checkBox_autoBrake->setEnabled(false);
    ui->checkBox_LED->blockSignals(true);
    ui->checkBox_LED->setChecked (model_base_data_.LED_);
    ui->checkBox_LED->setOriValue (model_base_data_.LED_);
    ui->checkBox_LED->blockSignals(false);
    adjust_combox_items(model_base_data_.battery_info_,ui->comboBox_batteryInfo_type);
    adjust_combox_items(model_base_data_.RFID_,ui->combox_RFID_type);
    adjust_combox_items(model_base_data_.magnetic_sensor_,ui->comboBox_magneticSensor_type);
    ui->spinBox_DI->setValue (model_base_data_.max_di_num_);
    ui->spinBox_DI->setOriValue (model_base_data_.max_di_num_);
    ui->spinBox_DO->setValue (model_base_data_.max_do_num_);
    ui->spinBox_DO->setOriValue (model_base_data_.max_do_num_);
    ui->spinBox_ultrasonic->setValue (model_base_data_.max_ultrasonic_num_);
    ui->spinBox_ultrasonic->setOriValue (model_base_data_.max_ultrasonic_num_);
    adjust_combox_items(model_base_data_.ultrasonic_,ui->comboBox_ultrasonic_type);
    update_do_size(model_base_data_.max_do_num_,widget_do_list_.size());
}

void ModelBasicSettingWidget::update_do_size(quint8 do_size, quint8 c_do_size)
{
    if(c_do_size<do_size)
    {
        for(int tt=0;tt<c_do_size;tt++)
        {
            if(widget_do_list_.at(tt)->isHidden())
                widget_do_list_.at(tt)->setVisible(true);
        }
        for(int tm=0;tm<do_size-c_do_size;tm++)
        {
            WidgetDOState *wdos=new WidgetDOState(this);
            wdos->set_id_text(QString::number(tm+c_do_size));
            wdos->setValue(0);
            widget_do_list_.append(wdos);
            ui->gridLayout_DO->addWidget(wdos,tm/4,tm%4,1,1);
        }
    }
    else
    {
        for(int tt=0;tt<c_do_size;tt++)
        {
            if(tt<do_size)
            {
                if(widget_do_list_.at(tt)->isHidden())
                    widget_do_list_.at(tt)->setVisible(true);
            }
            else
            {
                if(!widget_do_list_.at(tt)->isHidden())
                    widget_do_list_.at(tt)->setVisible(false);
            }
        }
    }
}

QList<quint8> ModelBasicSettingWidget::get_do_info_list()
{
    QList<quint8> d_list;
    for(int ti=0;ti<widget_do_list_.size();ti++)
    {
        if(!widget_do_list_.at(ti)->isHidden())
        {
            d_list.append(widget_do_list_.at(ti)->getValue());
        }
    }
    return d_list;
}

QList<WidgetDOState *> *ModelBasicSettingWidget::get_do_state_list()
{
    return &widget_do_list_;
}

void ModelBasicSettingWidget::set_do_info_list(QList<quint8> dil)
{
    int size = qMin(widget_do_list_.size(),dil.size());
    for(int ti=0;ti<size;ti++)
    {
        if(!widget_do_list_.at(ti)->isHidden())
        {
            widget_do_list_.at(ti)->blockSignals(true);
            widget_do_list_.at(ti)->setValue(dil.at(ti));
            widget_do_list_.at(ti)->blockSignals(false);
        }
    }
}

void ModelBasicSettingWidget::set_di_minNum(int num)
{
    ui->spinBox_DI->setMinimum(num);
}

void ModelBasicSettingWidget::set_do_minNum(int num)
{
    ui->spinBox_DO->setMinimum(num);
}

void ModelBasicSettingWidget::set_ultrasonic_minNum(int num)
{
    ui->spinBox_ultrasonic->setMinimum(num);
}

customLineEdit *ModelBasicSettingWidget::getModelName()
{
    return  ui->lineEdit_model_name;
}

customDoubleSpinBox *ModelBasicSettingWidget::getReduceRadio()
{
    return ui->DoubleSpinBox_reductionRatio;
}

customSpinBox *ModelBasicSettingWidget::getEncoderLine()
{
    return ui->SpinBox_encoderLine;
}

customSpinBox *ModelBasicSettingWidget::getMaxRpm()
{
    return ui->SpinBox_maxMotorRpm;
}

customComboBox *ModelBasicSettingWidget::getDrive()
{
    return  ui->ComboBox_driver_type;
}

customComboBox *ModelBasicSettingWidget::getDriveBrand()
{
    return ui->comboBox_driverBrand;
}

customComboBox *ModelBasicSettingWidget::getBattery()
{
    return ui->comboBox_batteryInfo_type;
}

customComboBox *ModelBasicSettingWidget::getGyro()
{
    return ui->comboBox_gyro;
}

customComboBox *ModelBasicSettingWidget::getRFID()
{
    return ui->combox_RFID_type;
}

customCheckBox *ModelBasicSettingWidget::getBrake()
{
    return ui->checkBox_brake;
}

customCheckBox *ModelBasicSettingWidget::getAutoBrake()
{
    return ui->checkBox_autoBrake;
}

customCheckBox *ModelBasicSettingWidget::getLed()
{
    return ui->checkBox_LED;
}

customCheckBox *ModelBasicSettingWidget::getAutoGyroCal()
{
    return ui->checkBox_autoGyroCal;
}

customComboBox *ModelBasicSettingWidget::getUltrasonicType()
{
    return ui->comboBox_ultrasonic_type;
}

customComboBox *ModelBasicSettingWidget::getMagneticSensorType()
{
    return ui->comboBox_magneticSensor_type;
}

customSpinBox *ModelBasicSettingWidget::getDINumSpinbox()
{
    return  ui->spinBox_DI;
}

customSpinBox *ModelBasicSettingWidget::getDONumSpinbox()
{
    return ui->spinBox_DO;
}

customSpinBox *ModelBasicSettingWidget::getUltrasonicNumSpinbox()
{
    return ui->spinBox_ultrasonic;
}

QScrollBar *ModelBasicSettingWidget::getVScrollAreaBar()
{
    return ui->scrollArea->verticalScrollBar();
}

QWidget *ModelBasicSettingWidget::getScrollAreaContent()
{
    return ui->scrollAreaWidgetContents;
}

WidgetForkSetting *ModelBasicSettingWidget::getForkWidget()
{
    return forkWid_;
}

bool ModelBasicSettingWidget::gestureEvent(QGestureEvent *event)
{
    if (QGesture *pan = event->gesture(Qt::PanGesture))
        panTriggered(static_cast<QPanGesture *>(pan));
    if (QGesture *pinch = event->gesture(Qt::PinchGesture))
        pinchTriggered(static_cast<QPinchGesture *>(pinch));
    if (QGesture *Swip = event->gesture(Qt::SwipeGesture))
        SwipeTriggered(static_cast<QSwipeGesture *>(Swip));
    return true;
}

void ModelBasicSettingWidget::panTriggered(QPanGesture *gesture)
{
#ifdef QT_NO_CURSOR
    switch (gesture->state()) {
    case Qt::GestureStarted:
    case Qt::GestureUpdated:
        setCursor(Qt::SizeAllCursor);
        break;
    default:
        setCursor(Qt::ArrowCursor);
    }
#endif
    qDebug()<<"emit sig offset 1---panTriggered";
}

void ModelBasicSettingWidget::pinchTriggered(QPinchGesture *gesture)
{
    qDebug()<<"emit sig offset  2----pinchTriggered";
}

void ModelBasicSettingWidget::SwipeTriggered(QSwipeGesture *gesture)
{
    qDebug()<<"emit sig offset  3-----SwipeTriggered";
}

void ModelBasicSettingWidget::mouseMoveEvent(QMouseEvent *event)
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

void ModelBasicSettingWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        m_lastMousePos = event->pos();
}

bool ModelBasicSettingWidget::event(QEvent *event)
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
void ModelBasicSettingWidget::on_ComboBox_driver_type_currentIndexChanged(int index)
{
    model_base_data_.driver_ = 20;
    //    //当驱动器类型为普通类型时，驱动器品牌不可选
    //    if(ui->ComboBox_driver_type->currentText()!="GeneralChassis")
    //    {
    //        ui->comboBox_driverBrand->setEnabled(false);
    //        model_base_data_.driver_ = 20;
    //    }
    //    else
    //    {
    //        ui->comboBox_driverBrand->setEnabled(true);
    //        model_base_data_.driver_ = ui->ComboBox_driver_type->currentData().toUInt();
    //    }


    //    if(m_undoStack==NULL)//如果为空则直接返回此时m_unStack还没有有效值
    //    {
    //        m_undoStack = UndoStack::InitStack();
    //    }
    //    //undo redo
    //    ui->ComboBox_driver_type->setNew_index(ui->ComboBox_driver_type->currentIndex());
    //    comboxCommand * driver_command = new comboxCommand(ui->ComboBox_driver_type,
    //                                                       ui->ComboBox_driver_type->getOri_index(),
    //                                                       ui->ComboBox_driver_type->getNew_index(),NULL);
    //    m_undoStack->push(driver_command);
    //    ui->ComboBox_driver_type->setOri_index(ui->ComboBox_driver_type->currentIndex());
}

void ModelBasicSettingWidget::on_comboBox_batteryInfo_type_currentIndexChanged(int index)
{
    //    model_base_data_.battery_info_=index;
    model_base_data_.battery_info_=ui->comboBox_batteryInfo_type->currentData().toInt();
    if(m_undoStack==NULL)//如果为空则直接返回此时m_unStack还没有有效值
    {
        return;
    }
    //undo redo
    ui->comboBox_batteryInfo_type->setNew_index(ui->comboBox_batteryInfo_type->currentIndex());
    comboxCommand* battery_command = new comboxCommand(ui->comboBox_batteryInfo_type,
                                                       ui->comboBox_batteryInfo_type->getOri_index(),
                                                       ui->comboBox_batteryInfo_type->getNew_index(),NULL);
    m_undoStack->push(battery_command);
    ui->comboBox_batteryInfo_type->setOri_index(ui->comboBox_batteryInfo_type->currentIndex());
}

void ModelBasicSettingWidget::on_comboBox_magneticSensor_type_currentIndexChanged(int index)
{
    model_base_data_.magnetic_sensor_=ui->comboBox_magneticSensor_type->currentData().toInt();
    if(m_undoStack==NULL)//如果为空则直接返回此时m_unStack还没有有效值
    {
        return;
    }
    //undo redo
    ui->comboBox_magneticSensor_type->setNew_index(index);
    comboxCommand* magneticSensor_command = new comboxCommand(ui->comboBox_magneticSensor_type,
                                                              ui->comboBox_magneticSensor_type->getOri_index(),
                                                              ui->comboBox_magneticSensor_type->getNew_index(),NULL);
    m_undoStack->push(magneticSensor_command);
    ui->comboBox_magneticSensor_type->setOri_index(index);
}


void ModelBasicSettingWidget::on_comboBox_gyro_currentIndexChanged(int index)
{
    model_base_data_.gyro_type_ = ui->comboBox_gyro->currentData().toInt();
    qDebug()<<"basicData_.gyro_type_ --------------is----2----------"<<model_base_data_.gyro_type_;
    if(m_undoStack==NULL)//如果为空则直接返回此时m_unStack还没有有效值
    {
        return;
    }
    //undo redo
    ui->comboBox_gyro->setNew_index(ui->comboBox_gyro->currentIndex());
    comboxCommand* gyro_command = new comboxCommand(ui->comboBox_gyro,
                                                    ui->comboBox_gyro->getOri_index(),
                                                    ui->comboBox_gyro->getNew_index(),NULL);
    m_undoStack->push(gyro_command);

    ui->comboBox_gyro->setOri_index(ui->comboBox_gyro->currentIndex());
}

void ModelBasicSettingWidget::on_comboBox_driverBrand_currentIndexChanged(int index)
{
    model_base_data_.driverBrand_ = ui->comboBox_driverBrand->currentText();
    if(m_undoStack==NULL)//如果为空则直接返回此时m_unStack还没有有效值
    {
        return;
    }
    //undo redo
    ui->comboBox_driverBrand->setNew_index(index);
    comboxCommand* driverBrand_command = new comboxCommand(ui->comboBox_driverBrand,
                                                           ui->comboBox_driverBrand->getOri_index(),
                                                           ui->comboBox_driverBrand->getNew_index(),NULL);
    m_undoStack->push(driverBrand_command);
    ui->comboBox_driverBrand->setOri_index(index);
}

void ModelBasicSettingWidget::on_checkBox_forkConfigure_clicked(bool checked)
{
    if(m_undoStack==NULL)
    {
        return;
    }
    ui->checkBox_forkConfigure->setNewValue(checked);
    checkForkCommand* tempCmd = new checkForkCommand(ui->checkBox_forkConfigure,forkWid_,
                                                     ui->checkBox_forkConfigure->getOriValue(),
                                                     ui->checkBox_forkConfigure->getNewValue(),
                                                     this,NULL);
    m_undoStack->push(tempCmd);
    ui->checkBox_forkConfigure->setOriValue(checked);
}
