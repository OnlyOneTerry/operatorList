#include "NumSetting.h"
#include "ui_NumSetting.h"
#include"RoLog.h"
NumSetting::NumSetting(QWidget *parent) :
    BasicContentSetting(parent),
    ui(new Ui::NumSetting),
    m_Num(0)
{
    ui->setupUi(this);
//    ui->groupBox_image->setFixedWidth(188);
}

NumSetting::~NumSetting()
{
    SCDebug<<"~NumSetting()--------------";
    delete ui;
}

int NumSetting::getNum()
{
    return m_Num;
}

void NumSetting::setNum(int num)
{
    m_Num = num;
    ui->spinBox_num->setValue(num);
}

QSpinBox *NumSetting::getNumBox()
{
    return ui->spinBox_num;
}

void NumSetting::setImagePath(QString path)
{
    ui->label_image->setPixmap(QPixmap(QString("%1").arg(path)));
}

void NumSetting::setIntroduction(QString &intro)
{
    ui->label_introduction->setText(intro);
}

void NumSetting::setImageGroupBoxTitle(QString &title)
{
    ui->groupBox_image->setTitle(title);
}

QLabel *NumSetting::getLabelIntro()
{
    return ui->label_introduction;
}

void NumSetting::on_spinBox_num_valueChanged(int arg1)
{
    m_Num = arg1;
}
