
#ifndef ITEMROBOTMODEL_H
#define ITEMROBOTMODEL_H

#include"ItemBase.h"
class WidgetRobotModelSetting;

class ItemRobotModel :public ItemBase
{
    Q_OBJECT
public:
    enum{Type = I_robot_model};int type() const{ return Type;}

    ItemRobotModel(ModelGraphicsScene * scene,QJsonObject json=QJsonObject());
    ~ItemRobotModel();
    void update_model();

public:
    void setMoveStyle(MoveStyle newStyle);
    MoveStyle getMoveStyle();

    void setChasiseType(ChassisType newType);
    ChassisType getChasisType();

    void setChasisShapeType(ShapeType type);
    ShapeType getChasisShapeType();

    void setMOriRadius(qreal r);
    void setMOriHead(qreal h);
    void setMOriTail(qreal t);
    void setMOriWidth(qreal w);
    WidgetSetting * getWidgetSetting();//获取当前robomodel属性设置框
    void add_setting_widget ();

    //矩形模型绘画
    void paintRectangleModel(QPainter *painter);
    //圆形模型绘画
    void paintCircleModel(QPainter *painter);

protected:
    QPainterPath shape() const;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void hoverEnterEvent (QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void position_changed();

signals:
    void sig_addSettingWidget();
public slots:
    void slot_recover_polygons();//类型切换时，地盘多边形区域恢复到初始状态
private:
    const quint8 k_controller_size_=5;

    MoveStyle moveModelStyle_;//运动方式
    ChassisType chasisModelType_;//底盘类型
    ShapeType   chasisShapeType_;//底盘形状

    //======================//用于undo redo 存储旧值,跟新频率慢
    qreal m_ori_head;
    qreal m_ori_tail;
    qreal m_ori_width;
    qreal m_ori_radius;
    //======================

public:
    RobotModelData robot_model_data_;//矩形模型数据
    RobotModelData robot_circle_model_data_;//圆模型数据
    QList<bool>  inverse_list_;//模型电机正反转列表
    QList<int>   driverId_list_;//模型电机编号
};


#endif // ITEMROBOTMODEL_H
