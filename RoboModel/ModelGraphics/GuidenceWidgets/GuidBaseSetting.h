#ifndef GUIDBASESETTING_H
#define GUIDBASESETTING_H

#include <QWidget>
#include"ModelCommon.h"
#include<QJsonObject>
#include<QJsonDocument>
#include<QJsonArray>

class GuidBaseSetting : public QWidget
{
    Q_OBJECT
public:
    explicit GuidBaseSetting(QWidget *parent = nullptr);

    void setType(GuidType tp);
    GuidType getType();
    int getNUM();
    void setConfigure(bool conf);
    bool getConfigure();
    virtual void enableNextStep();
    virtual void showNumWid();
    virtual void setNextPreBtnDisplay(bool flag);//设置切换按钮显示或因此
signals:
    void sig_configure();
    void sig_enableNextStep(bool );
public slots:
    //多个同类设备进行检测是否开启下一步
    virtual void slot_enableNextStep();
private:
    GuidType m_type;
protected:
    int num_ = 0;
    bool isConfigure_ = true;
};

#endif // GUIDBASESETTING_H
