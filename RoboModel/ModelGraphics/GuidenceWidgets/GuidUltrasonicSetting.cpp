#include "GuidUltrasonicSetting.h"
#include "ui_GuidUltrasonicSetting.h"
#include<QJsonObject>
#include<QJsonDocument>
#include<QJsonArray>
#include"StyleObject.h"
#include"NumSetting.h"
#include"UltrasonicContentSetting.h"
#include"RoLog.h"
GuidUltrasonicSetting::GuidUltrasonicSetting(QWidget *parent) :
    GuidBaseSetting(parent),
    ui(new Ui::GuidUltrasonicSetting),
    currentWidId_(0)
{
    ui->setupUi(this);
    setType(GUID_ULTRASONIC);
    ui->radioButton_set->setChecked(true);

    //初始化数量
    numSetting_ = new NumSetting;
    numSetting_->getNumBox()->setToolTip(tr("range[0,24]"));
    numSetting_->getNumBox()->setMaximum(24);
    numSetting_->setId(0);
    numSetting_->setNum(1);
    slot_enableNextStep();
   ui->vLayout_content->addWidget(numSetting_);
    QString imagePath= StyleObject::init()->getCurrentSkinDirPath()+"/images/RoModelPlugin/";
    numSetting_->setImagePath(imagePath+"Ultrasonic");
    numSetting_->setImageGroupBoxTitle(tr("Ultrasionic Picture"));
    numSetting_->setIntroduction(tr("The device is mainly used to detect obstacles and help robots react (stop, decelerate, etc.) when they encounter obstacles."));
    connect(numSetting_->getNumBox(),SIGNAL(valueChanged(int)),this,SLOT(slot_enableNextStep()));
}

GuidUltrasonicSetting::~GuidUltrasonicSetting()
{
    delete ui;
}


int GuidUltrasonicSetting::getUltrasonicType()
{
    return ultrasonicType_;
}


void GuidUltrasonicSetting::setNextPreBtnDisplay(bool flag)
{

}

void GuidUltrasonicSetting::setRadiouBtnstatus(bool flag)
{
    if(flag)
    {
        ui->radioButton_ignore->show();
        ui->radioButton_set->show();
    }
    else
    {
        ui->radioButton_ignore->hide();
        ui->radioButton_set->hide();
    }
}

void GuidUltrasonicSetting::showNumWid()
{
    for(int i = 0; i< ultrasonicContentList_.size();i++)
    {
        if(i==0)
        {
            ultrasonicContentList_.at(i)->show();
            ui->radioButton_ignore->show();
            ui->radioButton_set->show();
            currentWidId_ = 0;
        }
        else
        {
            ultrasonicContentList_.at(i)->hide();
        }
    }
}

void GuidUltrasonicSetting::slot_createSetingWidbyNum()
{
  //先清除
//    ultrasonicContentList_.clear();
//    while (ui->vLayout_content->count()!=0) {
//        ui->vLayout_content->takeAt(0);
//    }
//    //再添加
//    ultrasonicContentList_.append(numSetting_);
//    ui->vLayout_content->addWidget(numSetting_);

//    int num = numSetting_->getNum();

//    if(num==0)
//    {
//        ui->ptb_Start->setEnabled(false);
//        numSetting_->getLabel()->setText(tr(""));
//    }
//    else
//    {
//        ui->ptb_Start->setEnabled(true);
//        numSetting_->getLabel()->setText(tr("点击“进入设置”按钮进行属性设置。"));
//    }

//    for(int i  = 1; i <= num;i++)
//    {
//        UltrasonicContentSetting* temp = new UltrasonicContentSetting;
//        ultrasonicContentList_.append(temp);
//        ui->vLayout_content->addWidget(temp);
//        temp->setId(i);
//        temp->hide();

//        //连接信号与槽
//        connect(temp,SIGNAL(sig_range_valueChange()),this,SLOT(slot_enableNextStep()));
//    }
}

void GuidUltrasonicSetting::slot_enableNextStep()
{
    qDebug()<<"list size is -----------1"<<ultrasonicContentList_.size();
    if(!getConfigure())
    {
        emit sig_enableNextStep(true);
        return;
    }
    if(numSetting_->getNum()<1)
    {
        emit sig_enableNextStep(false);
        return;
    }
    emit sig_enableNextStep(true);
}

void GuidUltrasonicSetting::on_radioButton_set_clicked()
{
    setConfigure(true);
    emit sig_configure();
    //添加时将数量设置为0并且设置数量窗口不可用
    numSetting_->getNumBox()->setEnabled(true);
    numSetting_->setNum(1);
    slot_enableNextStep();//判断
}

void GuidUltrasonicSetting::on_radioButton_ignore_clicked()
{
    setConfigure(false);
    //不添加时将数量设置为0并且设置数量窗口不可用
    numSetting_->setNum(0);
    numSetting_->getNumBox()->setEnabled(false);
    emit sig_enableNextStep(true);

}



