#ifndef WIDGETCRASHDIPOSSETTING_H
#define WIDGETCRASHDIPOSSETTING_H

#include <QWidget>
#include"WidgetSetting.h"
namespace Ui {
class WidgetCrashDiPosSetting;
}

class ItemCrashDIpos;

class WidgetCrashDiPosSetting :  public WidgetSetting
{
    Q_OBJECT
public:
    enum{Type = 7};int type() const{return Type;}
    explicit WidgetCrashDiPosSetting(ItemCrashDIpos* itemPos,const QPointF&  pos, WidgetSetting *parent = 0);
    ~WidgetCrashDiPosSetting();
public:
    void set_value(const QPointF& pos);
protected:
    void update_value();
signals:
    void sig_value_changed(qreal,qreal);
private slots:
    void on_doubleSpinBox_x_valueChanged(double arg1);
    void on_doubleSpinBox_y_valueChanged(double arg1);
public:
     ItemBase* getRelateItem();
private:
    Ui::WidgetCrashDiPosSetting *ui;

   ItemCrashDIpos* item_crashDipos_;
};

#endif // WIDGETCRASHDIPOSSETTING_H
