#ifndef WIDGETCIRCLEROBOTMODELSETTING_H
#define WIDGETCIRCLEROBOTMODELSETTING_H

#include <QWidget>
#include<QUndoStack>
#include<QLabel>
#include"ModelCustomWidget.h"
#include"WidgetSetting.h"
class ItemRobotModel;
namespace Ui {
class WidgetCircleRobotModelSetting;
}

class WidgetCircleRobotModelSetting : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetCircleRobotModelSetting(ItemRobotModel *modeldata,QWidget *parent = 0);
    ~WidgetCircleRobotModelSetting();
    void initUI();
    void initEditingData();

    void setUndoStack(QUndoStack* stack);
    customDoubleSpinBox* get_outLine_radiusSpinbox();
    customComboBox* getMovCombx();
    customComboBox* getChassisCombx();
    //设置隐藏的轮子正反转设置框
    void setDriverInverAndIdDisplay(int num);
    //获取反转标记及id号
    void getInverseAndIdFlag();
    void setParentWid(WidgetSetting* parent);
    WidgetSetting* getParentWid();
protected slots:
    void slot_mvTypeComboxChange(int idex);
    void slot_chasisShapeComboxChange(int idex);
    void slot_Radius_Change(double reg);
    void slot_a_axis_Change(double reg);
    void slot_b_axis_Change(double reg);
    void slot_angle_Change(double reg);
    void slot_Wheel_toCenter_distance_Change(double reg);
    void slot_wheel_radius_Change(double reg);
private slots:

    void on_comboBox_1_currentIndexChanged(int index);

    void on_comboBox_2_currentIndexChanged(int index);

    void on_comboBox_3_currentIndexChanged(int index);

    void on_comboBox_4_currentIndexChanged(int index);

    void slot_spinbox_1_currentValueChanged(int val);

    void slot_spinbox_2_currentValueChanged(int val);

    void slot_spinbox_3_currentValueChanged(int val);

    void slot_spinbox_4_currentValueChanged(int val);


private:
    ItemRobotModel* robotModel_;
    Ui::WidgetCircleRobotModelSetting *ui;
    QUndoStack* m_unStack;
public:
    DataIndex m_ori_dataIndex;//mvTypeCombox ori index chasisTypeComBox index
    DataIndex m_new_dataIndex;//myTypeCombox new index chasisTypeComBox index

    RobotModelData m_ori_modelData;//记录底盘选型变化前的数据
    RobotModelData m_new_modelData;//记录底盘选型变化后的数据

    QMap<WidgetType,QLabel*> m_LabelMap;//存储label用于undo redo 时控制显示和隐藏
    QMap<WidgetType,QDoubleSpinBox*> m_DoubleSpinBoxMap;//存储属性框用于undo redo 时控制显示和隐藏
    QMap<WidgetType,QSpinBox*> m_SpinBoxMap;//存储属性框用于undo redo 时控制显示和隐藏
public:
    QList<bool> m_Cir_inverseList;//用于记录圆形底盘电机正反转设置
    QList<int> m_Cir_IdList;//用于记录圆形底盘电机编号
    QList<customComboBox*> driverInverseList_;
    QList<customSpinBox*>  driverIDspinboxList_;

    bool inverse1 = false;
    bool inverse2 = false;
    bool inverse3 = false;
    bool inverse4 = false;

    int id1 = 1;
    int id2 = 2;
    int id3 = 3;
    int id4 = 4;

private:
    WidgetSetting* parentWid_;//父窗口
};

#endif // WIDGETCIRCLEROBOTMODELSETTING_H
