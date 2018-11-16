#ifndef CUSTOMWIDGET_H
#define CUSTOMWIDGET_H

#include <QSpinBox>
#include<QWheelEvent>
#include<QComboBox>
#include<QDropEvent>
#include<QPushButton>
#include"ModelCommon.h"
#include<QLineEdit>
#include<QCheckBox>
#include"ListWidgets/SCDragDropListWidget.h"
#include<QKeyEvent>
class customComboBox:public  QComboBox
{
    Q_OBJECT
public:
    customComboBox(QWidget* parent = 0);

    void wheelEvent(QWheelEvent * e);

    void setOri_index(int idx);

    int getOri_index();

    void setNew_index(int idx);

    int getNew_index();

    void setIsClickedTrigger(bool trigger);

    bool getIsClickedTrigger();
    void sendSignal();
 signals:
    void sig_indexChange(int);

private:
    int m_ori_index;
    int m_new_index;
    bool m_isClickTriggered;
};



class customSpinBox : public QSpinBox
{
    Q_OBJECT
public:
    explicit customSpinBox(QWidget *parent = 0);

    void setOriValue(int value);
    int getOriValue();

    void setNewValue(int value);
    int getNewValue();

    void setIsClickedTrigger(bool trigger);
    bool getIsClickedTrigger();
    void setDataType(dataType type);
    dataType getDataType();
 signals:
    void sig_show_tooltips();
public:
    void wheelEvent(QWheelEvent *event);
    void enterEvent(QEvent *event);
private:
    int m_oriValue;
    int m_newValue;
    bool m_isClickedTrigger;
    dataType m_dataType;
};

class customDoubleSpinBox:  public QDoubleSpinBox //public  DoubleSpinBox
 {
 public:
     customDoubleSpinBox(QWidget* parent=0);

     void wheelEvent(QWheelEvent * e);
     void focusInEvent(QFocusEvent *event);
     void setOriValue(double value);
     double getOriValue();

     void setNewValue(double value);
     double getNewValue();

     void setIsClickedTrigger(bool trigger);
     bool getIsClickedTrigger();

     void setDataType(dataType type);
     dataType getDataType();

 private:
     double m_oriValue;
     double m_newValue;
     bool m_isClickedTrigger;
     dataType m_dataType;

 };

class customPushBtn:public QPushButton
{
public:
    customPushBtn(QWidget* parent = 0);

    void setOriState(int oriState);
    int getOriState();

    void setNewState(int newState);
    int getNewState();
private:
    int m_oriState;
    int m_newState;

};

class customLineEdit:public QLineEdit
{
public:
  customLineEdit(QWidget *parent = Q_NULLPTR);

  void setOriValue(QString val);
  QString getOriValue();

  void setNewValue(QString val);
  QString getNewValue();

private:
  QString m_oriValue;
  QString m_newValue;
};

class customCheckBox:public QCheckBox
{
  public:
    customCheckBox(QWidget *parent = Q_NULLPTR);

    void setOriValue(bool val);
    bool getOriValue();

    void setNewValue(bool val);
    bool getNewValue();

private:
    bool m_oriValue;
    bool m_newValue;

};

#endif // CUSTOMWIDGET_H
