#include "WidgetCrashDISetting.h"
#include "ui_WidgetCrashDISetting.h"
#include"ItemCrashDI.h"
#include"../ModelHelper/ModelCustomCommand.h"
#include<QJsonDocument>
#include<QJsonArray>
#include<QJsonObject>
#include"ModelCommon.h"
WidgetCrashDISetting::WidgetCrashDISetting(ItemCrashDI *item, QWidget *parent) :
    crashDi_(item),
    WidgetSetting(parent),
    ui(new Ui::WidgetCrashDISetting)
{
    ui->setupUi(this);
    setType(WIDGET_CRASHDI);
    ui->label_Type->setText(tr("DI"));
    initWidgetData();

    connect(&crashDi_->timer,SIGNAL(timeout()),this,SLOT(on_timer_out()));
    //读取文件
    ui->comboBox_func->blockSignals(true);
    ui->comboBox_func->setCurrentIndex(item->data_crashDi_.func_);
    ui->comboBox_func->blockSignals(false);
    ui->comboBox_func->setOri_index(item->data_crashDi_.func_);
    ui->checkBox_inverse->blockSignals(true);
    ui->checkBox_inverse->setChecked(item->data_crashDi_.inverse_);
    ui->checkBox_inverse->blockSignals(false);
    ui->spinBox_Id->blockSignals(true);
    ui->spinBox_Id->setValue(item->data_crashDi_.id_);
    ui->spinBox_Id->blockSignals(false);
    ui->spinBox_Id->setOriValue(item->data_crashDi_.id_);

}

WidgetCrashDISetting::~WidgetCrashDISetting()
{
    delete ui;
}

void WidgetCrashDISetting::update_value()
{

}

void WidgetCrashDISetting::initWidgetData()
{

    ui->checkBox_inverse->setChecked(false);
    ui->checkBox_inverse->setOriValue(false);
    //初始化func项
    ConfigureData funcData = read_funcConfigFile();

    QList<ItemData>::iterator iter_func = funcData.dataList.begin();
    ui->comboBox_func->blockSignals(true);
    for(;iter_func!= funcData.dataList.end();iter_func++)
    {
        ui->comboBox_func->addItem((*iter_func).describtion,(*iter_func).index);
    }
    ui->comboBox_func->setCurrentIndex(funcData.defaultidx);
    ui->comboBox_func->setOri_index(funcData.defaultidx);
    ui->comboBox_func->blockSignals(false);
}

ItemBase *WidgetCrashDISetting::getRelateItem()
{
    return crashDi_;
}

ConfigureData WidgetCrashDISetting::read_funcConfigFile()
{
    QString val;

    QFile file;

    file.setFileName(CONFIGFILE_PATH);

    file.open(QIODevice::ReadOnly|QIODevice::Text);

    val = file.readAll();

    file.close();

    QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject obj = d.object();

    QJsonValue chassis_value = obj.value(QString("chassis"));
    QJsonObject chassis_obj = chassis_value.toObject();

    QJsonValue  DI_value= chassis_obj["DI"];
    QJsonObject DI_obj = DI_value.toObject();

    QJsonValue  argu_value = DI_obj.value("func");
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

void WidgetCrashDISetting::on_checkBox_inverse_clicked(bool checked)
{

    crashDi_->data_crashDi_.inverse_ = checked;

    if(m_unStack ==NULL)
    {
        return;
    }

    ui->checkBox_inverse->setNewValue(checked);

    checkCommand* ck_command  = new checkCommand(ui->checkBox_inverse,ui->checkBox_inverse->getOriValue(),
                                                 ui->checkBox_inverse->getNewValue(),this,NULL);
    m_unStack->push(ck_command);

    ui->checkBox_inverse->setOriValue(checked);

}

void WidgetCrashDISetting::on_comboBox_func_currentIndexChanged(int index)
{
    crashDi_->data_crashDi_.func_ = index;

    if(m_unStack==NULL)
    {
        return;
    }

    ui->comboBox_func->setNew_index(index);
    comboxCommand* cbx_command = new comboxCommand(ui->comboBox_func,ui->comboBox_func->getOri_index(),
                                                   ui->comboBox_func->getNew_index(),NULL);
    m_unStack->push(cbx_command);
    ui->comboBox_func->setOri_index(index);

}

void WidgetCrashDISetting::on_spinBox_Id_editingFinished()
{
    int arg1=ui->spinBox_Id->value();

    if(!crashDi_->model_scene()->get_id_list(0).contains(QString::number(arg1)))
    {
        if(arg1 != crashDi_->data_crashDi_.id_)
        {
            crashDi_->data_crashDi_.id_=arg1;
            crashDi_->update();
            ui->label_Prompt->setText(tr("Setup Success"));
            ui->label_Prompt->setStyleSheet("color::green;");
        }
        else
        {
            ui->label_Prompt->setText(tr("The Id already exists"));
            ui->label_Prompt->setStyleSheet("color::red;");
        }
    }
    else
    {
        ui->spinBox_Id->setValue(crashDi_->data_crashDi_.id_);
        ui->label_Prompt->setText(tr("The Id already exists"));
        ui->label_Prompt->setStyleSheet("color::red;");
    }

    crashDi_->updata_item();

    //500毫秒后隐藏
    crashDi_->timer.setInterval(500);
    crashDi_->timer.setSingleShot(true);
    crashDi_->timer.start();
}

void WidgetCrashDISetting::on_spinBox_Id_valueChanged(int arg1)
{
    if(crashDi_->model_scene()->get_id_list(0).contains(QString::number(arg1)))
    {
        if(arg1 != crashDi_->data_crashDi_.id_)
        {
            ui->label_Prompt->setText(tr("The Id already exists"));
            ui->label_Prompt->setStyleSheet("color:red;");
        }
        else
        {
            crashDi_->data_crashDi_.id_=arg1;
            crashDi_->update();
            ui->label_Prompt->setText(tr("Setup Success"));
            ui->label_Prompt->setStyleSheet("color::green;");
        }
        return;
    }
    else
    {
        crashDi_->data_crashDi_.id_=arg1;
        crashDi_->update();
        ui->label_Prompt->setText(tr("Setup Success"));
        ui->label_Prompt->setStyleSheet("color::green;");
    }

    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->spinBox_Id->getIsClickedTrigger())
    {
        ui->spinBox_Id->setNewValue(arg1);

        SpinBoxCommand* sp_command = new SpinBoxCommand(
                    ui->spinBox_Id,ui->spinBox_Id->getOriValue(),
                    ui->spinBox_Id->getNewValue(),this,NULL);
        m_unStack->push(sp_command);
        ui->spinBox_Id->setOriValue(arg1);
    }
    ui->spinBox_Id->setIsClickedTrigger(true);
}

void WidgetCrashDISetting::on_timer_out()
{
    ui->label_Prompt->setText("");
}

void WidgetCrashDISetting::set_inverse_byGuidConfigure(bool arg)
{
    ui->checkBox_inverse->blockSignals(true);
    ui->checkBox_inverse->setChecked(arg);
    ui->checkBox_inverse->setOriValue(arg);
    ui->checkBox_inverse->blockSignals(false);

}

void WidgetCrashDISetting::set_func_byGuidConfigure(int index)
{
    ui->comboBox_func->blockSignals(true);
    ui->comboBox_func->setCurrentIndex(index);
    ui->comboBox_func->setOri_index(index);
    ui->comboBox_func->blockSignals(false);
}

void WidgetCrashDISetting::set_type_byGuidConfigure(int index)
{
    ui->comboBox_type->blockSignals(true);
    ui->comboBox_type->setCurrentIndex(index);
    ui->comboBox_type->setOri_index(index);
    ui->comboBox_type->blockSignals(false);

}
