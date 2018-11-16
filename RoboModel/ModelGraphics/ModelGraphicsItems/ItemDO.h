#ifndef ITEMDO_H
#define ITEMDO_H
#include "ItemBase.h"
#include<QPointF>

class WidgetDOSetting;
class DOContentSetting;
class ItemDO : public ItemBase
{
    Q_OBJECT
public:
    enum{Type = I_do};int type() const{ if(this)return Type;}
public:
    ItemDO(ModelGraphicsScene *scene,QJsonObject json=QJsonObject());
    ~ItemDO();
    void add_setting_widget ();
    void updateWidgetValue();
protected:
    void position_changed();

private:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent (QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

public:
    DOData data_do_;
public:
    QVariantMap get_do_data() const;
    void update_data ();
    void update_item();

protected slots:
    void slot_angel_changed(qreal);
     void slot_guidconfig_finished();//引导配置结束，将引导设置的属性更新到右侧属性框
private:
    const int k_radius_size_=6;

    DOContentSetting* doContentSetting_;//引导窗设置

public:
    static int count;//用于记录是否被点击
};

#endif // ITEMDO_H
