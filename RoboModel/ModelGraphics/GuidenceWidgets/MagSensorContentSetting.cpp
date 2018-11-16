#include "MagSensorContentSetting.h"
#include "ui_MagSensorContentSetting.h"

MagSensorContentSetting::MagSensorContentSetting(QWidget *parent) :
    BasicContentSetting(parent),
    ui(new Ui::MagSensorContentSetting),
    isValid_(false)
{
    ui->setupUi(this);

    initWigetData();
    isValid_ = ui->doubleSpinBox_step->value()>0? true:false;
    ui->ptb_previous->hide();
    groupBoxList_.append(ui->groupBox_0);
    groupBoxList_.append(ui->groupBox_1);
    groupBoxList_.append(ui->groupBox_2);
    groupBoxList_.append(ui->groupBox_3);

    for(int i = 0 ; i< groupBoxList_.size();i++)
    {
        if(0 == i)
            groupBoxList_.at(i)->show();
        else
            groupBoxList_.at(i)->hide();
    }
     ui->label_direction->setText(tr("set type"));
     this->resize(330,130);
     this->setWindowTitle(tr("MagneticSensor Guidence Setting"));
}

MagSensorContentSetting::~MagSensorContentSetting()
{
    delete ui;
}

void MagSensorContentSetting::initWigetData()
{
    magneticData_.resolution_ = 8;
    magneticData_.step_ = 1;

    ui->doubleSpinBox_step->setValue(magneticData_.step_/100.);
    ui->spinBox_resolution->setValue(magneticData_.resolution_);

    ConfigureData typeData = readConfigure_MagneticSensorType(CONFIGFILE_PATH);
    QList<ItemData>::iterator iter_type = typeData.dataList.begin();
    for(;iter_type!= typeData.dataList.end();iter_type++)
    {
        ui->comboBox_type->addItem((*iter_type).describtion,(*iter_type).index);
    }

    ui->comboBox_type->setCurrentIndex(typeData.defaultidx);
}

ConfigureData MagSensorContentSetting::readConfigure_MagneticSensorType(const QString &filePath)
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

    QJsonValue gyro_value = chassis_obj.value("magneticSensor");
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

void MagSensorContentSetting::setValid(bool flag)
{
    isValid_ = flag;
}

bool MagSensorContentSetting::getValid()
{
    return isValid_;
}

void MagSensorContentSetting::on_spinBox_resolution_valueChanged(int arg1)
{
    magneticData_.resolution_ = arg1;
}

void MagSensorContentSetting::on_doubleSpinBox_step_valueChanged(double arg1)
{
    magneticData_.step_ = arg1;
    if(arg1==0)
    {
        isValid_ = false;
    }
    else
    {
        isValid_  = true;
    }
    emit sig_step_valueChange();
}

void MagSensorContentSetting::on_ptb_previous_clicked()
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
        ui->ptb_previous->hide();
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
        ui->label_direction->setText(tr("set type"));
        break;
    case 1:
        ui->label_direction->setText(tr("set resolution"));
        break;
    case 2:
        ui->label_direction->setText(tr("set step"));
        break;
    default:
        break;
    }
}

void MagSensorContentSetting::on_ptb_Next_clicked()
{
    ui->ptb_previous->show();

    currentIndex_++;
    if(currentIndex_ > 3)
    {
        currentIndex_--;
        emit sig_finished();
        this->accept();
        return;
    }

    if(3 == currentIndex_)
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
        ui->label_direction->setText(tr("set type"));
        break;
    case 1:
        ui->label_direction->setText(tr("set resolution"));
        break;
    case 2:
        ui->label_direction->setText(tr("set step"));
        break;
    default:
        break;
    }
}

void MagSensorContentSetting::on_comboBox_type_currentIndexChanged(int index)
{
    magneticSensorType_ = index;
}
