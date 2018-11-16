#ifndef WIDGETCAMERASETTING_H
#define WIDGETCAMERASETTING_H

#include <QWidget>
#include"WidgetSetting.h"
class ItemCamera;
namespace Ui {
class WidgetCameraSetting;
}

class WidgetCameraSetting : public WidgetSetting
{
    Q_OBJECT

public:
    explicit WidgetCameraSetting(ItemCamera* item, QWidget *parent = 0);
    ~WidgetCameraSetting();
   void    update_value();
   void initWigetdata();
   ItemBase* getRelateItem();
   ConfigureData  readConfigData(const QString &filePath);
private slots:
   void on_comboBox_type_currentIndexChanged(int index);

   void on_doubleSpinBox_x_valueChanged(double arg1);

   void on_doubleSpinBox_y_valueChanged(double arg1);

   void on_doubleSpinBox_z_valueChanged(double arg1);

   void on_doubleSpinBox_pitch_valueChanged(double arg1);

   void on_doubleSpinBox_roll_valueChanged(double arg1);

   void on_doubleSpinBox_yaw_valueChanged(double arg1);

private:
    Ui::WidgetCameraSetting *ui;

    ItemCamera* cameral_;
};

#endif // WIDGETCAMERASETTING_H
