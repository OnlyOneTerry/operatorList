#ifndef ITEMRFIDSENSOR_H
#define ITEMRFIDSENSOR_H
#include "ItemBase.h"
#include<QPointF>

class ItemRFIDSensor : public ItemBase
{
    Q_OBJECT
public:
    enum{Type = I_RFID_sensor };int type() const{ if(this)return Type;}
public:
    ItemRFIDSensor(ModelGraphicsScene*,QJsonObject json=QJsonObject());
    ~ItemRFIDSensor();
    void add_setting_widget ();
    void updateWidgetValue();
protected:
    void position_changed();
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
     void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
public:
    RFIDSensorData data_RFID_;
public:
    QVariantMap get_RFID_data() const ;
    void update_data ();
    void update_item();
private:
    const int k_radius_size_=6;
};

#endif // ITEMMAGNETICSENSOR_H
