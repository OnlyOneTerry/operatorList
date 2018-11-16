#ifndef GUIDMAGNETICSENSORSETTING_H
#define GUIDMAGNETICSENSORSETTING_H

#include <QWidget>
#include"GuidBaseSetting.h"

namespace Ui {
class GuidMagneticSensorSetting;
}
class NumSetting;
class BasicContentSetting;

class GuidMagneticSensorSetting : public GuidBaseSetting
{
    Q_OBJECT

public:
    explicit GuidMagneticSensorSetting(QWidget *parent = 0);
    ~GuidMagneticSensorSetting();

    int getMagneticSensorType();
    void  setRadiouBtnstatus(bool flag);
    void showNumWid();//显示第一个数量设置界面
private slots:

    void slot_createSetingWidbyNum();//根据num创建窗口
    void slot_enableNextStep();

    void on_radioButton_set_clicked();

    void on_radioButton_ignore_clicked();

    void slot_ignore_byRFID();//由于RFID与磁传感器配合使用，所以当不添加RFID时，也不应添加磁传感器
    void slot_set_byRFID();
private:
    Ui::GuidMagneticSensorSetting *ui;
    int magnetic_sensorType_;       // 磁传感器类型, 0 = 没有, 1 =
public:
    MagneticSensorData magneticData_;

public:
    QList<BasicContentSetting*> magnSensorContentList_;
    NumSetting* numSetting_;
    int currentWidId_;//当前窗口id
};

#endif // GUIDMAGNETICSENSORSETTING_H
