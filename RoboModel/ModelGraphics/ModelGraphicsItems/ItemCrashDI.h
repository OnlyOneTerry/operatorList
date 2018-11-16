#ifndef ITEMCRASHDI_H
#define ITEMCRASHDI_H
#include"ItemBase.h"
#include"ItemCrashDIpos.h"
#include<QPointF>

class CrashDiContentSetting;
class ItemCrashDI :public ItemBase
{
    Q_OBJECT

public:
    enum{Type = I_crashDi};
    int type() const{if(this) return Type;}
public:
    ItemCrashDI(ModelGraphicsScene* scene,QJsonObject json = QJsonObject());
    ~ItemCrashDI();
    void add_setting_widget();
    QPainterPath shape() const;
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void updata_item();

    QList<ItemCrashDIpos*>* getLine_pos_list();
    QList<QPointF>* get_Middle_pos_list();

    QVariantMap getCrashDi_data();
    int getPosSize();//获取线上点总数
    void setID(int newid);
    ItemCrashDIpos* getStartPos();
    ItemCrashDIpos* getEndPos();
public:
    crashDiData data_crashDi_;
protected:
    void position_changed();
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
private:
    QList<ItemCrashDIpos * > line_pos_list_;
    QList<QPointF> middle_pos_list_;
    QPainterPath lineShape(const qreal& stroke_width) const;

    //for undo redo
    QGraphicsItem* m_addPos_oriItem;
    QGraphicsItem* m_addPos_newItem;

    ItemCrashDIpos* startPos_;
    ItemCrashDIpos* endPos_;
//------------------引导创建设置相关
private:
    CrashDiContentSetting* crashDicontentSetting_;
public:
   static int count ;
public slots:
    void slot_guidconfig_finished();//引导配置结束，将引导设置的属性更新到右侧属性框

};

#endif // ITEMCRASHDI_H
