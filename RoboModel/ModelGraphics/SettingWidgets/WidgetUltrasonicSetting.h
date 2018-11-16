#ifndef WIDGETULTRASONICSETTING_H
#define WIDGETULTRASONICSETTING_H
#include"ModelCommon.h"
#include"ModelCustomCommand.h"

#include "WidgetSetting.h"
class ItemUltrasonic;


namespace Ui {
class WidgetUltrasonicSetting;
}

class WidgetUltrasonicSetting : public WidgetSetting
{
    Q_OBJECT
public:
    enum{Type = 8};int type() const{return Type;}
public:
    explicit WidgetUltrasonicSetting(ItemUltrasonic* iu,QWidget *parent = 0);
    ~WidgetUltrasonicSetting();

public:
    void update_value();
    void set_r(const qreal& angel);
    ConfigureData readFuncTypeFile(const QString filePath);
    void initWigetData();
private slots:
    void on_spinBox_id_valueChanged(int arg1);
    void on_doubleSpinBox_x_valueChanged(double arg1);
    void on_doubleSpinBox_y_valueChanged(double arg1);
    void on_doubleSpinBox_z_valueChanged(double arg1);
    void on_doubleSpinBox_minDist_valueChanged(double arg1);
    void on_doubleSpinBox_maxDist_valueChanged(double arg1);
    void on_doubleSpinBox_range_valueChanged(double arg1);
    void on_doubleSpinBox_r_valueChanged(double arg1);
    void on_comboBox_func_currentIndexChanged(int index);

    void slot_max_mum_changed(quint8 type, quint8 max_value);
    void on_spinBox_id_editingFinished();
    void on_timer_out();

public:
    void set_func_byGuidConfigure(int idex);
    void set_range_byGuidConfigure(double arg);
    void set_minDist_byGuidConfigure(double arg);
    void set_maxDist_byGuidConfigure(double arg);
    void set_Type_byGuidConfigure(int idex);
public:
     ItemBase* getRelateItem();
private:
    Ui::WidgetUltrasonicSetting *ui;
    ItemUltrasonic *item_ultrasonic_;

};

#endif // WIDGETULTRASONICSETTING_H
