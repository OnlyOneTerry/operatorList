#ifndef GUIDOPTIONALDEVICE_H
#define GUIDOPTIONALDEVICE_H

#include <QWidget>
#include"GuidBaseSetting.h"
namespace Ui {
class GuidoptionalDevice;
}

class GuidoptionalDevice : public GuidBaseSetting
{
    Q_OBJECT

public:
    explicit GuidoptionalDevice(QWidget *parent = 0);
    ~GuidoptionalDevice();

private slots:
    void on_checkBox_existBrake_clicked(bool checked);

    void on_checkBox_existLED_clicked(bool checked);

    void on_checkBox_autoBrake_clicked(bool checked);

    void on_checkBox_autoZero_clicked(bool checked);

private:
    Ui::GuidoptionalDevice *ui;
public:
    ModelBaseData basicData_;
};

#endif // GUIDOPTIONALDEVICE_H
