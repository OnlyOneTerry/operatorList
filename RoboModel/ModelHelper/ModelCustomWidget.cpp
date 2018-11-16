#include "ModelCustomWidget.h"
#include<QDebug>
#include"GlobalVariant.h"
customComboBox::customComboBox(QWidget *parent):
    QComboBox(parent)
{
         m_ori_index = 0 ;
         m_new_index = 0 ;
         m_isClickTriggered = true;
}


void customComboBox::wheelEvent(QWheelEvent *e)
{
      e->ignore();
}

void customComboBox::setOri_index(int idx)
{
    m_ori_index = idx;
}

int customComboBox::getOri_index()
{
    return m_ori_index;
}

void customComboBox::setNew_index(int idx)
{
    m_new_index = idx;
}

int customComboBox::getNew_index()
{
    return m_new_index;
}

void customComboBox::setIsClickedTrigger(bool trigger)
{
    m_isClickTriggered = trigger;
}

bool customComboBox::getIsClickedTrigger()
{
    return m_isClickTriggered;
}

void customComboBox::sendSignal()
{
    emit sig_indexChange(this->currentIndex());
}

customSpinBox::customSpinBox(QWidget *parent)
    :QSpinBox(parent)
{
    m_oriValue = 0 ;
    m_newValue = 0 ;
    m_isClickedTrigger = true;
}

void customSpinBox::wheelEvent(QWheelEvent *e)
{
    e->ignore();
}

void customSpinBox::enterEvent(QEvent *event)
{
    emit sig_show_tooltips();
}

void customSpinBox::setOriValue(int value)
{
    m_oriValue = value;
}

int customSpinBox::getOriValue()
{
    return m_oriValue;
}

void customSpinBox::setNewValue(int value)
{
    m_newValue = value;
}

int customSpinBox::getNewValue()
{
    return m_newValue;
}

bool customSpinBox::getIsClickedTrigger()
{
    return m_isClickedTrigger;
}

void customSpinBox::setDataType(dataType type)
{
    m_dataType = type;
}

dataType customSpinBox::getDataType()
{
    return m_dataType;
}

void customSpinBox::setIsClickedTrigger(bool trigger)
{
    m_isClickedTrigger = trigger;
}

customDoubleSpinBox::customDoubleSpinBox(QWidget *parent):
    QDoubleSpinBox(parent)
{
    m_oriValue = 0;
    m_newValue = 0;
    m_isClickedTrigger = true;
}

void customDoubleSpinBox::wheelEvent(QWheelEvent *e)
{
    e->ignore();
}

void customDoubleSpinBox::focusInEvent(QFocusEvent *event)
{
    GlobalVariant::getInst()->isRedo = false;
    QDoubleSpinBox::focusInEvent(event);
    this->selectAll();
}


void customDoubleSpinBox::setOriValue(double value)
{
     m_oriValue  = value;
}

double customDoubleSpinBox::getOriValue()
{
   return m_oriValue;
}

void customDoubleSpinBox::setNewValue(double value)
{
//    SCDebug<<"set new value is "<<value;

   m_newValue = value;
}

double customDoubleSpinBox::getNewValue()
{
   return m_newValue;
}

void customDoubleSpinBox::setIsClickedTrigger(bool trigger)
{
   m_isClickedTrigger = trigger;
}

bool customDoubleSpinBox::getIsClickedTrigger()
{
    return m_isClickedTrigger;
}

void customDoubleSpinBox::setDataType(dataType type)
{
    m_dataType = type;
}

dataType customDoubleSpinBox::getDataType()
{
    return m_dataType;
}

customPushBtn::customPushBtn(QWidget *parent)
    :QPushButton(parent)
    ,m_oriState(0)
    ,m_newState(0)
{

}

void customPushBtn::setOriState(int oriState)
{
   m_oriState = oriState;
}

int customPushBtn::getOriState()
{
  return m_oriState;
}

void customPushBtn::setNewState(int newState)
{
  m_newState = newState;
}

int customPushBtn::getNewState()
{
  return m_newState;
}

customLineEdit::customLineEdit(QWidget *parent)
    :QLineEdit(parent)
{

}

void customLineEdit::setOriValue(QString val)
{
   m_oriValue = val;
}

QString customLineEdit::getOriValue()
{
  return m_oriValue;
}

void customLineEdit::setNewValue(QString val)
{
   m_newValue = val;
}

QString customLineEdit::getNewValue()
{
    return m_newValue;
}

customCheckBox::customCheckBox(QWidget *parent)
    :QCheckBox(parent)
{

}

void customCheckBox::setOriValue(bool val)
{
    m_oriValue = val;
}

bool customCheckBox::getOriValue()
{
   return m_oriValue;
}

void customCheckBox::setNewValue(bool val)
{
   m_newValue = val;
}

bool customCheckBox::getNewValue()
{
   return m_newValue;
}
