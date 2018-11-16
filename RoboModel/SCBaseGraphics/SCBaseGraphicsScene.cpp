#include "SCBaseGraphicsScene.h"
#include <QDebug>

/*******************************************************
 * Description:｛基类｝ 场景
 *
 * @author
 * @date
 *******************************************************/

SCBaseGraphicsScene::SCBaseGraphicsScene(QObject * parent):
    QGraphicsScene(parent)
{

}

void SCBaseGraphicsScene::add_mouse_moved_rect(const QPointF &init_mouse_pos)
{
    init_mouse_move_rect_pos_=init_mouse_pos;

    QRectF rectf(init_mouse_move_rect_pos_,init_mouse_move_rect_pos_);
    QGraphicsRectItem *mouse_moved_rect=new QGraphicsRectItem(rectf);
    mouse_moved_rect->setPen (QPen(QColor(40,128,230),0.4,Qt::SolidLine));
    mouse_moved_rect->setBrush (QColor(184,212,240,100));
    addItem (mouse_moved_rect);
    mouse_moved_rect_list_.append (mouse_moved_rect);
}

void SCBaseGraphicsScene::adjust_mouse_moved_rect(const QPointF &current_mouse_pos)
{
    if(mouse_moved_rect_list_.size ())
        mouse_moved_rect_list_.last ()->setRect (QRectF(init_mouse_move_rect_pos_,current_mouse_pos).normalized ());
}

void SCBaseGraphicsScene::remove_mouse_moved_rect()
{
    if(mouse_moved_rect_list_.size ())
    {
        removeItem (mouse_moved_rect_list_.last ());
        delete mouse_moved_rect_list_.takeLast ();
    }
}


QList<QGraphicsItem*> SCBaseGraphicsScene::find_item_by_pos(const QPointF& pos)
{
    QRectF rectf(pos.x() - 1, pos.y() - 1, 2, 2);   //选择的大小为2个像素
    QList<QGraphicsItem *> itemAll = items(rectf);
    return itemAll;
}

quint32 SCBaseGraphicsScene::set_items_selected_by_rect()
{
    qint32 n_s_size=0;
    if(mouse_moved_rect_list_.size ())
    {
        selected_item_list_.clear();
        selected_item_list_ = this->items (mouse_moved_rect_list_.last ()->rect (),Qt::ItemSelectionMode::ContainsItemBoundingRect);
        //先去除选框
        remove_mouse_moved_rect();

        foreach (auto var, selected_item_list_)
        {
            if(var->type()==I_polygon_pos||var->type()==I_robot_model
                    ||var->type()==I_crashDipos
                    ||var->type()==I_circle_robot_model
                    ||var->type()==I_angle_controller
                    ||var->type()==I_scanArea_model
                    ||var->type()==I_Axis)//此类型父类不是itembase类型！！！
            {
                selected_item_list_.removeOne(var);
                continue;
            }
          //设置被选中状态
            var->setSelected(true);

            n_s_size++;
        }

    }
    return n_s_size;
}

QList<QGraphicsItem *> *SCBaseGraphicsScene::get_selected_items()
{
    return &selected_item_list_;
}


