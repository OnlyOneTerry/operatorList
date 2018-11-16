#ifndef GUIDSIMPLEDEVICE_H
#define GUIDSIMPLEDEVICE_H


#include"GuidBaseSetting.h"
#include<QComboBox>
namespace Ui {
class GuidSimpleDevice;
}

class GuidSimpleDevice : public GuidBaseSetting
{
    Q_OBJECT

public:
    explicit GuidSimpleDevice(QWidget *parent = 0);
    ~GuidSimpleDevice();
    void initWidgetData();
    void initCombox(QComboBox *box);
    ConfigureData readConfigure_drvierType(const QString& filePath);
    ConfigureData readConfigure_gyroType(const QString& filePath);//读取陀螺仪类型配置文件
    ConfigureData readConfigure_BatteryType(const QString& filePath);//读取电池检测类型配置文件
    ConfigureData readConfigure_drvierBrand(const QString& filePath);
private slots:
    void on_comboBox_batteryType_currentIndexChanged(int index);

    void on_comboBox_gyroType_currentIndexChanged(int index);

    void on_comboBox_driverBrand_currentIndexChanged(int index);

private:
    Ui::GuidSimpleDevice *ui;
public:
    ModelBaseData basicData_;

};

#endif // GUIDSIMPLEDEVICE_H
