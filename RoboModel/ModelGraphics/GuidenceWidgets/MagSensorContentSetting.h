#ifndef MAGSENSORCONTENTSETTING_H
#define MAGSENSORCONTENTSETTING_H

#include"BasicContentSetting.h"
#include<QGroupBox>
namespace Ui {
class MagSensorContentSetting;
}

class MagSensorContentSetting : public BasicContentSetting
{
    Q_OBJECT

public:
    explicit MagSensorContentSetting(QWidget *parent = 0);
    ~MagSensorContentSetting();
    void initWigetData();
 ConfigureData readConfigure_MagneticSensorType(const QString& filePath);
 void setValid(bool flag);
 bool getValid();
signals:
 void sig_step_valueChange();
private:
    Ui::MagSensorContentSetting *ui;
    bool isValid_;
public:
     MagneticSensorData magneticData_;
     int magneticSensorType_ = 0;//，用于引导配置全局磁传感器类型
private slots:
     void on_spinBox_resolution_valueChanged(int arg1);
     void on_doubleSpinBox_step_valueChanged(double arg1);
     void on_ptb_previous_clicked();
     void on_ptb_Next_clicked();

     void on_comboBox_type_currentIndexChanged(int index);
};

#endif // MAGSENSORCONTENTSETTING_H
