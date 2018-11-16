#include "WidgetPosSetting.h"
#include "ui_WidgetPosSetting.h"
#include "ItemPolypos.h"
#include "QDebug"
#include"ModelCustomCommand.h"
#include"ItemCrashDIpos.h"
WidgetPosSetting::WidgetPosSetting(ItemPolypos *polypos, const QPointF&  pos, QWidget *parent) :
    WidgetSetting(parent),
    item_polypos_(polypos),
    ui(new Ui::WidgetPosSetting)
{
    ui->setupUi(this);
    set_value (pos);
    ui->label_type->setText(tr("Polygonal point"));
    ui->doubleSpinBox_x->setOriValue(ui->doubleSpinBox_x->value());
    ui->doubleSpinBox_y->setOriValue(ui->doubleSpinBox_y->value());

    connect(ui->doubleSpinBox_x,SIGNAL(valueChanged(double)),this,SLOT(on_doubleSpinBox_x_changed()));
    connect(ui->doubleSpinBox_y,SIGNAL(valueChanged(double)),this,SLOT(on_doubleSpinBox_y_changed()));
    connect(item_polypos_,SIGNAL(pos_changed(qreal,qreal)),this,SLOT(on_doubleSpinx_x_y_changed(qreal,qreal)));
}


WidgetPosSetting::~WidgetPosSetting()
{
    delete ui;
}

void WidgetPosSetting::set_value(const QPointF& pos)
{
    ui->doubleSpinBox_x->blockSignals (true);
    ui->doubleSpinBox_y->blockSignals (true);
    ui->doubleSpinBox_x->setValue (pos.x ()/100.0);
    ui->doubleSpinBox_y->setValue (pos.y ()/100.0);
    ui->doubleSpinBox_x->blockSignals (false);
    ui->doubleSpinBox_y->blockSignals (false);
}

ItemBase *WidgetPosSetting::getRelateItem()
{
    return item_polypos_;
}

void WidgetPosSetting::on_doubleSpinBox_x_editingFinished()
{
    //    item_polypos_->slot_change_pos (ui->doubleSpinBox_x->value ()*100,ui->doubleSpinBox_y->value ()*100);
    if(m_unStack == NULL)
    {
        return;
    }


    if(ui->doubleSpinBox_x->getIsClickedTrigger())
    {
        ui->doubleSpinBox_x->setNewValue(ui->doubleSpinBox_x->value());
        ui->doubleSpinBox_y->setNewValue(ui->doubleSpinBox_y->value());

        PosDoubleSpinBoxCommand* dbspinbx_command = new PosDoubleSpinBoxCommand(ui->doubleSpinBox_x,
                                                                                            ui->doubleSpinBox_y,
                                                                                            item_polypos_,
                                                                                            ui->doubleSpinBox_x->getOriValue(),
                                                                                            ui->doubleSpinBox_x->getNewValue(),
                                                                                            ui->doubleSpinBox_y->getOriValue(),
                                                                                            ui->doubleSpinBox_y->getNewValue(),
                                                                                            NULL );

        m_unStack->push(dbspinbx_command);

        ui->doubleSpinBox_x->setOriValue(ui->doubleSpinBox_x->value());
        ui->doubleSpinBox_y->setOriValue(ui->doubleSpinBox_y->value());
    }

    ui->doubleSpinBox_x->setIsClickedTrigger(true);
}

void WidgetPosSetting::on_doubleSpinBox_y_editingFinished()
{
    if(m_unStack == NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_y->getIsClickedTrigger())
    {
        ui->doubleSpinBox_x->setNewValue(ui->doubleSpinBox_x->value());
        ui->doubleSpinBox_y->setNewValue(ui->doubleSpinBox_y->value());

        PosDoubleSpinBoxCommand* dbspinbx_command = new PosDoubleSpinBoxCommand(ui->doubleSpinBox_x,
                                                                                            ui->doubleSpinBox_y,
                                                                                            item_polypos_,
                                                                                            ui->doubleSpinBox_x->getOriValue(),
                                                                                            ui->doubleSpinBox_x->getNewValue(),
                                                                                            ui->doubleSpinBox_y->getOriValue(),
                                                                                            ui->doubleSpinBox_y->getNewValue(),
                                                                                            NULL );
        m_unStack->push(dbspinbx_command);

        ui->doubleSpinBox_x->setOriValue(ui->doubleSpinBox_x->value());
        ui->doubleSpinBox_y->setOriValue(ui->doubleSpinBox_y->value());
    }

    ui->doubleSpinBox_y->setIsClickedTrigger(true);
}

void WidgetPosSetting::on_doubleSpinBox_x_changed()
{
    if(m_unStack == NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_x->getIsClickedTrigger())
    {
        ui->doubleSpinBox_x->setNewValue(ui->doubleSpinBox_x->value());
        ui->doubleSpinBox_y->setNewValue(ui->doubleSpinBox_y->value());

        PosDoubleSpinBoxCommand* dbspinbx_command = new PosDoubleSpinBoxCommand(ui->doubleSpinBox_x,
                                                                                            ui->doubleSpinBox_y,
                                                                                            item_polypos_,
                                                                                            ui->doubleSpinBox_x->getOriValue(),
                                                                                            ui->doubleSpinBox_x->getNewValue(),
                                                                                            ui->doubleSpinBox_y->getOriValue(),
                                                                                            ui->doubleSpinBox_y->getNewValue(),
                                                                                            NULL );
        m_unStack->push(dbspinbx_command);

        ui->doubleSpinBox_x->setOriValue(ui->doubleSpinBox_x->value());
        ui->doubleSpinBox_y->setOriValue(ui->doubleSpinBox_y->value());
    }

    ui->doubleSpinBox_x->setIsClickedTrigger(true);
}

void WidgetPosSetting::on_doubleSpinBox_y_changed()
{
    if(m_unStack == NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_x->getIsClickedTrigger()&&ui->doubleSpinBox_y->getIsClickedTrigger())
    {
        ui->doubleSpinBox_x->setNewValue(ui->doubleSpinBox_x->value());
        ui->doubleSpinBox_y->setNewValue(ui->doubleSpinBox_y->value());

        PosDoubleSpinBoxCommand* dbspinbx_command = new PosDoubleSpinBoxCommand(
                    ui->doubleSpinBox_x,
                    ui->doubleSpinBox_y,
                    item_polypos_,
                    ui->doubleSpinBox_x->getOriValue(),
                    ui->doubleSpinBox_x->getNewValue(),
                    ui->doubleSpinBox_y->getOriValue(),
                    ui->doubleSpinBox_y->getNewValue(),
                    NULL );

        m_unStack->push(dbspinbx_command);

        ui->doubleSpinBox_x->setOriValue(ui->doubleSpinBox_x->value());
        ui->doubleSpinBox_y->setOriValue(ui->doubleSpinBox_y->value());
    }

    ui->doubleSpinBox_x->setIsClickedTrigger(true);
    ui->doubleSpinBox_y->setIsClickedTrigger(true);
}

void WidgetPosSetting::on_doubleSpinx_x_y_changed(qreal x, qreal y)
{
    if(m_unStack == NULL)
    {
        return;
    }

    if(ui->doubleSpinBox_x->getIsClickedTrigger()|| ui->doubleSpinBox_y->getIsClickedTrigger())
    {
        ui->doubleSpinBox_x->setNewValue(ui->doubleSpinBox_x->value());
        ui->doubleSpinBox_y->setNewValue(ui->doubleSpinBox_y->value());

        PosDoubleSpinBoxCommand* dbspinbx_command = new PosDoubleSpinBoxCommand(ui->doubleSpinBox_x,
                                                                                            ui->doubleSpinBox_y,
                                                                                            item_polypos_,
                                                                                            ui->doubleSpinBox_x->getOriValue(),
                                                                                            ui->doubleSpinBox_x->getNewValue(),
                                                                                            ui->doubleSpinBox_y->getOriValue(),
                                                                                            ui->doubleSpinBox_y->getNewValue(),
                                                                                            NULL );
        m_unStack->push(dbspinbx_command);

        ui->doubleSpinBox_x->setOriValue(ui->doubleSpinBox_x->value());
        ui->doubleSpinBox_y->setOriValue(ui->doubleSpinBox_y->value());

    }
    ui->doubleSpinBox_y->setIsClickedTrigger(true);
    ui->doubleSpinBox_x->setIsClickedTrigger(true);
}

void WidgetPosSetting::update_value (){}
