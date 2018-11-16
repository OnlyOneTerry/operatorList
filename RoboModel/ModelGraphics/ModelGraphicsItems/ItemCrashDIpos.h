#ifndef ITEMCRASHDIPOS_H
#define ITEMCRASHDIPOS_H
#include"ItemBase.h"

class ItemCrashDI;

class ItemCrashDIpos :public  ItemBase
{
    Q_OBJECT
public:
    enum{Type = I_crashDipos};
    int type()const
    {
        if(this)
       return Type;
    }

    ItemCrashDIpos(ItemCrashDI *parent,QColor color=Qt::red);
    ~ItemCrashDIpos();

    qreal getRadius();
    void setRadius(qreal r);
    ItemCrashDI* getCrashDi();
    QPainterPath shape()const;
    QRectF boundingRect()const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void add_setting_widget ();
    void item_selected(const bool &is_selected);
    void setIndexInLine(int idx);
    int getIndexfromLine();
    void setDrawFlag(bool flag);
    ItemCrashDI* getParent();
    void updatePosWidget();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void position_changed();

signals:
    void pos_changed(qreal,qreal);
private:
    qreal radius_;
    QColor color_;
    ItemCrashDI* crashDi_;
    int indexInLine_;
    bool drawFlag_;//当前是第一个点时画父亲编号
};

#endif // ITEMCRASHDIPOS_H
