#ifndef ITEMULTRASONIC_H
#define ITEMULTRASONIC_H

#include "ItemBase.h"

class WidgetUltrasonicSetting;
class ItemScanArea;
class UltrasonicContentSetting;
class ItemUltrasonic : public ItemBase
{
    Q_OBJECT
public:
    enum{Type = I_ultrasonic};int type() const{ if(this)return Type;}
public:
    ItemUltrasonic(ModelGraphicsScene*,QJsonObject json=QJsonObject());
    ~ItemUltrasonic();
    void add_setting_widget ();
    void updateWidgetValue();
protected:
    void position_changed();
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

public:
    UltrasonicData data_ultrasonic_;

public:
    QVariantMap get_ultrasonic_data();
    ItemScanArea* getScanArea();
public:
    void update_item();
    void update_data ();

protected slots:
    void slot_angel_changed(qreal);
    void slot_guidconfig_finished();//引导配置结束，将引导设置的属性更新到右侧属性框
private:
    const int k_radius_size_=6;
    ItemScanArea *scanArea_;

    UltrasonicContentSetting* ultrasoinSetting_;
public:
    static int count;
};

#endif // ITEMULTRASONIC_H
