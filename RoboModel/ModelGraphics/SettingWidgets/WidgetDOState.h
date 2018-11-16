#ifndef WIDGETDOSTATE_H
#define WIDGETDOSTATE_H

#include <QWidget>
#include"UndoStack.h"
namespace Ui {
class WidgetDOState;
}

class WidgetDOState : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetDOState(QWidget *parent = 0);
    ~WidgetDOState();

    void setValue(quint8 value);
    quint8 getValue();
    void set_id_text(QString str);
private slots:
    void on_btn_do_toggled(bool checked);

private:
    Ui::WidgetDOState *ui;
    quint8 value_{0};

    QUndoStack* m_undoStack;
};

#endif // WIDGETDOSTATE_H
