#include "WidgetCrashDiPosSetting.h"
#include "ui_WidgetCrashDiPosSetting.h"
#include"ItemCrashDIpos.h"
#include"ItemCrashDI.h"
#include"../ModelHelper/ModelCustomCommand.h"
#include "RoLog.h"

WidgetCrashDiPosSetting::WidgetCrashDiPosSetting(ItemCrashDIpos* itemPos, const QPointF &pos, WidgetSetting *parent) :
    WidgetSetting(parent),
    item_crashDipos_(itemPos),
    ui(new Ui::WidgetCrashDiPosSetting)
{
    ui->setupUi(this);
     setType(WIDGET_CRASHDIPOS);
    ui->doubleSpinBox_x->setMinimum(-10);
    ui->doubleSpinBox_y->setMinimum(-10);//先设置坐标范围（默认大于0）
    set_value (pos);
    ui->label_type->setText(QString(tr("crashDI Contrl point")));
    ui->doubleSpinBox_x->setOriValue(ui->doubleSpinBox_x->value());
    ui->doubleSpinBox_y->setOriValue(ui->doubleSpinBox_y->value());

}

WidgetCrashDiPosSetting::~WidgetCrashDiPosSetting()
{
    delete ui;
}

void WidgetCrashDiPosSetting::set_value(const QPointF &pos)
{
    ui->doubleSpinBox_x->blockSignals (true);
    ui->doubleSpinBox_y->blockSignals (true);
    ui->doubleSpinBox_x->setValue (pos.x ()/100.0);
    ui->doubleSpinBox_y->setValue (pos.y ()/100.0);
    ui->doubleSpinBox_x->blockSignals (false);
    ui->doubleSpinBox_y->blockSignals (false);
}

void WidgetCrashDiPosSetting::update_value()
{

}

void WidgetCrashDiPosSetting::on_doubleSpinBox_x_valueChanged(double arg1)
{

    SCDebug<<"change x is "<<arg1;
    if(m_unStack == NULL)
    {
       m_unStack = UndoStack::InitStack();
    }
    SCDebug<<"ui->doubleSpinBox_x->getIsClickedTrigger() is "<<ui->doubleSpinBox_x->getIsClickedTrigger();
    if(ui->doubleSpinBox_x->getIsClickedTrigger())
    {
        ui->doubleSpinBox_x->setNewValue(arg1);
        ui->doubleSpinBox_y->setNewValue(ui->doubleSpinBox_y->value());
        PosDoubleSpinBoxCommand* dbspinbx_command = new PosDoubleSpinBoxCommand(ui->doubleSpinBox_x,
                                                                                ui->doubleSpinBox_y,
                                                                                item_crashDipos_,
                                                                                ui->doubleSpinBox_x->getOriValue(),
                                                                                ui->doubleSpinBox_x->getNewValue(),
                                                                                ui->doubleSpinBox_y->getOriValue(),
                                                                                ui->doubleSpinBox_y->getNewValue(),
                                                                                this,
                                                                                NULL);
        m_unStack->push(dbspinbx_command);
        ui->doubleSpinBox_x->setOriValue(arg1);
        ui->doubleSpinBox_y->setOriValue(ui->doubleSpinBox_y->value());
    }
    ui->doubleSpinBox_x->setIsClickedTrigger(true);
}

void WidgetCrashDiPosSetting::on_doubleSpinBox_y_valueChanged(double arg1)
{
    SCDebug<<"change y is "<<arg1;
    if(m_unStack == NULL)
    {
      m_unStack = UndoStack::InitStack();
    }
    if(ui->doubleSpinBox_y->getIsClickedTrigger())
    {
        SCDebug<<"polygonposdoublespinxcommand redo Finished y...";
        ui->doubleSpinBox_x->setNewValue(ui->doubleSpinBox_x->value());
        ui->doubleSpinBox_y->setNewValue(arg1);
        PosDoubleSpinBoxCommand* dbspinbx_command = new PosDoubleSpinBoxCommand(ui->doubleSpinBox_x,
                                                                                            ui->doubleSpinBox_y,
                                                                                            item_crashDipos_,
                                                                                            ui->doubleSpinBox_x->getOriValue(),
                                                                                            ui->doubleSpinBox_x->getNewValue(),
                                                                                            ui->doubleSpinBox_y->getOriValue(),
                                                                                            ui->doubleSpinBox_y->getNewValue(),
                                                                                            this,NULL );
        m_unStack->push(dbspinbx_command);
        ui->doubleSpinBox_x->setOriValue(ui->doubleSpinBox_x->value());
        ui->doubleSpinBox_y->setOriValue(arg1);
    }

    ui->doubleSpinBox_y->setIsClickedTrigger(true);
}

ItemBase *WidgetCrashDiPosSetting::getRelateItem()
{
    return item_crashDipos_;
}



