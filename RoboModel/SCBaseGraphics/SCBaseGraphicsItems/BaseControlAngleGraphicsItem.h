#ifndef BASECONTROLANGLEGRAPHICSITEM_H
#define BASECONTROLANGLEGRAPHICSITEM_H

#include "BaseDrawingFixGraphicsItem.h"

class BaseControlAngleGraphicsItem : public BaseDrawingFixGraphicsItem
{
public:
    BaseControlAngleGraphicsItem(QGraphicsItem *parent_item=Q_NULLPTR);

protected:
    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

protected:
    void initPolyF();
private:
    QPolygonF polyf_;
    QPointF press_pos_;
};

#endif // BASECONTROLANGLEGRAPHICSITEM_H
