#ifndef WIDGETPOSSETTING_H
#define WIDGETPOSSETTING_H

#include <QWidget>
#include <WidgetSetting.h>

class ItemPolypos;
namespace Ui {
class WidgetPosSetting;
}

class WidgetPosSetting : public WidgetSetting
{
    Q_OBJECT
public:
    enum{Type = 6};int type() const{return Type;}
public:
    explicit WidgetPosSetting(ItemPolypos* polypos, const QPointF&  pos, QWidget *parent = 0);
    ~WidgetPosSetting();

public:
    void set_value(const QPointF& pos);
     ItemBase* getRelateItem();
private:
    Ui::WidgetPosSetting *ui;
    ItemPolypos* item_polypos_;
signals:
    void sig_value_changed(qreal,qreal);
protected:
    void update_value();
private slots:
    void on_doubleSpinBox_x_editingFinished();
    void on_doubleSpinBox_y_editingFinished();
    void on_doubleSpinBox_x_changed();
    void on_doubleSpinBox_y_changed();
    void on_doubleSpinx_x_y_changed(qreal x,qreal y);

};

#endif // WIDGETPOSSETTING_H
