#ifndef WIDGETRFIDSETTING_H
#define WIDGETRFIDSETTING_H

#include <QWidget>
#include"WidgetSetting.h"
class ItemRFIDSensor;
namespace Ui {
class WidgetRFIDsetting;
}

class WidgetRFIDsetting : public  WidgetSetting
{
    Q_OBJECT

public:
    explicit WidgetRFIDsetting(ItemRFIDSensor* rfid, QWidget *parent = 0);
    ~WidgetRFIDsetting();
    void update_value();
    void initWidgetData();
private slots:
    void on_doubleSpinBox_x_valueChanged(double arg1);

    void on_doubleSpinBox_y_valueChanged(double arg1);
public:
     ItemBase* getRelateItem();
private:
    Ui::WidgetRFIDsetting *ui;

    ItemRFIDSensor* itemRFID_;
};

#endif // WIDGETRFIDSETTING_H
