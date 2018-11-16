#ifndef DICONTENTSETTING_H
#define DICONTENTSETTING_H

#include"BasicContentSetting.h"
#include<QGroupBox>
namespace Ui {
class DIContentSetting;
}

class DIContentSetting : public BasicContentSetting
{
    Q_OBJECT

public:
    explicit DIContentSetting(QWidget *parent = 0);
    explicit DIContentSetting(ItemType type, QWidget *parent = 0);
    ~DIContentSetting();
    void initWidgetData(ItemType type);
    void initWidgetData();
    ConfigureData  read_funcConfigFile();
    ConfigureData  read_typeConfigFile();
    ConfigureData  readConfigData(const QString &filePath,const QString& argu);
    bool getValid();
    void setValid(bool flag);

private:
    Ui::DIContentSetting *ui;
    bool isValid_;
public:
    DiData diData_;

signals:
    void sig_rang_valueChange();
    void sig_finished();
private slots:
    void on_spinBox_range_valueChanged(int arg1);
    void on_doubleSpinBox_minDist_valueChanged(double arg1);
    void on_doubleSpinBox_maxDist_valueChanged(double arg1);
    void on_checkBox_inverse_clicked(bool checked);
    void on_comboBox_func_currentIndexChanged(int index);
    void on_comboBox_type_currentIndexChanged(int index);
    void on_ptb_previous_clicked();
    void on_ptb_next_clicked();
};

#endif // DICONTENTSETTING_H
