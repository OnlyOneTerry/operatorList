#ifndef MODELGRAPHICSVIEW_H
#define MODELGRAPHICSVIEW_H

#include <QJsonObject>
#include<QUndoStack>
#include "ModelGraphicsScene.h"
#include "ModelPropertySettingWidget.h"
#include "ModelBasicSettingWidget.h"
#include "ModelCommon.h"
#include "SCBaseGraphicsScene.h"
#include "SCBaseGraphicsView.h"
#include "ModelCustomCommand.h"
class ModelBtn;
class QVBoxLayout;
class QHBoxLayout;
class QSpacerItem;
class QPushButton;
class SCBaseGraphicsView;

class ModelGraphicsView : public SCBaseGraphicsView
{
    Q_OBJECT
public:
    ModelGraphicsView(QWidget *parent=NULL);
    ~ModelGraphicsView();

    ModelPropertySettingWidget *property_widget() const;

    ModelBasicSettingWidget *basic_widget() const;
    void setBasic_widget(ModelBasicSettingWidget* basicWid);//方便与undo redo

    ModelGraphicsScene *model_scene() const;
    void reset_property_widget();

    QVariantMap get_model_variant_map() const;
    void get_rect_model_variant_map(QVariantMap& map1,QVariantMap& map2)const ;//获取矩形模型数据
    void get_circle_model_variant_map(QVariantMap& map1,QVariantMap& map2)const ;//获取圆形模型数据
    void set_model_jobj(const QJsonObject& json_object_robot_model);
    void parse_json();
    //隐藏属性框用于模型显示模式
    void hidePropertyWidget();
    //显示属性框用于模型编辑模式
    void showPropertyWidget();
    void zoomInOut(int scale);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
    /*布局相关*/
    void keyPressEvent(QKeyEvent *event);
    void resizeEvent(QResizeEvent *event);
private:
    void init_layout();
    /*面板相关*/
private:
    ModelBasicSettingWidget *widget_basic_setting_;
    ModelPropertySettingWidget *widget_property_setting_;

    ModelBtn* btn_polygon_lowspeed_;
    ModelBtn* btn_polygon_stop_;
    ModelBtn* btn_polygon_safe_;
    ModelBtn* btn_laser_;
    ModelBtn* btn_ultrasonic_;
    ModelBtn* btn_DI_;
    ModelBtn* btn_DO_;
    ModelBtn* btn_DI_crash_;//防碰撞DI
    ModelBtn* btn_magneticScesor_;
    ModelBtn* btn_RFID_;
    ModelBtn* btn_Vision_;//相机

    QPushButton *btn_scale_1x_;
    QPushButton *btn_scale_2x_;
    QPushButton *btn_scale_3x_;
    ModelGraphicsScene *model_scene_;//场景类
    //布局控件
    QVBoxLayout *verticalLayout;
    QHBoxLayout *hlayout_top;
    QVBoxLayout *vlayout_basic_setting;
    QSpacerItem *space1;
    QVBoxLayout *vlayout_property_setting;
    QHBoxLayout *hlayout_bottom;
    QSpacerItem *space_botton_left_;
    QSpacerItem *space2;
    QHBoxLayout *hlayout_btn;
    QSpacerItem *space3;

    //用于保存主窗口的QundoStack指针，传递到子窗口中
    QUndoStack* m_undoStack;
    int m_ori_scale;
    int m_new_scale;
    /*模型文件打开与保存*/
    QJsonObject json_object_robot_model_;
    //拖动视图
    QPoint beforePos;
    QPoint afterPos;
    bool m_bIsOnePoint  = false;
private slots:
    void slot_btn_scale_1x();
    void slot_btn_scale_2x();
    void slot_btn_scale_3x();

};

#endif // MODELGRAPHICSVIEW_H
