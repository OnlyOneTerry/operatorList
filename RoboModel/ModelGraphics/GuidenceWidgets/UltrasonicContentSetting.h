#ifndef ULTRASONICCONTENTSETTING_H
#define ULTRASONICCONTENTSETTING_H

#include"BasicContentSetting.h"
#include<QGroupBox>

namespace Ui {
class UltrasonicContentSetting;
}

class UltrasonicContentSetting : public BasicContentSetting
{
    Q_OBJECT

public:
    explicit UltrasonicContentSetting(QWidget *parent = 0);
    ~UltrasonicContentSetting();
     void initWigetData();
    ConfigureData readFuncTypeFile(const QString filePath);
    ConfigureData readConfigure_UltrasonicType(const QString& filePath);
    void setValid(bool flag);
    bool getValid();
private:
    Ui::UltrasonicContentSetting *ui;
    bool isValid_ = false;//range是否有效
public:
    UltrasonicData ultrasonicData_;
    int ultrasonicType_ = 0;//超声接口类型
signals:
    void sig_range_valueChange();
private slots:
    void on_doubleSpinBox_range_valueChanged(double arg1);
    void on_doubleSpinBox_maxDist_valueChanged(double arg1);
    void on_doubleSpinBox_minDist_valueChanged(double arg1);
    void on_comboBox_type_currentIndexChanged(int index);

    void on_comboBox_func_currentIndexChanged(int index);
    void on_ptb_Previous_clicked();
    void on_ptb_Next_clicked();
};

#endif // ULTRASONICCONTENTSETTING_H
