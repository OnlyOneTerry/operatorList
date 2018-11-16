#include "ItemAxis.h"
#include "ModelGraphics/ModelGraphicsScene.h"
#include "ModelGraphics/ModelGraphicsView.h"
#include <QPainter>

ItemAxis::ItemAxis()
{
    setZValue (-100);
}

ItemAxis::~ItemAxis()
{

}

QPainterPath ItemAxis::shape () const
{
    QPainterPath path;
    return path;
}
QRectF ItemAxis::boundingRect () const
{
//    QPointF TP=mapFromScene(scene()->views ().first ()->mapToScene (scene()->views ().first ()->width (),scene()->views ().first ()->height ()))-mapFromScene(scene()->views ().first ()->mapToScene (0,0));
//    int width=TP.x ();
//    int height=TP.y ();
//    return QRectF(-width/2,-height/2,width,height);

    return QRectF(-100,-100,200,200);
}
void ItemAxis::paint (QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //    QPointF TP=mapFromScene(scene()->views ().first ()->mapToScene (scene()->views ().first ()->width (),scene()->views ().first ()->height ()))-mapFromScene(scene()->views ().first ()->mapToScene (0,0));
    //    int width=-TP.x ()/2;
    //    int height=-TP.y ()/2;

    //    painter->setPen (QPen(QColor(88,88,88)));

    //    painter->drawLine (QPointF(-width,0),QPointF(width,0));
    //    painter->drawLine (QPointF(0,-height),QPointF(0,height));

    //    painter->setPen (QPen(QColor(122,122,122),0.1));
    //    for(int i=0;i<=height;i+=10)
    //    {
    //        painter->drawLine (QPointF(-width,i),QPointF(width,i));
    //    }
    //    for(int i=0;i>=-height;i-=10)
    //    {
    //        painter->drawLine (QPointF(-width,i),QPointF(width,i));
    //    }

    //    for(int i=0;i<=width;i+=10)
    //    {
    //        painter->drawLine (QPointF(i,-height),QPointF(i,height));
    //    }
    //    for(int i=0;i>=-width;i-=10)
    //    {
    //        painter->drawLine (QPointF(i,-height),QPointF(i,height));
    //    }

    painter->setBrush(Qt::red);
    painter->setPen (QPen(Qt::red,0.5));
    painter->drawLine (0,0,100,0);  //x
    painter->drawPolygon(QPolygonF()<<QPointF(100,2)<<QPointF(100,-2)<<QPointF(104,0));
    painter->drawPolygon(QPolygonF()<<QPointF(-2,100)<<QPointF(2,100)<<QPointF(0,104));
    painter->setPen (QPen(Qt::red,0.5));
    painter->drawLine (0,0,0,100);  //y


    QTransform tran(QMatrix(1, 0, 0, -1, 0, 0.));
    tran.rotate(90);
    painter->setTransform(tran,true);

    painter->setFont( QFont("Times",5));
    painter->drawText(QRectF(-98,2, 30, 30),"Y");
    painter->drawText(QRectF(4, -100, 30, 30), "X");
    painter->drawText(QRectF(10,2, 30, 30),"0");

    painter->resetMatrix();
}
