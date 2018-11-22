#ifndef GUIDBASICSETTING_H
#define GUIDBASICSETTING_H

#include"GuidBaseSetting.h"
#include<QComboBox>
namespace Ui {
class GuidBasicSetting;
}

class GuidBasicSetting : public GuidBaseSetting
{
    Q_OBJECT

public:
    explicit GuidBasicSetting(QWidget *parent = 0);
    ~GuidBasicSetting();  

    void enableNextStep();
private:
    Ui::GuidBasicSetting *ui;

public:
    ModelBaseData basicData_;

private slots:
    void on_spinBox_encoderLine_valueChanged(int arg1);
    void on_spinBox_maxSpeed_valueChanged(int arg1);
    void on_doubleSpinBox_reduceRatio_valueChanged(double arg1);
};

#endif // GUIDBASICSETTING_H
