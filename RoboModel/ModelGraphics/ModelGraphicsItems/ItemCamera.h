#ifndef ITEMCAMERAL_H
#define ITEMCAMERAL_H

#include"ItemBase.h"
#include"WidgetCameraSetting.h"
class ItemCamera:public ItemBase
{
    Q_OBJECT
public:
    enum{Type = I_Camera };int type() const{ if(this)return Type;}
public:
    ItemCamera(ModelGraphicsScene *scene=NULL,QJsonObject json=QJsonObject());
    ~ItemCamera();
    void add_setting_widget ();
    void updateWidgetValue();
    void update_item();
    void update_data();
protected:
    QPainterPath shape() const;
    QRectF boundingRect() const;
    void position_changed();
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

public:
    CameraData data_camera_;
};

#endif // ITEMCAMERAL_H
