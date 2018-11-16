#ifndef ITEMSCANAREA_H
#define ITEMSCANAREA_H

#include"ItemBase.h"

class ItemScanArea:public ItemBase
{
    Q_OBJECT

public:
    enum{Type = I_scanArea_model};int type() const{ if(this)return Type;}
public:
    ItemScanArea(ModelGraphicsScene *scene,ItemBase* parent = 0);
    ~ItemScanArea();

    void add_setting_widget();
    void position_changed();

    void setMaxDist(double arg);
    void setMinDist(double arg);
    void setRange(double range);
    void set_rotation(qreal angel);
    void set_rotateRadius(qreal radius);
    qreal get_rotateRadius();
    QPainterPath arcShape(const qreal& stroke_width) const;

    QGraphicsItem* getParent();
protected:
    QPainterPath shape()const;
    QRectF boundingRect()const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

 private:
    qreal m_maxDist;
    qreal m_minDist;
    qreal m_range;
    qreal m_rotateRadius;
    QGraphicsItem * m_parentItem;
};

#endif // ITEMSCANAREA_H
