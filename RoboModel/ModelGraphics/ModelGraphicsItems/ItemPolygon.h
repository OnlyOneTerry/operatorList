#ifndef ITEMPOLYGON_H
#define ITEMPOLYGON_H

#include "ItemBase.h"
#include<QPointF>

class ItemPolypos;
class WidgetPlygonSetting;
class ItemPolygon :public ItemBase
{
    Q_OBJECT
public:
    enum{Type = I_polygon};int type() const{ if(this)return Type;}

    enum PolyType
    {
        LOWSPEED=0, //减速
        STOPOBJ,    //停障
        SAFE        //安全
    };
public:
    ItemPolygon(ModelGraphicsScene *scene,PolyType poly_type,QJsonObject json=QJsonObject());
    ItemPolygon(ModelGraphicsScene *scene,PolyType poly_type,QList<QPointF> pos_list);
    ~ItemPolygon();

   void add_setting_widget();
public:
    PolyType poly_type_;
protected:
    QPainterPath shape () const;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void position_changed();

private:
    QList<ItemPolypos* > polygon_pos_list_;
    QPolygonF polygonf_;
    QList<QPointF> middle_pos_list_;
    QColor pos_color_;

public:
    void init_polypos();
    void add_polypos();
    void remove_polypos(QPointF pos);
    quint8 get_pos_num() const;
    void adjust_polygon(qreal head,qreal tail,qreal width);//改变量del_head,del_tail,del_width

    /*数据相关*/

public:
    PolygonData data_polygon_;
public:
    QVariantMap get_polygon_data();
    void update_polygon_data ();
    QList<QPointF>* get_Middle_pos_list();
    QList<ItemPolypos* >* get_polygon_pos_list();
private:
    QList<ItemPolypos *> polygon_pos_add_list_;
    qreal radius_=100.0;
    int poly_id_;//多边形编号

    //for undo redo
    QGraphicsItem* m_addPos_oriItem;
    QGraphicsItem* m_addPos_newItem;

};

#endif // ITEMPOLYGON_H
