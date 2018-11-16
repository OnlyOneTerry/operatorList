#ifndef SCBaseGraphicsScene_H
#define SCBaseGraphicsScene_H

#include <QGraphicsScene>
#include <QGraphicsRectItem>

#include"../ModelCommon.h"
class  SCBaseGraphicsScene :public QGraphicsScene
{
    Q_OBJECT
public:
    SCBaseGraphicsScene(QObject * parent =NULL);

    //框选相关
private:
    QPointF init_mouse_move_rect_pos_;
    QList<QGraphicsRectItem *> mouse_moved_rect_list_;
public:
    void add_mouse_moved_rect(const QPointF& init_mouse_pos);
    void adjust_mouse_moved_rect(const QPointF& current_mouse_pos);
    void remove_mouse_moved_rect();
    //查找QGraphicsItem
public:
    QList<QGraphicsItem *> find_item_by_pos(const QPointF &pos);
    quint32 set_items_selected_by_rect();
    QList<QGraphicsItem*>* get_selected_items();//获取被框选的item
private:
    QList<QGraphicsItem*> selected_item_list_;
};

#endif // SCBaseGraphicsScene_H
