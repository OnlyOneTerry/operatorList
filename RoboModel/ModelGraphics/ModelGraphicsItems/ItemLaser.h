#ifndef ITEMLASER_H
#define ITEMLASER_H

#include "ItemBase.h"

class WidgetLaserSetting;
class LaserContentSetting;

class ItemLaser :public ItemBase
{
    Q_OBJECT
public:
    enum{Type = I_laser};int type() const{return Type;}
public:
    ItemLaser(ModelGraphicsScene *,QJsonObject json=QJsonObject());
    ~ItemLaser();
    void add_setting_widget ();
    void updateWidgetValue();
protected:
    QPainterPath shape () const;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void position_changed();


private:
    qreal laser_distance_;
private:
    void item_selected(const bool &is_selected);

public:
    LaserData data_laser_;
    QVariantMap get_laser_data() const;
    void update_pos();
    void update_angle();
    void update_data();

protected slots:
    void slot_angel_changed(qreal);
    void slot_guidconfig_finished();//引导配置结束，将引导设置的属性更新到右侧属性框
private:
    LaserContentSetting* laserContentsetting_;
public:
    static int count ;
};

#endif // ITEMLASER_H
