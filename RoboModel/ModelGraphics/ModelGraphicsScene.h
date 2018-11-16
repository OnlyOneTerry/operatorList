#ifndef MODELGRAPHICSSCENE_H
#define MODELGRAPHICSSCENE_H

#include <QGraphicsRectItem>
#include<QUndoStack>
#include "ItemAxis.h"
#include "itemrobotmodel.h"
#include "SCBaseGraphicsScene.h"
#include<QTimer>
#include<QKeyEvent>
class ModelGraphicsView;
class ItemLaser;
class ItemPolypos;
class ItemPolygon;
class ItemDI;
class ItemCrashDI;
class ItemUltrasonic;
class ItemMagneticSensor;
class ItemRobotModel;
class ItemDO;
class ItemScanArea;
class ItemRFIDSensor;
class ItemBase;
class ItemCamera;
class ModelGraphicsScene :public SCBaseGraphicsScene
{
    Q_OBJECT
public:
    ModelGraphicsScene(QObject *parent=NULL);
    ~ModelGraphicsScene();

public:
    void delete_operation();
    QPointF get_scene_mousepos() const;
    int  getItemAcount();

public:
    ModelGraphicsView* model_view() const;
    QStringList get_id_list(quint8 type) const;//

    void init_new();
    void adjust_polygons(qreal head,qreal tail,qreal width);
    void clearFocusState();
    bool isAlldevsOnChassis();
    bool isWithInChassisArea(qreal x, qreal y,bool isCrashDI=false);//是否在底盘区域内
    bool isGuidenceFinished();//如果是引导创建判断是否结束
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
private:
    void add_axis();
public slots:
    void slot_add_polygon(QPoint,int);
    void slotPressElapsedEnough();
    void slot_rightClickToDelete();
public:
    RobotModelData get_robot_model_data();
    QGraphicsItem* item_robot_model() const;
    ItemRobotModel* getItem_robot_model()const;//根据底盘形状获取底盘对应的item
    void parse_robot_data(const RobotModelData& robot_model_data);//read data from file
    void parse_json_polygon(const QJsonObject& json_object_polygon,const quint8& poly_type);
    void parse_json_laser(const QJsonObject& json_object_laser);
    void parse_json_ultrasonic(const QJsonObject& json_object_ultrasonic);
    void parse_json_di(const QJsonObject& json_object_di);
    void parse_json_do(const QJsonObject& json_object_do);
    void parse_json_magnetic(const QJsonObject& json_magnetic);
    void parse_json_RFID(const QJsonObject &json_RFID);
    void parse_json_Cameral(const QJsonObject& json_Cameral);
    void parse_json_Inverse(QList<bool> list);//读取正反转设置
    void parse_json_Inverse(QJsonArray& array);//读取正反转设置
    void parse_json_driverID(QJsonArray& array);//读取电机编号
    /* variant map */
    QVariantMap get_polygon_data(const quint8& type);
    QVariantMap get_laser_data();
    QVariantMap get_ultrasonic_data();
    QVariantMap get_di_data();
    QVariantMap get_magnetic_data();
    QVariantMap get_do_data();
    QVariantMap  get_RFID_data();
    QVariantMap get_Camera_data();
    QVariantList getRobotInverse_data();//获取正反转标记
    QVariantList getRobotDriverId_data();//获取电机id
    /*Item 操作*/

    QList<ItemBase *> find_items(const QPointF& pos);
    QList<ItemPolygon*>* get_PolygonList();
    QList<ItemLaser *>* get_Laserlist();
    QList<ItemDI *>* get_Di_list();
    QList<ItemDO *>* get_Do_list();
    QList<ItemUltrasonic *>* get_ultrasonic_list();
    QList<ItemMagneticSensor *>* get_magneticSensor_list();
    QList<ItemCrashDI*>* get_crashDi_list();
    QList<ItemRFIDSensor*>* get_RFID_list();
    QList<ItemCamera*>*   get_Camera_list();
    //获取用于定位的激光数量
    int laserForLocationNum();
    //设置当前被点击选中的item
    void setCurrentPressItem(ItemBase* curItem);
    ItemBase* getCurrentPressItem();
private:
    QPointF mouse_pos_;
    ItemRobotModel *robot_model_;
    ItemScanArea* scanArea_;
    QList<ItemPolygon *> polygon_list_;
    QList<ItemLaser *> laser_list_;
    QList<ItemDI *> di_list_;
    QList<ItemDO *> do_list_;
    QList<ItemUltrasonic *> ultrasonic_list_;
    QList<ItemMagneticSensor *> magneticSensor_list_;
    QList<ItemCrashDI*> crashDi_list_;
    QList<ItemRFIDSensor*> RFID_list_;
    QList<ItemCamera*> camera_list_;
    QList<ItemBase *> items_under_pos_list_;
    /*模型数据存取相关*/

    QUndoStack* m_undoStack;
    QGraphicsItem* m_ori_activeItem;
    QGraphicsItem* m_new_activeItem;

    QPoint m_add_ori_point;
    QPoint m_add_new_point;
    int m_add_ori_type;
    int m_add_new_type;

    QPointF m_del_ori_pos;
    QPointF m_del_new_pos;
    int m_del_ori_type;
    int m_del_new_type;

    QGraphicsItem* m_del_oriItem;
    QGraphicsItem* m_del_newItem;

    QGraphicsItem* m_add_oriItem;
    QGraphicsItem* m_add_newItem;

    //防抖动误触移动操作
    QTimer *timer;
    bool m_isElapsedEnough;//按压时间足够长
    bool m_isPressed;
    bool m_isAllowAddItem;//允许添加item
    //当前点击的item
    ItemBase* m_currentPressItem=NULL;
    QGraphicsItem* m_currentRightClickedItem = NULL;
public:
    //记录拖动前后的items状态用于同时拖动多个items时 undo redo
    QMap<ItemBase*,QPointF> m_oriItemMap;
    QMap<ItemBase*,QPointF> m_newItemMap;

};

#endif // MODELGRAPHICSSCENE_H
