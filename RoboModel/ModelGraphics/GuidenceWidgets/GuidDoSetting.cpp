#include "GuidDoSetting.h"
#include "ui_GuidDoSetting.h"
#include"NumSetting.h"
#include"BasicContentSetting.h"
#include"DOContentSetting.h"
#include"StyleObject.h"
GuidDoSetting::GuidDoSetting(QWidget *parent) :
    GuidBaseSetting(parent),
    ui(new Ui::GuidDoSetting)
{
    ui->setupUi(this);
    setType(GUID_DO);
    ui->radioButton_set->setChecked(true);
    //-----------------------
    //初始化数量
    numSetting_ = new NumSetting;
    numSetting_->setId(0);
    numSetting_->setNum(1);
    slot_enableNextStep();
    setNextPreBtnDisplay(false);//初始化设置按钮
    ui->vLayout_content->addWidget(numSetting_);
    numSetting_->getNumBox()->setToolTip(tr("range[0,16]"));
    numSetting_->getNumBox()->setMaximum(16);
    connect(numSetting_->getNumBox(),SIGNAL(valueChanged(int)),this,SLOT(slot_enableNextStep()));
    QString imagePath= StyleObject::init()->getCurrentSkinDirPath()+"/images/RoModelPlugin/";
    numSetting_->setImagePath(imagePath+"DO");
    numSetting_->setImageGroupBoxTitle(tr("DO Picture"));
    numSetting_->setIntroduction(tr("The device is mainly used in the process of robot use signal output, the specific configuration method will be guided later."));
}

GuidDoSetting::~GuidDoSetting()
{
    delete ui;
}


void GuidDoSetting::setRadiouBtnstatus(bool flag)
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

void GuidDoSetting::showNumWid()
{
    for(int i = 0; i< doContentList_.size();i++)
    {
        if(i==0)
        {
            doContentList_.at(i)->show();
            setNextPreBtnDisplay(false);//隐藏切换按钮
            //            ui->ptb_Start->show();//显示配置按钮
            ui->radioButton_ignore->show();
            ui->radioButton_set->show();
            currentWidId_ = 0;
        }
        else
        {
            doContentList_.at(i)->hide();
        }
    }
}

void GuidDoSetting::slot_createSetingWidbyNum()
{
    //先清除
    //    doContentList_.clear();
    //    while(ui->vLayout_content->count()!=0)
    //    {
    //        ui->vLayout_content->takeAt(0);
    //    }
    //    //再添加
    //    doContentList_.append(numSetting_);
    //    ui->vLayout_content->addWidget(numSetting_);//num设置窗添加到布局
    //    //获取添加的数量
    //    int num = numSetting_->getNum();

    //     if(num==0)
    //     {
    //         ui->ptb_Start->setEnabled(false);
    //         numSetting_->getLabel()->setText(tr(""));
    //     }
    //     else
    //     {
    //         ui->ptb_Start->setEnabled(true);
    //         numSetting_->getLabel()->setText(tr("点击“进入设置”按钮进行属性设置。"));
    //     }


    //     for(int i = 1; i<=num;i++)
    //     {
    //         DOContentSetting* temp = new DOContentSetting;
    //         doContentList_.append(temp);
    //         temp->setId(i);
    //         ui->vLayout_content->addWidget(temp);
    //         temp->hide();//创建时全部隐藏

    //连接信号与槽
    //             connect(temp,SIGNAL(sig_ip_valueChange()),this,SLOT(slot_enableNextStep()));
    //     }
}

void GuidDoSetting::slot_enableNextStep()
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

void GuidDoSetting::on_radioButton_set_clicked()
{
    setConfigure(true);
    emit sig_configure();
    //添加时将数量设置为0并且设置数量窗口不可用
    numSetting_->getNumBox()->setEnabled(true);
    numSetting_->setNum(1);
    slot_enableNextStep();//判断
}

void GuidDoSetting::on_radioButton_ignore_clicked()
{
    setConfigure(false);
    //不添加时将数量设置为0并且设置数量窗口不可用
    numSetting_->setNum(0);
    numSetting_->getNumBox()->setEnabled(false);
    emit sig_enableNextStep(true);

}



