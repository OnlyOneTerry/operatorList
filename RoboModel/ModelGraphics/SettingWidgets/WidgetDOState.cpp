#include "WidgetDOState.h"
#include "ui_WidgetDOState.h"
#include"ModelCustomCommand.h"
#include"UndoStack.h"

WidgetDOState::WidgetDOState(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetDOState),
    m_undoStack(NULL)
{
    ui->setupUi(this);
    m_undoStack = UndoStack::InitStack();
    ui->btn_do->setFixedSize(28,28);
    ui->btn_do->setStyleSheet(QString("QPushButton{border-width: 1px;border-style: solid;border-color: rgb(255, 120, 0);border-radius:14px;}"));
}

WidgetDOState::~WidgetDOState()
{
    delete ui;
}

void WidgetDOState::setValue(quint8 value)
{
    if(value==0)
    {
        ui->btn_do->blockSignals(true);
        ui->btn_do->setChecked(false);
        ui->btn_do->setStyleSheet(QString("QPushButton{background-color:red;border-radius:14px;}"));
        ui->btn_do->blockSignals(false);
    }
    else
    {
        ui->btn_do->blockSignals(true);
        ui->btn_do->setChecked(true);
        ui->btn_do->setStyleSheet(QString("QPushButton{background-color:green;border-radius:14px;}"));
        ui->btn_do->blockSignals(false);
    }

    value_=value;

    ui->btn_do->setOriState(value);
}

quint8 WidgetDOState::getValue()
{
    return value_;
}

void WidgetDOState::set_id_text(QString str)
{
    ui->btn_do->setText(str);
}

void WidgetDOState::on_btn_do_toggled(bool checked)
{
    value_=checked?1:0;

    if(m_undoStack==NULL)
    {
        qDebug()<<"WidgetDOState undoStack is null.........";
        return;
    }

    ui->btn_do->setNewState(value_);

    DoStateCommand* do_stateCommand = new DoStateCommand(ui->btn_do,ui->btn_do->getOriState(),ui->btn_do->getNewState(),NULL);

    m_undoStack->push(do_stateCommand);

    ui->btn_do->setOriState(value_);

}
