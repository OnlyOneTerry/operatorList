#ifndef GUIDDISETTING_H
#define GUIDDISETTING_H

#include <QWidget>
#include"GuidBaseSetting.h"


namespace Ui {
class GuidDISetting;
}
class NumSetting;
class BasicContentSetting;
class GuidDISetting : public GuidBaseSetting
{
    Q_OBJECT

public:
    explicit GuidDISetting(QWidget *parent = 0);
    ~GuidDISetting();
    void setNextPreBtnDisplay(bool flag);//设置切换按钮显示或因此
    void  setRadiouBtnstatus(bool flag);
    void showNumWid();//显示第一个数量设置界面

public slots:
   void slot_createSetingWidbyNum();//根据num创建窗口
   void slot_enableNextStep();
   void slot_setDImaxNum(int num);
private slots:
    void on_radioButton_set_clicked();
    void on_radioButton_ignore_clicked();

private:
    Ui::GuidDISetting *ui;
public:
    DiData diData_;

public:
    QList<BasicContentSetting*> diContentList_;
    NumSetting* numSetting_;//普通DI
    NumSetting* numSetting_crashDI_;//碰撞开关

    int currentWidId_;//当前窗口id
};

#endif // GUIDDISETTING_H
