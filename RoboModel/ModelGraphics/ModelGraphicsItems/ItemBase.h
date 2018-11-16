#ifndef ITEMBASE_H
#define ITEMBASE_H

#include <QObject>
#include <QGraphicsItem>
#include <QDebug>
#include <QPainterPath>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include <QtMath>
#include <QGraphicsSceneMouseEvent>
#include <QCursor>
#include <QJsonObject>
#include <QJsonArray>
#include<QUndoStack>
#include<QTimer>
#include<QMenu>
#include<QAction>
#include"ModelCommon.h"
class ModelGraphicsScene;
class WidgetSetting;
class WidgetModelSetting;
class ItemAngelCtroller;


class ItemBase :public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    ItemBase(ModelGraphicsScene *scene=NULL,ItemBase* parent=NULL);
    ~ItemBase();

    virtual void setID(int newid);
    virtual int getId();

    ModelGraphicsScene* model_scene() const;
    WidgetSetting * getWidgetSetting();

    virtual void item_selected(const bool& is_selected);

    QPointF get_scene_pos_m() const;
    void set_d_r(const qreal& d_r);
    bool is_ppt_ready(const int& type) const;
    bool is_ppt_ready(widType type) const;
    quint32 selected_items_size() const;

    ItemAngelCtroller* getAngelCtroller();
public:
    QString text_infor_;//item上文字标记
    int layer_;   /*图层*/
    //timer
    QTimer timer;//用于隐藏id设置提示
    //isCreatByLoading 是否是加载模型文件创建用于设置配置文件默认项
     bool isCreatedByLoading_;

     void setOriPos(QPointF pos);
     QPointF getOriPos();

     void setNewPos(QPointF pos);
     QPointF getNewPos();

     QPen getPen();
     void setPen(QPen p);

     void setIsBeingDraged(bool flag);
     bool getIsBeingDraged();

protected:
    QPainterPath shape() const;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
public:
    virtual void add_setting_widget()=0;
    virtual void position_changed()=0;

protected:
    WidgetSetting *widget_setting_;
    ItemAngelCtroller  *item_angel_ctroller_ ;
    qreal d_r_; /*角度控制相关*/

    QPointF press_scene_pos_;     //鼠标点击的坐标
    AdjustType adjust_type_;        //调整类型

    QPen pen_;
    QBrush brush_;
    QPainterPath painter_path_;

    ModelGraphicsScene* model_scene_;

    QUndoStack* m_undoStack=NULL;
    int m_id;

    bool isPostion_changed;
    bool isBeingDraged_;//是否正在被拖拽
    QPointF m_ori_pos;
    QPointF m_new_pos;

signals:
    void sig_pressed(ItemBase*);
protected slots:
    void slot_widget_setting_deleted();
    void slot_angel_changed(qreal);

};


Q_DECLARE_METATYPE(ItemBase *)
#endif // ITEMBASE_H
