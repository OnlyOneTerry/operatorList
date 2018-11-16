#include "UltrasonicContentSetting.h"
#include "ui_UltrasonicContentSetting.h"

UltrasonicContentSetting::UltrasonicContentSetting(QWidget *parent) :
    BasicContentSetting(parent),
    ui(new Ui::UltrasonicContentSetting)
{
    ui->setupUi(this);
    initWigetData();
    isValid_ = ui->doubleSpinBox_range->value()>0? true:false;

    groupBoxList_.append(ui->groupBox_0);
    groupBoxList_.append(ui->groupBox_1);
    groupBoxList_.append(ui->groupBox_2);
    groupBoxList_.append(ui->groupBox_3);
    groupBoxList_.append(ui->groupBox_4);
    groupBoxList_.append(ui->groupBox_5);

    ui->ptb_Previous->hide();
    for(int i = 0;i<groupBoxList_.size();i++)
    {
        if(0 == i)
            groupBoxList_.at(i)->show();
        else
            groupBoxList_.at(i)->hide();
    }
    ui->label_direction->setText(tr("set type"));

    this->resize(330,150);
    this->setWindowTitle(tr("Ultrasonic Guidence Setting"));
}

UltrasonicContentSetting::~UltrasonicContentSetting()
{
    delete ui;
}

void UltrasonicContentSetting::initWigetData()
{
    ultrasonicData_.min_dist_ = 2;
    ultrasonicData_.max_dist_=30;
    ultrasonicData_.range_ = 30;

    ui->doubleSpinBox_maxDist->setValue(ultrasonicData_.max_dist_/100.);
    ui->doubleSpinBox_minDist->setValue(ultrasonicData_.min_dist_/100.);
    ui->doubleSpinBox_range->setValue(ultrasonicData_.range_);

    //初始化func类型
    ConfigureData funData = readFuncTypeFile(CONFIGFILE_PATH);

    int idx = funData.defaultidx;

    QList<ItemData> datalist = funData.dataList;
    for(int i = 0; i < datalist.size();i++)
    {
        ui->comboBox_func->addItem(datalist.at(i).describtion,datalist.at(i).index);
    }

    ui->comboBox_func->setCurrentIndex(idx);


    ConfigureData typeData = readConfigure_UltrasonicType(CONFIGFILE_PATH);
    QList<ItemData>::iterator iter_type = typeData.dataList.begin();
    for(;iter_type!= typeData.dataList.end();iter_type++)
    {
        ui->comboBox_type->addItem((*iter_type).describtion,(*iter_type).index);
    }

    ui->comboBox_type->setCurrentIndex(typeData.defaultidx);
}

ConfigureData UltrasonicContentSetting::readFuncTypeFile(const QString filePath)
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

    QJsonValue  ultrasonic_value= chassis_obj["ultrasonic"];
    QJsonObject ultrasonic_obj = ultrasonic_value.toObject();

    QJsonValue  func_value = ultrasonic_obj.value("func");
    QJsonObject  func_obj  = func_value.toObject();


    ConfigureData  cfData;

    QJsonValue defaultIdex = func_obj["defaultIndex"];

    cfData.defaultidx = defaultIdex.toInt();
    QJsonArray typeList = func_obj["typeList"].toArray();


    for(int i = 0;i < typeList.size();i++)
    {
        ItemData tempData;
        QJsonObject tempObj = typeList.at(i).toObject();
        QJsonValue tempValue = tempObj["index"];
        QJsonValue tempDesc  = tempObj["desc"];

        tempData.index = tempValue.toInt();
        tempData.describtion = tempDesc.toString();

        cfData.dataList.push_back(tempData);
    }

    return cfData;
}

ConfigureData UltrasonicContentSetting::readConfigure_UltrasonicType(const QString &filePath)
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

void UltrasonicContentSetting::setValid(bool flag)
{
    isValid_ = flag;
}

bool UltrasonicContentSetting::getValid()
{
    return  isValid_;
}

void UltrasonicContentSetting::on_doubleSpinBox_range_valueChanged(double arg1)
{
    ultrasonicData_.range_ = arg1;
    if(arg1>0)
    {
        isValid_ = true;
    }
    else
    {
        isValid_ = false;
    }
    emit sig_range_valueChange();
}

void UltrasonicContentSetting::on_doubleSpinBox_maxDist_valueChanged(double arg1)
{
    ultrasonicData_.max_dist_ = arg1;
}

void UltrasonicContentSetting::on_doubleSpinBox_minDist_valueChanged(double arg1)
{
    ultrasonicData_.min_dist_  = arg1;
}


void UltrasonicContentSetting::on_comboBox_func_currentIndexChanged(int index)
{
    ultrasonicData_.func_ = index;
}

void UltrasonicContentSetting::on_ptb_Previous_clicked()
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
        ui->label_direction->setText(tr("set type"));
        break;
    case 1:
        ui->label_direction->setText(tr("set func"));
        break;
    case 2:
        ui->label_direction->setText(tr("set minDistance"));
        break;
    case 3:
        ui->label_direction->setText(tr("set maxDistance"));
        break;
    case 4:
        ui->label_direction->setText(tr("set scanArea"));
        break;
    default:
        break;
    }
}

void UltrasonicContentSetting::on_ptb_Next_clicked()
{
    ui->ptb_Previous->show();

    currentIndex_++;
    if(currentIndex_ > 5)
    {
        currentIndex_--;
        emit sig_finished();
        this->accept();
        return;
    }

    if(5 == currentIndex_)
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
        ui->label_direction->setText(tr("set func"));
        break;
    case 2:
        ui->label_direction->setText(tr("set minDistance"));
        break;
    case 3:
        ui->label_direction->setText(tr("set maxDistance"));
        break;
    case 4:
        ui->label_direction->setText(tr("set scanArea"));
        break;
    default:
        break;
    }
}

void UltrasonicContentSetting::on_comboBox_type_currentIndexChanged(int index)
{
    ultrasonicType_ = index;
}
