#include "WidgetLaserSetting.h"
#include "ui_WidgetLaserSetting.h"
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonValue>
#include"ModelCustomCommand.h"
#include "ItemLaser.h"
#include "PublicClass.h"
#include"ItemAngelCtroller.h"
#include"ModelGraphicsView.h"
WidgetLaserSetting::WidgetLaserSetting(ItemLaser *item_laser,QWidget *parent) :
    WidgetSetting(parent),
    ui(new Ui::WidgetLaserSetting),
    item_laser_(item_laser)
{
    ui->setupUi(this);
     setType(WIDGET_LASER);
    ui->doubleSpinBox_r->setDecimals(3);
    ui->doubleSpinBox_max_angle->setDecimals(3);
    ui->doubleSpinBox_min_angle->setDecimals(3);
    connect(ui->widget_IP,SIGNAL(textChanged(const QString&)),this,SLOT(on_ip_changed(const QString&)));

    ConfigureData data = readLaserTypeFile(CONFIGFILE_PATH);

    QList<ItemData>::iterator iter = data.dataList.begin();

    for(;iter!= data.dataList.end();iter++)
    {
        ui->comboBox_model->blockSignals(true);
        ui->comboBox_model->addItem((*iter).describtion,(*iter).index);
        ui->comboBox_model->blockSignals(false);
    }
    ui->comboBox_model->blockSignals(true);
    if(item_laser->isCreatedByLoading_)
    {
        ui->comboBox_model->setCurrentText(item_laser->data_laser_.model_);
    }
    else
    {
        ui->comboBox_model->setCurrentIndex(data.defaultidx);
        item_laser->data_laser_.model_ = ui->comboBox_model->currentText();
    }
     item_laser->update();
    //    if(item_laser->data_laser_.model_!=ui->comboBox_model->itemText(data.defaultidx))
    //    {//默认存储的是字符串不是编号，要进行相应转换
    //    }
    //    else
    //    {
    //    }
    ui->comboBox_model->setOri_index(ui->comboBox_model->currentIndex());
    ui->comboBox_model->blockSignals(false);

    update_value ();
    //为旋转控制角设置初始值.
    item_laser->getAngelCtroller()->setOriAngel(item_laser_->data_laser_.r_);
    //set ori index for undo redo.
    ui->comboBox_model->setOri_index(ui->comboBox_model->currentIndex());
    ui->doubleSpinBox_x->setOriValue(ui->doubleSpinBox_x->value());
    ui->doubleSpinBox_r->setOriValue(ui->doubleSpinBox_r->value());
    ui->doubleSpinBox_z->setOriValue(ui->doubleSpinBox_z->value());
    ui->doubleSpinBox_y->setOriValue(ui->doubleSpinBox_y->value());
    ui->doubleSpinBox_step->setOriValue(ui->doubleSpinBox_step->value());
    ui->doubleSpinBox_min_angle->setOriValue(ui->doubleSpinBox_min_angle->value());
    ui->doubleSpinBox_max_angle->setOriValue(ui->doubleSpinBox_max_angle->value());
    ui->spinBox_port->setOriValue(item_laser_->data_laser_.port_);
    ui->checkBox_upside->setOriValue(item_laser_->data_laser_.upside_);
    ui->checkBox_useForLocalization->setOriValue(item_laser_->data_laser_.use_for_localization_);
    ui->widget_IP->setOriAddress(item_laser->data_laser_.ip_);


}

WidgetLaserSetting::~WidgetLaserSetting()
{
    delete ui;
}

ConfigureData WidgetLaserSetting::readLaserTypeFile(const QString &filePath)
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

void WidgetLaserSetting::update_value ()
{

    //  ui->comboBox_model->setCurrentText (item_laser_->data_laser_.model_);
    ui->doubleSpinBox_x->blockSignals(true);
    ui->doubleSpinBox_x->setValue (item_laser_->data_laser_.x_);
    ui->doubleSpinBox_x->blockSignals(false);
    ui->doubleSpinBox_y->blockSignals(true);
    ui->doubleSpinBox_y->setValue (item_laser_->data_laser_.y_);
    ui->doubleSpinBox_y->blockSignals(false);
    ui->doubleSpinBox_z->blockSignals(true);
    ui->doubleSpinBox_z->setValue (item_laser_->data_laser_.z_);
    ui->doubleSpinBox_z->blockSignals(false);
    ui->doubleSpinBox_r->blockSignals(true);
    ui->doubleSpinBox_r->setValue (item_laser_->data_laser_.r_);
    ui->doubleSpinBox_r->blockSignals(false);

    ui->checkBox_upside->blockSignals(true);
    ui->checkBox_upside->setChecked (item_laser_->data_laser_.upside_);
    ui->checkBox_upside->blockSignals(false);

    ui->checkBox_useForLocalization->blockSignals(true);
    ui->checkBox_useForLocalization->setChecked (item_laser_->data_laser_.use_for_localization_);
    ui->checkBox_useForLocalization->blockSignals(false);

    ui->doubleSpinBox_step->blockSignals(true);
    ui->doubleSpinBox_step->setValue (item_laser_->data_laser_.step_);
    ui->doubleSpinBox_step->blockSignals(false);

    ui->doubleSpinBox_min_angle->blockSignals(true);
    ui->doubleSpinBox_min_angle->setValue (item_laser_->data_laser_.min_angle_);
    ui->doubleSpinBox_min_angle->blockSignals(false);
    ui->doubleSpinBox_max_angle->blockSignals(true);
    ui->doubleSpinBox_max_angle->setValue (item_laser_->data_laser_.max_angle_);
    ui->doubleSpinBox_max_angle->blockSignals(false);

    ui->widget_IP->blockSignals(true);
    ui->widget_IP->setText(item_laser_->data_laser_.ip_);
    ui->widget_IP->blockSignals(false);
    ui->spinBox_port->blockSignals(true);
    ui->spinBox_port->setValue (item_laser_->data_laser_.port_);
    ui->spinBox_port->blockSignals(false);

}

void WidgetLaserSetting::set_r(const qreal &angel)
{
    //非鼠标点击改变编辑框改变值，阻塞改变消息
    ui->doubleSpinBox_r->blockSignals(true);
    ui->doubleSpinBox_r->setValue(angel);
    ui->doubleSpinBox_r->blockSignals(false);
    //更新laser图标角度
    item_laser_->data_laser_.r_=angel;
    item_laser_->model_scene()->model_view()->update();
}

void WidgetLaserSetting::set_upside_byGuidConfigure(bool arg)
{
    ui->checkBox_upside->blockSignals(true);
    ui->checkBox_upside->setChecked(arg);
    ui->checkBox_upside->setOriValue(arg);
    ui->checkBox_upside->blockSignals(false);
}

void WidgetLaserSetting::set_forLocation_byGuidConfigure(bool arg)
{
    ui->checkBox_useForLocalization->blockSignals(true);
    ui->checkBox_useForLocalization->setChecked(arg);
    ui->checkBox_useForLocalization->setOriValue(arg);
    ui->checkBox_useForLocalization->blockSignals(false);
}

void WidgetLaserSetting::set_step_byGuidConfigure(double arg)
{
    ui->doubleSpinBox_step->blockSignals(true);
    ui->doubleSpinBox_step->setValue(arg);
    ui->doubleSpinBox_step->setOriValue(arg);
    ui->doubleSpinBox_step->blockSignals(false);
}

void WidgetLaserSetting::set_minAngle_byGuidConfigure(double arg)
{
    ui->doubleSpinBox_min_angle->blockSignals(true);
    ui->doubleSpinBox_min_angle->setValue(arg);
    ui->doubleSpinBox_min_angle->setOriValue(arg);
    ui->doubleSpinBox_min_angle->blockSignals(false);
}

void WidgetLaserSetting::set_maxAngle_byGuidConfigure(double arg)
{
    ui->doubleSpinBox_max_angle->blockSignals(true);
    ui->doubleSpinBox_max_angle->setValue(arg);
    ui->doubleSpinBox_max_angle->setOriValue(arg);
    ui->doubleSpinBox_max_angle->blockSignals(false);
}

void WidgetLaserSetting::set_Ip_byGuidConfigure(QString &ip)
{
    ui->widget_IP->blockSignals(true);
    ui->widget_IP->setText(ip);
    ui->widget_IP->setOriAddress(ip);
    ui->widget_IP->blockSignals(false);
}

void WidgetLaserSetting::set_port_byGuidConfigure(int port)
{
    ui->spinBox_port->blockSignals(true);
    ui->spinBox_port->setValue(port);
    ui->spinBox_port->setOriValue(port);
    ui->spinBox_port->blockSignals(false);
}

void WidgetLaserSetting::set_model_byGuidConfigure(QString model)
{
    ui->comboBox_model->blockSignals(true);
    ui->comboBox_model->setCurrentText(model);
    ui->comboBox_model->setOri_index(ui->comboBox_model->currentIndex());
    ui->comboBox_model->blockSignals(false);
}

void WidgetLaserSetting::on_doubleSpinBox_r_valueChanged(double arg1)
{
    item_laser_->data_laser_.r_=arg1;

    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_r->getIsClickedTrigger())
    {
        ui->doubleSpinBox_r->setNewValue(ui->doubleSpinBox_r->value());

        doubleSpinBoxCommand* dbspinBox_command = new doubleSpinBoxCommand(ui->doubleSpinBox_r,item_laser_,
                                                                           ui->doubleSpinBox_r->getOriValue(),
                                                                           ui->doubleSpinBox_r->getNewValue(),
                                                                           this,NULL);

        m_unStack->push(dbspinBox_command);

        ui->doubleSpinBox_r->setOriValue(ui->doubleSpinBox_r->getNewValue());

    }
    ui->doubleSpinBox_r->setIsClickedTrigger(true);

    item_laser_->update_angle();
}

void WidgetLaserSetting::on_comboBox_model_currentIndexChanged(const QString &arg1)
{
    item_laser_->data_laser_.model_=arg1;
    item_laser_->update();

    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->comboBox_model->getIsClickedTrigger())
    {
        ui->comboBox_model->setNew_index(ui->comboBox_model->currentIndex());
        customboxCommand* combox_command = new customboxCommand(
                    ui->comboBox_model,ui->comboBox_model->getOri_index(),
                    ui->comboBox_model->getNew_index(),this,NULL);
        m_unStack->push(combox_command);
        ui->comboBox_model->setOri_index(ui->comboBox_model->getNew_index());
    }

    ui->comboBox_model->setIsClickedTrigger(true);

}

void WidgetLaserSetting::on_doubleSpinBox_x_valueChanged(double arg1)
{

    item_laser_->data_laser_.x_=arg1;
    item_laser_->update_pos ();

    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_x->getIsClickedTrigger())
    {

        ui->doubleSpinBox_x->setNewValue(arg1);

        doubleSpinBoxCommand* dbspinBox_command = new doubleSpinBoxCommand(ui->doubleSpinBox_x,item_laser_,
                                                                           ui->doubleSpinBox_x->getOriValue(),
                                                                           ui->doubleSpinBox_x->getNewValue(),
                                                                           this,NULL);
        m_unStack->push(dbspinBox_command);

        ui->doubleSpinBox_x->setOriValue(arg1);

    }
    ui->doubleSpinBox_x->setIsClickedTrigger(true);
}

void WidgetLaserSetting::on_doubleSpinBox_y_valueChanged(double arg1)
{
    item_laser_->data_laser_.y_=arg1;
    item_laser_->update_pos ();

    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_y->getIsClickedTrigger())
    {
        ui->doubleSpinBox_y->setNewValue(arg1);

        doubleSpinBoxCommand* dbspinBox_command = new doubleSpinBoxCommand(ui->doubleSpinBox_y,item_laser_,
                                                                           ui->doubleSpinBox_y->getOriValue(),
                                                                           ui->doubleSpinBox_y->getNewValue(),
                                                                           this,NULL);

        m_unStack->push(dbspinBox_command);

        ui->doubleSpinBox_y->setOriValue(arg1);

    }
    ui->doubleSpinBox_y->setIsClickedTrigger(true);
}

void WidgetLaserSetting::on_doubleSpinBox_z_valueChanged(double arg1)
{
    item_laser_->data_laser_.z_=arg1;

    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_z->getIsClickedTrigger())
    {
        ui->doubleSpinBox_z->setNewValue(arg1);

        doubleSpinBoxCommand* dbspinBox_command = new doubleSpinBoxCommand(ui->doubleSpinBox_z,item_laser_,
                                                                           ui->doubleSpinBox_z->getOriValue(),
                                                                           ui->doubleSpinBox_z->getNewValue(),
                                                                           this,NULL);

        m_unStack->push(dbspinBox_command);

        ui->doubleSpinBox_z->setOriValue(arg1);

    }
    ui->doubleSpinBox_z->setIsClickedTrigger(true);
}

void WidgetLaserSetting::on_checkBox_upside_toggled(bool checked)
{
    item_laser_->data_laser_.upside_=checked;

    if(m_unStack==NULL)
    {
        return;
    }

    ui->checkBox_upside->setNewValue(checked);
    checkCommand* check_command = new checkCommand(ui->checkBox_upside,ui->checkBox_upside->getOriValue()
                                                   ,ui->checkBox_upside->getNewValue(),this,NULL);
    m_unStack->push(check_command);
    ui->checkBox_upside->setOriValue(checked);

}

void WidgetLaserSetting::on_checkBox_useForLocalization_toggled(bool checked)
{
    item_laser_->data_laser_.use_for_localization_=checked;
    if(m_unStack==NULL)
    {
        return;
    }
    ui->checkBox_useForLocalization->setNewValue(checked);
    checkCommand* check_command = new checkCommand(ui->checkBox_useForLocalization,
                                                   ui->checkBox_useForLocalization->getOriValue(),
                                                   ui->checkBox_useForLocalization->getNewValue(),this,NULL);
    m_unStack->push(check_command);
    ui->checkBox_useForLocalization->setOriValue(checked);
}



void WidgetLaserSetting::on_doubleSpinBox_step_valueChanged(double arg1)
{
    item_laser_->data_laser_.step_=arg1;
    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_step ->getIsClickedTrigger())
    {
        ui->doubleSpinBox_step ->setNewValue(arg1);

        doubleSpinBoxCommand* dbspinBox_command = new doubleSpinBoxCommand(ui->doubleSpinBox_step,item_laser_,
                                                                           ui->doubleSpinBox_step->getOriValue(),
                                                                           ui->doubleSpinBox_step->getNewValue(),
                                                                           this,NULL);

        m_unStack->push(dbspinBox_command);
        ui->doubleSpinBox_step->setOriValue(arg1);

    }
    ui->doubleSpinBox_step->setIsClickedTrigger(true);
}

void WidgetLaserSetting::on_doubleSpinBox_min_angle_valueChanged(double arg1)
{
//    ui->doubleSpinBox_max_angle->setMinimum(arg1);
//    ui->doubleSpinBox_min_angle->setMaximum(ui->doubleSpinBox_max_angle->value());
    item_laser_->data_laser_.min_angle_=arg1;
    if(m_unStack==NULL)
    {
        return;
    }
    if(ui->doubleSpinBox_min_angle ->getIsClickedTrigger())
    {
        ui->doubleSpinBox_min_angle ->setNewValue(arg1);

        doubleSpinBoxCommand* dbspinBox_command = new doubleSpinBoxCommand(ui->doubleSpinBox_min_angle,item_laser_,
                                                                           ui->doubleSpinBox_min_angle->getOriValue(),
                                                                           ui->doubleSpinBox_min_angle->getNewValue(),
                                                                           this,NULL);

        m_unStack->push(dbspinBox_command);

        ui->doubleSpinBox_min_angle->setOriValue(arg1);

    }
    ui->doubleSpinBox_min_angle->setIsClickedTrigger(true);
}

void WidgetLaserSetting::on_doubleSpinBox_max_angle_valueChanged(double arg1)
{
//    ui->doubleSpinBox_min_angle->setMaximum(arg1);
//    ui->doubleSpinBox_max_angle->setMinimum(ui->doubleSpinBox_min_angle->value());
    item_laser_->data_laser_.max_angle_=arg1;
    if(m_unStack==NULL)
    {
        return;
    }
    if(ui->doubleSpinBox_max_angle ->getIsClickedTrigger())
    {

        ui->doubleSpinBox_max_angle ->setNewValue(arg1);

        doubleSpinBoxCommand* dbspinBox_command = new doubleSpinBoxCommand(ui->doubleSpinBox_max_angle,item_laser_,
                                                                           ui->doubleSpinBox_max_angle->getOriValue(),
                                                                           ui->doubleSpinBox_max_angle->getNewValue(),
                                                                           this,NULL);

        m_unStack->push(dbspinBox_command);

        ui->doubleSpinBox_max_angle->setOriValue(arg1);

    }
    ui->doubleSpinBox_max_angle->setIsClickedTrigger(true);
}

void WidgetLaserSetting::on_ip_changed(const QString &newIp)
{
    item_laser_->data_laser_.ip_=newIp;

    if(m_unStack==NULL)
    {
        return;
    }

    ui->widget_IP->setNewAddress(newIp);
    ipEditCommand* ip_command = new ipEditCommand(ui->widget_IP,ui->widget_IP->getOriAddress(),
                                                  ui->widget_IP->getNewAddress(),this,NULL);
    m_unStack->push(ip_command);
    ui->widget_IP->setOriAddress(newIp);
}

void WidgetLaserSetting::on_spinBox_port_valueChanged(int arg1)
{
    item_laser_->data_laser_.port_=arg1;

    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->spinBox_port->getIsClickedTrigger())
    {
        ui->spinBox_port->setNewValue(arg1);
        SpinBoxCommand* spin_command = new SpinBoxCommand(ui->spinBox_port,ui->spinBox_port->getOriValue(),
                                                          ui->spinBox_port->getNewValue(),this,NULL);
        m_unStack->push(spin_command);
        ui->spinBox_port->setOriValue(arg1);
    }
    ui->spinBox_port->setIsClickedTrigger(true);
}

ItemBase *WidgetLaserSetting::getRelateItem()
{
    return item_laser_;
}
