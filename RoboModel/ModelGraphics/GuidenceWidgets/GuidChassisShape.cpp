#include "GuidChassisShape.h"
#include "ui_GuidChassisShape.h"
#include"StyleObject.h"
GuidChassisShape::GuidChassisShape(QWidget *parent) :
    GuidBaseSetting(parent),
    ui(new Ui::GuidChassisShape),
    ShapeType_(RECTANGLE)
{
    ui->setupUi(this);
    setType(GUID_CHASSIS_SHAPE);
    InitUI();
    checkIfNeedEnableByShape();
    enableNextStep();
}

GuidChassisShape::~GuidChassisShape()
{
    delete ui;
}

ShapeType GuidChassisShape::getShape()
{
    return ShapeType_;
}

void GuidChassisShape::InitUI()
{
    ui->comboBox_shape->clear();
    ui->comboBox_shape->addItem(tr("Rectangle"),RECTANGLE);
    ui->comboBox_shape->addItem(tr("Circle"),CIRCLE);

    imagePath_ = StyleObject::init()->getCurrentSkinDirPath()+"/images/"+"/RoModelPlugin/RoboChassis/";
    ui->label_image->setPixmap(QPixmap(QString("%1").arg(imagePath_+"Rectangle")));
    ui->comboBox_shape->setCurrentIndex(0);

    ui->doubleSpinBox_head->show();
    ui->label_head->show();
    ui->label_tail->show();
    ui->doubleSpinBox_tail->show();
    ui->label_width->show();
    ui->doubleSpinBox_width->show();
    ui->doubleSpinBox_radius->hide();
    ui->label_radius->hide();
}

void GuidChassisShape::enableNextStep()
{
    if(headFlag)
    {
        if(ui->doubleSpinBox_head->value() == 0)
        {
            emit sig_enableNextStep(false);
            return;
        }
    }

    if(radiusFlag)
    {
        if(ui->doubleSpinBox_radius->value()==0)
        {
            emit sig_enableNextStep(false);
            return;
        }
    }

    if(tailFlag)
    {
        if(ui->doubleSpinBox_tail->value() == 0)
        {
            emit sig_enableNextStep(false);
            return;
        }
    }

    if(widthFlag)
    {
        if(ui->doubleSpinBox_width->value() == 0)
        {
            emit sig_enableNextStep(false);
            return;
        }
    }

    emit sig_enableNextStep(true);
    return;
}

void GuidChassisShape::checkIfNeedEnableByShape()
{
    robotData_.shapType_ = (ShapeType)ui->comboBox_shape->currentData().toInt();
    if(robotData_.shapType_==RECTANGLE)
    {
        headFlag=true ;
        tailFlag=true ;
        widthFlag=true ;
        radiusFlag = false;
    }
    else
    {
        headFlag=false ;
        tailFlag=false ;
        widthFlag=false ;
        radiusFlag = true;
    }
}

void GuidChassisShape::on_comboBox_shape_currentIndexChanged(int index)
{
    if( 0 == index )
    {
        //rectangle
        ShapeType_ = RECTANGLE;
        robotData_.shapType_ = RECTANGLE;
        ui->label_image->setPixmap(QPixmap(QString("%1").arg(imagePath_+"Rectangle")));

        //------------

        ui->doubleSpinBox_head->show();
        ui->label_head->show();
        ui->label_tail->show();
        ui->doubleSpinBox_tail->show();
        ui->label_width->show();
        ui->doubleSpinBox_width->show();
        ui->doubleSpinBox_radius->hide();
        ui->label_radius->hide();

        //-------------------------send massage to next page to make it know the shape is changed

        emit sig_shape_change(RECTANGLE);
    }
    else
    {
        //Circle
        ShapeType_ = CIRCLE;
        robotData_.shapType_ = CIRCLE;
        ui->label_image->setPixmap(QPixmap(QString("%1").arg(imagePath_+"Circle")));
        //------------------
        ui->doubleSpinBox_head->hide();
        ui->label_head->hide();
        ui->label_tail->hide();
        ui->doubleSpinBox_tail->hide();
        ui->label_width->hide();
        ui->doubleSpinBox_width->hide();
        ui->doubleSpinBox_radius->show();
        ui->label_radius->show();
        //-------------------------send massage to next page to make it know the shape is changed
        emit sig_shape_change(CIRCLE);
    }
    checkIfNeedEnableByShape();
    enableNextStep();



}

void GuidChassisShape::on_doubleSpinBox_head_valueChanged(double arg1)
{
    robotData_.head_ = arg1*100;
    enableNextStep();
}

void GuidChassisShape::on_doubleSpinBox_tail_valueChanged(double arg1)
{
    robotData_.tail_ = arg1*100;
    enableNextStep();
}

void GuidChassisShape::on_doubleSpinBox_width_valueChanged(double arg1)
{
    robotData_.width_ = arg1*100;
    enableNextStep();
}

void GuidChassisShape::on_doubleSpinBox_radius_valueChanged(double arg1)
{
    robotData_.radius_ = arg1*100;
    enableNextStep();
}
