#include "CrashDiContentSetting.h"
#include "ui_CrashDiContentSetting.h"
#include"RoLog.h"

CrashDiContentSetting::CrashDiContentSetting(int id, QWidget *parent) :
    BasicContentSetting(parent),
    ui(new Ui::CrashDiContentSetting)
{
    ui->setupUi(this);
    initWidgetData();
    groupBoxList_.append(ui->groupBox_0);
    groupBoxList_.append(ui->groupBox_1);
    groupBoxList_.append(ui->groupBox_2);
    groupBoxList_.append(ui->groupBox_3);

    ui->ptb_previous->hide();
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
    this->resize(330,130);
    this->setWindowTitle(tr("crashDI")+QString("  id:%1").arg(id));
}

CrashDiContentSetting::~CrashDiContentSetting()
{
    delete ui;
}

void CrashDiContentSetting::initWidgetData()
{
   ui->comboBox_type->addItem(tr("crashDI(line)"));
   //初始化func类型
   ConfigureData funData = read_funcConfigFile();

   int idx = funData.defaultidx;

   QList<ItemData> datalist = funData.dataList;

   for(int i = 0; i < datalist.size();i++)
   {
       ui->comboBox_func->addItem(datalist.at(i).describtion,datalist.at(i).index);
   }

   ui->comboBox_func->setCurrentIndex(idx);
}

ConfigureData CrashDiContentSetting::read_funcConfigFile()
{
 return  readConfigData(CONFIGFILE_PATH,"func");
}

ConfigureData CrashDiContentSetting::readConfigData(const QString &filePath, const QString &argu)
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

void CrashDiContentSetting::on_comboBox_func_currentIndexChanged(int index)
{
   data_crashDi_.func_ = index;
}

void CrashDiContentSetting::on_comboBox_type_currentIndexChanged(int index)
{
   data_crashDi_.type_ = index;
}

void CrashDiContentSetting::on_checkBox_inverse_2_clicked(bool checked)
{
   data_crashDi_.inverse_ = checked;
}

void CrashDiContentSetting::on_ptb_previous_clicked()
{
    ui->label_direction->show();
    ui->label_prompt->show();
    ui->ptb_next->setText(tr("  Next  "));
    currentIndex_--;
    if(currentIndex_<0)
    {
        currentIndex_++;
        return;
    }

    if(currentIndex_==0)
    {
        ui->ptb_previous->hide();
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

void CrashDiContentSetting::on_ptb_next_clicked()
{
    ui->ptb_previous->show();
    currentIndex_++;
    if(currentIndex_>3)
    {
        currentIndex_--;
        emit sig_finished();
        this->accept();
        SCDebug<<"------------------------1-----------------currentIndex is "<<currentIndex_;
        return;
    }
    SCDebug<<"------------------------2------------------currentIndex is "<<currentIndex_;
    if(currentIndex_==3)
    {
        ui->ptb_next->setText(tr(" Finish "));
        ui->label_direction->hide();
        ui->label_prompt->hide();
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
