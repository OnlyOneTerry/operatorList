#ifndef WIDGETROBOTMODELSETTING_H
#define WIDGETROBOTMODELSETTING_H

#include<WidgetSetting.h>
#include<QGroupBox>
#include<QGridLayout>
#include<QRadioButton>
#include<QLabel>
#include<QComboBox>
#include"ModelCommon.h"
#include"ModelCustomWidget.h"
#include"ModelCustomCommand.h"
#include"WidgetCircleRobotModelSetting.h"
#include"WidgetRectRobotModelSetting.h"
class ItemRobotModel;
class WidgetRobotModelSetting : public WidgetSetting
{
    Q_OBJECT
public:
    enum{Type = 7};int type() const{return Type;}
public:
    explicit WidgetRobotModelSetting(ItemRobotModel *rm,QWidget *parent = 0);
    ~WidgetRobotModelSetting();
    void initUI(ShapeType type);
    customDoubleSpinBox* getHeadSpinBox();
    customDoubleSpinBox* getTailSpinBox();
    customDoubleSpinBox* getWidthSpinBox();
    customDoubleSpinBox* getRadiusSpinbox();
    //获取圆形属性设置窗口
    WidgetCircleRobotModelSetting* getCirSetting();
    //获取矩形属性设置窗口
    WidgetRectRobotModelSetting* getRectSetting();
    //读取模型时设置反正标记及id号
    void setInverseAndId(ItemRobotModel *rm,ShapeType type );
protected:
    void update_value();//用于更新矩形长宽尾控件数据
public slots:
    //底盘形状
    void slot_shapeIndexChange();
public:
     ItemBase* getRelateItem();
private:
    ItemRobotModel *robot_model_;
    customComboBox* chasisShapTypeComBox;//底盘外形类型
    QMap<WidgetType,QDoubleSpinBox*> m_DoubleSpinBoxMap;
    QMap<WidgetType,QSpinBox*> m_SpinBoxMap;
    QMap<WidgetType,QLabel*> m_LabelMap;
    QGroupBox* m_engineInverGroupbox;//设置电机正反转
private:
    WidgetRectRobotModelSetting* Wid_rect_setting_;
    WidgetCircleRobotModelSetting* Wid_circle_setting_;
};

#endif // WIDGETROBOTMODELSETTING_H
