#include "GuidLaserSetting.h"
#include "ui_GuidLaserSetting.h"
#include"LaserContentSetting.h"
#include"NumSetting.h"
#include"StyleObject.h"
GuidLaserSetting::GuidLaserSetting(QWidget *parent) :
    GuidBaseSetting(parent),
    ui(new Ui::GuidLaserSetting),
    currentWidId_(0)
{
    ui->setupUi(this);
    setType(GUID_LASER);
    ui->radioButton_set->setChecked(true);

    //初始化数量
    numSetting_ = new NumSetting;
    numSetting_->setId(0);
    numSetting_->setNum(1);
    setNextPreBtnDisplay(false);//初始化设置按钮
    ui->vLayout_content->addWidget(numSetting_);
    numSetting_->getNumBox()->setToolTip(tr("range[0,10]"));
    numSetting_->getNumBox()->setMaximum(10);
    QString imagePath= StyleObject::init()->getCurrentSkinDirPath()+"/images/RoModelPlugin/";
    numSetting_->setImagePath(imagePath+"Laser");
    numSetting_->setImageGroupBoxTitle(tr("Laser Picture"));
    connect(numSetting_->getNumBox(),SIGNAL(valueChanged(int)),this,SLOT(slot_enableNextStep()));
    numSetting_->setIntroduction(tr("The device is mainly used to help robots locate, build maps, stop obstacles and avoid obstacles."));

}

GuidLaserSetting::~GuidLaserSetting()
{
    delete ui;
}

ConfigureData GuidLaserSetting::readLaserTypeFile(const QString &filePath)
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

    QJsonValue  laser_value= chassis_obj["laser"];
    QJsonObject laser_obj = laser_value.toObject();

    ConfigureData  laserData;
    QJsonValue defaultIdex = laser_obj["defaultIndex"].toInt();
    laserData.defaultidx = defaultIdex.toInt();
    QJsonArray typeList = laser_obj["typeList"].toArray();

    for(int i = 0;i < typeList.size();i++)
    {
        ItemData tempData;
        QJsonObject tempObj = typeList.at(i).toObject();
        QJsonValue tempValue = tempObj["index"];
        QJsonValue tempDesc  = tempObj["desc"];

        tempData.index = tempValue.toInt();
        tempData.describtion = tempDesc.toString();

        laserData.dataList.push_back(tempData);
    }
    return laserData;
}

void GuidLaserSetting::showNumWid()
{
   for(int i = 0; i< laserContentList_.size();i++)
   {
       if(i==0)
       {
           laserContentList_.at(i)->show();
           setNextPreBtnDisplay(false);//隐藏切换按钮
           ui->radioButton_ignore->show();
           ui->radioButton_set->show();
           currentWidId_ = 0;
       }
       else
       {
           laserContentList_.at(i)->hide();
       }
   }
}

void GuidLaserSetting::setRadiouBtnstatus(bool flag)
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



void GuidLaserSetting::slot_createSetingWidbyNum()
{
//   先清除
//    laserContentList_.clear();
//    while(ui->vLayout_content->count()!=0)
//    {
//        ui->vLayout_content->takeAt(0);
//    }
//    //再添加
//    laserContentList_.append(numSetting_);
//    ui->vLayout_content->addWidget(numSetting_);//num设置窗添加到布局
    //获取添加的数量
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


//    for(int i = 1; i<=num;i++)
//    {
//        LaserContentSetting* temp = new LaserContentSetting;
//        temp->setIndex(i);
//        laserContentList_.append(temp);
//        temp->setId(i);
//        ui->vLayout_content->addWidget(temp);
//        temp->hide();//创建时全部隐藏

//        //连接信号与槽
//        connect(temp,SIGNAL(sig_ip_valueChange()),this,SLOT(slot_enableNextStep()));
//    }
}

void GuidLaserSetting::slot_enableNextStep()
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

void GuidLaserSetting::on_radioButton_set_clicked()
{

    setConfigure(true);
    emit sig_configure();
    //添加时将数量设置为0并且设置数量窗口不可用
    numSetting_->getNumBox()->setEnabled(true);
    numSetting_->setNum(1);
    slot_enableNextStep();//判断
}

void GuidLaserSetting::on_radioButton_ignore_clicked()
{
    setConfigure(false);
    //不添加时将数量设置为0并且设置数量窗口不可用
    numSetting_->setNum(0);
    numSetting_->getNumBox()->setEnabled(false);
    emit sig_enableNextStep(true);

}

