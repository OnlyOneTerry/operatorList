#ifndef DOCONTENTSETTING_H
#define DOCONTENTSETTING_H
#include<QGroupBox>
#include"BasicContentSetting.h"
namespace Ui {
class DOContentSetting;
}

class DOContentSetting : public BasicContentSetting
{
    Q_OBJECT

public:
    explicit DOContentSetting(QWidget *parent = 0);
    ~DOContentSetting();
    void initWidgetData();
    ConfigureData  read_funcConfigFile();
    ConfigureData  read_typeConfigFile();
    ConfigureData  readConfigData(const QString &filePath,const QString& argu);
    void setValid(bool flag);
    bool getValid();
private:
    Ui::DOContentSetting *ui;
    bool isValid_;
public:
    DOData doData_;

private slots:
    void on_ptb_Previous_clicked();
    void on_ptb_Next_clicked();
    void on_comboBox_func_currentIndexChanged(int index);

    void on_comboBox_type_currentIndexChanged(int index);

    void on_checkBox_inverse_clicked(bool checked);
};

#endif // DOCONTENTSETTING_H
