#ifndef BASEGRAPHICSITEM_H
#define BASEGRAPHICSITEM_H

#include "BasicGraphicsItem.h"

class BaseDrawingFixGraphicsItem : public BasicGraphicsItem
{
public:
    BaseDrawingFixGraphicsItem(QGraphicsItem *parent_item = Q_NULLPTR);
    ~BaseDrawingFixGraphicsItem();

public:
    void set_radius(const qreal& radius);
protected:

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
    qreal radius_{4.};
    QColor color_pen_{Qt::black},color_brush_{Qt::red};
};

#endif // BASEGRAPHICSITEM_H
