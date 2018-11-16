#include "GuidoptionalDevice.h"
#include "ui_GuidoptionalDevice.h"

GuidoptionalDevice::GuidoptionalDevice(QWidget *parent) :
    GuidBaseSetting(parent),
    ui(new Ui::GuidoptionalDevice)
{
    ui->setupUi(this);
    setType(GUID_OPTIONALDEVICE);
    basicData_.auto_brake_ = false;
    basicData_.LED_ = true;
    basicData_.brake_ = true;
    basicData_.auto_gyro_cal_ = true;
    ui->checkBox_existBrake->setChecked(true);
    ui->checkBox_autoZero->setChecked(true);
    ui->checkBox_autoBrake->setChecked(false);
    ui->checkBox_existLED->setChecked(true);
}

GuidoptionalDevice::~GuidoptionalDevice()
{
    delete ui;
}

void GuidoptionalDevice::on_checkBox_existBrake_clicked(bool checked)
{
    basicData_.brake_ = checked;
    if(checked)
    {
        ui->checkBox_autoBrake->setEnabled(true);
    }
    else
    {
        ui->checkBox_autoBrake->setChecked(false);
        ui->checkBox_autoBrake->setEnabled(false);
        basicData_.auto_brake_ = false;
    }
}

void GuidoptionalDevice::on_checkBox_existLED_clicked(bool checked)
{
    basicData_.LED_ = checked;
}

void GuidoptionalDevice::on_checkBox_autoBrake_clicked(bool checked)
{
    basicData_.auto_brake_ = checked;
}

void GuidoptionalDevice::on_checkBox_autoZero_clicked(bool checked)
{
    basicData_.auto_gyro_cal_ = checked;
}
