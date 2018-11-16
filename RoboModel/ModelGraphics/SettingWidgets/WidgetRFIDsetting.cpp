#include "WidgetRFIDsetting.h"
#include "ui_WidgetRFIDsetting.h"
#include"ItemRFID.h"
#include"ModelCustomCommand.h"
WidgetRFIDsetting::WidgetRFIDsetting(ItemRFIDSensor *rfid, QWidget *parent) :
    WidgetSetting(parent),
    itemRFID_(rfid),
    ui(new Ui::WidgetRFIDsetting)
{
    ui->setupUi(this);
     setType(WIDGET_RFID);
    initWidgetData();
}

WidgetRFIDsetting::~WidgetRFIDsetting()
{
    delete ui;
}

void WidgetRFIDsetting::update_value()
{
    ui->doubleSpinBox_x->blockSignals(true);
    ui->doubleSpinBox_x->setValue(itemRFID_->data_RFID_.x_);
    ui->doubleSpinBox_x->blockSignals(false);
    ui->doubleSpinBox_y->blockSignals(true);
    ui->doubleSpinBox_y->setValue(itemRFID_->data_RFID_.y_);
    ui->doubleSpinBox_y->blockSignals(false);
    ui->doubleSpinBox_z->blockSignals(true);
    ui->doubleSpinBox_z->setValue(itemRFID_->data_RFID_.z_);
    ui->doubleSpinBox_z->blockSignals(false);
}

void WidgetRFIDsetting::initWidgetData()
{
    ui->doubleSpinBox_x->blockSignals(true);
    ui->doubleSpinBox_x->setValue(itemRFID_->data_RFID_.x_);
    ui->doubleSpinBox_x->setOriValue(itemRFID_->data_RFID_.x_);
    ui->doubleSpinBox_x->blockSignals(false);
    ui->doubleSpinBox_y->blockSignals(true);
    ui->doubleSpinBox_y->setValue(itemRFID_->data_RFID_.y_);
    ui->doubleSpinBox_y->setOriValue(itemRFID_->data_RFID_.y_);
    ui->doubleSpinBox_y->blockSignals(false);
    ui->doubleSpinBox_z->blockSignals(true);
    ui->doubleSpinBox_z->setValue(itemRFID_->data_RFID_.z_);
    ui->doubleSpinBox_z->setOriValue(itemRFID_->data_RFID_.z_);
    ui->doubleSpinBox_z->blockSignals(false);
}

void WidgetRFIDsetting::on_doubleSpinBox_x_valueChanged(double arg1)
{
    qDebug()<<"on_doubleSpinBox_x_valueChanged-----"<<arg1;
    itemRFID_->data_RFID_.x_ = arg1;
    itemRFID_->update_item();

    if(m_unStack==NULL) return;

    if(ui->doubleSpinBox_x->getIsClickedTrigger())
    {

        ui->doubleSpinBox_x->setNewValue(arg1);

        doubleSpinBoxCommand* dbspinBox_command = new doubleSpinBoxCommand(ui->doubleSpinBox_x,itemRFID_,
                                                                           ui->doubleSpinBox_x->getOriValue(),
                                                                           ui->doubleSpinBox_x->getNewValue(),
                                                                           this,NULL);
        m_unStack->push(dbspinBox_command);
        qDebug()<<"on_doubleSpinBox_x_valueChanged-----";
        ui->doubleSpinBox_x->setOriValue(arg1);

    }
    ui->doubleSpinBox_x->setIsClickedTrigger(true);
}

void WidgetRFIDsetting::on_doubleSpinBox_y_valueChanged(double arg1)
{
    itemRFID_->data_RFID_.y_ = arg1;
    itemRFID_->update_item();
    if(m_unStack==NULL) return;

    if(ui->doubleSpinBox_y->getIsClickedTrigger())
    {
        ui->doubleSpinBox_y->setNewValue(arg1);
        doubleSpinBoxCommand* dbspinBox_command = new doubleSpinBoxCommand(ui->doubleSpinBox_y,itemRFID_,
                                                                           ui->doubleSpinBox_y->getOriValue(),
                                                                           ui->doubleSpinBox_y->getNewValue(),
                                                                           this,NULL);
        m_unStack->push(dbspinBox_command);
        ui->doubleSpinBox_y->setOriValue(arg1);
    }
    ui->doubleSpinBox_y->setIsClickedTrigger(true);
}

ItemBase *WidgetRFIDsetting::getRelateItem()
{
    return itemRFID_;
}
