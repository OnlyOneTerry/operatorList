#include "DIContentSetting.h"
#include "ui_DIContentSetting.h"
#include"RoLog.h"
DIContentSetting::DIContentSetting(QWidget *parent) :
    BasicContentSetting(parent),
    ui(new Ui::DIContentSetting),
    isValid_(false)
{
    ui->setupUi(this);

    initWidgetData();
    isValid_ = ui->spinBox_range->value()>0?true:false;
}

DIContentSetting::DIContentSetting(ItemType type, int id, QWidget *parent):
    BasicContentSetting(parent),
    ui(new Ui::DIContentSetting),
    isValid_(false)
{
    ui->setupUi(this);
    //    this->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowCloseButtonHint);
    //    setWindowFlags(this->windowFlags()&~Qt::WindowCloseButtonHint);

    initWidgetData(type);
//    ui->label_prohibitPromt->setStyleSheet("color:red;");
    isValid_ = ui->spinBox_range->value()>0?true:false;
    groupBoxList_.append(ui->groupBox_0);
    groupBoxList_.append(ui->groupBox_1);
    groupBoxList_.append(ui->groupBox_2);
    groupBoxList_.append(ui->groupBox_3);
    groupBoxList_.append(ui->groupBox_4);
    groupBoxList_.append(ui->groupBox_5);
    groupBoxList_.append(ui->groupBox_6);

    for(int i = 0; i< groupBoxList_.size();i++)
    {
        if(0 == i) groupBoxList_.at(i)->setVisible(true);
        else  groupBoxList_.at(i)->setVisible(false);
    }
    ui->label_direction->setText(tr("set func"));
    ui->ptb_previous->hide();

    this->resize(340,130);
    this->setWindowTitle(tr("DI guidence Setting")+QString("  id:%1").arg(id));

}


DIContentSetting::~DIContentSetting()
{
    delete ui;
}

void DIContentSetting::initWidgetData(ItemType type)
{
    diData_.maxDist_= 50;
    diData_.minDist_ = 10;
    diData_.rang_ = 30;

    ui->doubleSpinBox_maxDist->setValue(diData_.maxDist_/100.);
    ui->doubleSpinBox_minDist->setValue(diData_.minDist_/100.);
    ui->spinBox_range->setValue(30);

    //初始化func类型
    ConfigureData funData = read_funcConfigFile();

    int idx = funData.defaultidx;

    QList<ItemData> datalist = funData.dataList;

    for(int i = 0; i < datalist.size();i++)
    {
        ui->comboBox_func->addItem(datalist.at(i).describtion,datalist.at(i).index);
    }
    ui->comboBox_func->setCurrentIndex(idx);

    //初始化type类型
    ConfigureData typeData = read_typeConfigFile();

    int idx2 = typeData.defaultidx;

    QList<ItemData> datalist2 = typeData.dataList;

    if(type == I_crashDi )
    {
        ui->comboBox_type->addItem(tr("crashDI(line)"));
    }
    else if(type == I_di)
    {
        for(int i = 0; i < datalist2.size();i++)
        {
            ui->comboBox_type->addItem(datalist2.at(i).describtion,datalist2.at(i).index);
        }
        ui->comboBox_type->setCurrentIndex(idx2);
    }

}

void DIContentSetting::initWidgetData()
{
    diData_.maxDist_= 50;
    diData_.minDist_ = 10;
    diData_.rang_ = 30;

    ui->doubleSpinBox_maxDist->setValue(diData_.maxDist_/100.);
    ui->doubleSpinBox_minDist->setValue(diData_.minDist_/100.);
    ui->spinBox_range->setValue(30);

    //初始化func类型
    ConfigureData funData = read_funcConfigFile();

    int idx = funData.defaultidx;

    QList<ItemData> datalist = funData.dataList;

    for(int i = 0; i < datalist.size();i++)
    {
        ui->comboBox_func->addItem(datalist.at(i).describtion,datalist.at(i).index);
    }

    ui->comboBox_func->setCurrentIndex(idx);

    //初始化type类型
    ConfigureData typeData = read_typeConfigFile();

    int idx2 = typeData.defaultidx;

    QList<ItemData> datalist2 = typeData.dataList;

    for(int i = 0; i < datalist2.size();i++)
    {
        if(i==3)
        {
            ui->comboBox_type->addItem(tr("crashDI(line)"));
        }

        ui->comboBox_type->addItem(datalist2.at(i).describtion,datalist2.at(i).index);
    }
    ui->comboBox_type->setCurrentIndex(idx2);
}

ConfigureData DIContentSetting::read_funcConfigFile()
{
    return  readConfigData(CONFIGFILE_PATH,"func");
}

ConfigureData DIContentSetting::read_typeConfigFile()
{
    return  readConfigData(CONFIGFILE_PATH,"type");
}

ConfigureData DIContentSetting::readConfigData(const QString &filePath, const QString &argu)
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

    QJsonValue  DI_value= chassis_obj["DI"];
    QJsonObject DI_obj = DI_value.toObject();

    QJsonValue  argu_value = DI_obj.value(argu);
    QJsonObject  argu_obj  = argu_value.toObject();


    ConfigureData  cfData;

    QJsonValue defaultIdex = argu_obj["defaultIndex"];

    cfData.defaultidx = defaultIdex.toInt();
    QJsonArray typeList = argu_obj["typeList"].toArray();

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

bool DIContentSetting::getValid()
{
    return isValid_;
}

void DIContentSetting::setValid(bool flag)
{
    isValid_ = flag;
}


void DIContentSetting::on_spinBox_range_valueChanged(int arg1)
{
    diData_.rang_ = arg1;
    if(arg1==0)
    {
        isValid_ = false;
    }
    else
    {
        isValid_ = true;
    }
    emit sig_rang_valueChange();
}

void DIContentSetting::on_doubleSpinBox_minDist_valueChanged(double arg1)
{
    ui->doubleSpinBox_maxDist->setMinimum(arg1);
    diData_.minDist_ = arg1;
}

void DIContentSetting::on_doubleSpinBox_maxDist_valueChanged(double arg1)
{
    ui->doubleSpinBox_minDist->setMaximum(arg1);
    diData_.maxDist_ = arg1;
}

void DIContentSetting::on_checkBox_inverse_clicked(bool checked)
{
    diData_.inverse_ = checked;
}

void DIContentSetting::on_comboBox_func_currentIndexChanged(int index)
{
    diData_.func_ = index;
}

void DIContentSetting::on_comboBox_type_currentIndexChanged(int index)
{
    diData_.type_ = index;
    if(index==3)
    {
        ui->label_maxTag->hide();
        ui->label_maxDist->hide();
        ui->label_minDist->hide();
        ui->label_minTag->hide();
        ui->label_range->hide();
        ui->label_rangTag->hide();
        ui->doubleSpinBox_maxDist->hide();
        ui->doubleSpinBox_minDist->hide();
        ui->spinBox_range->hide();
    }
    else
    {
        ui->label_maxTag->show();
        ui->label_maxDist->show();
        ui->label_minDist->show();
        ui->label_minTag->show();
        ui->label_range->show();
        ui->label_rangTag->show();
        ui->doubleSpinBox_maxDist->show();
        ui->doubleSpinBox_minDist->show();
        ui->spinBox_range->show();
    }
}

void DIContentSetting::on_ptb_previous_clicked()
{
    ui->label_promt->show();
    ui->label_direction->show();
    ui->ptb_next->setText(tr("  Next  "));

    if(currentIndex_>0)
        currentIndex_--;
    if(currentIndex_==0)
        ui->ptb_previous->hide();
    else
        ui->ptb_previous->show();
    for(int i = 0; i < groupBoxList_.size();i++)
    {
        groupBoxList_.at(i)->hide();
    }
    for(int i = 0; i< groupBoxList_.size();i++)
    {
        if(i == currentIndex_)
            groupBoxList_.at(i)->show();
    }

    switch (currentIndex_) {
    case 0:
        ui->label_direction->setText(tr("set func"));
        break;
    case 1:

        ui->label_direction->setText(tr("set type"));
        break;
    case 2:

        ui->label_direction->setText(tr("set maxDistance"));
        break;
    case 3:
        ui->label_direction->setText(tr("set minDistance"));

        break;
    case 4:
        ui->label_direction->setText(tr("set scanArea"));

        break;
    case 5:

        ui->label_direction->setText(tr("set inverse"));
        break;
    default:
        break;
    }
}

void DIContentSetting::on_ptb_next_clicked()
{
    SCDebug<<"current indx is "<<currentIndex_;
    ui->ptb_previous->show();
    currentIndex_++;
    //完成
    if(currentIndex_>6)
    {
        emit sig_finished();
        this->accept();
        return;
    }
    if(currentIndex_==6)
    {
        ui->label_promt->hide();
        ui->label_direction->hide();
        ui->ptb_next->setText(tr(" Finished "));
    }

    for(int i = 0; i < groupBoxList_.size();i++)
    {
        groupBoxList_.at(i)->hide();
    }
    for(int i = 0; i< groupBoxList_.size();i++)
    {
        if(i == currentIndex_)
            groupBoxList_.at(i)->show();
    }
    switch (currentIndex_) {
    case 0:
        ui->label_direction->setText(tr("set func"));
        break;
    case 1:

        ui->label_direction->setText(tr("set type"));
        break;
    case 2:

        ui->label_direction->setText(tr("set maxDistance"));
        break;
    case 3:
        ui->label_direction->setText(tr("set minDistance"));

        break;
    case 4:
        ui->label_direction->setText(tr("set scanArea"));

        break;
    case 5:

        ui->label_direction->setText(tr("set inverse"));
        break;
    default:
        break;
    }
}
