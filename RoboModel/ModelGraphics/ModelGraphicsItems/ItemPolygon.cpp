#include "ItemPolygon.h"
#include "ItemPolypos.h"
#include "WidgetPlygonSetting.h"
#include "ModelGraphicsView.h"
#include "ModelHelper.h"

ItemPolygon::ItemPolygon(ModelGraphicsScene *scene,PolyType poly_type,QJsonObject json):
    ItemBase(scene),
    poly_type_(poly_type)
  ,poly_id_(1000)
  ,m_addPos_oriItem(NULL)
{
    setAcceptHoverEvents (true);
    //  setFlag(QGraphicsPathItem::ItemSelectedHasChanged);

    if(!json.isEmpty ())
    {
        if(poly_type==0)
        {
            data_polygon_.max_speed_=json["maxSpeed"].toDouble ();
        }

        if(json.contains("workingMode"))
        {
            data_polygon_.working_mode_=json["workingMode"].toInt ();
        }

        QJsonArray json_array_polypos=json["vertex"].toArray ();
        foreach (QJsonValue j_value, json_array_polypos)
        {
            QJsonObject j_object=j_value.toObject ();
            data_polygon_.vertex_list_.append (QPointF(j_object["x"].toDouble (),j_object["y"].toDouble ()));
        }
    }

    while(model_scene()->get_id_list(5).contains(QString("%1").arg(data_polygon_.id_)))
    {
        data_polygon_.id_++;
    }

    setID(data_polygon_.id_);
    setToolTip(QString("ItemPolygon_%1").arg(data_polygon_.id_));
    init_polypos ();
    setCursor(Qt::SizeAllCursor);
}

ItemPolygon::ItemPolygon(ModelGraphicsScene *scene, ItemPolygon::PolyType poly_type, QList<QPointF> pos_list):
    ItemBase(scene),
    poly_type_(poly_type),poly_id_(1000),
    m_addPos_oriItem(NULL)
{
    data_polygon_.vertex_list_=pos_list;
    init_polypos ();
    while(model_scene()->get_id_list(5).contains(QString("%1").arg(data_polygon_.id_)))
    {
        data_polygon_.id_++;
        qDebug()<<"2222data_polygon_.id_"<<data_polygon_.id_;
    }
    setID(data_polygon_.id_);
    setToolTip(QString("ItemPolygon_%1").arg(data_polygon_.id_));
}

ItemPolygon::~ItemPolygon()
{
    foreach (auto var, polygon_pos_list_)
    {
        delete var;
    }
    polygon_pos_list_.clear ();
}


QPainterPath ItemPolygon::shape() const
{
    QPainterPath path;

    QPolygonF polyf;

    foreach (auto var, polygon_pos_list_)
    {
        polyf.append (var->pos ());
    }

    path.addPolygon (polyf);

    return path;
}

QRectF ItemPolygon::boundingRect() const
{
    return shape().boundingRect ().normalized().adjusted (-10,-10,20,20);
}

void ItemPolygon::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    polygonf_.clear ();
    middle_pos_list_.clear ();
    foreach (auto var, polygon_pos_list_)
    {
        polygonf_.append (var->pos ());
        var->setIndexInPolygon(polygon_pos_list_.indexOf(var));//获取polypos在多边形中的位置索引
    }

    painter->setPen (Qt::NoPen);
    painter->setBrush (brush_);
    painter->drawPolygon (polygonf_);
    painter->setPen (pen_);
    int ns=polygonf_.size ();

    if(ns<=1)
    {
        //当只有一个点时直接返回
        return;
    }

    for(int ti=0;ti<ns-1;ti++)
    {
        QPointF cpos((polygonf_.at (ti)+polygonf_.at (ti+1))/2.0);
        middle_pos_list_.append (cpos);
        painter->drawEllipse (cpos,1,1);
    }
    QPointF cpos((polygonf_.at (ns-1)+polygonf_.at (0))/2.0);//最后一个点与第一个点连线的中点
    middle_pos_list_.append (cpos);
    painter->drawEllipse (cpos,1,1);

}

void ItemPolygon::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button ()!=Qt::LeftButton) return;
    model_scene()->setCurrentPressItem(this);
    QPointF mpos=mapFromScene(model_scene()->get_scene_mousepos());

    if(m_undoStack==NULL)
    {
        return;
    }

    int tn=middle_pos_list_.size ();
    for(int ti=0;ti<tn;ti++)
    {
        if(qPow(mpos.x()-middle_pos_list_.at(ti).x(),2)+qPow(mpos.y()-middle_pos_list_.at(ti).y(),2)<36)
        {
            ItemPolypos* item_polypos=new ItemPolypos(this,pos_color_);
            m_addPos_newItem = item_polypos;
            item_polypos->setPos (middle_pos_list_.at (ti));
            //  polygon_pos_list_.insert (ti+1,item_polypos);
            addPosCommand* pos_command = new addPosCommand(this, ti+1,m_addPos_oriItem ,m_addPos_newItem,
                                                           model_scene(),NULL);
            m_undoStack->push(pos_command);
            m_addPos_oriItem = m_addPos_newItem;
            break;
        }
    }

    ItemBase::mousePressEvent (event);

    if(event->button()==Qt::LeftButton)
    {
        setNewPos(event->pos());
    }
}

void ItemPolygon::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    ItemBase::mouseMoveEvent (event);
}

void ItemPolygon::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{

    ItemBase::hoverMoveEvent(event);
    QPointF mpos=mapFromScene(model_scene()->get_scene_mousepos());
    bool is_cursor_need_change=false;
    int tn=middle_pos_list_.size ();
    for(int ti=0;ti<tn;ti++)
    {
        qreal distance = qPow(mpos.x()-middle_pos_list_.at(ti).x(),2)+qPow(mpos.y()-middle_pos_list_.at(ti).y(),2);
        if(qPow(mpos.x()-middle_pos_list_.at(ti).x(),2)+qPow(mpos.y()-middle_pos_list_.at(ti).y(),2)<36)
        {
            is_cursor_need_change=true;
            break;
        }

    }
    if(is_cursor_need_change)
    {
        if(cursor().shape()!=Qt::CrossCursor)
        {
            setCursor(Qt::CrossCursor);
        }
    }
    else if(cursor().shape()!=Qt::SizeAllCursor)
    {
        setCursor(Qt::SizeAllCursor);
    }
}

void ItemPolygon::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    ItemBase::mouseReleaseEvent(event);

    if(m_undoStack==NULL )
    {
        return;
    }

    if(isPostion_changed)
    {
        m_new_pos = this->pos();
        moveItemCommand* mv_command = new moveItemCommand(m_ori_pos,m_new_pos,model_scene(),this,NULL);
        m_undoStack->push(mv_command);
        m_ori_pos = this->pos();

    }

}

void ItemPolygon::add_setting_widget()
{
    if(widget_setting_==NULL)
    {
        widget_setting_=new WidgetPlygonSetting(int(poly_type_),this);
    }
}

void ItemPolygon::position_changed()
{
    isPostion_changed = true;
    setNewPos(this->pos());
}

void ItemPolygon::init_polypos()
{
    switch (poly_type_) {
    case 0:
        pos_color_=Qt::cyan;
        radius_=85;
        break;
    case 1:
        pos_color_=Qt::red;
        radius_=57;
        break;
    case 2:
        pos_color_=Qt::green;
        radius_=21;
        break;
    default:
        break;
    }

    if(data_polygon_.vertex_list_.size ())
    {
        foreach (auto var,data_polygon_.vertex_list_)
        {
            ItemPolypos* item_polypos=new ItemPolypos(this,pos_color_);
            polygon_pos_list_.append (item_polypos);
            item_polypos->setPos (mapFromScene (var)*100);
        }
    }
    else
    {
        qreal angle=45;
        for(int i=0;i<4;i++)
        {
            angle+=90;
            ItemPolypos* item_polypos=new ItemPolypos(this,pos_color_);
            polygon_pos_list_.append (item_polypos);
            item_polypos->setPos (radius_*qSin (angle*M_PI/180.0),radius_*qCos(angle*M_PI/180.0));
        }
    }

    pos_color_.setAlpha (100);

    pen_=QPen(pos_color_);

    pos_color_.setAlpha (20);
    brush_=QBrush(pos_color_);
}

void ItemPolygon::remove_polypos(QPointF pos)
{
    foreach (auto var, polygon_pos_list_)
    {
        if(qFuzzyCompare(var->scenePos().x(),pos.x())  &&  qFuzzyCompare(var->scenePos().y(),pos.y()))
            polygon_pos_list_.removeOne(var);
    }
    update ();
    scene ()->update ();
}

quint8 ItemPolygon::get_pos_num() const
{
    return this->polygon_pos_list_.size ();
}

void ItemPolygon::adjust_polygon(qreal del_head,qreal del_tail,qreal del_width)
{

    foreach (auto var_pos_item, polygon_pos_list_)
    {
        if(!is0(del_head))
        {
            if(var_pos_item->scenePos().x()>0)
            {
                var_pos_item->setPos(mapFromScene(var_pos_item->scenePos()+QPointF(del_head,0)));
            }
        }

        if(!is0(del_tail))
        {
            if(var_pos_item->scenePos().x()<0)
            {
                var_pos_item->setPos(mapFromScene(var_pos_item->scenePos()+QPointF(-del_tail,0)));
            }
        }

        if(!is0(del_width))
        {
            if(var_pos_item->scenePos().y()>0)
                var_pos_item->setPos(mapFromScene(var_pos_item->scenePos()+QPointF(0,del_width/2.0)));
            else if(var_pos_item->scenePos().y()<0)
                var_pos_item->setPos(mapFromScene(var_pos_item->scenePos()+QPointF(0,-del_width/2.0)));
        }
    }
    update_polygon_data();
}

QVariantMap ItemPolygon::get_polygon_data()
{
    update_polygon_data ();

    QVariantMap variant_map_polygon;

    switch (poly_type_) {

    case LOWSPEED:
    {
        variant_map_polygon.insert ("vertexNum",data_polygon_.vertex_num ());
        variant_map_polygon.insert ("maxSpeed",data_polygon_.max_speed_);
        variant_map_polygon.insert ("workingMode",data_polygon_.working_mode_);

    }break;

    case STOPOBJ:
    {
        variant_map_polygon.insert ("vertexNum",data_polygon_.vertex_num ());
        variant_map_polygon.insert ("workingMode",data_polygon_.working_mode_);
    }break;

    case SAFE:
    {
        variant_map_polygon.insert ("vertexNum",data_polygon_.vertex_num ());
        variant_map_polygon.insert ("workingMode",data_polygon_.working_mode_);
    }break;

    default:
        break;
    }

    QVariantList variant_list_pos;
    foreach (auto var, data_polygon_.vertex_list_)
    {
        QVariantMap variant_map_pos;
        variant_map_pos.insert ("x",var.x ());
        variant_map_pos.insert ("y",var.y ());
        variant_list_pos.append (variant_map_pos);
    }
    variant_map_polygon.insert ("vertex",variant_list_pos);

    return variant_map_polygon;
}

void ItemPolygon::update_polygon_data()
{
    data_polygon_.vertex_list_.clear ();

    foreach (auto var, polygon_pos_list_)
    {
        data_polygon_.vertex_list_.append (var->get_scene_pos_m ());
    }
}

QList<QPointF> *ItemPolygon::get_Middle_pos_list()
{
    return &middle_pos_list_;
}

QList<ItemPolypos *> *ItemPolygon::get_polygon_pos_list()
{
    return &polygon_pos_list_;
}

