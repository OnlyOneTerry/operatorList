#ifndef ITEMANGELCTROLLER_H
#define ITEMANGELCTROLLER_H
#include "ItemBase.h"

class ItemAngelCtroller : public ItemBase
{
    Q_OBJECT
public:
    enum{Type = I_angle_controller};int type() const{ if(this)return Type;}
public:
    ItemAngelCtroller(ModelGraphicsScene* scene,ItemBase *parent=0,qreal angle=0.);
    ~ItemAngelCtroller();

    void add_setting_widget();

    //for undo redo
    void setOriAngel(qreal ang);
    void setNewAngel(qreal ang);
    qreal getOriAngel();
    qreal getNewAngel();
    qreal getRoationRadius();
    void sentAngelChange(qreal ang);
protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPainterPath shape() const;
    QRectF boundingRect() const;

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void item_selected(const bool &is_selected);
    void position_changed();
private:
    qreal angel_;
    const int k_radius_=15;

    //undo redo
    qreal m_oriAngel;
    qreal m_newAngel;
    QUndoStack* m_undostack;
public:
    void set_rotation(const qreal& angel);

signals:
    void sig_angel_changed(qreal);
};

#endif // ITEMANGELCTROLLER_H
