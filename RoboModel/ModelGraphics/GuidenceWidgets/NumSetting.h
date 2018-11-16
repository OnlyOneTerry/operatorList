#ifndef NUMSETTING_H
#define NUMSETTING_H
#include<QSpinBox>
#include<QLabel>
#include"BasicContentSetting.h"
namespace Ui {
class NumSetting;
}

class NumSetting : public BasicContentSetting
{
    Q_OBJECT

public:
    explicit NumSetting(QWidget *parent = 0);
    ~NumSetting();
    int getNum();
    void setNum(int num);
    QSpinBox* getNumBox();
    void setImagePath(QString path);
    void setIntroduction(QString& intro);
    void setImageGroupBoxTitle(QString& title);
    QLabel* getLabelIntro();
private slots:
    void on_spinBox_num_valueChanged(int arg1);

private:
    Ui::NumSetting *ui;
    int m_Num;
};

#endif // NUMSETTING_H
