#ifndef WIDGETDOSETTING_H
#define WIDGETDOSETTING_H

#include "WidgetSetting.h"
#include"ModelCommon.h"

class ItemDO;

namespace Ui {
class WidgetDOSetting;
}

class WidgetDOSetting : public WidgetSetting
{
    Q_OBJECT
public:
    enum{Type = 10};int type() const{return Type;}
public:
    explicit WidgetDOSetting(ItemDO* item_do,QWidget *parent = 0);
    ~WidgetDOSetting();

    void initWidgetData();
    ConfigureData  read_funcConfigFile();
    ConfigureData  read_typeConfigFile();
    ConfigureData  readConfigData(const QString &filePath,const QString& argu);

public:
    void update_value();
    void set_r(const qreal &angel);

private slots:
    void on_spinBox_id_valueChanged(int arg1);
    void on_spinBox_id_editingFinished();
    void on_doubleSpinBox_x_valueChanged(double arg1);
    void on_doubleSpinBox_y_valueChanged(double arg1);
    void on_doubleSpinBox_z_valueChanged(double arg1);
    void on_doubleSpinBox_r_valueChanged(double arg1);
    void on_comboBox_func_currentIndexChanged(int index);
    void on_comboBox_type_currentIndexChanged(int index);
    void on_checkBox_inverse_toggled(bool checked);
    void slot_max_mum_changed(quint8,quint8);
    void on_timer_out();

public:
    void set_func_byConfigure(int index);
    void set_type_byConfigure(int index);
    void set_inverse_byConfigure(bool arg);
public:
     ItemBase* getRelateItem();
private:
    Ui::WidgetDOSetting *ui;
    ItemDO *item_do_;
};

#endif // WIDGETDOSETTING_H
