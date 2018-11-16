#ifndef GUIDLASERSETTING_H
#define GUIDLASERSETTING_H

#include <QWidget>
#include"GuidBaseSetting.h"

namespace Ui {
class GuidLaserSetting;
}
class NumSetting;
class BasicContentSetting;

class GuidLaserSetting : public GuidBaseSetting
{
    Q_OBJECT

public:
    explicit GuidLaserSetting(QWidget *parent = 0);
    ~GuidLaserSetting();

    ConfigureData readLaserTypeFile(const QString& filePath);
    void showNumWid();//显示第一个数量设置界面
    void setRadiouBtnstatus(bool flag);
public slots:
    void slot_createSetingWidbyNum();//根据num创建窗口
    void slot_enableNextStep();

private slots:
    void on_radioButton_set_clicked();

    void on_radioButton_ignore_clicked();

private:
    Ui::GuidLaserSetting *ui;

public:
    QList<BasicContentSetting*> laserContentList_;
    NumSetting* numSetting_;
    int currentWidId_;//当前窗口id
};

#endif // GUIDLASERSETTING_H
