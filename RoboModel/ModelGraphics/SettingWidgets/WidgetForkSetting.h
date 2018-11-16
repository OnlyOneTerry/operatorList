#ifndef WIDGETFORKSETTING_H
#define WIDGETFORKSETTING_H

#include <QWidget>
#include"ModelCommon.h"
#include"UndoStack.h"
#include"WidgetSetting.h"
namespace Ui {
class WidgetForkSetting;
}
class ItemRobotModel ;
class WidgetForkSetting : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetForkSetting(QWidget *parent = 0);
    ~WidgetForkSetting();
    void setParentWid(PropertyBasicWidget* wid);
    void initUI();
    void initWidgetData();
    void setForkData(ForkData data);
    ForkData getForkdata();
    void setIsConfigureFork(bool configure);
    bool getIsConfigureFork();
private slots:
    void on_dbSpBox_blkLaserDist_valueChanged(double arg1);

    void on_spinBox_encoderSteps_valueChanged(int arg1);

    void on_dbSpBox_lengthFactor_valueChanged(double arg1);

    void on_dbSpinBox_lengthPerTurn_valueChanged(double arg1);

    void on_dbSpinBox_maxHeight_valueChanged(double arg1);

    void on_dbSpinBox_minHeight_valueChanged(double arg1);

    void on_dbSpinBox_downBuffer_valueChanged(double arg1);

    void on_dbSpinBox_upBuffer_valueChanged(double arg1);

    void on_comboBox_pump_currentIndexChanged(int index);

    void on_comboBox_forkType_currentIndexChanged(int index);

    void on_comboBox_pressureSensor_currentIndexChanged(int index);
private:
    Ui::WidgetForkSetting *ui;
    ForkData forkData_;
    QUndoStack* m_undostack;
    PropertyBasicWidget* parentWid_=NULL;//父窗口
    bool m_isConfigureFork = false;
};

#endif // WIDGETFORKSETTING_H
