#ifndef WIDGETRECTROBOTMODEL_H
#define WIDGETRECTROBOTMODEL_H

#include <QWidget>
#include"ModelCommon.h"
#include"ModelCustomWidget.h"
#include"ModelCustomCommand.h"
#include"WidgetForkSetting.h"
namespace Ui {
class WidgetRectRobotModelSetting;
}
class ItemRobotModel;
class WidgetRectRobotModelSetting : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetRectRobotModelSetting(ItemRobotModel *model,QWidget *parent = 0);
    ~WidgetRectRobotModelSetting();
    void initWidgetData();
    void initUI();
    ConfigureData readAbsEncoderTypeFile(const QString& filePath);
    //设置各项参数
    void set_Diff_two_wheel_argument(double adistance);
    void set_Diff_four_wheel_argument(double adistance,double bdistance);
    void set_Mecanum_four_wheel_argument(double adistance,double bdistance);
    void set_AllDirection_three_wheel_argument(double adistance,double angle);
    void set_SingleWheelDriver_argument(double adisance,double bdistance,double steerAngleMax,double steerAngleMin,
                                        double absEncoderRangeMax,double absEncoderRangeMin,int encoderType = 0);
    //设置电机编号及正反转设置框
    void setDriverInverAndIdDisplay(int num);
    //获取反转标记及id号
    void getInverseAndIdFlag();
    WidgetSetting* getParentWid();
    void setParentWid(WidgetSetting* parent);
    customDoubleSpinBox* get_doubleSpinbox_head();
    customDoubleSpinBox* get_doubleSpinbox_width();
    customDoubleSpinBox* get_doubleSpinbox_tail();
private slots:
    void on_comboBox_moveType_currentIndexChanged(int index);

    void on_comboBox_chassisType_currentIndexChanged(int index);

    void on_doubleSpinBox_width_valueChanged(double arg1);

    void on_doubleSpinBox_head_valueChanged(double arg1);

    void on_doubleSpinBox_tail_valueChanged(double arg1);

    void on_doubleSpinBox_wheelRadius_valueChanged(double arg1);

    void on_doubleSpinBox_corePoint_distance_valueChanged(double arg1);

    void on_doubleSpinBox_Baxis_valueChanged(double arg1);

    void on_spinBox_steer_angle_min_valueChanged(int arg1);

    void on_spinBox_steer_angle_max_valueChanged(int arg1);

    void on_comboBox_absEncoderType_currentIndexChanged(int index);

    void on_spinBox_ID1_valueChanged(int arg1);

    void on_spinBox_ID2_valueChanged(int arg1);

    void on_spinBox_ID3_valueChanged(int arg1);

    void on_spinBox_ID4_valueChanged(int arg1);

    void on_comboBox_inverse1_currentIndexChanged(int index);

    void on_comboBox_inverse2_currentIndexChanged(int index);

    void on_comboBox_inverse3_currentIndexChanged(int index);

    void on_comboBox_inverse4_currentIndexChanged(int index);

    void on_doubleSpinBox_Aaxis_valueChanged(double arg1);

    void on_spinBox_absEncoderRange_min_valueChanged(int value);

    void on_doubleSpinBox_firstWheel_delta_angle_toPostiveDirction_valueChanged(double arg1);

    void on_spinBox_absEncoderRange_max_valueChanged(int arg1);

    void on_comboBox_currentIndexChanged(int index);

private:
    Ui::WidgetRectRobotModelSetting *ui;
    ItemRobotModel* robot_model_;
public:

    RobotModelData m_ori_modelData;
    RobotModelData m_new_modelData;
    DataIndex m_ori_dataIndex;
    DataIndex m_new_dataIndex;

    QMap<WidgetType,QDoubleSpinBox*> m_DoubleSpinBoxMap;
    QMap<WidgetType,QSpinBox*> m_SpinBoxMap;
    QMap<WidgetType,QLabel*> m_LabelMap;
    QUndoStack* m_unStack=NULL;

public:
    QList<customComboBox*> driverInverseList_;//存储电机正反转(矩形)
    QList<customSpinBox*>  driverIDspinboxList_;//存储电机编号（矩形）
    QList<bool> m_Rect_inverseList;//用于记录矩形底盘电机正反转设置
    QList<int> m_Rect_IdList;//用于记录矩形底盘电机编号

public:
    bool inverse1 = false;
    bool inverse2 = false;//正转，inverse = false；反转，inverse = true
    bool inverse3 = false;
    bool inverse4 = false;
    int  id1 = 1;
    int  id2 = 2;
    int  id3 = 3;
    int  id4 = 4;
    WidgetSetting* parentWid_=NULL;//父窗口

};

#endif // WIDGETRECTROBOTMODEL_H
