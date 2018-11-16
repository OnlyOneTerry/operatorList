#ifndef CUSTOMCOMMAND_H
#define CUSTOMCOMMAND_H


//undo redo command
#include<QUndoCommand>
//自定义命令
#include<QComboBox>
#include<QLineEdit>
#include<QCheckBox>
#include<QDoubleSpinBox>
#include<QLabel>
#include<QGraphicsItem>
#include<QGraphicsScene>
#include<QCoreApplication>
#include<QPointF>
#include<QPoint>
#include<QGroupBox>
#include"QIpAddressEdit.h"
#include<QListWidgetItem>
#include"WidgetDOState.h"
#include"ModelCustomWidget.h"
#include"ItemBase.h"
#include"WidgetSetting.h"
#include"ModelGraphicsScene.h"
#include"ModelCommon.h"
#include"SCBaseGraphicsScene.h"
#include"SCDragDropListWidget.h"
#include"ItemPolygon.h"
#include"ItemPolypos.h"
#include"ItemRobotModel.h"
#include"PropertyBasicWidget.h"
#include"WidgetForkSetting.h"
//右侧属性窗 undo 显示item对应的属性窗口
void undoToShowRelativeWidget(PropertyBasicWidget* parentWid);
//设置当前选中的item选中状态
void setCurrentItemStatus(ModelGraphicsScene* scene,PropertyBasicWidget* parentWid);
//去除所有设备选中状态
void removeAllItemFocusStatus(ModelGraphicsScene* scene);
//去除DI设备选中状态
void removeDIFocusStatus(ModelGraphicsScene* scene);
//去除DO设备选中状态
void removeDOFocusStatus(ModelGraphicsScene* scene);
//去除Laser设备选中状态
void removeLaserFocusStatus(ModelGraphicsScene* scene);
//去除Magnetic设备选中状态
void removeMagneticFocusStatus(ModelGraphicsScene* scene);
//去除RFID设备选中状态
void removeRFIDFocusStatus(ModelGraphicsScene* scene);
//去除Ultrasonic设备选中状态
void removeUltrasonicFocusStatus(ModelGraphicsScene* scene);
//去除RobotModel设备选中状态
void removeRobotModelFocusStatus(ModelGraphicsScene* scene);
//去除CrashDI设备选中状态
void removeCrashDIFocusStatus(ModelGraphicsScene* scene);
//去除CrashDiPos设备选中状态
void removeCrashDIPosFocusStatus(ModelGraphicsScene* scene);
//去除相机设备选中状态
void removeCameraFocusStatus(ModelGraphicsScene*scene);


class customboxCommand :public QUndoCommand
{
public:
    customboxCommand( customComboBox* box1 = 0 ,int ori_indx = 0,int new_index = 0,PropertyBasicWidget* parentWid = 0,
                      QUndoCommand *parent = 0);

    void undo();//undo redo 过程不可以进行压栈操作否则会导致undoStack栈溢出
    void redo();

private:
    int m_oriIndex;
    int m_newIndex;
    PropertyBasicWidget* m_parentWid;
    customComboBox* m_combox1;
};


class modelboxCommand :public QUndoCommand
{
public:
    modelboxCommand(ItemBase *item,
                    customComboBox* box1  , customComboBox* box2 , customComboBox* box3 ,
                    DataIndex m_oriIndex , DataIndex m_newIndex , QMap<WidgetType,QLabel*> labelMap,
                    QMap<WidgetType,QDoubleSpinBox*> doubleSpinBoxMap,
                    QMap<WidgetType,QSpinBox*> spinBoxMap,
                    RobotModelData oriData, RobotModelData newData,
                    PropertyBasicWidget* parentWid = 0,
                    QUndoCommand *parent = 0);

    void undo();//undo redo 过程不可以进行压栈操作否则会导致undoStack栈溢出
    void redo();

    void chasisTypeComboxchange(RobotModelData &modelData);
    void Diff_two_wheel_selected(RobotModelData &modelData);
    void Diff_four_wheel_selected(RobotModelData &modelData);
    void AllDirection_three_wheel_selected(RobotModelData &modelData);
    void Mecanum_four_wheel_selected(RobotModelData &modelData);
    void SingleWheelDriver_selected(RobotModelData &modelData);
private:

    DataIndex m_oriIndex;
    DataIndex m_newIndex;

    RobotModelData m_oriModelData;
    RobotModelData m_newModelData;

    customComboBox* moveTypeCombox;
    customComboBox* chasisTypeComBox;
    customComboBox* absEncoderTypeComBox;
    customComboBox* chasisShapeComBox;

    ItemBase *m_item;

    QMap<WidgetType,QDoubleSpinBox*> m_DoubleSpinBoxMap;
    QMap<WidgetType,QSpinBox*> m_SpinBoxMap;
    QMap<WidgetType,QLabel*> m_LabelMap;

    PropertyBasicWidget* m_parentWid;
};


class comboxCommand :public QUndoCommand
{
public:
    comboxCommand(QComboBox* box = 0 , int ori_indx = 0, int new_index = 0,
                  QUndoCommand *parent = 0);

    void undo();//undo redo 过程不可以进行压栈操作否则会导致undoStack栈溢出
    void redo();

private:
    int m_oriIndex;
    int m_newIndex;
    QComboBox* m_combox;
};

class lineEditCommand : public QUndoCommand
{
public:
    lineEditCommand(QLineEdit* lineEdit,QString ori_text="",QString new_text ="",QUndoCommand* parent = 0);

    void undo();
    void redo();


private:
    QString m_oritext;
    QString m_newtext;
    QLineEdit* m_lineEdit;
};

class ipEditCommand : public QUndoCommand
{
public:
    ipEditCommand(QIpAddressEdit* ipEdit,QString ori_ip="",QString new_ip ="",PropertyBasicWidget* parentWid = 0,QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    QString m_ori_ip;
    QString m_new_ip;
    QIpAddressEdit* m_ipEdit;
    PropertyBasicWidget* m_parentWid;
};


class checkCommand:public QUndoCommand
{
public:
    checkCommand(QCheckBox* checkbox,bool ori_check,bool new_check,PropertyBasicWidget* parentWid = 0,QUndoCommand* parent = 0);
    void undo();
    void redo();

private:
    bool m_oriCheck;
    bool m_newCheck;
    QCheckBox* m_checkBox;
    PropertyBasicWidget* m_parentWid;
};
class checkForkCommand:public QUndoCommand
{
public:
    checkForkCommand(QCheckBox* checkbox,QWidget* forkWidget,bool ori_check,bool new_check,PropertyBasicWidget* parentWid = 0,QUndoCommand* parent = 0);
    void undo();
    void redo();

private:
    bool m_oriCheck;
    bool m_newCheck;
    QCheckBox* m_checkBox;
    QWidget* m_forkWidget;
    PropertyBasicWidget* m_parentWid;
};
class checkBrakeCommand:public QUndoCommand
{
public:
    checkBrakeCommand(QCheckBox* checkBrakebox,QCheckBox* checkAutoBrake,bool ori_check,bool new_check,PropertyBasicWidget* parentWid = 0,QUndoCommand* parent = 0);
    void undo();
    void redo();

private:
    bool m_oriCheck;
    bool m_newCheck;
    QCheckBox* m_checkBrakeBox;
    QCheckBox* m_checkAutoBox;
    PropertyBasicWidget* m_parentWid;
};

class doubleSpinBoxCommand:public QUndoCommand
{
public:
    doubleSpinBoxCommand(customDoubleSpinBox* spinBox,ItemBase *item=0,double ori_value = 0,double new_value = 0,PropertyBasicWidget* parentWid = 0,QUndoCommand* parent=0);
    void undo();
    void redo();

private:
    double m_oriValue;
    double m_newValue;
    customDoubleSpinBox* m_doubleSpinBox;
    ItemBase *m_item;
    PropertyBasicWidget* m_parentWid;
};
//点
class PosDoubleSpinBoxCommand:public QUndoCommand
{
public:
    PosDoubleSpinBoxCommand(customDoubleSpinBox* x_spinBox,customDoubleSpinBox* y_spinBox,
                            QGraphicsItem* item=0,
                            double x_ori_value = 0,double x_new_value = 0,
                            double y_ori_value = 0,double y_new_value = 0,
                            PropertyBasicWidget* parentWid = 0,
                            QUndoCommand* parent=0);

    void undo();
    void redo();
private:
    double m_x_oriValue;
    double m_x_newValue;
    double m_y_oriValue;
    double m_y_newValue;
    customDoubleSpinBox* m_x_doubleSpinBox;
    customDoubleSpinBox* m_y_doubleSpinBox;
    QGraphicsItem* m_item;
    PropertyBasicWidget* m_parentWid;
};


class SpinBoxCommand:public QUndoCommand
{
public:
    SpinBoxCommand(customSpinBox* spinBox,int ori_value = 0,int new_value = 0,PropertyBasicWidget* parentWid = 0,QUndoCommand* parent=0);
    void undo();
    void redo();

private:
    int m_oriValue;
    int m_newValue;
    customSpinBox* m_spinBox;
    PropertyBasicWidget* m_parentWid;
};


//鼠标拖动底盘矩形框命令
class DragChassisCommand:public QUndoCommand
{
public:
    DragChassisCommand(ItemBase *item=0,double ori_headValue = 0,double new_headValue = 0,
                       double ori_tailValue = 0,double new_tailValue = 0,
                       double ori_widValue = 0,double new_widValue = 0,
                       customDoubleSpinBox* headbox=0,
                       customDoubleSpinBox* tailbox=0,
                       customDoubleSpinBox* widbox=0,
                       ModelGraphicsScene* scene=0,
                       QUndoCommand* parent=0);

    DragChassisCommand(ItemBase* item=0, double ori_radius = 0, double new_radius = 0,
                       customDoubleSpinBox* radiusbox = 0,
                       ModelGraphicsScene* scene=0,
                       QUndoCommand* parent=0);
    void undo();
    void redo();

private:
    int m_ori_headValue;
    int m_new_headValue;
    int m_ori_tailValue;
    int m_new_tailValue;
    int m_ori_widValue;
    int m_new_widValue;
    int m_ori_radius;
    int m_new_radius;
    customDoubleSpinBox* m_headbox;
    customDoubleSpinBox* m_tailbox;
    customDoubleSpinBox* m_widbox;
    customDoubleSpinBox* m_radiusBox;
    ItemBase* m_item;
    ModelGraphicsScene* m_scene;
};

//layer

class  layerCommand :public QUndoCommand
{

public:
    layerCommand(SCDragDropListWidget* m_listWiget,QList<ItemBase*>selected_item_list,QList<QListWidgetItem*> ori_list,QList<QListWidgetItem*> new_list, QUndoCommand* parent = 0);

    void undo();
    void redo();
private:

    QList<QListWidgetItem*>  m_ori_itemlist;
    QList<QListWidgetItem*>  m_new_itemlist;
    QList<ItemBase*> m_selected_item_list;
    SCDragDropListWidget* m_listWiget;
    QList<QListWidgetItem*> templist;
};

//QgraphicScene relative

class  zoomInOutCommand:public QUndoCommand
{
public:
    zoomInOutCommand(ModelGraphicsScene* scene=0, int oriScale = 1,int newScale = 1,QUndoCommand* parent =0);

    void undo();
    void redo();

private:

    int  m_ori_scale;
    int  m_new_scale;
    ModelGraphicsScene* m_scene;
};
//移动多个item
class moveItemsCommand:public QUndoCommand
{
  public:
    moveItemsCommand(ModelGraphicsScene* scene ,QMap<ItemBase*,QPointF> oriItems,QMap<ItemBase*,QPointF> newItems, QUndoCommand* parent = 0);
    void undo();
    void redo();

private:
   QMap<ItemBase*,QPointF> m_oriItemMap;
   QMap<ItemBase*,QPointF> m_newItemMap;
   ModelGraphicsScene* m_scene;
};

class moveItemCommand:public QUndoCommand
{
public:
    moveItemCommand(QPointF ori_pos,QPointF new_pos,ModelGraphicsScene* scene=0,ItemBase* m_item = 0,QUndoCommand* parent=0);
    void undo();
    void redo();

private:
    QPointF m_ori_pos;
    QPointF m_new_pos;
    ItemBase* m_item;
    ModelGraphicsScene* m_scene;
};

class rotateItemCommand:public QUndoCommand
{
public:
    rotateItemCommand(qreal ori_angel,qreal new_angel,ModelGraphicsScene* scene=0,ItemBase* item = 0,QUndoCommand* parent=0);
    void undo();
    void redo();

private:
    qreal m_ori_angel;
    qreal m_new_angel;
    ItemBase* m_item;
    ModelGraphicsScene* m_scene;
};
class addItemCommand:public QUndoCommand
{
public:
    addItemCommand( int oritype,int newtype,QPoint oriPos,QPoint newPos,
                    QGraphicsItem* ori_item=0,QGraphicsItem* new_item = 0,
                    ModelGraphicsScene* scene=0, QUndoCommand* parent=0);

    void undo();
    void redo();
private:
    int m_ori_Type;
    int m_new_Type;
    QPoint m_ori_pos;
    QPoint m_new_pos;
    QGraphicsItem* m_ori_item;
    QGraphicsItem* m_new_item;
    ModelGraphicsScene* m_scene;
};
//删除选中的多个item
class deleteItemsCommand:public QUndoCommand
{
public:
    deleteItemsCommand(ModelGraphicsScene* scene, QList<ItemBase*> items,QUndoCommand* parent = 0);

    void undo();
    void redo();

private:
    QList<ItemBase*> m_itemlist;
    ModelGraphicsScene* m_scene;
};

class deleteItemCommand:public QUndoCommand
{
public:
    deleteItemCommand(int oritype,int newtype,QPointF oriPos,QPointF newPos,
                      ModelGraphicsScene* scene=0,QGraphicsItem* oriItem=0,
                      QGraphicsItem* newItem=0,QGraphicsItem * roboItem=0, QUndoCommand* parent=0);

    void undo();
    void redo();
private:
    int m_ori_Type;
    int m_new_Type;
    QPointF m_ori_pos;
    QPointF m_new_pos;
    QGraphicsItem* m_oriItem;
    QGraphicsItem* m_newItem;
    ModelGraphicsScene* m_scene;
    QGraphicsItem * m_roboItem;
};


class addPosCommand:public QUndoCommand
{
public:
    addPosCommand(ItemBase *item = 0,int index = 0,
                  QGraphicsItem* oriItem = 0, QGraphicsItem* newItem = 0,
                  ModelGraphicsScene* scene=0, QUndoCommand* parent= 0);
    void undo();
    void redo();
private:
    QGraphicsItem* m_oriItem;
    QGraphicsItem* m_newItem;
    ItemBase* m_Item ;
    ModelGraphicsScene* m_scene;
    int m_index;
};


class DoStateCommand:public QUndoCommand
{
public:
    DoStateCommand(customPushBtn* doBtn,int oriState,int newState,  QUndoCommand* parent =0);
    void undo();
    void redo();
private:
    int m_oriState;
    int m_newState;
    customPushBtn* m_doBtn;
};





#endif // CUSTOMCOMMAND_H
