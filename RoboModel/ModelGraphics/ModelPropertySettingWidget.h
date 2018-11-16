#ifndef MODELPROPERTYSETTINGWIDGET_H
#define MODELPROPERTYSETTINGWIDGET_H

#include <QListWidget>
#include <QModelIndexList>
#include <QWidget>
#include<QListWidgetItem>
#include<QUndoStack>
#include"../ModelHelper/ModelCustomCommand.h"
#include"SettingWidgets/WidgetModelSetting.h"
class ItemBase;
class WidgetSetting;
class WidgetModelSetting;
namespace Ui {
class ModelPropertySettingWidget;
}

class ModelPropertySettingWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ModelPropertySettingWidget(QWidget *parent = 0);
    ~ModelPropertySettingWidget();

public:
    void update_property_widget(WidgetSetting *wid);
    WidgetSetting* get_current_property_widget() const;

    WidgetModelSetting* getWidgetModelSetting();
    void setUndostack(QUndoStack* stack);
    QWidget* getScrollcontentWidget();
    QUndoStack* getUndoStack();
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void showTopItemWid(ItemBase* item);
private:
    Ui::ModelPropertySettingWidget *ui;
    WidgetModelSetting* item_infor_widget_;//infor widget
    QList<ItemBase *> selected_item_list_;

    QList<QListWidgetItem*> m_ori_itemlist;
    QList<QListWidgetItem*> m_new_itemlist;

    QUndoStack* m_unstack;

    QPoint m_lastMousePos;
    /*图层相关操作*/
public:
    void update_layer(QList<ItemBase *> selected_item_list_);
private slots:
    void slot_current_row_changed();
    void slot_moveToTop(QListWidgetItem*item);
};

#endif // MODELPROPERTYSETTINGWIDGET_H
