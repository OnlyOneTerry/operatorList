#include "ItemAngelCtroller.h"
#include"../ModelHelper/ModelCustomCommand.h"
#include"../ModelGraphicsScene.h"
#include"../ModelGraphicsView.h"
#include"ItemDI.h"
#include"ItemDO.h"

ItemAngelCtroller::ItemAngelCtroller(ModelGraphicsScene *scene, ItemBase *parent, qreal angel):angel_(angel)

{
    setParentItem(parent);                          //无需手动删除，随parent删除
    model_scene_=scene;
    setCursor(Qt::CrossCursor);

    set_rotation(0.);

    connect(this,SIGNAL(sig_angel_changed(qreal)),qgraphicsitem_cast<ItemBase *>(parentItem ()),SLOT(slot_angel_changed(qreal)));

}

ItemAngelCtroller::~ItemAngelCtroller()
{

}

void ItemAngelCtroller::add_setting_widget()
{

}
void ItemAngelCtroller::setOriAngel(qreal ang)
{
  m_oriAngel = ang;
}

void ItemAngelCtroller::setNewAngel(qreal ang)
{
   m_newAngel = ang;
}

qreal ItemAngelCtroller::getOriAngel()
{
   return m_oriAngel;
}

qreal ItemAngelCtroller::getNewAngel()
{
    return m_newAngel;
}

qreal ItemAngelCtroller::getRoationRadius()
{
    return k_radius_;
}

void ItemAngelCtroller::sentAngelChange(qreal ang)
{
     emit sig_angel_changed(ang);
}
void ItemAngelCtroller::position_changed()
{
    isPostion_changed = true;
}

void ItemAngelCtroller::set_rotation(const qreal &angel)
{
    setPos(k_radius_*qCos(angel*M_PI/180.),k_radius_*qSin(angel*M_PI/180.));
    setRotation(angel);
    qgraphicsitem_cast<ItemBase *>(parentItem())->set_d_r(angel);

}

void ItemAngelCtroller::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(Qt::green);

    painter->drawPath(shape());
}

QPainterPath ItemAngelCtroller::shape() const
{
    QPainterPath paint_path;

    QPolygonF polyf;
    paint_path.addPolygon(QPolygonF()<<QPointF(4,0)<<QPointF(0,4)<<QPointF(0,-4)<<QPointF(4,0));
    return paint_path;
}

QRectF ItemAngelCtroller::boundingRect() const
{
    return shape().boundingRect().normalized();
}

void ItemAngelCtroller::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    model_scene()->setCurrentPressItem(this);
    ItemBase::mousePressEvent(event);
//    if(event->button()==Qt::LeftButton)
//    {
//        setNewPos(event->pos());
//    }
//    //点击当前方位角图标添加对应item属性编辑框
//     qDebug()<<"angeItem clicked ....";
//     if(parentItem()->type()==I_di)
//     {
//         qDebug()<<"parent item type is di";
//       ItemBase* tempBase =   qgraphicsitem_cast<ItemBase*>(parentItem());
//       ItemDI* tempDI = dynamic_cast<ItemDI*>(tempBase);
//       tempDI->add_setting_widget();
//       tempDI->model_scene()->model_view()->property_widget ()->update_property_widget (tempDI->getWidgetSetting());
//       tempDI->model_scene()->model_view ()->property_widget ()->update_layer (tempDI->model_scene()->find_items (event->scenePos ()));
//     }
//     else if(parentItem()->type()==I_do)
//     {
//         qDebug()<<"parent item type is do";
//        ItemBase* tempBase =  qgraphicsitem_cast<ItemBase*>(parentItem());
//        ItemDO* tempDO = dynamic_cast<ItemDO*>(tempBase);
//        tempDO->add_setting_widget();
//        tempDO->model_scene()->model_view ()->property_widget()->update_property_widget(tempDO->getWidgetSetting());
//        tempDO->model_scene()->model_view ()->property_widget ()->update_layer (tempDO->model_scene()->find_items (event->scenePos ()));
//     }
}
void ItemAngelCtroller::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    ItemBase::mouseMoveEvent(event);

    QLineF line(0,0,pos().x(),pos().y());
    angel_=360-line.angle();
    if (angel_>180)
    {
        angel_-=360;
    }

    setRotation(angel_);
    setPos(k_radius_*qCos(angel_*M_PI/180.),k_radius_*qSin(angel_*M_PI/180.));//

    emit sig_angel_changed(angel_);

}
void ItemAngelCtroller::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    //resetMatrix();
    ItemBase::mouseReleaseEvent(event);
       if(m_undoStack==NULL||!isPostion_changed)
       {
           return;
       }
       m_newAngel = angel_;
       rotateItemCommand * rot_command = new rotateItemCommand(m_oriAngel,m_newAngel,model_scene(), this,NULL);
       m_undoStack->push(rot_command);
       m_oriAngel = m_newAngel;

}

void ItemAngelCtroller::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsItem::hoverLeaveEvent (event);

//    if(isSelected())
//    {
//     qgraphicsitem_cast<ItemBase*>(this)->getPen().setColor(Qt::magenta);
//    }
//    update ();
}

void ItemAngelCtroller::item_selected(const bool &is_selected)
{
    qgraphicsitem_cast<ItemBase *>(parentItem())->item_selected(is_selected);
}
