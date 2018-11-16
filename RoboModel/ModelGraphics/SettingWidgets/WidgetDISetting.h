#ifndef WIDGETDISETTING_H
#define WIDGETDISETTING_H

#include "WidgetSetting.h"
#include"ModelCommon.h"
class ItemDI;


namespace Ui {
class WidgetDISetting;
}

class WidgetDISetting : public WidgetSetting
{
    Q_OBJECT
public:
    enum{Type = 1};int type() const{return Type;}
public:
    explicit WidgetDISetting(ItemDI* itemdi,QWidget *parent = 0);
    ~WidgetDISetting();

public:
    void update_value();
    void set_r(const qreal &angel);
    void initWidgetData();
    ConfigureData  read_funcConfigFile();
    ConfigureData  read_typeConfigFile();
    ConfigureData  readConfigData(const QString &filePath,const QString& argu);

private slots:

    void on_spinBox_id_editingFinished();
    void on_spinBox_id_valueChanged(int arg1);

    void on_doubleSpinBox_x_valueChanged(double arg1);
    void on_doubleSpinBox_y_valueChanged(double arg1);
    void on_doubleSpinBox_z_valueChanged(double arg1);
    void on_doubleSpinBox_r_valueChanged(double arg1);

    void on_comboBox_func_currentIndexChanged(int index);
    void on_comboBox_type_currentIndexChanged(int index);

    void on_checkBox_inverse_toggled(bool checked);
    void slot_max_mum_changed(quint8,quint8);

    void on_doubleSpinBox_maxDist_valueChanged(double arg1);

    void on_doubleSpinBox_minDist_valueChanged(double arg1);

    void on_timer_out();

    void on_doubleSpinBox_range_valueChanged(double arg1);

public:
    void set_func_byConfigure(int index);
    void set_type_byConfigure(int index);
    void set_maxDist_byConfigure(double arg);
    void set_minDist_byConfigure(double arg);
    void set_rang_byConfigure(double arg);
    void set_inverse_byConfigure(bool arg);

public:
     ItemBase* getRelateItem();
private:
    Ui::WidgetDISetting *ui;
    ItemDI* item_di_;
};

#endif // WIDGETDISETTING_H
