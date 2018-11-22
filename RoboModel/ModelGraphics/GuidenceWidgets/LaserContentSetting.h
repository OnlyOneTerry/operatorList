#ifndef LASERCONTENTSETTING_H
#define LASERCONTENTSETTING_H

#include"BasicContentSetting.h"
#include<QGroupBox>

namespace Ui {
class LaserContentSetting;
}

class LaserContentSetting : public BasicContentSetting
{
    Q_OBJECT

public:
    explicit LaserContentSetting(int id,QWidget *parent = 0);
    ~LaserContentSetting();
    void initWidgetData();
    ConfigureData readLaserTypeFile(const QString& filePath);
    bool isTextValid(const QString &strIP);
    void setValid(bool flag);
    bool getValid();
    void setIndex(int idx);
signals:
    void sig_ip_valueChange();
private slots:
    void on_comboBox_type_currentIndexChanged(int index);

    void on_doubleSpinBox_step_valueChanged(double arg1);

    void on_doubleSpinBox_minAngle_valueChanged(double arg1);

    void on_doubleSpinBox_maxAngle_valueChanged(double arg1);

    void on_checkBox_inverse_clicked(bool checked);

    void on_checkBox_forLocation_clicked(bool checked);
    void slot_ip_textChange(const QString & ipstr);
    void on_ptb_Previous_clicked();

    void on_ptb_Next_clicked();

private:
    Ui::LaserContentSetting *ui;
    bool isValid_ = false;//ip是否有效
public:
    LaserData laserData_;
};

#endif // LASERCONTENTSETTING_H
