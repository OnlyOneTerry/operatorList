#ifndef ITEMDI_H
#define ITEMDI_H
#include"ItemBase.h"
#include<QTimer>
class WidgetDISetting;
class ItemScanArea;
class DIContentSetting;
class ItemDI : public ItemBase
{
    Q_OBJECT
public:
    enum{Type = I_di};int type() const{ if(this)return Type;}
public:
    ItemDI(ModelGraphicsScene *scene,QJsonObject json=QJsonObject());
    ~ItemDI();
    void add_setting_widget ();
    ItemScanArea* getScanArea();
    void updateWidgetValue();
protected:
    void position_changed();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent (QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
public:
    DiData data_di_;
public:
    QVariantMap get_di_data() const;
    void update_data ();
    void update_item();
protected slots:
    void slot_angel_changed(qreal);
    void slot_guidconfig_finished();//引导配置结束，将引导设置的属性更新到右侧属性框
private:
    const int k_radius_size_=6;

     ItemScanArea* scanArea_;
 public:
     DIContentSetting* diContentSetting_=NULL;//引导设置窗口
     static int  count;//用于记录是否被点击,引导设置时只提示引导其中某一个，仅一次
};

#endif // ITEMDI_H
