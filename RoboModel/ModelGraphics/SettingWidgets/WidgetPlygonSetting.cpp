#include "WidgetPlygonSetting.h"
#include "ui_WidgetPlygonSetting.h"
#include "ItemPolygon.h"
#include"ModelCommon.h"
#include"ModelCustomCommand.h"
#include"ModelCustomWidget.h"
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonValue>
#include "PublicClass.h"

WidgetPlygonSetting::WidgetPlygonSetting(const int& type,ItemPolygon *item_polygon,QWidget *parent) :
    WidgetSetting(parent),
    ui(new Ui::WidgetPlygonSetting),
    item_polygon_(item_polygon)
{
    ui->setupUi(this);

    ui->Label_maxSpeed->setVisible(!type);
    ui->QdoubleSpinBox_maxSpeed->setVisible(!type);
    switch (type) {
    case 0:
    {
        ui->label_type->setText (tr("Deceleration polygon"));
    }break;
    case 1:
    {
        ui->label_type->setText (tr("Stop barrier polygon"));
        ui->groupBox_2->hide();
    }break;
    case 2:
    {
        ui->label_type->setText (tr("Security polygon"));
        ui->groupBox_2->hide();
    }break;
    default:
    {
    }break;
    }

    update_value();
}

WidgetPlygonSetting::~WidgetPlygonSetting()
{
    delete ui;
}

void WidgetPlygonSetting::update_value ()
{
    ui->QdoubleSpinBox_maxSpeed->blockSignals(true);
    ui->QdoubleSpinBox_maxSpeed->setValue(item_polygon_->data_polygon_.max_speed_);
    ui->QdoubleSpinBox_maxSpeed->blockSignals(false);
    ui->QdoubleSpinBox_maxSpeed->setOriValue(ui->QdoubleSpinBox_maxSpeed->value());
}

void WidgetPlygonSetting::on_QdoubleSpinBox_maxSpeed_valueChanged(double arg1)
{
    item_polygon_->data_polygon_.max_speed_=arg1;
    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->QdoubleSpinBox_maxSpeed->getIsClickedTrigger())
    {
        ui->QdoubleSpinBox_maxSpeed->setNewValue(arg1);
        doubleSpinBoxCommand* dbSpinx_command = new doubleSpinBoxCommand(
                    ui->QdoubleSpinBox_maxSpeed,NULL,
                    ui->QdoubleSpinBox_maxSpeed->getOriValue(),
                    ui->QdoubleSpinBox_maxSpeed->getNewValue(),NULL);
        m_unStack->push(dbSpinx_command);

        ui->QdoubleSpinBox_maxSpeed->setOriValue(arg1);
    }

    ui->QdoubleSpinBox_maxSpeed->setIsClickedTrigger(true);

}

void WidgetPlygonSetting::on_comboBox_workingMode_currentIndexChanged(int index)
{
    item_polygon_->data_polygon_.working_mode_=index;

    if(m_unStack==NULL)
    {
        return;
    }
}

ItemBase *WidgetPlygonSetting::getRelateItem()
{
    return item_polygon_;
}
