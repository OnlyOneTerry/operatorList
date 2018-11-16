#ifndef ITEMAXIS_H
#define ITEMAXIS_H

#include <QGraphicsItem>
#include"ModelCommon.h"
class ItemAxis : public QGraphicsItem
{
public:
    enum{Type = I_Axis};int type() const{ if(this)return Type;}
public:
    ItemAxis();
    ~ItemAxis();

private:
    QPainterPath shape() const;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
    const qreal k_referline_radius_=380;    //参考线半径
};

#endif // ITEMAXIS_H
