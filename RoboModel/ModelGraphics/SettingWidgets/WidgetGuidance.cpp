#include "WidgetGuidance.h"
#include "ui_WidgetGuidance.h"
#include<QDebug>
#include"ModelCommon.h"
#include"StyleObject.h"
#include"UiClass.h"
int WidgetGuidance::m_idx = 0;
WidgetGuidance::WidgetGuidance(QWidget *parent) :
    CustomChildBaseDialog(parent),
    ui(new Ui::WidgetGuidance)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("New model guidence"));
    //    this->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowCloseButtonHint);
    this->resize(794,380);
    m_guidArrowList.append(ui->label_arraow_name);
    m_guidArrowList.append(ui->label_arrow_shape);
    m_guidArrowList.append(ui->label_arrow_chassis );
    m_guidArrowList.append(ui->label_arrow_basicParameter);
    m_guidArrowList.append(ui->label_arrow_simpleDev);
    m_guidArrowList.append(ui->label_arrow_optionDev);
    m_guidArrowList.append(ui->label_arrow_laser);
    m_guidArrowList.append(ui->label_arrow_ultrasonic);
    m_guidArrowList.append(ui->label_arrow_DI);
    m_guidArrowList.append(ui->label_arrow_Do);
    m_guidArrowList.append(ui->label_arrow_RFID);
    m_guidArrowList.append(ui->label_arrow_magnetic);
    m_guidArrowList.append(ui->label_arrow_finished);
    ui->line_2->setObjectName("line");
    ui->line_3->setObjectName("rightLine");

    QString imagePath_ = StyleObject::init()->getCurrentSkinDirPath()+"/images/"+"/RoModelPlugin/RoboChassis/";

    for(int i = 0; i< m_guidArrowList.size();++i)
    {
        m_guidArrowList.at(i)->setPixmap(QPixmap(QString("%1").arg(imagePath_+"arrow")));
        m_guidArrowList.at(i)->hide();
    }

    m_shapeWid = new GuidChassisShape;
    m_name = new GuidModelName;
    m_basicWid = new GuidBasicSetting;
    m_chassisWid  = new GuidChassisTypeSelect(m_shapeWid);
    m_simpleDev = new GuidSimpleDevice;
    m_optionalDev = new GuidoptionalDevice;
    m_diWid = new GuidDISetting;
    m_doWid = new GuidDoSetting;
    m_laserWid = new GuidLaserSetting;
    m_RFIDWid = new GuidRFIDSetting;
    m_magneticWid = new GuidMagneticSensorSetting;
    m_ultrasonicWid = new GuidUltrasonicSetting;
    m_finishedWid   = new GuidFinishedWid;
    m_guidWidList.append(m_name);
    m_guidWidList.append(m_shapeWid);
    m_guidWidList.append(m_chassisWid);
    m_guidWidList.append(m_basicWid);
    m_guidWidList.append(m_simpleDev);
    m_guidWidList.append(m_optionalDev);
    m_guidWidList.append(m_laserWid);
    m_guidWidList.append(m_ultrasonicWid);
    m_guidWidList.append(m_diWid);
    m_guidWidList.append(m_doWid);
    m_guidWidList.append(m_RFIDWid);
    m_guidWidList.append(m_magneticWid);
    m_guidWidList.append(m_finishedWid);

    ui->horizontalLayout_setWid->addWidget(m_guidWidList.at(0));
    for(int i = 0; i < m_guidWidList.size();i++)
    {
        connect(m_guidWidList.at(i),SIGNAL(sig_configure()),this,SLOT(Slot_updateForwardStatus()));
        connect(m_guidWidList.at(i),SIGNAL(sig_enableNextStep(bool)),this,SLOT(Slot_enableForwardBtn(bool)));
    }
    //初始化第一个为基本数据配置界面
    ShowCurrentGuidWid(0);
    SetGuidInstruction();

    connect(m_shapeWid,SIGNAL(sig_shape_change(ShapeType)),m_chassisWid,SLOT(slot_shape_currentIndexChanged(ShapeType)));

    connect(m_RFIDWid,SIGNAL(sig_ignore()),m_magneticWid,SLOT(slot_ignore_byRFID()));
    connect(m_RFIDWid,SIGNAL(sig_set()),m_magneticWid,SLOT(slot_set_byRFID()));
}

WidgetGuidance::~WidgetGuidance()
{
    delete ui;
}

void WidgetGuidance::SetGuidInstruction()
{
    //更新箭头指向
    for(int i = 0; i< m_guidArrowList.size();i++)
    {
        if(m_idx == i)
        {
            m_guidArrowList.at(i)->show();
        }
        else
        {
            m_guidArrowList.at(i)->hide();
        }
    }

    GuidType type = m_guidWidList.at(m_idx)->getType();
    setNextStepEnabled(type);

    switch (type) {
    case GUID_BASIC:
        ui->label_item->setText(tr("Basic parameter configuration"));
        break;
    case GUID_CHASSIS_SHAPE :
        ui->label_item->setText(tr("Chassis shape selection"));
        break;
    case GUID_SIMPLEDEVICE :
        ui->label_item->setText(tr("Simple device type selection"));
        break;
    case GUID_OPTIONALDEVICE:
        ui->label_item->setText(tr("Optional equipment and function configuration"));
        break;
    case GUID_MODEL_NAME:
        ui->label_item->setText(tr("Model name"));
        break;
    case GUID_CHASSIS_TYPE:
        ui->label_item->setText(tr("Chassis selection"));
        break;
    case GUID_DI:
        ui->label_item->setText(tr("DI configuration"));

        break;
    case GUID_DO:
        ui->label_item->setText(tr("DO configuration"));
        break;
    case GUID_LASER:

        ui->label_item->setText(tr("Laser configuration"));
        break;
    case GUID_MAGENATICSENSOR:
        ui->label_item->setText(tr("Magnetic sensor configuration"));
        break;
    case GUID_RFIDSENSOR:
        ui->label_item->setText(tr("RFID sensor configuration"));
        break;
    case GUID_ULTRASONIC:
        ui->label_item->setText(tr("Ultrasonic configuration"));
        break;
    case GUID_FINISHED:
        ui->label_item->setText(tr("Configuration finish"));
        break;
    default:
        break;
    }

    if(type == GUID_FINISHED)
    {
        ui->pushButton_forward->setEnabled(true);
        ui->pushButton_forward->setText(tr(" Finished "));        
    }
    else
    {
        ui->pushButton_forward->setText(tr("  Next  "));
        m_isFinished = false;
    }

}

void WidgetGuidance::setNextStepEnabled(GuidType type)
{
    //因为RFID只有一个，配置或不配置都允许下一步可用
    if(type == GUID_RFIDSENSOR)
    {
        ui->pushButton_forward->setEnabled(true);
    }

    if(type == GUID_CHASSIS_TYPE)
    {
        dynamic_cast<GuidChassisTypeSelect*>(m_guidWidList.at(m_idx))->updateData();
    }

    if(type != GUID_CHASSIS_TYPE&& type != GUID_RFIDSENSOR)
    {
        if(m_guidWidList.at(m_idx)->getConfigure())
        {
            m_guidWidList.at(m_idx)->enableNextStep();
        }
        else
        {
            ui->pushButton_forward->setEnabled(true);
        }
        m_guidWidList.at(m_idx)->showNumWid();//显示第一个num设置窗
    }

}

void WidgetGuidance::ShowCurrentGuidWid(int idx)
{
    m_idx = idx;

    if(m_idx == 0)
    {
        ui->pushButton_backward->hide();
        ui->pushButton_forward->setEnabled(true);
    }
    else
    {
        ui->pushButton_backward->show();
    }

    for(int i = 0; i < m_guidWidList.size();i++)
    {
        if(i==m_idx)
        {
            ui->horizontalLayout_setWid->takeAt(0)->widget()->hide();
            ui->horizontalLayout_setWid->addWidget(m_guidWidList.at(i));
            m_guidWidList.at(i)->show();
        }
        else
        {
            //            m_guidWidList.at(i)->hide();
        }
    }
}

QList<GuidBaseSetting *> *WidgetGuidance::getGuidWidList()
{
    return &m_guidWidList;
}

bool WidgetGuidance::willCloseWidget()
{
    //如果配置完成则关闭窗口不用提示
    if(m_isFinished) return true;
    QStringList list;
    list<<tr("Yes")<<tr("No");
    int rb = UiClass::init()->showDialog(tr("Do you want to turn off the current Guidence ?"),tr("Information"),0,list);
    if(rb==0)
    {
        return true;
    }
    else
    {

        return false;
    }
}

void WidgetGuidance::setfinished(bool flg)
{
    m_isFinished = flg;
}

void WidgetGuidance::on_pushButton_backward_clicked()
{
    //未完成
    m_isFinished = false;

    if(m_idx>0)
    {
        m_idx--;
        ShowCurrentGuidWid(m_idx);
        SetGuidInstruction();
        if(m_idx == 0 || m_idx == 1)
        {
            ui->pushButton_forward->setEnabled(true);
        }
    }
    m_guidWidList.at(m_idx)->slot_enableNextStep();
}

void WidgetGuidance::on_pushButton_forward_clicked()
{
    if(m_idx<m_guidWidList.size()-1)
    {
        m_idx++;
        ShowCurrentGuidWid(m_idx);
        SetGuidInstruction();
    }
    else
    {
        m_isFinished = true;
    }
    m_guidWidList.at(m_idx)->enableNextStep();
    m_guidWidList.at(m_idx)->slot_enableNextStep();//对每个设备进行判断设置

    if(m_isFinished)
    {
        emit sig_finished();
    }
}

void WidgetGuidance::Slot_updateForwardStatus()
{
    GuidType type = m_guidWidList.at(m_idx)->getType();
    setNextStepEnabled(type);
}

void WidgetGuidance::Slot_enableForwardBtn(bool flag)
{
    ui->pushButton_forward->setEnabled(flag);
}

void WidgetGuidance::Slot_shape_changed(ShapeType shp)
{

}
