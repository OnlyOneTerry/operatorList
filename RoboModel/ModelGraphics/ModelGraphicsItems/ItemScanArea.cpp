#include "ItemScanArea.h"
#include "ModelGraphicsScene.h"
ItemScanArea::ItemScanArea(ModelGraphicsScene *scene, ItemBase* parent ):
    ItemBase(scene),
    m_parentItem(parent),
    m_maxDist(0.500),
    m_minDist(0.10),
    m_range(60)
{
    setParentItem(parent);
}

ItemScanArea::~ItemScanArea()
{

}

void ItemScanArea::add_setting_widget()
{

}

void ItemScanArea::position_changed()
{

}

void ItemScanArea::setMaxDist(double arg)
{
    m_maxDist = arg;
}

void ItemScanArea::setMinDist(double arg)
{
    m_minDist = arg;
}

void ItemScanArea::setRange(double range)
{
    m_range = range;
}

void ItemScanArea::set_rotation(qreal angel)
{
    //调整scanArea相对于父亲位置时坐标原点发生变化，此时要更新scanArea位置
    setPos(m_rotateRadius*qCos(angel*M_PI/180.),m_rotateRadius*qSin(angel*M_PI/180.));
    set_d_r(angel);
}

void ItemScanArea::set_rotateRadius(qreal radius)
{
    m_rotateRadius = radius;
}

qreal ItemScanArea::get_rotateRadius()
{
    return m_rotateRadius;
}

QPainterPath ItemScanArea::arcShape(const qreal &stroke_width) const
{
    // 扇形起始角度
    int startAngle =  (180 - m_range)/2.0;
    // 扇形覆盖范围（120度的）
    int spanAngle = m_range;
    QPainterPath arcPath;
    QRectF rect(-m_maxDist*100,-m_maxDist*100, 2*m_maxDist*100, 2*m_maxDist*100);
    arcPath.arcTo(rect,startAngle,spanAngle);
    arcPath.lineTo(0,0);
    QPainterPathStroker ps;
    ps.setWidth(stroke_width);
    return ps.createStroke(arcPath);
}

QGraphicsItem *ItemScanArea::getParent()
{
    return m_parentItem;
}

QPainterPath ItemScanArea::shape() const
{
//     return arcShape(1.);
//     // 扇形起始角度
     int startAngle =  (180 - m_range)/2.0;
     // 扇形覆盖范围（120度的）
     int spanAngle = m_range;
     QRectF rect(-m_maxDist*100,-m_maxDist*100, 2*m_maxDist*100, 2*m_maxDist*100);
     QPainterPath arcPath;
     arcPath.arcTo(rect,startAngle,spanAngle);
     arcPath.lineTo(0,0);
     return arcPath;
}

QRectF ItemScanArea::boundingRect() const
{
//    return arcShape(1.).boundingRect();
    return shape().boundingRect();
}

void ItemScanArea::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->rotate(d_r_);//绘制角度
    QPen pen(Qt::yellow);

    pen.setWidth(0);
    QBrush brush(Qt::gray);
    QColor clr(Qt::gray);
    clr.setAlpha(85);
    brush.setColor(clr);
    painter->setRenderHint(QPainter::Antialiasing, true);//抗锯齿
    painter->setPen(pen);
    painter->setBrush(brush);
    painter->drawPath(shape());
}

void ItemScanArea::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

}

void ItemScanArea::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
}



