#ifndef WIDGETSETTING_H
#define WIDGETSETTING_H

#include <QObject>
#include <QWidget>
#include <QDebug>
#include<QUndoStack>
#include"PropertyBasicWidget.h"
#include"ItemBase.h"
#include"GlobalVariant.h"
class WidgetSetting : public PropertyBasicWidget
{
    Q_OBJECT
public:
    enum{Type = 0};virtual int type() const{return Type; }
public:
    explicit WidgetSetting(QWidget *parent = 0);
    ~WidgetSetting();


public:
    virtual void update_value()=0;
    virtual void set_r(const qreal& angel);
    virtual ItemBase* getRelateItem();
signals:
    void sig_deleted(int);

public slots:

protected:
    QUndoStack* m_unStack;
};

#endif // WIDGETSETTING_H
