#include "GuidBasicSetting.h"
#include "ui_GuidBasicSetting.h"

GuidBasicSetting::GuidBasicSetting(QWidget *parent) :
    GuidBaseSetting(parent),
    ui(new Ui::GuidBasicSetting)
{
    ui->setupUi(this);
    setType(GUID_BASIC);
    enableNextStep();

    ui->spinBox_encoderLine->setValue(2500);
    ui->spinBox_maxSpeed->setValue(3000);
    ui->spinBox_reduceRadio->setValue(40);
    basicData_.model_name_="untitled";
    basicData_.encoder_line_ = 2500;
    basicData_.max_motor_rpm_ = 3000;
    basicData_.reduction_ratio_ = 40;
    basicData_.auto_brake_ = true;
    basicData_.LED_ = true;
//    ui->label_6->setFixedWidth(185);

}

GuidBasicSetting::~GuidBasicSetting()
{
    delete ui;
}


void GuidBasicSetting::enableNextStep()
{
    if(ui->spinBox_encoderLine->value()==0)
    {
        emit sig_enableNextStep(false);
        return;
    }
    else if(ui->spinBox_maxSpeed->value()==0)
    {
        emit sig_enableNextStep(false);
        return;
    }
    else if(ui->spinBox_reduceRadio->value() == 0)
    {
        emit sig_enableNextStep(false);
        return;
    }
    else
    {
        emit sig_enableNextStep(true);
    }
}

void GuidBasicSetting::on_spinBox_reduceRadio_valueChanged(int arg1)
{
    basicData_.reduction_ratio_ = arg1;
    enableNextStep();
}

void GuidBasicSetting::on_spinBox_encoderLine_valueChanged(int arg1)
{
    basicData_.encoder_line_ = arg1;
    enableNextStep();
}

void GuidBasicSetting::on_spinBox_maxSpeed_valueChanged(int arg1)
{
    basicData_.max_motor_rpm_ = arg1;
    enableNextStep();
}
