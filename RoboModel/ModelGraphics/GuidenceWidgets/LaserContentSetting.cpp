#include "LaserContentSetting.h"
#include "ui_LaserContentSetting.h"
#include"RoLog.h"
LaserContentSetting::LaserContentSetting(QWidget *parent) :
    BasicContentSetting(parent),
    ui(new Ui::LaserContentSetting),
    isValid_(false)
{

    ui->setupUi(this);
    initWidgetData();
    isValid_ = isTextValid(ui->widget_IP->text());
    connect(ui->widget_IP,SIGNAL(textChanged(QString)),this,SLOT(slot_ip_textChange(QString)));
    ui->ptb_Previous->hide();
    groupBoxList_.append(ui->groupBox_0);
    groupBoxList_.append(ui->groupBox_1);
    groupBoxList_.append(ui->groupBox_2);
    groupBoxList_.append(ui->groupBox_3);
    groupBoxList_.append(ui->groupBox_4);
    groupBoxList_.append(ui->groupBox_5);
    groupBoxList_.append(ui->groupBox_6);
    groupBoxList_.append(ui->groupBox_7);
    groupBoxList_.append(ui->groupBox_8);

    for(int i = 0; i< groupBoxList_.size();i++)
    {
        if(0 == i)
        {
            groupBoxList_.at(i)->show();
        }
        else
        {
            groupBoxList_.at(i)->hide();
        }
    }
     ui->label_direction->setText(tr("Set the installation order"));
     this->resize(330,130);
     this->setWindowTitle(tr("Laser guide setting"));
//     ui->label_prohibitPromt->setStyleSheet("color:red;");
}

LaserContentSetting::~LaserContentSetting()
{
    delete ui;
}

void LaserContentSetting::initWidgetData()
{
    laserData_.step_ = 50;
    laserData_.ip_ ="192.168.192.100";
    laserData_.port_ = 2111;
    laserData_.max_angle_ = 90;
    laserData_.min_angle_ = -90;
    laserData_.use_for_localization_ = true;
    laserData_.upside_ = false;

    ui->checkBox_forLocation->setChecked(laserData_.use_for_localization_);
    ui->checkBox_inverse->setChecked(laserData_.upside_);
    ui->doubleSpinBox_maxAngle->setValue(laserData_.max_angle_);
    ui->doubleSpinBox_minAngle->setValue(laserData_.min_angle_);
    ui->spinBox_port->setValue(laserData_.port_);
    ui->widget_IP->setText(laserData_.ip_);
    ui->doubleSpinBox_step->setValue(laserData_.step_/100.);

    ConfigureData data = readLaserTypeFile(CONFIGFILE_PATH);

    QList<ItemData>::iterator iter = data.dataList.begin();

    for(;iter!= data.dataList.end();iter++)
    {
        ui->comboBox_type->addItem((*iter).describtion,(*iter).index);
    }

    ui->comboBox_type->setCurrentIndex(data.defaultidx);
}

// 判断IP地址
bool LaserContentSetting::isTextValid(const QString &strIP)
{    QRegExp rx2("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
     if (!rx2.exactMatch(strIP))
         return false;
     return true;
}

void LaserContentSetting::setValid(bool flag)
{
    isValid_ = flag;
}

bool LaserContentSetting::getValid()
{
    return isValid_;
}

void LaserContentSetting::setIndex(int idx)
{
    laserData_.ip_ ="192.168.1.11"+QString("%1").arg(idx);
    ui->widget_IP->setText(laserData_.ip_);
}


ConfigureData LaserContentSetting::readLaserTypeFile(const QString &filePath)
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

    QJsonValue  laser_value= chassis_obj["laser"];
    QJsonObject laser_obj = laser_value.toObject();

    ConfigureData  laserData;
    QJsonValue defaultIdex = laser_obj["defaultIndex"].toInt();
    laserData.defaultidx = defaultIdex.toInt();
    QJsonArray typeList = laser_obj["typeList"].toArray();

    for(int i = 0;i < typeList.size();i++)
    {
        ItemData tempData;
        QJsonObject tempObj = typeList.at(i).toObject();
        QJsonValue tempValue = tempObj["index"];
        QJsonValue tempDesc  = tempObj["desc"];

        tempData.index = tempValue.toInt();
        tempData.describtion = tempDesc.toString();

        laserData.dataList.push_back(tempData);
    }
    return laserData;
}

void LaserContentSetting::on_comboBox_type_currentIndexChanged(int index)
{
    laserData_.model_ = ui->comboBox_type->currentText();
}

void LaserContentSetting::on_doubleSpinBox_step_valueChanged(double arg1)
{
    laserData_.step_ = arg1;
}

void LaserContentSetting::on_doubleSpinBox_minAngle_valueChanged(double arg1)
{
    ui->doubleSpinBox_maxAngle->setMinimum(arg1);
    laserData_.min_angle_ = arg1;
}

void LaserContentSetting::on_doubleSpinBox_maxAngle_valueChanged(double arg1)
{
    ui->doubleSpinBox_minAngle->setMaximum(arg1);
    laserData_.max_angle_ = arg1;
}

void LaserContentSetting::on_checkBox_inverse_clicked(bool checked)
{
    laserData_.upside_ = checked;
}

void LaserContentSetting::on_checkBox_forLocation_clicked(bool checked)
{
    laserData_.use_for_localization_ = checked;
}

void LaserContentSetting::slot_ip_textChange(const QString &ipstr)
{
    laserData_.ip_ = ipstr;
    isValid_ =  isTextValid(ipstr);
    SCDebug<<"isvalid is --------------------"<<isValid_;
    emit sig_ip_valueChange();//发送ip变化信号给主界面判断是否可以启用下一步
}

void LaserContentSetting::on_ptb_Previous_clicked()
{
    ui->label_prompt->show();
    ui->label_direction->show();
     //设置下一步按钮
    ui->ptb_Next->setText(tr("  Next  "));

    currentIndex_--;
    if(currentIndex_ < 0)
    {
        currentIndex_++;
        return;
    }

    if(0 == currentIndex_ )
    {
        ui->ptb_Previous->hide();
    }

    for(int i = 0 ;i< groupBoxList_.size();i++)
    {     
        groupBoxList_.at(i)->hide();
    }

    for(int i = 0 ;i< groupBoxList_.size();i++)
    {
        if(currentIndex_ == i)
        {
            groupBoxList_.at(i)->show();
        }
    }

    switch (currentIndex_)
    {
    case 0:
        ui->label_direction->setText(tr("set the installation order"));
        break;
    case 1:
        ui->label_direction->setText(tr("set location or not"));
        break;
    case 2:
        ui->label_direction->setText(tr("set model"));
        break;
    case 3:
        ui->label_direction->setText(tr("set step"));
        break;
    case 4:
        ui->label_direction->setText(tr("set minAngle"));
        break;
    case 5:
        ui->label_direction->setText(tr("set maxAngle"));
        break;
    case 6:
        ui->label_direction->setText(tr("set Ip"));
        break;
    case 7:
        ui->label_direction->setText(tr("set port"));
        break;
    default:
        break;
    }

}

void LaserContentSetting::on_ptb_Next_clicked()
{
    ui->ptb_Previous->show();
    currentIndex_++;

    if(currentIndex_ > 8)
    {
        currentIndex_--;
        emit sig_finished();
        this->accept();
        return;
    }

    if(8 == currentIndex_)
    {
        ui->label_prompt->hide();
        ui->label_direction->hide();
        ui->ptb_Next->setText(tr(" Finished "));
    }

    for(int i = 0 ;i< groupBoxList_.size();i++)
    {
        groupBoxList_.at(i)->hide();
    }

    for(int i = 0 ;i< groupBoxList_.size();i++)
    {
        if(currentIndex_ == i)
        {
            groupBoxList_.at(i)->show();
        }
    }

    switch (currentIndex_)
    {
    case 0:
        ui->label_direction->setText(tr("set the installation order"));
        break;
    case 1:
        ui->label_direction->setText(tr("for location or not"));
        break;
    case 2:
        ui->label_direction->setText(tr("set model"));
        break;
    case 3:
        ui->label_direction->setText(tr("set step"));
        break;
    case 4:
        ui->label_direction->setText(tr("set minAngle"));
        break;
    case 5:
        ui->label_direction->setText(tr("set maxAngle"));
        break;
    case 6:
        ui->label_direction->setText(tr("set Ip"));
        break;
    case 7:
        ui->label_direction->setText(tr("set Port"));
        break;
    default:
        break;
    }
}
