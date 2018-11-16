#ifndef GUIDULTRASONICSETTING_H
#define GUIDULTRASONICSETTING_H

#include <QWidget>
#include"GuidBaseSetting.h"
namespace Ui {
class GuidUltrasonicSetting;
}
class NumSetting;
class BasicContentSetting;
class GuidUltrasonicSetting : public GuidBaseSetting
{
    Q_OBJECT

public:
    explicit GuidUltrasonicSetting(QWidget *parent = 0);
    ~GuidUltrasonicSetting();
    int getUltrasonicType();
    void setNextPreBtnDisplay(bool flag);//设置切换按钮显示或因此
    void  setRadiouBtnstatus(bool flag);
    void  showNumWid();
public slots:
    void slot_createSetingWidbyNum();//根据num创建窗口
    void slot_enableNextStep();

    void on_radioButton_set_clicked();

    void on_radioButton_ignore_clicked();

private:
    Ui::GuidUltrasonicSetting *ui;
    int ultrasonicType_;
public:
    UltrasonicData ultrasonicData_;
    QList<BasicContentSetting*> ultrasonicContentList_;
    NumSetting* numSetting_;
    int  currentWidId_;
};

#endif // GUIDULTRASONICSETTING_H
