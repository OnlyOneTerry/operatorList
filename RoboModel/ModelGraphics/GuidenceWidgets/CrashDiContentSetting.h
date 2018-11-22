#ifndef CRASHDICONTENTSETTING_H
#define CRASHDICONTENTSETTING_H

#include"BasicContentSetting.h"
#include<QGroupBox>

namespace Ui {
class CrashDiContentSetting;
}

class CrashDiContentSetting : public BasicContentSetting
{
    Q_OBJECT

public:
    explicit CrashDiContentSetting(int id,QWidget *parent = 0);
    ~CrashDiContentSetting();
    void initWidgetData();
    ConfigureData  read_funcConfigFile();
    ConfigureData  readConfigData(const QString &filePath,const QString& argu);
private slots:
    void on_comboBox_func_currentIndexChanged(int index);

    void on_comboBox_type_currentIndexChanged(int index);

    void on_checkBox_inverse_2_clicked(bool checked);

    void on_ptb_previous_clicked();

    void on_ptb_next_clicked();

private:
    Ui::CrashDiContentSetting *ui;
public:
    crashDiData  data_crashDi_;
};

#endif // CRASHDICONTENTSETTING_H
