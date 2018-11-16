#ifndef BASECONTROLPOSGRAPHICSITEM_H
#define BASECONTROLPOSGRAPHICSITEM_H

#include "BaseDrawingFixGraphicsItem.h"

class BaseControlPosGraphicsitem : public BaseDrawingFixGraphicsItem
{
public:
    BaseControlPosGraphicsitem(QGraphicsItem *parent_item = Q_NULLPTR);

protected:
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // BASECONTROLPOSGRAPHICSITEM_H
