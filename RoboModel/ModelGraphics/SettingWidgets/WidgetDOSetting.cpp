#include "WidgetDOSetting.h"
#include "ui_WidgetDOSetting.h"
#include "ItemDO.h"
#include "ModelGraphicsView.h"
#include "ModelCommon.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include "PublicClass.h"
#include"ItemAngelCtroller.h"
WidgetDOSetting::WidgetDOSetting(ItemDO *item_do, QWidget *parent):
    WidgetSetting(parent),
    ui(new Ui::WidgetDOSetting),
    item_do_(item_do)
{
    ui->setupUi(this);
    setType(WIDGET_DO);
    initWidgetData();
    ui->doubleSpinBox_r->setDecimals(3);
    //    ui->spinBox_id->setMaximum(item_do_->model_scene()->model_view()->basic_widget()->model_base_data_.max_do_num_-1);
    connect(item_do_->model_scene()->model_view()->basic_widget(),SIGNAL(sig_max_num_changed(quint8,quint8)),this,SLOT(slot_max_mum_changed(quint8,quint8)));
    connect(&item_do_->timer,SIGNAL(timeout()),this,SLOT(on_timer_out()));
}

WidgetDOSetting::~WidgetDOSetting()
{
    delete ui;
}

void WidgetDOSetting::initWidgetData()
{
    ui->spinBox_id->blockSignals(true);
    ui->spinBox_id->setValue (item_do_->data_do_.id_);
    ui->spinBox_id->blockSignals(false);
    ui->spinBox_id->setOriValue(item_do_->data_do_.id_);

    ui->doubleSpinBox_x->blockSignals(true);
    ui->doubleSpinBox_x->setValue (item_do_->data_do_.x_);
    ui->doubleSpinBox_x->blockSignals(false);
    ui->doubleSpinBox_x->setOriValue(item_do_->data_do_.x_);

    ui->doubleSpinBox_y->blockSignals(true);
    ui->doubleSpinBox_y->setValue (item_do_->data_do_.y_);
    ui->doubleSpinBox_y->blockSignals(false);
    ui->doubleSpinBox_y->setOriValue(item_do_->data_do_.y_);

    ui->doubleSpinBox_z->blockSignals(true);
    ui->doubleSpinBox_z->setValue (item_do_->data_do_.z_);
    ui->doubleSpinBox_z->blockSignals(false);
    ui->doubleSpinBox_z->setOriValue(item_do_->data_do_.z_);

    ui->doubleSpinBox_r->blockSignals(true);
    ui->doubleSpinBox_r->setValue (item_do_->data_do_.r_);
    ui->doubleSpinBox_r->blockSignals(false);
    ui->doubleSpinBox_r->setOriValue(item_do_->data_do_.r_);
    item_do_->getAngelCtroller()->setOriAngel(item_do_->data_do_.r_);
    ui->checkBox_inverse->blockSignals(true);
    ui->checkBox_inverse->setChecked (item_do_->data_do_.inverse_);
    ui->checkBox_inverse->blockSignals(false);
    ui->checkBox_inverse->setOriValue(item_do_->data_do_.inverse_);

    ConfigureData funcData = read_funcConfigFile();

    QList<ItemData>::iterator iter_func = funcData.dataList.begin();
    ui->comboBox_func->blockSignals(true);
    for(;iter_func!= funcData.dataList.end();iter_func++)
    {
        ui->comboBox_func->addItem((*iter_func).describtion,(*iter_func).index);
    }

    if(item_do_->isCreatedByLoading_)
    {
        int idx = ui->comboBox_func->findData(item_do_->data_do_.func_);
        ui->comboBox_func->setCurrentIndex(idx);
    }
    else
    {
        ui->comboBox_func->setCurrentIndex(funcData.defaultidx);
    }

    ui->comboBox_func->setOri_index(ui->comboBox_func->currentIndex());
    ui->comboBox_func->blockSignals(false);

    ConfigureData typeData = read_typeConfigFile();
    QList<ItemData>::iterator iter_type = typeData.dataList.begin();
    ui->comboBox_type->blockSignals(true);
    for(;iter_type!= typeData.dataList.end();iter_type++)
    {
        ui->comboBox_type->addItem((*iter_type).describtion,(*iter_type).index);
    }

    if(item_do_->isCreatedByLoading_)
    {
        int idx = ui->comboBox_type->findData(item_do_->data_do_.type_);
        ui->comboBox_type->setCurrentIndex(idx);
    }
    else
    {
        int idx = ui->comboBox_type->findData(typeData.defaultidx);
        ui->comboBox_type->setCurrentIndex(idx);
    }

    ui->comboBox_type->setOri_index(ui->comboBox_type->currentIndex());
    ui->comboBox_type->blockSignals(false);
}

ConfigureData WidgetDOSetting::read_funcConfigFile()
{
    return readConfigData(CONFIGFILE_PATH,"func");
}

ConfigureData WidgetDOSetting::read_typeConfigFile()
{
    return readConfigData(CONFIGFILE_PATH,"type");
}

ConfigureData WidgetDOSetting::readConfigData(const QString &filePath, const QString &argu)
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

void WidgetDOSetting::update_value()
{
    ui->spinBox_id->blockSignals(true);
    ui->spinBox_id->setValue (item_do_->data_do_.id_);
    ui->spinBox_id->blockSignals(false);

    ui->doubleSpinBox_x->blockSignals(true);
    ui->doubleSpinBox_x->setIsClickedTrigger(false);
    ui->doubleSpinBox_x->setValue (item_do_->data_do_.x_);
    ui->doubleSpinBox_x->blockSignals(false);
    ui->doubleSpinBox_y->blockSignals(true);
    ui->doubleSpinBox_y->setValue (item_do_->data_do_.y_);
    ui->doubleSpinBox_y->blockSignals(false);
    ui->doubleSpinBox_z->blockSignals(false);
    ui->doubleSpinBox_z->setValue (item_do_->data_do_.z_);
    ui->doubleSpinBox_z->blockSignals(false);
    ui->doubleSpinBox_r->blockSignals(true);
    ui->doubleSpinBox_r->setValue (item_do_->data_do_.r_);
    ui->doubleSpinBox_r->blockSignals(false);
    ui->comboBox_func->blockSignals(true);
    ui->comboBox_func->setCurrentIndex (item_do_->data_do_.func_);
    ui->comboBox_func->setOri_index(item_do_->data_do_.func_);
    ui->comboBox_func->blockSignals(false);
    ui->comboBox_type->blockSignals(true);
    ui->comboBox_type->setCurrentIndex (item_do_->data_do_.type_);
    ui->comboBox_type->setOri_index(item_do_->data_do_.type_);
    ui->comboBox_type->blockSignals(false);
    ui->checkBox_inverse->blockSignals(true);
    ui->checkBox_inverse->setChecked (item_do_->data_do_.inverse_);
    ui->checkBox_inverse->blockSignals(false);
}

void WidgetDOSetting::set_r(const qreal &angel)
{
    //非鼠标点击改变编辑框改变值，阻塞改变消息
    ui->doubleSpinBox_r->blockSignals(true);
    ui->doubleSpinBox_r->setValue(angel);
    ui->doubleSpinBox_r->blockSignals(false);
    //更新do图标角度
    item_do_->data_do_.r_=angel;
    item_do_->model_scene()->model_view()->update();
}

void WidgetDOSetting::on_spinBox_id_valueChanged(int arg1)
{
    if(item_do_->model_scene()->get_id_list(2).contains(QString::number(arg1)))
    {
        if(arg1 != item_do_->data_do_.id_)
        {
            ui->label_Prompt->setText(tr("The Id already exists"));
            ui->label_Prompt->setStyleSheet("color:red;");
        }
        else
        {
            item_do_->data_do_.id_=arg1;
            item_do_->update_item ();
            ui->label_Prompt->setText(tr("Setup success"));
            ui->label_Prompt->setStyleSheet("color::green;");
        }
        return;
    }
    else
    {
        item_do_->data_do_.id_=arg1;
        item_do_->update_item ();
        ui->label_Prompt->setText(tr("Setup success"));
        ui->label_Prompt->setStyleSheet("color::green;");
    }

    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->spinBox_id->getIsClickedTrigger())
    {
        ui->spinBox_id->setNewValue(arg1);

        SpinBoxCommand* spbx_command = new SpinBoxCommand(ui->spinBox_id,ui->spinBox_id->getOriValue(),
                                                          ui->spinBox_id->getNewValue(),this,NULL);

        m_unStack->push(spbx_command);
        ui->spinBox_id->setOriValue(arg1);

    }

    ui->spinBox_id->setIsClickedTrigger(true);
}

void WidgetDOSetting::on_doubleSpinBox_x_valueChanged(double arg1)
{
    item_do_->data_do_.x_=arg1;


    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_x->getIsClickedTrigger())
    {
        ui->doubleSpinBox_x->setNewValue(arg1);

        doubleSpinBoxCommand* dbspbx_command = new doubleSpinBoxCommand(ui->doubleSpinBox_x,item_do_,
                                                                        ui->doubleSpinBox_x->getOriValue(),
                                                                        ui->doubleSpinBox_x->getNewValue(),this,NULL);

        m_unStack->push(dbspbx_command);
        ui->doubleSpinBox_x->setOriValue(arg1);

    }

    ui->doubleSpinBox_x->setIsClickedTrigger(true);

    item_do_->update_item ();
}

void WidgetDOSetting::on_doubleSpinBox_y_valueChanged(double arg1)
{
    item_do_->data_do_.y_=arg1;


    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_y->getIsClickedTrigger())
    {
        ui->doubleSpinBox_y->setNewValue(arg1);

        doubleSpinBoxCommand* dbspbx_command = new doubleSpinBoxCommand(ui->doubleSpinBox_y,item_do_,
                                                                        ui->doubleSpinBox_y->getOriValue(),
                                                                        ui->doubleSpinBox_y->getNewValue(),this,NULL);

        m_unStack->push(dbspbx_command);
        ui->doubleSpinBox_y->setOriValue(arg1);

    }

    ui->doubleSpinBox_y->setIsClickedTrigger(true);
    item_do_->update_item ();
}

void WidgetDOSetting::on_doubleSpinBox_z_valueChanged(double arg1)
{
    item_do_->data_do_.z_=arg1;
    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_z->getIsClickedTrigger())
    {
        ui->doubleSpinBox_z->setNewValue(arg1);

        doubleSpinBoxCommand* dbspbx_command = new doubleSpinBoxCommand(ui->doubleSpinBox_z,item_do_,
                                                                        ui->doubleSpinBox_z->getOriValue(),
                                                                        ui->doubleSpinBox_z->getNewValue(),this,NULL);

        m_unStack->push(dbspbx_command);
        ui->doubleSpinBox_z->setOriValue(arg1);

    }

    ui->doubleSpinBox_z->setIsClickedTrigger(true);
}

void WidgetDOSetting::on_doubleSpinBox_r_valueChanged(double arg1)
{
    item_do_->data_do_.r_=arg1;

    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_r->getIsClickedTrigger())
    {
        ui->doubleSpinBox_r->setNewValue(arg1);

        doubleSpinBoxCommand* dbspbx_command = new doubleSpinBoxCommand(ui->doubleSpinBox_r,item_do_,
                                                                        ui->doubleSpinBox_r->getOriValue(),
                                                                        ui->doubleSpinBox_r->getNewValue(),this,NULL);

        m_unStack->push(dbspbx_command);

        ui->doubleSpinBox_r->setOriValue(arg1);
    }

    ui->doubleSpinBox_r->setIsClickedTrigger(true);
    item_do_->update_item ();
}

void WidgetDOSetting::on_comboBox_func_currentIndexChanged(int index)
{
    item_do_->data_do_.func_=index;

    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->comboBox_func->getIsClickedTrigger())
    {
        ui->comboBox_func->setNew_index(index);

        customboxCommand* cobx_command = new customboxCommand(ui->comboBox_func,
                                                              ui->comboBox_func->getOri_index(),ui->comboBox_func->getNew_index(),this,NULL);

        m_unStack->push(cobx_command);

        ui->comboBox_func->setOri_index(index);
    }

    ui->comboBox_func->setIsClickedTrigger(true);
}

void WidgetDOSetting::on_comboBox_type_currentIndexChanged(int index)
{
    item_do_->data_do_.type_=index;

    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->comboBox_type->getIsClickedTrigger())
    {
        ui->comboBox_type->setNew_index(index);

        customboxCommand* cobx_command = new customboxCommand(ui->comboBox_type,
                                                              ui->comboBox_type->getOri_index(),ui->comboBox_type->getNew_index(),this,NULL);

        m_unStack->push(cobx_command);

        ui->comboBox_type->setOri_index(index);
    }

    ui->comboBox_type->setIsClickedTrigger(true);
}

void WidgetDOSetting::on_checkBox_inverse_toggled(bool checked)
{
    item_do_->data_do_.inverse_=checked;

    if(m_unStack==NULL)
    {
        return;
    }

    ui->checkBox_inverse->setNewValue(checked);
    checkCommand* chk_command = new checkCommand(ui->checkBox_inverse,ui->checkBox_inverse->getOriValue(),
                                                 ui->checkBox_inverse->getNewValue(),this,NULL);

    m_unStack->push(chk_command);
    ui->checkBox_inverse->setOriValue(checked);
}

void WidgetDOSetting::slot_max_mum_changed(quint8 type, quint8 max_value)
{
    if(type==3)
    {
        ui->spinBox_id->setMaximum(max_value-1);
    }
}

void WidgetDOSetting::on_timer_out()
{
    ui->label_Prompt->setText("");
}

void WidgetDOSetting::set_func_byConfigure(int index)
{
    ui->comboBox_func->blockSignals(true);
    ui->comboBox_func->setCurrentIndex(index);
    ui->comboBox_func->setOri_index(index);
    ui->comboBox_func->blockSignals(false);
}

void WidgetDOSetting::set_type_byConfigure(int index)
{
    ui->comboBox_type->blockSignals(true);
    ui->comboBox_type->setCurrentIndex(index);
    ui->comboBox_type->setOri_index(index);
    ui->comboBox_type->blockSignals(false);
}

void WidgetDOSetting::set_inverse_byConfigure(bool arg)
{
    ui->checkBox_inverse->blockSignals(true);
    ui->checkBox_inverse->setChecked(arg);
    ui->checkBox_inverse->setOriValue(arg);
    ui->checkBox_inverse->blockSignals(false);
}

ItemBase *WidgetDOSetting::getRelateItem()
{
    return  item_do_;
}

void WidgetDOSetting::on_spinBox_id_editingFinished()
{
    //    int arg1=ui->spinBox_id->value();

    //    if(! item_do_->model_scene()->get_id_list(2).contains(QString::number(arg1)))
    //    {
    //        item_do_->data_do_.id_=arg1;
    //        item_do_->update_item ();
    //        ui->label_Prompt->setText(tr("Setup success"));
    //        ui->label_Prompt->setStyleSheet("color::green;");
    //    }
    //    else
    //    {
    //        if(arg1 != item_do_->data_do_.id_)
    //        {
    //            ui->spinBox_id->setValue(item_do_->data_do_.id_);
    //            ui->label_Prompt->setText(tr("The Id already exists"));
    //            ui->label_Prompt->setStyleSheet("color:red;");
    //        }
    //        else
    //        {
    //            ui->label_Prompt->setText(tr("Setup success"));
    //            ui->label_Prompt->setStyleSheet("color::green;");
    //        }
    //    }
    //500毫秒后隐藏
    item_do_->timer.setInterval(500);
    item_do_->timer.setSingleShot(true);
    item_do_->timer.start();
}
