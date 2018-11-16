#ifndef WIDGETLASERSETTING_H
#define WIDGETLASERSETTING_H

#include <QWidget>
#include <WidgetSetting.h>
#include"ModelCommon.h"
class ItemLaser;

namespace Ui {
class WidgetLaserSetting;
}

class WidgetLaserSetting : public WidgetSetting
{
    Q_OBJECT
public:
    enum{Type = 2};int type() const{return Type;}
public:
    explicit WidgetLaserSetting(ItemLaser *item_laser,QWidget *parent = 0);
    ~WidgetLaserSetting();

    ConfigureData readLaserTypeFile(const QString& filePath);
public:
    void update_value();
    void set_r(const qreal& angel);

public:
    void set_upside_byGuidConfigure(bool arg);
    void set_forLocation_byGuidConfigure(bool arg);
    void set_step_byGuidConfigure(double arg);
    void set_minAngle_byGuidConfigure(double arg);
    void set_maxAngle_byGuidConfigure(double arg);
    void set_Ip_byGuidConfigure(QString& ip);
    void set_port_byGuidConfigure(int port);
    void set_model_byGuidConfigure(QString model);
private slots:
    void on_comboBox_model_currentIndexChanged(const QString &arg1);
    void on_doubleSpinBox_r_valueChanged(double arg1);
    void on_doubleSpinBox_x_valueChanged(double arg1);
    void on_doubleSpinBox_y_valueChanged(double arg1);
    void on_doubleSpinBox_z_valueChanged(double arg1);
    void on_checkBox_upside_toggled(bool checked);
    void on_checkBox_useForLocalization_toggled(bool checked);
    void on_doubleSpinBox_step_valueChanged(double arg1);
    void on_doubleSpinBox_min_angle_valueChanged(double arg1);
    void on_doubleSpinBox_max_angle_valueChanged(double arg1);
    void on_ip_changed(const QString &newIp);
    void on_spinBox_port_valueChanged(int arg1);
public:
     ItemBase* getRelateItem();
private:
    Ui::WidgetLaserSetting *ui;
    ItemLaser *item_laser_;
};

#endif // WIDGETLASERSETTING_H
