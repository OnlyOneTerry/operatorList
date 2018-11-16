#include "WidgetRobotModelSetting.h"
#include "ItemRobotModel.h"
#include "ui_WidgetCircleRobotModelSetting.h"
#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonArray>
#include <ModelCommon.h>
#include "PublicClass.h"
#include<QFormLayout>

WidgetRobotModelSetting::WidgetRobotModelSetting(ItemRobotModel *rm, QWidget *parent) :
    WidgetSetting(parent),
    robot_model_(rm),
    Wid_rect_setting_(new WidgetRectRobotModelSetting(rm)),
    Wid_circle_setting_(new WidgetCircleRobotModelSetting(rm))
{
    Wid_circle_setting_->setParentWid(this);
    Wid_rect_setting_->setParentWid(this);
    setType(WIDGET_ROBOTMODEL);
    initUI(rm->robot_model_data_.shapType_);//初始化界面
    //加载模型时设置驱动器编号及正反转标记
    setInverseAndId(rm,rm->robot_model_data_.shapType_);
    //底盘形状变化
    connect(chasisShapTypeComBox,SIGNAL(currentIndexChanged(int)),this,SLOT(slot_shapeIndexChange()));
}

WidgetRobotModelSetting::~WidgetRobotModelSetting()
{
    SCDebug<<"WidgetRobotModelSetting delete ..........";
}

void WidgetRobotModelSetting::initUI(ShapeType type)
{
    QGridLayout* mainBoxlayout = new QGridLayout;
    QGroupBox* shapeBox = new QGroupBox(tr("Chasiss Shape"));

    QHBoxLayout* shapeLayout  = new QHBoxLayout;
    chasisShapTypeComBox = new customComboBox;
    chasisShapTypeComBox->setStyleSheet("QComboBox{min-height: 24px;}");
    chasisShapTypeComBox->blockSignals(true);
    chasisShapTypeComBox->clear();
    chasisShapTypeComBox->addItem(tr("Rectangle"),RECTANGLE);
    chasisShapTypeComBox->addItem(tr("Circle"),CIRCLE);
    chasisShapTypeComBox->blockSignals(false);
    shapeLayout->addWidget(chasisShapTypeComBox);
    shapeBox->setLayout(shapeLayout);

    mainBoxlayout->addWidget(shapeBox,0,0,1,3);
    mainBoxlayout->addWidget(Wid_circle_setting_,1,0,1,3);
    mainBoxlayout->addWidget(Wid_rect_setting_,2,0,1,3);
    mainBoxlayout->setContentsMargins(10,0,10,0);
    setLayout(mainBoxlayout);

    if(type  == RECTANGLE)
    {
        SCDebug<<"type is rectangle";
        int idex = chasisShapTypeComBox->findData(RECTANGLE);
        chasisShapTypeComBox->blockSignals(true);
        chasisShapTypeComBox->setCurrentIndex(idex);
        chasisShapTypeComBox->blockSignals(false);
        chasisShapTypeComBox->setOri_index(idex);
        Wid_rect_setting_->show();
        Wid_circle_setting_->hide();
        Wid_rect_setting_->m_ori_modelData.shapType_=RECTANGLE;
    }
    else if(type == CIRCLE)
    {
        SCDebug<<"type is circle";
        int idx = chasisShapTypeComBox->findData(CIRCLE);
        chasisShapTypeComBox->setCurrentIndex(idx);
        chasisShapTypeComBox->blockSignals(true);
        chasisShapTypeComBox->setOri_index(idx);
        chasisShapTypeComBox->blockSignals(false);
        Wid_rect_setting_->hide();
        Wid_circle_setting_->show();
        Wid_circle_setting_->m_ori_modelData.shapType_ = CIRCLE;
        int idx1 = Wid_circle_setting_->getMovCombx()->findData(robot_model_->robot_circle_model_data_.mvStyle_);
        Wid_circle_setting_->getMovCombx()->blockSignals(true);
        Wid_circle_setting_->getMovCombx()->setCurrentIndex(idx1);
        Wid_circle_setting_->getMovCombx()->blockSignals(false);
        Wid_circle_setting_->getMovCombx()->setOri_index(idx1);
        int idx2 = Wid_circle_setting_->getChassisCombx()->findData(robot_model_->robot_circle_model_data_.chasisType_);
        Wid_circle_setting_->getChassisCombx()->blockSignals(true);
        Wid_circle_setting_->getChassisCombx()->setCurrentIndex(idx2);
        Wid_circle_setting_->getChassisCombx()->blockSignals(false);
        Wid_circle_setting_->getChassisCombx()->setOri_index(idx2);
    }
}

WidgetRectRobotModelSetting *WidgetRobotModelSetting::getRectSetting()
{
    return Wid_rect_setting_;
}

customDoubleSpinBox *WidgetRobotModelSetting::getHeadSpinBox()
{
    return Wid_rect_setting_->get_doubleSpinbox_head();
}

customDoubleSpinBox *WidgetRobotModelSetting::getTailSpinBox()
{
    return Wid_rect_setting_->get_doubleSpinbox_tail();
}

customDoubleSpinBox *WidgetRobotModelSetting::getWidthSpinBox()
{
    return Wid_rect_setting_->get_doubleSpinbox_width();
}

customDoubleSpinBox *WidgetRobotModelSetting::getRadiusSpinbox()
{
    return Wid_circle_setting_->get_outLine_radiusSpinbox();
}

WidgetCircleRobotModelSetting *WidgetRobotModelSetting::getCirSetting()
{
    return Wid_circle_setting_;
}

void WidgetRobotModelSetting::setInverseAndId(ItemRobotModel *rm, ShapeType type)
{
    if(type==RECTANGLE)
    {
        //获取电机编号列表
        for(int i = 0; i < rm->driverId_list_.size();i++)
        {

            if(Wid_rect_setting_->driverIDspinboxList_.at(i)==NULL)
            {
                SCDebug<<"driverIDspinboxList_"<<i<<"doesn't existed!!!!!";
                return;
            }
            else
            {
                Wid_rect_setting_->driverIDspinboxList_.at(i)->blockSignals(true);
                Wid_rect_setting_->driverIDspinboxList_.at(i)->setValue(rm->driverId_list_.at(i));
                Wid_rect_setting_->driverIDspinboxList_.at(i)->setOriValue(rm->driverId_list_.at(i));
                Wid_rect_setting_->driverIDspinboxList_.at(i)->blockSignals(false);

            }

            if(0==i)
            {
                Wid_rect_setting_->id1 = rm->driverId_list_.at(0);
            }
            else if(1==i )
            {
                Wid_rect_setting_->id2 = rm->driverId_list_.at(1);
            }
            else if(2 == i)
            {
                Wid_rect_setting_->id3 = rm->driverId_list_.at(2);
            }
            else if(3 == i)
            {
                Wid_rect_setting_->id4 = rm->driverId_list_.at(3);
            }

        }

        //获取正反转标记列表
        for(int i = 0; i<rm->inverse_list_.size();i++)
        {
            if(Wid_rect_setting_->driverInverseList_.at(i)==NULL)
            {
                SCDebug<<"check"<<i<<"doesn't existed!!!!!";
                return;
            }
            Wid_rect_setting_->driverInverseList_.at(i)->blockSignals(true);
            if(rm->inverse_list_.at(i))
            {
                Wid_rect_setting_->driverInverseList_.at(i)->setCurrentIndex(1);
                Wid_rect_setting_->driverInverseList_.at(i)->setOri_index(1);
            }
            else
            {
                Wid_rect_setting_->driverInverseList_.at(i)->setCurrentIndex(0);
                Wid_rect_setting_->driverInverseList_.at(i)->setOri_index(0);
            }
            Wid_rect_setting_->driverInverseList_.at(i)->blockSignals(false);

            if(0==i)
            {
                Wid_rect_setting_->inverse1 = rm->inverse_list_.at(0);
            }
            else if(1==i )
            {
                Wid_rect_setting_->inverse2 = rm->inverse_list_.at(1);
            }
            else if(2 == i)
            {
                Wid_rect_setting_->inverse3 = rm->inverse_list_.at(2);
            }
            else if(3 == i)
            {
                Wid_rect_setting_->inverse4 = rm->inverse_list_.at(3);
            }
        }
    }
    else if(type==CIRCLE)
    {

        //获取电机编号列表
        for(int i = 0; i < rm->driverId_list_.size();i++)
        {

            if(Wid_circle_setting_->driverIDspinboxList_.at(i)==NULL)
            {
                SCDebug<<"driverIDspinboxList_"<<i<<"doesn't existed!!!!!";
                return;
            }
            else
            {
                Wid_circle_setting_->driverIDspinboxList_.at(i)->blockSignals(true);
                Wid_circle_setting_->driverIDspinboxList_.at(i)->setValue(rm->driverId_list_.at(i));
                Wid_circle_setting_->driverIDspinboxList_.at(i)->setOriValue(rm->driverId_list_.at(i));
                Wid_circle_setting_->driverIDspinboxList_.at(i)->blockSignals(false);
            }

            if(0==i)
            {
                Wid_circle_setting_->id1 = rm->driverId_list_.at(0);
            }
            else if(1==i )
            {
                Wid_circle_setting_->id2 = rm->driverId_list_.at(1);
            }
            else if(2 == i)
            {
                Wid_circle_setting_->id3 = rm->driverId_list_.at(2);
            }
            else if(3 == i)
            {
                Wid_circle_setting_->id4 = rm->driverId_list_.at(3);
            }

        }

        //获取正反转标记列表
        for(int i = 0; i<rm->inverse_list_.size();i++)
        {
            SCDebug<<"inverse"<<i<<"is ---------`---"<<rm->inverse_list_.at(i);
            if(Wid_circle_setting_->driverInverseList_.at(i)==NULL)
            {
                SCDebug<<"check"<<i<<"doesn't existed!!!!!";
                return;
            }
            Wid_circle_setting_->driverInverseList_.at(i)->blockSignals(true);
            if(rm->inverse_list_.at(i))
            {
                Wid_circle_setting_->driverInverseList_.at(i)->setCurrentIndex(1);
                Wid_circle_setting_->driverInverseList_.at(i)->setOri_index(1);
            }
            else
            {
                Wid_circle_setting_->driverInverseList_.at(i)->setCurrentIndex(0);
                Wid_circle_setting_->driverInverseList_.at(i)->setOri_index(0);
            }

            Wid_circle_setting_->driverInverseList_.at(i)->blockSignals(false);

            if(0 ==i)
            {
                Wid_circle_setting_->inverse1 = rm->inverse_list_.at(0);
            }
            else if(1==i )
            {
                Wid_circle_setting_->inverse2 = rm->inverse_list_.at(1);
            }
            else if(2 == i)
            {
                Wid_circle_setting_->inverse3 = rm->inverse_list_.at(2);
            }
            else if(3 == i)
            {
                Wid_circle_setting_->inverse4 = rm->inverse_list_.at(3);
            }
        }

    }
}

//拖动底盘边框时更新属性框对应的属性值
void WidgetRobotModelSetting::update_value()
{
    if(robot_model_->getChasisShapeType()==RECTANGLE)
    {
        Wid_rect_setting_->get_doubleSpinbox_head()->blockSignals (true);
        Wid_rect_setting_->get_doubleSpinbox_tail()->blockSignals (true);
        Wid_rect_setting_->get_doubleSpinbox_width()->blockSignals (true);
        Wid_rect_setting_->get_doubleSpinbox_head()->setValue (robot_model_->robot_model_data_.head_/100.0);
        Wid_rect_setting_->get_doubleSpinbox_tail()->setValue (robot_model_->robot_model_data_.tail_/100.0);
        Wid_rect_setting_->get_doubleSpinbox_width()->setValue (robot_model_->robot_model_data_.width_/100.0);
        Wid_rect_setting_->get_doubleSpinbox_head()->blockSignals (false);
        Wid_rect_setting_->get_doubleSpinbox_tail()->blockSignals (false);
        Wid_rect_setting_->get_doubleSpinbox_width()->blockSignals (false);

        //保存切换之前的数据
        Wid_rect_setting_->m_ori_modelData.head_ = robot_model_->robot_model_data_.head_/100.;
        Wid_rect_setting_->m_ori_modelData.width_ = robot_model_->robot_model_data_.width_/100.;
        Wid_rect_setting_->m_ori_modelData.tail_ = robot_model_->robot_model_data_.tail_/100.;
    }
    else if(robot_model_->getChasisShapeType()==CIRCLE)
    {
        Wid_circle_setting_->get_outLine_radiusSpinbox()->blockSignals(true);
        Wid_circle_setting_->get_outLine_radiusSpinbox()->setValue(robot_model_->robot_circle_model_data_.radius_/100.0);
        Wid_circle_setting_->get_outLine_radiusSpinbox()->blockSignals(false);
    }
}


void WidgetRobotModelSetting::slot_shapeIndexChange()
{
    SCDebug<<"shape index is changed.........getIsClickedTrigger"<<chasisShapTypeComBox->getIsClickedTrigger();
    if(m_unStack==NULL)
    {
        return;
    }

    if(chasisShapTypeComBox->getIsClickedTrigger())
    {
        SCDebug<<"new index is "<<chasisShapTypeComBox->currentIndex();
        chasisShapTypeComBox->setNew_index(chasisShapTypeComBox->currentIndex());
        customboxCommand* cmbox_cmd = new customboxCommand(chasisShapTypeComBox,
                                                           chasisShapTypeComBox->getOri_index(),
                                                           chasisShapTypeComBox->getNew_index(),
                                                           this,
                                                           NULL);
        SCDebug<<"cmbox_cmd push ============555555";
        m_unStack->push(cmbox_cmd);
        chasisShapTypeComBox->setOri_index(chasisShapTypeComBox->currentIndex());
    }

    chasisShapTypeComBox->setIsClickedTrigger(true);

    ShapeType shpType = (ShapeType)chasisShapTypeComBox->currentData().toInt();
    switch (shpType) {
    case RECTANGLE:
    {
        robot_model_->setChasisShapeType(RECTANGLE);
        Wid_circle_setting_->hide();
        Wid_rect_setting_->show();
        SCDebug<<"1 current type is rectangle=======================";
    }
        break;
    case CIRCLE:
    {
        robot_model_->setChasisShapeType(CIRCLE);
        Wid_rect_setting_->hide();
        Wid_circle_setting_->show();
        SCDebug<<"2 current type is circle=======================";
    }
        break;
    default:
        break;
    }
    robot_model_->update_model();
}

ItemBase *WidgetRobotModelSetting::getRelateItem()
{
    return robot_model_;
}

