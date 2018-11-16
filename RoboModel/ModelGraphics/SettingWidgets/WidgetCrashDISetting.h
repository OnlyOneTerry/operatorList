#ifndef WIDGETCRASHDISETTING_H
#define WIDGETCRASHDISETTING_H
#include"WidgetSetting.h"
#include <QWidget>
#include"ModelCommon.h"

class ItemCrashDI;

namespace Ui {
class WidgetCrashDISetting;
}

class WidgetCrashDISetting : public WidgetSetting
{
    Q_OBJECT

public:
    explicit WidgetCrashDISetting(ItemCrashDI* item, QWidget *parent = 0);
    ~WidgetCrashDISetting();
    void update_value();
    void initWidgetData();
    ItemBase* getRelateItem();
ConfigureData  read_funcConfigFile();
private slots:

    void on_checkBox_inverse_clicked(bool checked);
    void on_comboBox_func_currentIndexChanged(int index);
    void on_spinBox_Id_editingFinished();
    void on_spinBox_Id_valueChanged(int arg1);
    void on_timer_out();

public:
    void set_inverse_byGuidConfigure(bool arg);
    void set_func_byGuidConfigure(int index);
    void set_type_byGuidConfigure(int index);
private:
  Ui::WidgetCrashDISetting *ui;

  ItemCrashDI* crashDi_;
};

#endif // WIDGETCRASHDISETTING_H
