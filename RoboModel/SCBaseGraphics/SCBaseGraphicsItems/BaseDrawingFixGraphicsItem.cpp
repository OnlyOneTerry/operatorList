#include "BaseDrawingFixGraphicsItem.h"
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QStyle>
#include <QDebug>

BaseDrawingFixGraphicsItem::BaseDrawingFixGraphicsItem(QGraphicsItem *parent_item):
    BasicGraphicsItem(parent_item)
{
    setFlags(ItemIgnoresTransformations|ItemIsSelectable | ItemIsMovable);
    //setCursor(Qt::DragMoveCursor);
    //setAcceptHoverEvents(true);
}

BaseDrawingFixGraphicsItem::~BaseDrawingFixGraphicsItem()
{

}

void BaseDrawingFixGraphicsItem::set_radius(const qreal &radius)
{
    radius_=radius;
}

QRectF BaseDrawingFixGraphicsItem::boundingRect() const
{
    return QRectF(-20,-20,40,40);
}

QPainterPath BaseDrawingFixGraphicsItem::shape() const
{
    QPainterPath path;
    // path.addEllipse(QRectF(-16,-16,32,32));
    return path;
}

void BaseDrawingFixGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //    if (option->state & QStyle::State_Selected)
    //    {

    //    }
    //    // option->state = QStyle::State_None;
    //    painter->setPen(QColor(0,0,255,30));
    //    painter->setBrush(QColor(255,0,255,30));
    //    painter->drawEllipse(QRectF(-15,-15,30,30));
    //    painter->setPen(QColor(0,0,0,30));

    //    painter->setFont(QFont("MS Shell Dlg 2",7,QFont::Medium));
    //    painter->drawText(QRectF(-20,-20,40,40),"LM69",QTextOption(Qt::AlignCenter));

}
