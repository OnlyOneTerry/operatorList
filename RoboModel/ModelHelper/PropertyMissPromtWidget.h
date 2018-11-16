#ifndef PROPERTYMISSPROMTWIDGET_H
#define PROPERTYMISSPROMTWIDGET_H

#include <QWidget>

namespace Ui {
class PropertyMissPromtWidget;
}

class PropertyMissPromtWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PropertyMissPromtWidget(QWidget *parent = 0);
    ~PropertyMissPromtWidget();
    void setPromptInfor(QString missInfor, QString extralInfor, QList<QString> repeatIdlist);
    void updateWidget();
signals:
    void sig_promptWid_close();
private slots:
    void on_pushButton_close_clicked();

private:
    Ui::PropertyMissPromtWidget *ui;
    QImage* m_image;
};

#endif // PROPERTYMISSPROMTWIDGET_H
