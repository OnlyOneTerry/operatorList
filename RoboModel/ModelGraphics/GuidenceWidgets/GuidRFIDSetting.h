#ifndef GUIDRGIDSETTING_H
#define GUIDRGIDSETTING_H
#include <QWidget>
#include"GuidenceWidgets/GuidBaseSetting.h"

namespace Ui {
class GuidRFIDSetting;
}

class GuidRFIDSetting : public GuidBaseSetting
{
    Q_OBJECT

public:
    explicit GuidRFIDSetting(QWidget *parent = 0);
    ~GuidRFIDSetting();
    void initWidgetData();
   ConfigureData readConfigure_RFIDType(const QString& filePath);
signals:
   void sig_ignore();
   void sig_set();
private slots:
    void on_radioButton_set_clicked();

    void on_radioButton_ignore_clicked();

    void on_comboBox_Type_currentIndexChanged(int index);

private:
    Ui::GuidRFIDSetting *ui;

public:
    ModelBaseData basicData_;
};

#endif // GUIDRGIDSETTING_H
