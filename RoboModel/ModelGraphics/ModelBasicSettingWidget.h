#ifndef MODELBASICSETTINGWIDGET_H
#define MODELBASICSETTINGWIDGET_H

#include <QWidget>
#include<QComboBox>
#include<QUndoStack>
#include"ModelHelper/ModelCustomCommand.h"
#include "ModelCommon.h"
#include"PropertyBasicWidget.h"
#include"WidgetForkSetting.h"
class QGestureEvent;
class QPanGesture;
class QPinchGesture;
class QSwipeGesture;
class ModelGraphicsView;
class WidgetDOState;
namespace Ui {
class ModelBasicSettingWidget;
}

class ModelBasicSettingWidget : public PropertyBasicWidget
{
    Q_OBJECT

public:
    explicit ModelBasicSettingWidget(ModelGraphicsView *parent = 0);
    ~ModelBasicSettingWidget();

    void initBasicSettingWidgetInfo();
    void initForkWidget();
    void initDrvierTypeCombox();
    void initDrvierBrandCombox();
    void initGyroTypeCombox();
    void initBatteryTypeCombox();
    void initRFIDTypeCombox();
    void initMagneticSensorTypeCombox();
    void initUltrasonicTypeCombox();
    void initModelsCombox();
    void initCombox(customComboBox *box);//适用于所有可配置的combox

    ConfigureData readMechineConfigureData(const QString& mechineConfigName,const QString& filePath);//仅适用于与底盘相关配置文件结构一致的设备（不通用）
    ConfigureData readConfigure_drvierBrand(const QString& filePath);
    ConfigureData readConfigure_AMBdrvierBrand(const QString& filePath);
    ConfigureData readConfigure_drvierType(const QString& filePath);
    ConfigureData readConfigure_gyroType(const QString& filePath);//读取陀螺仪类型配置文件
    ConfigureData readConfigure_BatteryType(const QString& filePath);//读取电池检测类型配置文件
    ConfigureData readConfigure_RFIDType(const QString& filePath);
    ConfigureData readConfigure_MagneticSensorType(const QString& filePath);
    ConfigureData readConfigure_CameralType(const QString& filePath);
    ConfigureData readConfigure_UltrasonicType(const QString& filePath);

    //用于redo unod 操作
signals:
    void sig_moveStyleChanged(MoveStyle style);//运动方式切换
    void sig_max_num_changed(quint8,quint8);
    void sig_driverType_changed();
public slots:
    void slot_driverType_change();
    void slot_set_SpinboxtoolTips();
    void slot_updateScrollBar();
private slots:
    void on_lineEdit_model_name_textChanged(const QString &arg1);
    void on_DoubleSpinBox_reductionRatio_valueChanged(double arg1);

    void on_checkBox_autoGyroCal_toggled(bool checked);
    void on_checkBox_brake_toggled(bool checked);
    void on_checkBox_autoBrake_toggled(bool checked);
    void on_checkBox_LED_toggled(bool checked);

    void on_SpinBox_encoderLine_valueChanged(int arg1);
    void on_SpinBox_maxMotorRpm_valueChanged(int arg1);
    void on_spinBox_DI_valueChanged(int arg1);
    void on_spinBox_DO_valueChanged(int arg1);
    void on_spinBox_ultrasonic_valueChanged(int arg1);

    void on_combox_RFID_type_currentIndexChanged(int index);
    void on_comboBox_ultrasonic_type_currentIndexChanged(int index);
    void on_ComboBox_driver_type_currentIndexChanged(int index);
    void on_comboBox_batteryInfo_type_currentIndexChanged(int index);
    void on_comboBox_magneticSensor_type_currentIndexChanged(int index);
    void on_comboBox_gyro_currentIndexChanged(int index);
    void on_comboBox_driverBrand_currentIndexChanged(int index);
    void on_checkBox_forkConfigure_clicked(bool checked);

public:
    ModelBaseData model_base_data_;//编辑框对应结构体
    void update_panel_infor();
    void update_do_size(quint8 m_size,quint8 c_size);
    QList<quint8> get_do_info_list();
    QList<WidgetDOState *>* get_do_state_list();
    void set_do_info_list(QList<quint8>dil);
    //设置最大值编辑框
    void  set_di_minNum(int num);
    void  set_do_minNum(int num);
    void  set_ultrasonic_minNum(int num);

    customLineEdit* getModelName();//获取modelName控件
    customDoubleSpinBox* getReduceRadio();//获取减速比控件
    customSpinBox* getEncoderLine();//编码器线数控件
    customSpinBox *getMaxRpm();//获取最大速度控件
    customComboBox *getDrive();//驱动器类型控件
    customComboBox* getDriveBrand();//驱动器品牌
    customComboBox *getBattery();//电池类型控件
    customComboBox *getGyro();//陀螺仪类型控件
    customComboBox *getRFID();//RFID类型控件
    customCheckBox *getBrake();//是否有抱闸控件
    customCheckBox *getAutoBrake();//是否自动抱闸控件
    customCheckBox *getLed();//是否有Led控件
    customCheckBox *getAutoGyroCal();//自动标零控件
    customComboBox* getUltrasonicType();//超声接口类型控件
    customComboBox* getMagneticSensorType();
    customSpinBox* getDINumSpinbox();//DI数量设置控件
    customSpinBox* getDONumSpinbox();//DO数量设置控件
    customSpinBox* getUltrasonicNumSpinbox();//超声数量设置控件
    QScrollBar* getVScrollAreaBar();
    QWidget* getScrollAreaContent();
    WidgetForkSetting* getForkWidget();
protected:
    bool gestureEvent(QGestureEvent *event);
    void panTriggered(QPanGesture *gesture);
    void pinchTriggered(QPinchGesture *gesture);
    void SwipeTriggered(QSwipeGesture *gesture);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    bool event(QEvent *event);
private:
     QPoint m_lastMousePos;
private:
    Ui::ModelBasicSettingWidget *ui;
    ModelGraphicsView *model_view_;
    QList<WidgetDOState *> widget_do_list_;
    QUndoStack* m_undoStack;
    WidgetForkSetting* forkWid_=NULL;
    QTimer timer;
    bool m_bIsOnePoint  = false;
};

#endif // MODELBASICSETTINGWIDGET_H
