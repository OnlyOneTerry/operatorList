#ifndef WIDGETMAGNETICSENSORSETTING_H
#define WIDGETMAGNETICSENSORSETTING_H

#include <WidgetSetting.h>

class ItemMagneticSensor;


namespace Ui {
class WidgetMagneticSensorSetting;
}

class WidgetMagneticSensorSetting : public WidgetSetting
{
    Q_OBJECT
public:
    enum{Type = 3};int type() const{return Type;}
public:
    explicit WidgetMagneticSensorSetting(ItemMagneticSensor *isensor,QWidget *parent = 0);
    ~WidgetMagneticSensorSetting();
    void initWidgetData();
public:
    void update_value();
    void set_r(const qreal &angel);

private slots:
    void on_spinBox_id_valueChanged(int arg1);
    void on_doubleSpinBox_x_valueChanged(double arg1);
    void on_doubleSpinBox_y_valueChanged(double arg1);
    void on_doubleSpinBox_z_valueChanged(double arg1);
    void on_doubleSpinBox_r_valueChanged(double arg1);
    void on_spinBox_resolution_valueChanged(int arg1);
    void on_doubleSpinBox_step_valueChanged(double arg1);

    void on_spinBox_id_editingFinished();
    void on_timer_out();

public:
    void set_step_byGuidConfigure(double arg);
    void set_resolution_byGuidConfigure(int arg);
    void set_Type_byGuidConfigure(int type);
public:
     ItemBase* getRelateItem();
private:
    Ui::WidgetMagneticSensorSetting *ui;
    ItemMagneticSensor* item_magnetic_sensor_;
};

#endif // WIDGETMAGNETICSENSORSETTING_H
