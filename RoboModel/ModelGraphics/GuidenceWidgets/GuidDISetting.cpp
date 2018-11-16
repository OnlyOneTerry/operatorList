#include "GuidDISetting.h"
#include "ui_GuidDISetting.h"
#include"BasicContentSetting.h"
#include"DIContentSetting.h"
#include"StyleObject.h"
#include"NumSetting.h"
GuidDISetting::GuidDISetting(QWidget *parent) :
    GuidBaseSetting(parent),
    ui(new Ui::GuidDISetting)
{
    ui->setupUi(this);
    setType(GUID_DI);
    ui->radioButton_set->setChecked(true);

    //初始化数量
    numSetting_ = new NumSetting;
    numSetting_->setId(0);
    numSetting_->setNum(1);
    numSetting_crashDI_ = new NumSetting;
    numSetting_crashDI_->setId(1);
    numSetting_crashDI_->setNum(0);

    slot_enableNextStep();
    ui->vLayout_content->addWidget(numSetting_);
    ui->vLayout_content_crash->addWidget(numSetting_crashDI_);

    numSetting_->setIntroduction(tr("The device is mainly used to input the sensor signal in the process of using the robot, and the specific configuration method will be guided later."));
    numSetting_crashDI_->setIntroduction(tr("The device is mainly used to help the robot to check whether it collides with an obstacle, and then the configuration method is used to guide the robot."));
    setNextPreBtnDisplay(false);//初始化设置按钮
//    ui->label_promt->setStyleSheet("color:red;");
    connect(numSetting_->getNumBox(),SIGNAL(valueChanged(int)),this,SLOT(slot_enableNextStep()));

    QString imagePath= StyleObject::init()->getCurrentSkinDirPath()+"/images/RoModelPlugin/";
    numSetting_->setImagePath(imagePath+"DI");
    numSetting_->setImageGroupBoxTitle(tr("DI Picture"));


    numSetting_crashDI_->setImagePath(imagePath+"crashDi");
    numSetting_crashDI_->setImageGroupBoxTitle(tr("crashDI Picture"));
    //确保数量之和不大于16
    numSetting_->getNumBox()->setMaximum(16-numSetting_crashDI_->getNum());
    numSetting_crashDI_->getNumBox()->setMaximum(16-numSetting_->getNum());

    connect(numSetting_->getNumBox(),SIGNAL(valueChanged(int)),this,SLOT(slot_setDImaxNum(int)));
    connect(numSetting_crashDI_->getNumBox(),SIGNAL(valueChanged(int)),this,SLOT(slot_setDImaxNum(int)));
    connect(numSetting_crashDI_->getNumBox(),SIGNAL(valueChanged(int)),this,SLOT(slot_enableNextStep()));

}

GuidDISetting::~GuidDISetting()
{
    delete ui;
}


void GuidDISetting::setNextPreBtnDisplay(bool flag)
{

}

void GuidDISetting::setRadiouBtnstatus(bool flag)
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

void GuidDISetting::showNumWid()
{
//    for(int i = 0; i< diContentList_.size();i++)
//    {
//        if(i==0)
//        {
//            diContentList_.at(i)->show();
//            setNextPreBtnDisplay(false);//隐藏切换按钮
//            ui->radioButton_ignore->show();
//            ui->radioButton_set->show();
//            currentWidId_ = 0;
//        }
//        else
//        {
//            diContentList_.at(i)->hide();
//        }
//    }
}

void GuidDISetting::slot_createSetingWidbyNum()
{
    //先清除
//     diContentList_.clear();
//     while(ui->vLayout_content->count()!=0)
//     {
//         ui->vLayout_content->takeAt(0);
//     }
     //再添加
//     diContentList_.append(numSetting_);
//     ui->vLayout_content->addWidget(numSetting_);//num设置窗添加到布局
//     //获取添加的数量
//     int num = numSetting_->getNum();

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
//         DIContentSetting* temp = new DIContentSetting;
//         diContentList_.append(temp);
//         temp->setId(i);
//         ui->vLayout_content->addWidget(temp);
//         temp->hide();//创建时全部隐藏

//         //连接信号与槽
//         connect(temp,SIGNAL(sig_rang_valueChange()),this,SLOT(slot_enableNextStep()));
//     }
}

void GuidDISetting::slot_enableNextStep()
{
    if(!getConfigure())
    {
        emit sig_enableNextStep(true);
        return;
    }

    if(numSetting_->getNum()+numSetting_crashDI_->getNum()<1)
    {
        emit sig_enableNextStep(false);//发到父窗口
        return;
    }

    emit sig_enableNextStep(true);//发到父窗口
}

void GuidDISetting::slot_setDImaxNum(int num)
{
    numSetting_->getNumBox()->setMaximum(16-numSetting_crashDI_->getNum());
    numSetting_crashDI_->getNumBox()->setMaximum(16-numSetting_->getNum());
}

void GuidDISetting::on_radioButton_set_clicked()
{
    setConfigure(true);
    emit sig_configure();
    //添加时将数量设置为0并且设置数量窗口不可用
    numSetting_->getNumBox()->setEnabled(true);
    numSetting_->setNum(1);
    numSetting_crashDI_->getNumBox()->setEnabled(true);
    slot_enableNextStep();//判断

}

void GuidDISetting::on_radioButton_ignore_clicked()
{
    setConfigure(false);
    //不添加时将数量设置为0并且设置数量窗口不可用
    numSetting_->setNum(0);
    numSetting_->getNumBox()->setEnabled(false);
    numSetting_crashDI_->getNumBox()->setEnabled(false);
    numSetting_crashDI_->setNum(0);
    emit sig_enableNextStep(true);
}

