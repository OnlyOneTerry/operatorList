#ifndef GUIDDOSETTING_H
#define GUIDDOSETTING_H

#include <QWidget>
#include"GuidBaseSetting.h"

namespace Ui {
class GuidDoSetting;
}
class NumSetting;
class BasicContentSetting;
class Spinbox;
class GuidDoSetting : public GuidBaseSetting
{
    Q_OBJECT

public:
    explicit GuidDoSetting(QWidget *parent = 0);
    ~GuidDoSetting();

    void  setRadiouBtnstatus(bool flag);
    void showNumWid();//显示第一个数量设置界面

public slots:
    void slot_createSetingWidbyNum();//根据num创建窗口
    void slot_enableNextStep();

private slots:

    void on_radioButton_set_clicked();

    void on_radioButton_ignore_clicked();

private:
    Ui::GuidDoSetting *ui;

public:
    DOData doData_;

public:
    QList<BasicContentSetting*> doContentList_;
    NumSetting* numSetting_;
    int currentWidId_;//当前窗口id
};

#endif // GUIDDOSETTING_H
