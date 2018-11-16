#ifndef BASICGRAPHICSITEM_H
#define BASICGRAPHICSITEM_H
#include <QGraphicsItem>

class BasicGraphicsItem : public QGraphicsItem
{
public:

    enum
    {
        Line=1230,
        LineControlPos,
        Mark,
        Bezier,
        Area
    };


public:
    QColor sucolor(const QColor&,const quint8& alpha);
    QFont sufont(const qreal& point_size,const bool& is_bold=false) const;
    QPointF linefootpoint(const QPointF& pos,const QPointF& lp0,const QPointF& lp1 ) const;
    qreal distancePoint(QPointF p1,QPointF p2) const;
    QPointF m_2_cm(QPointF m_pos) const;
    QPointF cm_2_m(QPointF cm_pos) const;
    qreal m_2_cm(qreal m_value) const;
    qreal cm_2_m(qreal cm_value) const;
    qreal m_2_mm(qreal m_value) const;
    qreal mm_2_m(qreal mm_value) const;
    qreal retained(qreal value,quint8 digit=3) const;//保留位数

public:
    BasicGraphicsItem(QGraphicsItem *parent = Q_NULLPTR);

protected:
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    qreal view_scale_size() const;
protected:
    volatile bool is_hovered_{false};
};

#endif // BASICGRAPHICSITEM_H
