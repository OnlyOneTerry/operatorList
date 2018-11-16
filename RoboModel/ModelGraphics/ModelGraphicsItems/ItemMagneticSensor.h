#ifndef ITEMMAGNETICSENSOR_H
#define ITEMMAGNETICSENSOR_H
#include "ItemBase.h"
#include<QPointF>
class WidgetMagneticSensorSetting;

class MagSensorContentSetting;
class ItemMagneticSensor : public ItemBase
{
    Q_OBJECT
public:
    enum{Type = I_magnetic_sensor};int type() const{ if(this)return Type;}
public:
    ItemMagneticSensor(ModelGraphicsScene*,QJsonObject json=QJsonObject());
    ~ItemMagneticSensor();
    void add_setting_widget ();
    void updateWidgetValue();
protected:
    void position_changed();
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
public:
    MagneticSensorData data_magnetic_;
    static int count ;
public:
    QVariantMap get_magnetic_data() const ;
    void update_data ();
    void update_item();

protected slots:
    void slot_angel_changed(qreal);
    void slot_guidconfig_finished();//引导配置结束，将引导设置的属性更新到右侧属性框
private:
    const int k_radius_size_=6;

    MagSensorContentSetting* magSensorSetting_= NULL;

};

#endif // ITEMMAGNETICSENSOR_H
