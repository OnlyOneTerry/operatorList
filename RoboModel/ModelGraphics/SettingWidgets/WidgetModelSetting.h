#ifndef WIDGETMODELSETTING_H
#define WIDGETMODELSETTING_H
#include "WidgetSetting.h"

class ModelGraphicsScene;


namespace Ui {
class WidgetModelSetting;
}

class WidgetModelSetting : public WidgetSetting
{
    Q_OBJECT
public:
    enum{Type = 4};int type() const{return Type;}
public:
    explicit WidgetModelSetting(QWidget *parent = 0);
    ~WidgetModelSetting();

protected:
    void update_value();
private:
    Ui::WidgetModelSetting *ui;
    ModelGraphicsScene* model_scene_;
};

#endif // WIDGETMODELSETTING_H
