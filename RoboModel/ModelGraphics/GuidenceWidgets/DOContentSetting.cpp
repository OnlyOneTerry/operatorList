#include "DOContentSetting.h"
#include "ui_DOContentSetting.h"

DOContentSetting::DOContentSetting(QWidget *parent) :
    BasicContentSetting(parent),
    ui(new Ui::DOContentSetting)
{
    ui->setupUi(this);
    initWidgetData();
    groupBoxList_.append(ui->groupBox_0);
    groupBoxList_.append(ui->groupBox_1);
    groupBoxList_.append(ui->groupBox_2);
    groupBoxList_.append(ui->groupBox_3);
    ui->ptb_Previous->hide();
    for(int i = 0; i< groupBoxList_.size();i++)
    {
        if(i == 0)
        {
            groupBoxList_.at(i)->show();
        }
        else
        {
            groupBoxList_.at(i)->hide();
        }
    }
    ui->label_direction->setText(tr("set func"));
    this->resize(340,130);
    this->setWindowTitle(tr("DO Guidence Setting"));
}

DOContentSetting::~DOContentSetting()
{
    delete ui;
}

void DOContentSetting::initWidgetData()
{
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
        ui->comboBox_type->addItem(datalist2.at(i).describtion,datalist2.at(i).index);
    }

    ui->comboBox_type->setCurrentIndex(idx2);
}

ConfigureData DOContentSetting::read_funcConfigFile()
{
    return readConfigData(CONFIGFILE_PATH,"func");
}

ConfigureData DOContentSetting::read_typeConfigFile()
{
    return readConfigData(CONFIGFILE_PATH,"type");
}

ConfigureData DOContentSetting::readConfigData(const QString &filePath, const QString &argu)
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

    QJsonValue  DO_value= chassis_obj["DO"];
    QJsonObject DO_obj = DO_value.toObject();

    QJsonValue  argu_value = DO_obj.value(argu);
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

void DOContentSetting::setValid(bool flag)
{
    isValid_  = flag;
}

bool DOContentSetting::getValid()
{
    return isValid_;
}

void DOContentSetting::on_ptb_Previous_clicked()
{
    ui->label_promt->show();
    ui->label_direction->show();
    ui->ptb_Next->setText(tr("  Next  "));
    currentIndex_--;
    if(currentIndex_<0)
    {
        currentIndex_++;
        return;
    }

    if(currentIndex_==0)
    {
        ui->ptb_Previous->hide();
    }

    for(int i = 0; i< groupBoxList_.size();i++)
    {
        groupBoxList_.at(i)->hide();
    }

    for(int i = 0; i< groupBoxList_.size();i++)
    {
        if(i == currentIndex_)
        {
            groupBoxList_.at(i)->show();
        }
    }

    switch (currentIndex_)
    {
    case 0:
        ui->label_direction->setText(tr("set func"));
        break;
    case 1:
        ui->label_direction->setText(tr("set type"));
        break;
    case 2:
        ui->label_direction->setText(tr("set inverse"));
        break;
    default:
        break;
    }

}

void DOContentSetting::on_ptb_Next_clicked()
{
    ui->ptb_Previous->show();
    currentIndex_++;
    if(currentIndex_>3)
    {
        emit sig_finished();
        this->accept();
        currentIndex_--;
        return;
    }
    if(currentIndex_==3)
    {
        ui->label_promt->hide();
        ui->label_direction->hide();
        ui->ptb_Next->setText(tr(" Finished "));
    }

    for(int i = 0; i< groupBoxList_.size();i++)
    {
        groupBoxList_.at(i)->hide();
    }

    for(int i = 0; i< groupBoxList_.size();i++)
    {
        if(i == currentIndex_)
        {
            groupBoxList_.at(i)->show();
        }
    }

    switch (currentIndex_)
    {
    case 0:
        ui->label_direction->setText(tr("set func"));
        break;
    case 1:
        ui->label_direction->setText(tr("set type"));
        break;
    case 2:
        ui->label_direction->setText(tr("set inverse"));
        break;
    default:
        break;
    }
}

void DOContentSetting::on_comboBox_func_currentIndexChanged(int index)
{
    doData_.func_ = index;
}

void DOContentSetting::on_comboBox_type_currentIndexChanged(int index)
{
    doData_.type_  = index;
}

void DOContentSetting::on_checkBox_inverse_clicked(bool checked)
{
    doData_.inverse_ = checked;
}
