#include "WidgetCameraSetting.h"
#include "ui_WidgetCameraSetting.h"
#include<QJsonDocument>
#include<QJsonArray>
#include<QJsonObject>
#include"UndoStack.h"
#include"ModelCustomCommand.h"
#include"ItemCamera.h"
WidgetCameraSetting::WidgetCameraSetting(ItemCamera *item, QWidget *parent) :
    WidgetSetting(parent),
    ui(new Ui::WidgetCameraSetting),
    cameral_(item)
{
    ui->setupUi(this);
    setType(WIDGET_VISION);
    initWigetdata();
}

WidgetCameraSetting::~WidgetCameraSetting()
{
    delete ui;
}
//场景item位置改变同步属性框数据
void WidgetCameraSetting::update_value()
{
    ui->doubleSpinBox_x->blockSignals(true);
    ui->doubleSpinBox_x->setValue(cameral_->data_camera_.x_);
    ui->doubleSpinBox_x->blockSignals(false);
    ui->doubleSpinBox_y->blockSignals(true);
    ui->doubleSpinBox_y->setValue(cameral_->data_camera_.y_);
    ui->doubleSpinBox_y->blockSignals(false);
}

void WidgetCameraSetting::initWigetdata()
{
    ui->doubleSpinBox_x->blockSignals(true);
    ui->doubleSpinBox_x->setValue(cameral_->data_camera_.x_);
    ui->doubleSpinBox_x->blockSignals(false);
    ui->doubleSpinBox_y->blockSignals(true);
    ui->doubleSpinBox_y->setValue(cameral_->data_camera_.y_);
    ui->doubleSpinBox_y->blockSignals(false);
    ui->doubleSpinBox_z->blockSignals(true);
    ui->doubleSpinBox_z->setValue(cameral_->data_camera_.z_);
    ui->doubleSpinBox_z->blockSignals(false);
    ui->doubleSpinBox_pitch->blockSignals(true);
    ui->doubleSpinBox_pitch->setValue(cameral_->data_camera_.pitch_);
    ui->doubleSpinBox_pitch->blockSignals(false);
    ui->doubleSpinBox_roll->blockSignals(true);
    ui->doubleSpinBox_roll->setValue(cameral_->data_camera_.roll_);
    ui->doubleSpinBox_roll->blockSignals(false);
    ui->doubleSpinBox_yaw->blockSignals(true);
    ui->doubleSpinBox_yaw->setValue(cameral_->data_camera_.yaw_);
    ui->doubleSpinBox_yaw->blockSignals(false);

    ConfigureData typeData  = readConfigData(CONFIGFILE_PATH);
    QList<ItemData>::iterator iter_type = typeData.dataList.begin();
    ui->comboBox_type->blockSignals(true);
    for(;iter_type!= typeData.dataList.end();iter_type++)
    {
        ui->comboBox_type->addItem((*iter_type).describtion,(*iter_type).index);
    }

    if(cameral_->isCreatedByLoading_)
    {
        int idx = ui->comboBox_type->findData(cameral_->data_camera_.type_);
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

ItemBase *WidgetCameraSetting::getRelateItem()
{
    return cameral_;
}

ConfigureData WidgetCameraSetting::readConfigData(const QString &filePath)
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

    QJsonValue  cameral_value= chassis_obj["vision"];
    QJsonObject cameral_obj = cameral_value.toObject();

    ConfigureData  cfData;

    QJsonValue defaultIdex = cameral_obj["defaultIndex"];

    cfData.defaultidx = defaultIdex.toInt();
    QJsonArray typeList = cameral_obj["typeList"].toArray();

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

void WidgetCameraSetting::on_comboBox_type_currentIndexChanged(int index)
{
    cameral_->data_camera_.type_ = ui->comboBox_type->currentData().toInt();

    if(m_unStack==NULL) m_unStack =UndoStack::InitStack();

    if(ui->comboBox_type->getIsClickedTrigger())
    {
        ui->comboBox_type->setNew_index(index);

        customboxCommand* combx_command = new customboxCommand(ui->comboBox_type,ui->comboBox_type->getOri_index(),
                                                               ui->comboBox_type->getNew_index(),this,NULL);
        m_unStack->push(combx_command);

        ui->comboBox_type->setOri_index(index);
    }
    ui->comboBox_type->setIsClickedTrigger(true);
}

void WidgetCameraSetting::on_doubleSpinBox_x_valueChanged(double arg1)
{
    cameral_->data_camera_.x_ = arg1;
    if(m_unStack==NULL) m_unStack =UndoStack::InitStack();

    if(ui->doubleSpinBox_x->getIsClickedTrigger())
    {
        ui->doubleSpinBox_x->setNewValue(arg1);
        doubleSpinBoxCommand* dbspinbox_command = new doubleSpinBoxCommand(ui->doubleSpinBox_x,cameral_,ui->doubleSpinBox_x->getOriValue(),
                                                                           ui->doubleSpinBox_x->getNewValue(),this,NULL);
        m_unStack->push(dbspinbox_command);

        ui->doubleSpinBox_x->setOriValue(arg1);
    }
    ui->doubleSpinBox_x->setIsClickedTrigger(true);
    cameral_->update_item();
}

void WidgetCameraSetting::on_doubleSpinBox_y_valueChanged(double arg1)
{
    cameral_->data_camera_.y_ = arg1;
    if(ui->doubleSpinBox_y->getIsClickedTrigger())
    {
        ui->doubleSpinBox_y->setNewValue(arg1);
        doubleSpinBoxCommand* dbspinbox_command = new doubleSpinBoxCommand(ui->doubleSpinBox_y,cameral_,ui->doubleSpinBox_y->getOriValue(),
                                                                           ui->doubleSpinBox_y->getNewValue(),this,NULL);
        m_unStack->push(dbspinbox_command);

        ui->doubleSpinBox_y->setOriValue(arg1);
    }
    ui->doubleSpinBox_y->setIsClickedTrigger(true);
    cameral_->update_item();
}

void WidgetCameraSetting::on_doubleSpinBox_z_valueChanged(double arg1)
{
    cameral_->data_camera_.z_ = arg1;
    if(ui->doubleSpinBox_z->getIsClickedTrigger())
    {
        ui->doubleSpinBox_z->setNewValue(arg1);
        doubleSpinBoxCommand* dbspinbox_command = new doubleSpinBoxCommand(ui->doubleSpinBox_z,cameral_,ui->doubleSpinBox_z->getOriValue(),
                                                                           ui->doubleSpinBox_z->getNewValue(),this,NULL);
        m_unStack->push(dbspinbox_command);

        ui->doubleSpinBox_z->setOriValue(arg1);
    }
    ui->doubleSpinBox_z->setIsClickedTrigger(true);
    cameral_->update_item();
}

void WidgetCameraSetting::on_doubleSpinBox_pitch_valueChanged(double arg1)
{
    cameral_->data_camera_.pitch_ = arg1;
    if(ui->doubleSpinBox_pitch->getIsClickedTrigger())
    {
        ui->doubleSpinBox_pitch->setNewValue(arg1);
        doubleSpinBoxCommand* dbspinbox_command = new doubleSpinBoxCommand(ui->doubleSpinBox_pitch,cameral_,ui->doubleSpinBox_pitch->getOriValue(),
                                                                           ui->doubleSpinBox_pitch->getNewValue(),this,NULL);
        m_unStack->push(dbspinbox_command);

        ui->doubleSpinBox_pitch->setOriValue(arg1);
    }
    ui->doubleSpinBox_pitch->setIsClickedTrigger(true);

    cameral_->update_item();
}

void WidgetCameraSetting::on_doubleSpinBox_roll_valueChanged(double arg1)
{
    cameral_->data_camera_.roll_ = arg1;
    if(ui->doubleSpinBox_roll->getIsClickedTrigger())
    {
        ui->doubleSpinBox_roll->setNewValue(arg1);
        doubleSpinBoxCommand* dbspinbox_command = new doubleSpinBoxCommand(ui->doubleSpinBox_roll,cameral_,ui->doubleSpinBox_roll->getOriValue(),
                                                                           ui->doubleSpinBox_roll->getNewValue(),this,NULL);
        m_unStack->push(dbspinbox_command);

        ui->doubleSpinBox_roll->setOriValue(arg1);
    }
    ui->doubleSpinBox_roll->setIsClickedTrigger(true);

    cameral_->update_item();
}

void WidgetCameraSetting::on_doubleSpinBox_yaw_valueChanged(double arg1)
{
    cameral_->data_camera_.yaw_ = arg1;
    if(ui->doubleSpinBox_yaw->getIsClickedTrigger())
    {
        ui->doubleSpinBox_yaw->setNewValue(arg1);
        doubleSpinBoxCommand* dbspinbox_command = new doubleSpinBoxCommand(ui->doubleSpinBox_yaw,cameral_,ui->doubleSpinBox_yaw->getOriValue(),
                                                                           ui->doubleSpinBox_yaw->getNewValue(),this,NULL);
        m_unStack->push(dbspinbox_command);

        ui->doubleSpinBox_yaw->setOriValue(arg1);
    }
    ui->doubleSpinBox_yaw->setIsClickedTrigger(true);

    cameral_->update_item();
}
