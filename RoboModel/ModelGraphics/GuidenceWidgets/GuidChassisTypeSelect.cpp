#include "GuidChassisTypeSelect.h"
#include "ui_GuidChassisTypeSelect.h"
#include"GuidChassisShape.h"
#include"StyleObject.h"
GuidChassisTypeSelect::GuidChassisTypeSelect(GuidChassisShape* shpe, QWidget *parent) :
    GuidBaseSetting(parent),
    ui(new Ui::GuidChassisTypeSelect),
    shapeWid_(shpe)
{
    ui->setupUi(this);
    setType(GUID_CHASSIS_TYPE);
    ui->doubleSpinBox_wheelRadius->setSingleStep(0.01);
    ui->doubleSpinBox_Aaxis->setSingleStep(0.01);
    ui->doubleSpinBox_Baxis->setSingleStep(0.01);
    //下一步是否可用判断
    checkIfNeedEnableByFuncType();
    initWidgetData();
    enableNextStep();
    //最后设置初始化差动两轮的a轴，由于肉搏Data_类型时union类型所以之前的a_axis_distance 初始值会被覆盖掉
//    roboData_.chasisModel.DiffTwoWheel.a_axis_distance=20;

    imagePath_ = StyleObject::init()->getCurrentSkinDirPath()+"/images/"+"/RoModelPlugin/RoboChassis/";

    if(shapeWid_->getShape()==RECTANGLE)
    {
        ui->label_image->setPixmap(QString("%1").arg(imagePath_+"DifTwo"));
        slot_shape_currentIndexChanged(RECTANGLE);
    }
    else if(shapeWid_->getShape()==CIRCLE)
    {
        ui->label_image->setPixmap(QString("%1").arg(imagePath_+"CircleDifTwo"));
        slot_shape_currentIndexChanged(CIRCLE);
    }
//    ui->groupBox_chassisParameter->setFixedWidth(240);
}

GuidChassisTypeSelect::~GuidChassisTypeSelect()
{
    delete ui;
}

void GuidChassisTypeSelect::initWidgetData()
{
    //初始化数据

    roboData_.shapType_  = shapeWid_->robotData_.shapType_;

    roboData_.head_ = shapeWid_->robotData_.head_;
    roboData_.tail_ = shapeWid_->robotData_.tail_;
    roboData_.width_ = shapeWid_->robotData_.width_;
    roboData_.radius_ = shapeWid_->robotData_.radius_;

    roboData_.wheelRadius_ = 0;
    roboData_.shapType_=RECTANGLE;
    roboData_.chasisType_=DIFFERENTIAL_TWOWHEEL;
    roboData_.mvStyle_ =DIFFERENTIAL;

    ui->doubleSpinBox_wheelRadius->setValue(roboData_.wheelRadius_/100.);
    ui->doubleSpinBox_Aaxis->setValue(0/100.);
    ui->doubleSpinBox_Baxis->setValue(0/100.);
    ui->doubleSpinBox_steerMaxAngel->setValue(90);
    ui->doubleSpinBox_steerMinAngel->setValue(-90);
    ui->doubleSpinBox_encoderMin->setValue(0);
    ui->doubleSpinBox_encoderMax->setValue(1800);
    ConfigureData data = readAbsEncoderTypeFile(CONFIGFILE_PATH);

    QList<ItemData>::iterator iter = data.dataList.begin();

    for(;iter!= data.dataList.end();iter++)
    {
        ui->comboBox_encoderType->addItem((*iter).describtion,(*iter).index);
    }

    int idx = ui->comboBox_funcType->findData(data.defaultidx);

    ui->comboBox_encoderType->setCurrentIndex(idx);

}

ConfigureData GuidChassisTypeSelect::readAbsEncoderTypeFile(const QString &filePath)
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

void GuidChassisTypeSelect::enableNextStep()
{
    if(AaxisFlag)
    {
        if(ui->doubleSpinBox_Aaxis->value()==0)
        {
            emit sig_enableNextStep(false);
            return;
        }
    }

    if(BaxisFlag)
    {
        if(ui->doubleSpinBox_Baxis->value()==0)
        {
            emit sig_enableNextStep(false);
            return;
        }
    }

    if(ui->doubleSpinBox_wheelRadius->value() == 0)
    {
        emit sig_enableNextStep(false);
        return;
    }
    emit sig_enableNextStep(true);
    return;
}

void GuidChassisTypeSelect::checkIfNeedEnableByFuncType()
{
    ChassisType chasType = (ChassisType)ui->comboBox_funcType->currentData(Qt::UserRole).toInt();
    switch (chasType)
    {
    case DIFFERENTIAL_TWOWHEEL://两轮差动
    {
        AaxisFlag = true   ;
        BaxisFlag = false  ;
        angleFlag = false ;
    }
        break;
    case DIFFERENTIAL_FOURWHEEL://四轮差动
    {
        AaxisFlag = true   ;
        BaxisFlag = true  ;
        angleFlag = false ;
    }
        break;
    case ALLDIRECTION_THREEWHEEL://全向三轮
    {
        AaxisFlag = true   ;
        BaxisFlag = false  ;
        angleFlag = true ;
    }
        break;
    case MECANUM_FOURWHEEL://四轮麦克纳姆
    {
        AaxisFlag = true;
        BaxisFlag = true;
        angleFlag = false;
    }
        break;
    case SINGLESTEERWHEEL_FRONTDRIVER_FRONT_CODERTYPE://前驱+前转动编码器 单舵轮
    case SINGLESTEERWHEEL_FRONTDRIVER_BACK_CODERTYPE://前驱+后转动编码器
    case SINGLESTEERWHEEL_BACKDRIVER_FRONT_CODERTYPE://后驱+前转动编码器
    case SINGLESTEERWHEEL_BACKDRIVER_BACK_CODERTYPE://后驱+后转动编码器
    {
        AaxisFlag = true;
        BaxisFlag = true;
        angleFlag = false;
    }
        break;
    }
}

void GuidChassisTypeSelect::updateData()
{
    roboData_.shapType_  = shapeWid_->robotData_.shapType_;
    roboData_.head_ = shapeWid_->robotData_.head_;
    roboData_.tail_ = shapeWid_->robotData_.tail_;
    roboData_.width_ = shapeWid_->robotData_.width_;
    roboData_.radius_ = shapeWid_->robotData_.radius_;
}

void GuidChassisTypeSelect::slot_shape_currentIndexChanged(ShapeType shape)
{
    if(shape==RECTANGLE)
    {
        ui->comboBox_moveStyle->clear();
        ui->comboBox_moveStyle->addItem(tr("Differential"),DIFFERENTIAL);
        ui->comboBox_moveStyle->addItem(tr("Omnidirectional"),ALLDIRECTION);
        ui->comboBox_moveStyle->addItem(tr("Mecanum"),MECANUM);
        ui->comboBox_moveStyle->addItem(tr("Single rudder"),SINGLESTEERWHEEL);
    }
    else if(shape == CIRCLE)
    {
        ui->comboBox_moveStyle->clear();
        ui->comboBox_moveStyle->addItem(tr("Differential"),DIFFERENTIAL);
        ui->comboBox_moveStyle->addItem(tr("Omnidirectional"),ALLDIRECTION);
    }
}

void GuidChassisTypeSelect::on_comboBox_moveStyle_currentIndexChanged(int index)
{
    MoveStyle mvstyle = (MoveStyle)ui->comboBox_moveStyle->currentData(Qt::UserRole).toInt();
    roboData_.mvStyle_ = mvstyle;
    switch (mvstyle)
    {
    case DIFFERENTIAL:
        ui->comboBox_funcType->clear();
        ui->comboBox_funcType->addItem(tr("Two wheel differential"),DIFFERENTIAL_TWOWHEEL);
        ui->comboBox_funcType->addItem(tr("Four wheel differential"),DIFFERENTIAL_FOURWHEEL);
        break;
    case ALLDIRECTION:
        ui->comboBox_funcType->clear();
        ui->comboBox_funcType->addItem(tr("Three wheel omni"),ALLDIRECTION_THREEWHEEL);
        break;
    case MECANUM:
        ui->comboBox_funcType->clear();
        ui->comboBox_funcType->addItem(tr("Four wheel Mecanum"),MECANUM_FOURWHEEL);
        break;
    case SINGLESTEERWHEEL:
        ui->comboBox_funcType->clear();
        ui->comboBox_funcType->addItem(tr("Single rudder front drive and front encoder"),SINGLESTEERWHEEL_FRONTDRIVER_FRONT_CODERTYPE);
        ui->comboBox_funcType->addItem(tr("Single rudder front dirve and rear encoder"),SINGLESTEERWHEEL_FRONTDRIVER_BACK_CODERTYPE);
        ui->comboBox_funcType->addItem(tr("Single rudder rear drive and front encoder"),SINGLESTEERWHEEL_BACKDRIVER_FRONT_CODERTYPE);
        ui->comboBox_funcType->addItem(tr("Single rudder rear drive and rear encoder"),SINGLESTEERWHEEL_BACKDRIVER_BACK_CODERTYPE);
        break;
    default:
        break;
    }
}

void GuidChassisTypeSelect::on_comboBox_funcType_currentIndexChanged(int index)
{

    ChassisType chasType = (ChassisType)ui->comboBox_funcType->currentData(Qt::UserRole).toInt();
    roboData_.chasisType_ = chasType;
    switch (chasType)
    {
    case DIFFERENTIAL_TWOWHEEL://两轮差动
    {
        roboData_.chasisModel.DiffTwoWheel.a_axis_distance = 0;
        ui->doubleSpinBox_Aaxis->setValue(0/100.);

        ui->label_Aaxis->show();
        ui->doubleSpinBox_Aaxis->show();
        ui->label_Baxis->hide();
        ui->doubleSpinBox_Baxis->hide();
        ui->label_encoderRange->hide();
        ui->doubleSpinBox_encoderMax->hide();
        ui->doubleSpinBox_encoderMin->hide();
        ui->label_EncoderType->hide();
        ui->comboBox_encoderType->hide();
        ui->label_steerAngelRange->hide();
        ui->doubleSpinBox_steerMaxAngel->hide();
        ui->doubleSpinBox_steerMinAngel->hide();
        ui->label_theta->hide();
        ui->doubleSpinBox_theta->hide();
        ui->label_encoderRange_max->hide();
        ui->label_steerAngelRange_max->hide();

        //--------------------------------------------
        if(shapeWid_->getShape()==RECTANGLE)
        {
            ui->label_image->setPixmap(QString("%1").arg(imagePath_+"DifTwo"));
        }
        else if(shapeWid_->getShape()==CIRCLE)
        {
            ui->label_image->setPixmap(QString("%1").arg(imagePath_+"CircleDifTwo"));
        }
    }
        break;
    case DIFFERENTIAL_FOURWHEEL://四轮差动
    {
        roboData_.chasisModel.DiffFourWheel.a_axis_distance = 0;
        roboData_.chasisModel.DiffFourWheel.b_axis_distance = 0;
        ui->doubleSpinBox_Aaxis->setValue(0/100.);
        ui->doubleSpinBox_Baxis->setValue(0/100.);


        ui->label_Aaxis->show();
        ui->doubleSpinBox_Aaxis->show();
        ui->label_Baxis->show();
        ui->doubleSpinBox_Baxis->show();
        ui->label_encoderRange->hide();
        ui->doubleSpinBox_encoderMax->hide();
        ui->doubleSpinBox_encoderMin->hide();
        ui->label_EncoderType->hide();
        ui->comboBox_encoderType->hide();
        ui->label_steerAngelRange->hide();
        ui->doubleSpinBox_steerMaxAngel->hide();
        ui->doubleSpinBox_steerMinAngel->hide();
        ui->label_encoderRange_max->hide();
        ui->label_steerAngelRange_max->hide();
        ui->label_theta->hide();
        ui->doubleSpinBox_theta->hide();
        //--------------------------------------------
        if(shapeWid_->getShape()==RECTANGLE)
        {
            ui->label_image->setPixmap(QString("%1").arg(imagePath_+"DifFour"));
        }
        else if(shapeWid_->getShape()==CIRCLE)
        {
            ui->label_image->setPixmap(QString("%1").arg(imagePath_+"CircleDifFour"));
        }
    }
        break;
    case ALLDIRECTION_THREEWHEEL://全向三轮
    {
        roboData_.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle = 0;
        roboData_.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance = 0;
        ui->doubleSpinBox_theta->setValue(0);
        ui->doubleSpinBox_Aaxis->setValue(0/100.);

        ui->label_Aaxis->show();
        ui->doubleSpinBox_Aaxis->show();
        ui->label_Baxis->hide();
        ui->doubleSpinBox_Baxis->hide();
        ui->label_encoderRange->hide();
        ui->doubleSpinBox_encoderMax->hide();
        ui->doubleSpinBox_encoderMin->hide();
        ui->label_EncoderType->hide();
        ui->comboBox_encoderType->hide();
        ui->label_steerAngelRange->hide();
        ui->doubleSpinBox_steerMaxAngel->hide();
        ui->doubleSpinBox_steerMinAngel->hide();
        ui->label_encoderRange_max->hide();
        ui->label_steerAngelRange_max->hide();
        ui->label_theta->show();
        ui->doubleSpinBox_theta->show();
        //--------------------------------------------
        if(shapeWid_->getShape()==RECTANGLE)
        {
            ui->label_image->setPixmap(QString("%1").arg(imagePath_+"AllDirectThree"));
        }
        else if(shapeWid_->getShape()==CIRCLE)
        {
            ui->label_image->setPixmap(QString("%1").arg(imagePath_+"CircleAllDirectThree"));
        }
    }
        break;
    case MECANUM_FOURWHEEL://四轮麦克纳姆
    {
        roboData_.chasisModel.MecanumFourWheel.a_axis_distance = 0;
        roboData_.chasisModel.MecanumFourWheel.b_axis_distance = 0;
        ui->doubleSpinBox_Aaxis->setValue(0/100.);
        ui->doubleSpinBox_Baxis->setValue(0/100.);


        ui->label_Aaxis->show();
        ui->doubleSpinBox_Aaxis->show();
        ui->label_Baxis->show();
        ui->doubleSpinBox_Baxis->show();
        ui->label_encoderRange->hide();
        ui->doubleSpinBox_encoderMax->hide();
        ui->doubleSpinBox_encoderMin->hide();
        ui->label_encoderRange_max->hide();
        ui->label_steerAngelRange_max->hide();
        ui->label_EncoderType->hide();
        ui->comboBox_encoderType->hide();
        ui->label_steerAngelRange->hide();
        ui->doubleSpinBox_steerMaxAngel->hide();
        ui->doubleSpinBox_steerMinAngel->hide();
        ui->label_theta->hide();
        ui->doubleSpinBox_theta->hide();
        //--------------------------------------------
        if(shapeWid_->getShape()==RECTANGLE)
        {
            ui->label_image->setPixmap(QString("%1").arg(imagePath_+"MecNumFourWheel"));
        }
        else if(shapeWid_->getShape()==CIRCLE)
        {
        }

    }
        break;
    case SINGLESTEERWHEEL_FRONTDRIVER_FRONT_CODERTYPE://前驱+前转动编码器 单舵轮
    case SINGLESTEERWHEEL_FRONTDRIVER_BACK_CODERTYPE://前驱+后转动编码器
    case SINGLESTEERWHEEL_BACKDRIVER_FRONT_CODERTYPE://后驱+前转动编码器
    case SINGLESTEERWHEEL_BACKDRIVER_BACK_CODERTYPE://后驱+后转动编码器
    {
        roboData_.chasisModel.SingleWheelDriver.a_axis_distance = 0;
        roboData_.chasisModel.SingleWheelDriver.b_axis_distance = 0;
        roboData_.chasisModel.SingleWheelDriver.absEncoderRange_max = 90;
        roboData_.chasisModel.SingleWheelDriver.absEncoderRange_min = -90;
        roboData_.chasisModel.SingleWheelDriver.absEncoderType = 0;
        roboData_.chasisModel.SingleWheelDriver.steer_angle_max = 1800;
        roboData_.chasisModel.SingleWheelDriver.steer_angle_min = 0;

        ui->doubleSpinBox_Aaxis->setValue(0/100.);
        ui->doubleSpinBox_Baxis->setValue(0/100.);
        ui->doubleSpinBox_encoderMax->setValue(90);
        ui->doubleSpinBox_encoderMin->setValue(-90);
        ui->doubleSpinBox_steerMaxAngel->setValue(1800);
        ui->doubleSpinBox_steerMinAngel->setValue(0);
        ui->comboBox_encoderType->setCurrentIndex(0);

        ui->label_Aaxis->show();
        ui->doubleSpinBox_Aaxis->show();
        ui->label_Baxis->show();
        ui->doubleSpinBox_Baxis->show();
        ui->label_encoderRange->show();
        ui->doubleSpinBox_encoderMax->show();
        ui->doubleSpinBox_encoderMin->show();
        ui->label_EncoderType->show();
        ui->comboBox_encoderType->show();
        ui->label_steerAngelRange->show();
        ui->doubleSpinBox_steerMaxAngel->show();
        ui->doubleSpinBox_steerMinAngel->show();
        ui->label_encoderRange_max->show();
        ui->label_steerAngelRange_max->show();
        ui->label_theta->hide();
        ui->doubleSpinBox_theta->hide();
        //--------------------------------------------
        if(shapeWid_->getShape()==RECTANGLE)
        {
            ui->label_image->setPixmap(QString("%1").arg(imagePath_+"SingleSteer"));
        }
        else if(shapeWid_->getShape()==CIRCLE)
        {
        }
    }
        break;
    }
    checkIfNeedEnableByFuncType();
    enableNextStep();
}

void GuidChassisTypeSelect::on_doubleSpinBox_wheelRadius_valueChanged(double arg1)
{
    roboData_.wheelRadius_ = arg1*100;
    enableNextStep();
}

void GuidChassisTypeSelect::on_doubleSpinBox_Aaxis_valueChanged(double arg1)
{
    ChassisType chasType = (ChassisType)ui->comboBox_funcType->currentData(Qt::UserRole).toInt();
    switch (chasType)
    {
    case DIFFERENTIAL_TWOWHEEL://两轮差动
    {
        roboData_.chasisModel.DiffTwoWheel.a_axis_distance = arg1*100;
    }
        break;
    case DIFFERENTIAL_FOURWHEEL://四轮差动
    {
        roboData_.chasisModel.DiffFourWheel.a_axis_distance = arg1*100;
    }
        break;
    case ALLDIRECTION_THREEWHEEL://全向三轮
    {
        roboData_.chasisModel.AllDirectThreeWheel.wheel_corePoint_distance = arg1*100;
    }
        break;
    case MECANUM_FOURWHEEL://四轮麦克纳姆
    {
        roboData_.chasisModel.MecanumFourWheel.a_axis_distance = arg1*100;
    }
        break;
    case SINGLESTEERWHEEL_FRONTDRIVER_FRONT_CODERTYPE://前驱+前转动编码器 单舵轮
    case SINGLESTEERWHEEL_FRONTDRIVER_BACK_CODERTYPE://前驱+后转动编码器
    case SINGLESTEERWHEEL_BACKDRIVER_FRONT_CODERTYPE://后驱+前转动编码器
    case SINGLESTEERWHEEL_BACKDRIVER_BACK_CODERTYPE://后驱+后转动编码器
    {
        roboData_.chasisModel.SingleWheelDriver.a_axis_distance = arg1*100;
    }
        break;
    }
    enableNextStep();
}

void GuidChassisTypeSelect::on_doubleSpinBox_Baxis_valueChanged(double arg1)
{
    ChassisType chasType = (ChassisType)ui->comboBox_funcType->currentData(Qt::UserRole).toInt();
    switch (chasType)
    {
    case DIFFERENTIAL_TWOWHEEL://两轮差动
        break;
    case DIFFERENTIAL_FOURWHEEL://四轮差动
    {
        roboData_.chasisModel.DiffFourWheel.b_axis_distance = arg1*100;
    }
        break;
    case ALLDIRECTION_THREEWHEEL://全向三轮
        break;
    case MECANUM_FOURWHEEL://四轮麦克纳姆
    {
        roboData_.chasisModel.MecanumFourWheel.b_axis_distance = arg1*100;
    }
        break;
    case SINGLESTEERWHEEL_FRONTDRIVER_FRONT_CODERTYPE://前驱+前转动编码器 单舵轮
    case SINGLESTEERWHEEL_FRONTDRIVER_BACK_CODERTYPE://前驱+后转动编码器
    case SINGLESTEERWHEEL_BACKDRIVER_FRONT_CODERTYPE://后驱+前转动编码器
    case SINGLESTEERWHEEL_BACKDRIVER_BACK_CODERTYPE://后驱+后转动编码器
    {
        roboData_.chasisModel.SingleWheelDriver.b_axis_distance = arg1*100;
    }
        break;
    }
    enableNextStep();
}

void GuidChassisTypeSelect::on_doubleSpinBox_theta_valueChanged(double arg1)
{
    roboData_.chasisModel.AllDirectThreeWheel.firstWheel_delta_angle = arg1;
    enableNextStep();
}

void GuidChassisTypeSelect::on_doubleSpinBox_steerMinAngel_valueChanged(double arg1)
{
    ui->doubleSpinBox_steerMinAngel->setMaximum(ui->doubleSpinBox_steerMaxAngel->value());
    roboData_.chasisModel.SingleWheelDriver.steer_angle_min = arg1;
}

void GuidChassisTypeSelect::on_doubleSpinBox_steerMaxAngel_valueChanged(double arg1)
{
    ui->doubleSpinBox_steerMaxAngel->setMinimum(ui->doubleSpinBox_steerMinAngel->value());
    roboData_.chasisModel.SingleWheelDriver.steer_angle_max  = arg1;
    enableNextStep();
}

void GuidChassisTypeSelect::on_doubleSpinBox_encoderMin_valueChanged(double arg1)
{
    ui->doubleSpinBox_encoderMin->setMaximum(ui->doubleSpinBox_encoderMax->value());
    roboData_.chasisModel.SingleWheelDriver.absEncoderRange_min = arg1;
}

void GuidChassisTypeSelect::on_doubleSpinBox_encoderMax_valueChanged(double arg1)
{
    ui->doubleSpinBox_encoderMax->setMinimum(ui->doubleSpinBox_encoderMin->value());
    roboData_.chasisModel.SingleWheelDriver.absEncoderRange_max = arg1;
    enableNextStep();
}

void GuidChassisTypeSelect::on_comboBox_encoderType_currentIndexChanged(int index)
{
    roboData_.chasisModel.SingleWheelDriver.absEncoderType = index;
    enableNextStep();
}
