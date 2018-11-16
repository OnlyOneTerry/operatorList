#include "WidgetMagneticSensorSetting.h"
#include "ui_WidgetMagneticSensorSetting.h"
#include "ItemMagneticSensor.h"
#include"ModelCustomCommand.h"
#include"ModelCustomWidget.h"
#include"ItemAngelCtroller.h"
#include"ModelGraphicsView.h"

WidgetMagneticSensorSetting::WidgetMagneticSensorSetting(ItemMagneticSensor *isensor,QWidget *parent) :
    WidgetSetting(parent),
    item_magnetic_sensor_(isensor),
    ui(new Ui::WidgetMagneticSensorSetting)
{
    ui->setupUi(this);
     setType(WIDGET_MAGENETIC);
    ui->doubleSpinBox_r->setDecimals(3);
    initWidgetData();
    connect(&item_magnetic_sensor_->timer,SIGNAL(timeout()),this,SLOT(on_timer_out()));

}

WidgetMagneticSensorSetting::~WidgetMagneticSensorSetting()
{
    delete ui;
}

void WidgetMagneticSensorSetting::initWidgetData()
{
    ui->spinBox_id->blockSignals(true);
    ui->spinBox_id->setValue (item_magnetic_sensor_->data_magnetic_.id_);
    ui->spinBox_id->blockSignals(false);
    ui->spinBox_id->setOriValue(item_magnetic_sensor_->data_magnetic_.id_);

    ui->doubleSpinBox_x->blockSignals(true);
    ui->doubleSpinBox_x->setValue (item_magnetic_sensor_->data_magnetic_.x_);
    ui->doubleSpinBox_x->blockSignals(false);
    ui->doubleSpinBox_x->setOriValue(item_magnetic_sensor_->data_magnetic_.x_);

    ui->doubleSpinBox_y->blockSignals(true);
    ui->doubleSpinBox_y->setValue (item_magnetic_sensor_->data_magnetic_.y_);
    ui->doubleSpinBox_y->blockSignals(false);
    ui->doubleSpinBox_y->setOriValue(item_magnetic_sensor_->data_magnetic_.y_);

    ui->doubleSpinBox_z->blockSignals(true);
    ui->doubleSpinBox_z->setValue (item_magnetic_sensor_->data_magnetic_.z_);
    ui->doubleSpinBox_z->blockSignals(false);
    ui->doubleSpinBox_z->setOriValue(item_magnetic_sensor_->data_magnetic_.z_);

    ui->doubleSpinBox_r->blockSignals(true);
    ui->doubleSpinBox_r->setValue (item_magnetic_sensor_->data_magnetic_.r_);
    ui->doubleSpinBox_r->blockSignals(false);
    ui->doubleSpinBox_r->setOriValue(item_magnetic_sensor_->data_magnetic_.r_);
    item_magnetic_sensor_->getAngelCtroller()->setOriAngel(item_magnetic_sensor_->data_magnetic_.r_);
    ui->spinBox_resolution->blockSignals(true);
    ui->spinBox_resolution->setValue (item_magnetic_sensor_->data_magnetic_.resolution_);
    ui->spinBox_resolution->blockSignals(false);
    ui->spinBox_resolution->setOriValue(item_magnetic_sensor_->data_magnetic_.resolution_);

    ui->doubleSpinBox_step->blockSignals(true);
    ui->doubleSpinBox_step->setValue (item_magnetic_sensor_->data_magnetic_.step_);
    ui->doubleSpinBox_step->blockSignals(false);
    ui->doubleSpinBox_step->setOriValue(item_magnetic_sensor_->data_magnetic_.step_);
}

void WidgetMagneticSensorSetting::update_value ()
{
    ui->spinBox_id->blockSignals(true);
    ui->spinBox_id->setValue (item_magnetic_sensor_->data_magnetic_.id_);
    ui->spinBox_id->blockSignals(false);
    ui->doubleSpinBox_x->blockSignals(true);
    ui->doubleSpinBox_x->setValue (item_magnetic_sensor_->data_magnetic_.x_);
    ui->doubleSpinBox_x->blockSignals(false);
    ui->doubleSpinBox_y->blockSignals(true);
    ui->doubleSpinBox_y->setValue (item_magnetic_sensor_->data_magnetic_.y_);
    ui->doubleSpinBox_y->blockSignals(false);
    ui->doubleSpinBox_z->blockSignals(true);
    ui->doubleSpinBox_z->setValue (item_magnetic_sensor_->data_magnetic_.z_);
    ui->doubleSpinBox_z->blockSignals(false);
    ui->doubleSpinBox_r->blockSignals(true);
    ui->doubleSpinBox_r->setValue (item_magnetic_sensor_->data_magnetic_.r_);
    ui->doubleSpinBox_r->blockSignals(false);
    ui->spinBox_resolution->blockSignals(true);
    ui->spinBox_resolution->setValue (item_magnetic_sensor_->data_magnetic_.resolution_);
    ui->spinBox_resolution->blockSignals(false);
    ui->doubleSpinBox_step->blockSignals(true);
    ui->doubleSpinBox_step->setValue (item_magnetic_sensor_->data_magnetic_.step_);
    ui->doubleSpinBox_step->blockSignals(false);
}

void WidgetMagneticSensorSetting::set_r(const qreal &angel)
{
    ui->doubleSpinBox_r->blockSignals(true);
    ui->doubleSpinBox_r->setValue(angel);
    ui->doubleSpinBox_r->blockSignals(false);
    item_magnetic_sensor_->data_magnetic_.r_=angel;

}

void WidgetMagneticSensorSetting::on_spinBox_id_valueChanged(int arg1)
{
    item_magnetic_sensor_->update_item();

    if(item_magnetic_sensor_->model_scene()->get_id_list(3).contains(QString::number(arg1)))
    {
        if(arg1 != item_magnetic_sensor_->data_magnetic_.id_)
        {
            ui->label_Prompt->setText(tr("The Id already exists"));
            ui->label_Prompt->setStyleSheet("color:red;");
        }
        else
        {
            item_magnetic_sensor_->data_magnetic_.id_=arg1;
            item_magnetic_sensor_->update_item ();
            ui->label_Prompt->setText(tr("Setup success"));
            ui->label_Prompt->setStyleSheet("color::green;");
        }
        return;
    }
    else
    {
        item_magnetic_sensor_->data_magnetic_.id_=arg1;
        item_magnetic_sensor_->update_item ();
        ui->label_Prompt->setText(tr("Setup success"));
        ui->label_Prompt->setStyleSheet("color::green;");
    }


    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->spinBox_id->getIsClickedTrigger())
    {
        ui->spinBox_id->setNewValue(arg1);

        SpinBoxCommand* spbx_command = new SpinBoxCommand(ui->spinBox_id,ui->spinBox_id->getOriValue(),
                                                          ui->spinBox_id->getNewValue(),this,NULL);
        m_unStack->push(spbx_command);

        ui->spinBox_id->setOriValue(arg1);

    }

    ui->spinBox_id->setIsClickedTrigger(true);
}

void WidgetMagneticSensorSetting::on_doubleSpinBox_x_valueChanged(double arg1)
{
    item_magnetic_sensor_->data_magnetic_.x_=arg1;
    item_magnetic_sensor_->update_item();

    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_x->getIsClickedTrigger())
    {
        ui->doubleSpinBox_x->setNewValue(arg1);

        doubleSpinBoxCommand* dbspbx_command = new doubleSpinBoxCommand(ui->doubleSpinBox_x,item_magnetic_sensor_,
                                                                        ui->doubleSpinBox_x->getOriValue(),
                                                                        ui->doubleSpinBox_x->getNewValue(),
                                                                        this,NULL);
        m_unStack->push(dbspbx_command);

        ui->doubleSpinBox_x->setOriValue(arg1);

    }

    ui->doubleSpinBox_x->setIsClickedTrigger(true);


}

void WidgetMagneticSensorSetting::on_doubleSpinBox_y_valueChanged(double arg1)
{
    item_magnetic_sensor_->data_magnetic_.y_=arg1;
    item_magnetic_sensor_->update_item();

    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_y->getIsClickedTrigger())
    {
        ui->doubleSpinBox_y->setNewValue(arg1);

        doubleSpinBoxCommand* dbspbx_command = new doubleSpinBoxCommand(ui->doubleSpinBox_y,item_magnetic_sensor_,
                                                                        ui->doubleSpinBox_y->getOriValue(),
                                                                        ui->doubleSpinBox_y->getNewValue(),
                                                                        this,NULL);
        m_unStack->push(dbspbx_command);

        ui->doubleSpinBox_y->setOriValue(arg1);

    }

    ui->doubleSpinBox_y->setIsClickedTrigger(true);

}

void WidgetMagneticSensorSetting::on_doubleSpinBox_z_valueChanged(double arg1)
{
    item_magnetic_sensor_->data_magnetic_.z_=arg1;
    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_z->getIsClickedTrigger())
    {
        ui->doubleSpinBox_z->setNewValue(arg1);

        doubleSpinBoxCommand* dbspbx_command = new doubleSpinBoxCommand(ui->doubleSpinBox_z,item_magnetic_sensor_,
                                                                        ui->doubleSpinBox_z->getOriValue(),
                                                                        ui->doubleSpinBox_z->getNewValue(),
                                                                        this,NULL);
        m_unStack->push(dbspbx_command);

        ui->doubleSpinBox_z->setOriValue(arg1);

    }

    ui->doubleSpinBox_z->setIsClickedTrigger(true);

}

void WidgetMagneticSensorSetting::on_doubleSpinBox_r_valueChanged(double arg1)
{
    item_magnetic_sensor_->data_magnetic_.r_=arg1;
    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_r->getIsClickedTrigger())
    {
        ui->doubleSpinBox_r->setNewValue(arg1);

        doubleSpinBoxCommand* dbspbx_command = new doubleSpinBoxCommand(ui->doubleSpinBox_r,item_magnetic_sensor_,
                                                                        ui->doubleSpinBox_r->getOriValue(),
                                                                        ui->doubleSpinBox_r->getNewValue(),this,NULL);
        m_unStack->push(dbspbx_command);

        ui->doubleSpinBox_r->setOriValue(arg1);

    }

    ui->doubleSpinBox_r->setIsClickedTrigger(true);
    item_magnetic_sensor_->update_item();
}

void WidgetMagneticSensorSetting::on_spinBox_resolution_valueChanged(int arg1)
{
    item_magnetic_sensor_->data_magnetic_.resolution_=arg1;

    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->spinBox_resolution->getIsClickedTrigger())
    {
        ui->spinBox_resolution->setNewValue(arg1);

        SpinBoxCommand* dbspbx_command = new SpinBoxCommand(ui->spinBox_resolution,
                                                            ui->spinBox_resolution->getOriValue(),
                                                            ui->spinBox_resolution->getNewValue(),this,NULL);
        m_unStack->push(dbspbx_command);

        ui->spinBox_resolution->setOriValue(arg1);

    }

    ui->spinBox_resolution->setIsClickedTrigger(true);


}

void WidgetMagneticSensorSetting::on_doubleSpinBox_step_valueChanged(double arg1)
{
    item_magnetic_sensor_->data_magnetic_.step_=arg1;

    if(m_unStack==NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_step->getIsClickedTrigger())
    {
        ui->doubleSpinBox_step->setNewValue(arg1);

        doubleSpinBoxCommand* dbspbx_command = new doubleSpinBoxCommand(ui->doubleSpinBox_step,item_magnetic_sensor_,
                                                                        ui->doubleSpinBox_step->getOriValue(),
                                                                        ui->doubleSpinBox_step->getNewValue(),this,NULL);
        m_unStack->push(dbspbx_command);

        ui->doubleSpinBox_step->setOriValue(arg1);

    }

    ui->doubleSpinBox_step->setIsClickedTrigger(true);
}

void WidgetMagneticSensorSetting::on_spinBox_id_editingFinished()
{
    int arg1=ui->spinBox_id->value();
    if(! item_magnetic_sensor_->model_scene()->get_id_list(3).contains(QString::number(arg1)))
    {
        item_magnetic_sensor_->data_magnetic_.id_=arg1;
        item_magnetic_sensor_->update_item ();
        ui->label_Prompt->setText(tr("Setup success"));
        ui->label_Prompt->setStyleSheet("color::green;");
    }
    else
    {
        if(arg1 != item_magnetic_sensor_->data_magnetic_.id_)
        {
            ui->spinBox_id->setValue(item_magnetic_sensor_->data_magnetic_.id_);
            ui->label_Prompt->setText(tr("The Id already exists"));
            ui->label_Prompt->setStyleSheet("color:red;");
        }
        else
        {
            ui->label_Prompt->setText(tr("Setup success"));
            ui->label_Prompt->setStyleSheet("color::green;");
        }
    }

    //500毫秒后隐藏
    item_magnetic_sensor_->timer.setInterval(500);
    item_magnetic_sensor_->timer.setSingleShot(true);
    item_magnetic_sensor_->timer.start();
}

void WidgetMagneticSensorSetting::on_timer_out()
{
    ui->label_Prompt->setText("");
}

void WidgetMagneticSensorSetting::set_step_byGuidConfigure(double arg)
{
    ui->doubleSpinBox_step->blockSignals(true);
    ui->doubleSpinBox_step->setValue(arg);
    ui->doubleSpinBox_step->setOriValue(arg);
    ui->doubleSpinBox_step->blockSignals(false);
}

void WidgetMagneticSensorSetting::set_resolution_byGuidConfigure(int arg)
{
    ui->spinBox_resolution->blockSignals(true);
    ui->spinBox_resolution->setValue(arg);
    ui->spinBox_resolution->setOriValue(arg);
    ui->spinBox_resolution->blockSignals(false);
}

void WidgetMagneticSensorSetting::set_Type_byGuidConfigure(int type)
{
    item_magnetic_sensor_->model_scene()->model_view()->basic_widget()->getMagneticSensorType()->blockSignals(true);
    item_magnetic_sensor_->model_scene()->model_view()->basic_widget()->getMagneticSensorType()->setCurrentIndex(type);
    item_magnetic_sensor_->model_scene()->model_view()->basic_widget()->getMagneticSensorType()->setOri_index(type);
    item_magnetic_sensor_->model_scene()->model_view()->basic_widget()->getMagneticSensorType()->blockSignals(false);
}

ItemBase *WidgetMagneticSensorSetting::getRelateItem()
{
    return item_magnetic_sensor_;
}
