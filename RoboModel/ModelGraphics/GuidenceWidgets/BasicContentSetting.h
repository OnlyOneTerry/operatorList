#ifndef BASICCONTENTSETTING_H
#define BASICCONTENTSETTING_H

#include "CustomBaseDialog/CustomChildBaseDialog.h"
#include"ModelCommon.h"
#include<QJsonObject>
#include<QJsonDocument>
#include<QJsonArray>
#include<QGroupBox>

namespace Ui {
class BasicContentSetting;
}

class BasicContentSetting : public CustomChildBaseDialog
{
    Q_OBJECT

public:
    explicit BasicContentSetting(QWidget *parent = 0);
    ~BasicContentSetting();
    void setId(int id);
    int getId();

protected:

protected:
    int id_;
    int currentIndex_ = 0 ;
    QList<QGroupBox*>  groupBoxList_;
signals:
    void sig_finished();
private:
    Ui::BasicContentSetting *ui;
};

#endif // BASICCONTENTSETTING_H
