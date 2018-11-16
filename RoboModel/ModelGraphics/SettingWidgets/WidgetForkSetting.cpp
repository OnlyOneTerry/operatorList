#include "WidgetForkSetting.h"
#include "ui_WidgetForkSetting.h"
#include"ModelCustomCommand.h"
WidgetForkSetting::WidgetForkSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WidgetForkSetting)
{
    ui->setupUi(this);
    m_undostack = UndoStack::InitStack();
    initUI();
    initWidgetData();
}

WidgetForkSetting::~WidgetForkSetting()
{
    delete ui;
}

void WidgetForkSetting::on_dbSpBox_blkLaserDist_valueChanged(double arg1)
{
    forkData_.blockLaserDist= arg1;

    if(m_undostack==NULL)
    {
        return;
    }

    ui->dbSpBox_blkLaserDist->setNewValue(arg1);
    if(ui->dbSpBox_blkLaserDist->getIsClickedTrigger())
    {
        doubleSpinBoxCommand* tempCmd = new doubleSpinBoxCommand(ui->dbSpBox_blkLaserDist,
                                                                 NULL,ui->dbSpBox_blkLaserDist->getOriValue(),
                                                                 ui->dbSpBox_blkLaserDist->getNewValue(),parentWid_,
                                                                 NULL
                                                                 );
        m_undostack->push(tempCmd);
    }
    ui->dbSpBox_blkLaserDist->setOriValue(arg1);
    ui->dbSpBox_blkLaserDist->setIsClickedTrigger(true);
}

void WidgetForkSetting::on_spinBox_encoderSteps_valueChanged(int arg1)
{
    forkData_.encoderSteps = arg1;
    if(m_undostack==NULL)
    {
        return;
    }
    ui->spinBox_encoderSteps->setNewValue(arg1);
    if(ui->spinBox_encoderSteps->getIsClickedTrigger())
    {
        SpinBoxCommand* tempCmd = new SpinBoxCommand(ui->spinBox_encoderSteps,
                                                     ui->spinBox_encoderSteps->getOriValue(),
                                                     ui->spinBox_encoderSteps->getNewValue(),parentWid_,
                                                     NULL
                                                     );
        m_undostack->push(tempCmd);
    }
    ui->spinBox_encoderSteps->setOriValue(arg1);
    ui->spinBox_encoderSteps->setIsClickedTrigger(true);
}

void WidgetForkSetting::on_dbSpBox_lengthFactor_valueChanged(double arg1)
{
    forkData_.lengthFactor = arg1;
    if(m_undostack==NULL)
    {
        return;
    }
    ui->dbSpBox_lengthFactor->setNewValue(arg1);
    if(ui->dbSpBox_lengthFactor->getIsClickedTrigger())
    {
        doubleSpinBoxCommand* tempCmd = new doubleSpinBoxCommand(ui->dbSpBox_lengthFactor,
                                                                 NULL,ui->dbSpBox_lengthFactor->getOriValue(),
                                                                 ui->dbSpBox_lengthFactor->getNewValue(),parentWid_,
                                                                 NULL
                                                                 );
        m_undostack->push(tempCmd);
    }
    ui->dbSpBox_lengthFactor->setOriValue(arg1);
    ui->dbSpBox_lengthFactor->setIsClickedTrigger(true);
}

void WidgetForkSetting::on_dbSpinBox_lengthPerTurn_valueChanged(double arg1)
{
    forkData_.lengthPerTurn = arg1;
    if(m_undostack==NULL)
    {
        return;
    }
    ui->dbSpinBox_lengthPerTurn->setNewValue(arg1);
    if(ui->dbSpinBox_lengthPerTurn->getIsClickedTrigger())
    {
        doubleSpinBoxCommand* tempCmd = new doubleSpinBoxCommand(ui->dbSpinBox_lengthPerTurn,
                                                                 NULL,ui->dbSpinBox_lengthPerTurn->getOriValue(),
                                                                 ui->dbSpinBox_lengthPerTurn->getNewValue(),parentWid_,
                                                                 NULL
                                                                 );
        m_undostack->push(tempCmd);
    }
    ui->dbSpinBox_lengthPerTurn->setOriValue(arg1);
    ui->dbSpinBox_lengthPerTurn->setIsClickedTrigger(true);
}

void WidgetForkSetting::on_dbSpinBox_maxHeight_valueChanged(double arg1)
{
    forkData_.maxHeight = arg1;
    if(m_undostack==NULL)
    {
        return;
    }
    ui->dbSpinBox_maxHeight->setNewValue(arg1);
    if(ui->dbSpinBox_maxHeight->getIsClickedTrigger())
    {
        doubleSpinBoxCommand* tempCmd = new doubleSpinBoxCommand(ui->dbSpinBox_maxHeight,
                                                                 NULL,ui->dbSpinBox_maxHeight->getOriValue(),
                                                                 ui->dbSpinBox_maxHeight->getNewValue(),parentWid_,
                                                                 NULL
                                                                 );
        m_undostack->push(tempCmd);
    }
    ui->dbSpinBox_maxHeight->setOriValue(arg1);
    ui->dbSpinBox_maxHeight->setIsClickedTrigger(true);
}

void WidgetForkSetting::on_dbSpinBox_minHeight_valueChanged(double arg1)
{
    forkData_.minHeight = arg1;
    if(m_undostack==NULL)
    {
        return;
    }
    ui->dbSpinBox_minHeight->setNewValue(arg1);
    if(ui->dbSpinBox_minHeight->getIsClickedTrigger())
    {
        doubleSpinBoxCommand* tempCmd = new doubleSpinBoxCommand(ui->dbSpinBox_minHeight,
                                                                 NULL,ui->dbSpinBox_minHeight->getOriValue(),
                                                                 ui->dbSpinBox_minHeight->getNewValue(),parentWid_,
                                                                 NULL
                                                                 );
        m_undostack->push(tempCmd);
    }
    ui->dbSpinBox_minHeight->setOriValue(arg1);
    ui->dbSpinBox_minHeight->setIsClickedTrigger(true);
}

void WidgetForkSetting::on_dbSpinBox_downBuffer_valueChanged(double arg1)
{
    forkData_.downBuffer = arg1;
    if(m_undostack==NULL)
    {
        return;
    }
    ui->dbSpinBox_downBuffer->setNewValue(arg1);
    if(ui->dbSpinBox_downBuffer->getIsClickedTrigger())
    {
        doubleSpinBoxCommand* tempCmd = new doubleSpinBoxCommand(ui->dbSpinBox_downBuffer,
                                                                 NULL,ui->dbSpinBox_downBuffer->getOriValue(),
                                                                 ui->dbSpinBox_downBuffer->getNewValue(),parentWid_,
                                                                 NULL
                                                                 );
        m_undostack->push(tempCmd);
    }
    ui->dbSpinBox_downBuffer->setOriValue(arg1);
    ui->dbSpinBox_downBuffer->setIsClickedTrigger(true);
}

void WidgetForkSetting::on_dbSpinBox_upBuffer_valueChanged(double arg1)
{
    forkData_.upBuffer = arg1;
    if(m_undostack==NULL)
    {
        return;
    }
    ui->dbSpinBox_upBuffer->setNewValue(arg1);
    if(ui->dbSpinBox_upBuffer->getIsClickedTrigger())
    {
        doubleSpinBoxCommand* tempCmd = new doubleSpinBoxCommand(ui->dbSpinBox_upBuffer,
                                                                 NULL,ui->dbSpinBox_upBuffer->getOriValue(),
                                                                 ui->dbSpinBox_upBuffer->getNewValue(),parentWid_,
                                                                 NULL
                                                                 );
        m_undostack->push(tempCmd);
    }
    ui->dbSpinBox_upBuffer->setOriValue(arg1);
    ui->dbSpinBox_upBuffer->setIsClickedTrigger(true);
}

void WidgetForkSetting::on_comboBox_pump_currentIndexChanged(int index)
{
    forkData_.pump = ui->comboBox_pump->currentData(Qt::UserRole).toInt();
    if(m_undostack==NULL)
    {
        return;
    }
    ui->comboBox_pump->setNew_index(index);
    if(ui->comboBox_pump->getIsClickedTrigger())
    {
        customboxCommand* tempCmd = new customboxCommand(ui->comboBox_pump,
                                                         ui->comboBox_pump->getOri_index(),
                                                         ui->comboBox_pump->getNew_index(),parentWid_,
                                                         NULL
                                                         );
        m_undostack->push(tempCmd);
    }
    ui->comboBox_pump->setOri_index(index);
    ui->comboBox_pump->setIsClickedTrigger(true);
}

void WidgetForkSetting::on_comboBox_forkType_currentIndexChanged(int index)
{
    forkData_.type = ui->comboBox_forkType->currentData(Qt::UserRole).toInt();
    if(m_undostack==NULL)
    {
        return;
    }
    ui->comboBox_forkType->setNew_index(index);
    if(ui->comboBox_forkType->getIsClickedTrigger())
    {
        customboxCommand* tempCmd = new customboxCommand(ui->comboBox_forkType,
                                                         ui->comboBox_forkType->getOri_index(),
                                                         ui->comboBox_forkType->getNew_index(),parentWid_,
                                                         NULL
                                                         );
        m_undostack->push(tempCmd);
    }
    ui->comboBox_forkType->setOri_index(index);
    ui->comboBox_forkType->setIsClickedTrigger(true);
}

void WidgetForkSetting::on_comboBox_pressureSensor_currentIndexChanged(int index)
{
    forkData_.pressureSensor = ui->comboBox_pressureSensor->currentData(Qt::UserRole).toInt();
    if(m_undostack==NULL)
    {
        return;
    }
    ui->comboBox_pressureSensor->setNew_index(index);
    if(ui->comboBox_pressureSensor->getIsClickedTrigger())
    {
        customboxCommand* tempCmd = new customboxCommand(ui->comboBox_pressureSensor,
                                                         ui->comboBox_pressureSensor->getOri_index(),
                                                         ui->comboBox_pressureSensor->getNew_index(),parentWid_,
                                                         NULL
                                                         );
        m_undostack->push(tempCmd);
    }
    ui->comboBox_pressureSensor->setOri_index(index);
    ui->comboBox_pressureSensor->setIsClickedTrigger(true);
}

void WidgetForkSetting::setParentWid(PropertyBasicWidget *wid)
{
    parentWid_ = wid;
}

void WidgetForkSetting::initUI()
{
    ui->comboBox_forkType->setStyleSheet("QComboBox{min-height: 20px;}");
    ui->comboBox_pressureSensor->setStyleSheet("QComboBox{min-height: 20px;}");
    ui->comboBox_pump->setStyleSheet("QComboBox{min-height: 20px;}");
    ui->comboBox_forkType->blockSignals(true);
    ui->comboBox_forkType->addItem(tr("No cargo fork"),0);
    ui->comboBox_forkType->addItem(tr("Van"),1);
    ui->comboBox_forkType->addItem(tr("Heap high device"),2);
    ui->comboBox_forkType->blockSignals(false);
    ui->comboBox_pressureSensor->blockSignals(true);
    ui->comboBox_pressureSensor->addItem(tr("No"),0);
    ui->comboBox_pressureSensor->addItem(tr("Yes"),1);
    ui->comboBox_pressureSensor->blockSignals(false);

    ui->comboBox_pump->blockSignals(true);
    ui->comboBox_pump->addItem(tr("No"),0);
    ui->comboBox_pump->addItem(tr("Yes"),1);
    ui->comboBox_pump->blockSignals(false);
}

void WidgetForkSetting::initWidgetData()
{
    ui->comboBox_pressureSensor->blockSignals(true);
    int idx = ui->comboBox_pressureSensor->findData(forkData_.pressureSensor);
    ui->comboBox_pressureSensor->setCurrentIndex(idx);
    ui->comboBox_pressureSensor->setOri_index(idx);
    ui->comboBox_pressureSensor->blockSignals(false);
    ui->comboBox_forkType->blockSignals(true);
    idx = ui->comboBox_forkType->findData(forkData_.type);
    ui->comboBox_forkType->setCurrentIndex(idx);
    ui->comboBox_forkType->setOri_index(idx);
    ui->comboBox_forkType->blockSignals(false);
    ui->comboBox_pump->blockSignals(true);
    idx = ui->comboBox_pump->findData(forkData_.pump);
    ui->comboBox_pump->setCurrentIndex(idx);
    ui->comboBox_pump->setOri_index(idx);
    ui->comboBox_pump->blockSignals(false);
    ui->dbSpBox_blkLaserDist->blockSignals(true);
    ui->dbSpBox_blkLaserDist->setValue(forkData_.blockLaserDist);
    ui->dbSpBox_blkLaserDist->setOriValue(forkData_.blockLaserDist);
    ui->dbSpBox_blkLaserDist->blockSignals(false);
    ui->spinBox_encoderSteps->blockSignals(true);
    ui->spinBox_encoderSteps->setValue(forkData_.encoderSteps);
    ui->spinBox_encoderSteps->setOriValue(forkData_.encoderSteps);
    ui->spinBox_encoderSteps->blockSignals(false);
    ui->dbSpBox_lengthFactor->blockSignals(true);
    ui->dbSpBox_lengthFactor->setValue(forkData_.lengthFactor);
    ui->dbSpBox_lengthFactor->setOriValue(forkData_.lengthFactor);
    ui->dbSpBox_lengthFactor->blockSignals(false);
    ui->dbSpinBox_downBuffer->blockSignals(true);
    ui->dbSpinBox_downBuffer->setValue(forkData_.downBuffer);
    ui->dbSpinBox_downBuffer->setOriValue(forkData_.downBuffer);
    ui->dbSpinBox_downBuffer->blockSignals(false);
    ui->dbSpinBox_upBuffer->blockSignals(true);
    ui->dbSpinBox_upBuffer->setValue(forkData_.upBuffer);
    ui->dbSpinBox_upBuffer->setOriValue(forkData_.upBuffer);
    ui->dbSpinBox_upBuffer->blockSignals(false);
    ui->dbSpinBox_maxHeight->blockSignals(true);
    ui->dbSpinBox_maxHeight->setValue(forkData_.maxHeight);
    ui->dbSpinBox_maxHeight->setOriValue(forkData_.maxHeight);
    ui->dbSpinBox_maxHeight->blockSignals(false);
    ui->dbSpinBox_minHeight->blockSignals(true);
    ui->dbSpinBox_minHeight->setValue(forkData_.minHeight);
    ui->dbSpinBox_minHeight->setOriValue(forkData_.minHeight);
    ui->dbSpinBox_minHeight->blockSignals(false);
}

void WidgetForkSetting::setForkData(ForkData data)
{
    forkData_ = data;
}

ForkData WidgetForkSetting::getForkdata()
{
    return forkData_;
}

void WidgetForkSetting::setIsConfigureFork(bool configure)
{
    m_isConfigureFork = configure;
}

bool WidgetForkSetting::getIsConfigureFork()
{
    return m_isConfigureFork;
}
