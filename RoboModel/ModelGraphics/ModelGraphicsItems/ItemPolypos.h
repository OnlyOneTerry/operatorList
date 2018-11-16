#ifndef ITEMPOLYPOS_H
#define ITEMPOLYPOS_H

#include "ItemBase.h"

class ItemPolygon;
class WidgetPosSetting;

class ItemPolypos :public ItemBase
{
    Q_OBJECT
public:
    enum{Type = I_polygon_pos};int type() const{ if(this)return Type;}
public:
    ItemPolypos(ItemPolygon *parent,QColor color=Qt::cyan);

    ~ItemPolypos();
    void add_setting_widget ();
    ItemPolygon* getparentPolygon();
    int  getIndexInPolygon();//获取当前点在多边形polygon_中的位置索引
    void setIndexInPolygon(int index);

    qreal getRadius();
    void  setRadius(qreal r);
protected:
    QPainterPath shape () const;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

    void position_changed();
private:
    qreal radius_;   //控制点半径
    QColor color_;
    ItemPolygon *polygon_;
    int indexInPolyon_;
    void item_selected(const bool &is_selected);
public:
    void set_color(const QColor& color);
signals:
    void pos_changed(qreal,qreal);
public slots:
    void slot_change_pos(qreal,qreal);
};

#endif // ITEMPOLYPOS_H
