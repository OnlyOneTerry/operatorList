#include "GuidMagneticSensorSetting.h"
#include "ui_GuidMagneticSensorSetting.h"
#include<QJsonObject>
#include<QJsonDocument>
#include<QJsonArray>
#include"StyleObject.h"
#include"NumSetting.h"
#include"MagSensorContentSetting.h"
GuidMagneticSensorSetting::GuidMagneticSensorSetting(QWidget *parent) :
    GuidBaseSetting(parent),
    ui(new Ui::GuidMagneticSensorSetting),
    currentWidId_(0)
{
    ui->setupUi(this);
    setType(GUID_MAGENATICSENSOR);
    ui->radioButton_set->setChecked(true);

    //初始化数量
    numSetting_ = new NumSetting;
    numSetting_->setId(0);
    numSetting_->setNum(1);
    setNextPreBtnDisplay(false);//初始化设置按钮
    slot_enableNextStep();
    connect(numSetting_->getNumBox(),SIGNAL(valueChanged(int)),this,SLOT(slot_enableNextStep()));
    ui->vLayout_content->addWidget(numSetting_);
    QString imagePath= StyleObject::init()->getCurrentSkinDirPath()+"/images/RoModelPlugin/";
    numSetting_->setImagePath(imagePath+"MagneticSensor");
    numSetting_->getNumBox()->setToolTip(tr("range[0,5]"));
    numSetting_->getNumBox()->setMaximum(5);
    numSetting_->setImageGroupBoxTitle(tr("MagneticSensor Picture"));
    numSetting_->setIntroduction(tr("The device is suitable for magnetic stripe navigation and is used in conjunction with RFID."));
}

GuidMagneticSensorSetting::~GuidMagneticSensorSetting()
{
    delete ui;
}

int GuidMagneticSensorSetting::getMagneticSensorType()
{
    return magnetic_sensorType_;
}


void GuidMagneticSensorSetting::setRadiouBtnstatus(bool flag)
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

void GuidMagneticSensorSetting::showNumWid()
{
    for(int i = 0; i< magnSensorContentList_.size();i++)
    {
        if(i==0)
        {
            magnSensorContentList_.at(i)->show();
            setNextPreBtnDisplay(false);//隐藏切换按钮
//            ui->ptb_Start->show();//显示配置按钮
            ui->radioButton_ignore->show();
            ui->radioButton_set->show();
            currentWidId_ = 0;
        }
        else
        {
            magnSensorContentList_.at(i)->hide();
        }
    }
}

void GuidMagneticSensorSetting::slot_createSetingWidbyNum()
{
  //先清除
//    magnSensorContentList_.clear();
//      while (ui->vLayout_content->count()!=0) {
//          ui->vLayout_content->takeAt(0);
//      }
//      //再添加
//      magnSensorContentList_.append(numSetting_);
//      ui->vLayout_content->addWidget(numSetting_);

//      int num = numSetting_->getNum();

//      if(num==0)
//      {
//          ui->ptb_Start->setEnabled(false);
//          numSetting_->getLabel()->setText(tr(""));
//      }
//      else
//      {
//          ui->ptb_Start->setEnabled(true);
//      }

//      for(int i  = 1; i <= num;i++)
//      {
//          MagSensorContentSetting * temp = new MagSensorContentSetting;
//          magnSensorContentList_.append(temp);
//          ui->vLayout_content->addWidget(temp);
//          temp->setId(i);
//          temp->hide();

          //连接信号与槽
//          connect(temp,SIGNAL(sig_step_valueChange()),this,SLOT(slot_enableNextStep()));
//      }

}

void GuidMagneticSensorSetting::slot_enableNextStep()
{
    if(!getConfigure())
    {
        emit sig_enableNextStep(true);
        return;
    }
    if(numSetting_->getNum()<1)
    {
        emit sig_enableNextStep(false);//发到父窗口
        return;
    }
    emit sig_enableNextStep(true);//发到父窗口
}

void GuidMagneticSensorSetting::on_radioButton_set_clicked()
{
    setConfigure(true);
    emit sig_configure();
    //添加时将数量设置为0并且设置数量窗口不可用
    numSetting_->getNumBox()->setEnabled(true);
    numSetting_->getLabelIntro()->setStyleSheet("color:white");
    numSetting_->setNum(1);
    slot_enableNextStep();//判断
}

void GuidMagneticSensorSetting::on_radioButton_ignore_clicked()
{
    setConfigure(false);
    //不添加时将数量设置为0并且设置数量窗口不可用
    numSetting_->setNum(0);
    numSetting_->getNumBox()->setEnabled(false);
    numSetting_->getLabelIntro()->setStyleSheet("color:rgb(255, 25, 148)");
    emit sig_enableNextStep(true);
}

void GuidMagneticSensorSetting::slot_ignore_byRFID()
{
    on_radioButton_ignore_clicked();
    numSetting_->getLabelIntro()->setStyleSheet("color:rgb(255, 25, 148)");
    ui->radioButton_ignore->setEnabled(true);
    ui->radioButton_ignore->setChecked(true);
    ui->radioButton_set->setEnabled(false);
}

void GuidMagneticSensorSetting::slot_set_byRFID()
{
    on_radioButton_set_clicked();
    ui->radioButton_set->setEnabled(true);
    ui->radioButton_set->setChecked(true);
    ui->radioButton_ignore->setEnabled(false);
}

