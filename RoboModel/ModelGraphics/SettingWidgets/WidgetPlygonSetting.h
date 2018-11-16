#ifndef WIDGETPLYGONSETTING_H
#define WIDGETPLYGONSETTING_H

#include <QWidget>
#include "WidgetSetting.h"
#include"ModelCommon.h"

class ItemPolygon;

namespace Ui {
class WidgetPlygonSetting;
}

class WidgetPlygonSetting : public WidgetSetting
{
    Q_OBJECT
public:
    enum{Type = 5};int type() const{return Type;}
public:
    explicit WidgetPlygonSetting(const int& type,ItemPolygon *item_polygon,QWidget *parent = 0);
    ~WidgetPlygonSetting();


protected:
    void update_value();
private slots:
    void on_QdoubleSpinBox_maxSpeed_valueChanged(double arg1);
    void on_comboBox_workingMode_currentIndexChanged(int index);
public:
     ItemBase* getRelateItem();
private:
    Ui::WidgetPlygonSetting *ui;
    ItemPolygon *item_polygon_;
};

#endif // WIDGETPLYGONSETTING_H
