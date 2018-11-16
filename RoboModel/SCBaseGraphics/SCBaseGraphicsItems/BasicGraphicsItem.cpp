#include "BasicGraphicsItem.h"
#include <QGraphicsView>
#include <QPainter>
#include <QFont>
#include <QtMath>
QColor BasicGraphicsItem::sucolor(const QColor &color, const quint8 &alpha)
{
    return QColor(color.red(),color.green(),color.blue(),alpha);
}

QFont BasicGraphicsItem::sufont(const qreal& point_size,const bool& is_bold) const
{
    QFont font;
    font.setFamily("MS Shell Dlg 2");
    font.setPointSize(point_size);
    font.setBold(is_bold);
    return font;
}

QPointF BasicGraphicsItem::linefootpoint(const QPointF &pos, const QPointF &lp0, const QPointF &lp1) const
{
    if(distancePoint(lp0,lp1)<1) return lp1;
    qreal A=lp1.y()-lp0.y();
    qreal B=lp0.x()-lp1.x();
    qreal C=lp1.x()*lp0.y()-lp0.x()*lp1.y();

    qreal x=(B*B*pos.x()-A*B*pos.y()-A*C)/(A*A+B*B);
    qreal y=(-A*B*pos.x()+A*A*pos.y()-B*C)/(A*A+B*B);

    //点到直线距离
    //float d=(A*x0.x+B*x0.y+C)/sqrt(A*A+B*B);
    return QPointF(x,y);
}

qreal BasicGraphicsItem::distancePoint(QPointF p1, QPointF p2) const
{
    QPointF tp=p1-p2;
    return qSqrt(tp.x()*tp.x()+tp.y()*tp.y());
}

QPointF BasicGraphicsItem::m_2_cm(QPointF m_pos) const
{
    return QPointF(m_2_cm(m_pos.x()),m_2_cm(m_pos.y()));
}

QPointF BasicGraphicsItem::cm_2_m(QPointF cm_pos) const
{
     return QPointF(cm_2_m(cm_pos.x()),cm_2_m(cm_pos.y()));
}

qreal BasicGraphicsItem::m_2_cm(qreal m_value) const
{
 return qRound(m_value*1000)/10.0;
}

qreal BasicGraphicsItem::cm_2_m(qreal cm_value) const
{
    return (qRound(10*cm_value))/1000.0;
}

qreal BasicGraphicsItem::m_2_mm(qreal m_value) const
{
return qRound (100*m_value)*10;
}

qreal BasicGraphicsItem::mm_2_m(qreal mm_value) const
{
return qRound(mm_value)/1000.0;
}

qreal BasicGraphicsItem::retained(qreal value, quint8 digit) const
{
    return QString::number(value,'f',digit).toDouble();
}


BasicGraphicsItem::BasicGraphicsItem(QGraphicsItem *parent):
    QGraphicsItem(parent)
{
    setAcceptHoverEvents(true);
}

void BasicGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    if(is_hovered_==false) is_hovered_=true;
    QGraphicsItem::hoverEnterEvent(event);
}

void BasicGraphicsItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    if(is_hovered_==true) is_hovered_=false;
    QGraphicsItem::hoverLeaveEvent(event);
}

qreal BasicGraphicsItem::view_scale_size() const
{
    QMatrix mtr=scene()->views().first()->matrix();
    return qMax(fabs(mtr.m11()),fabs(mtr.m12()));
}
