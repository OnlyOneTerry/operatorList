#ifndef MODELBTN_H
#define MODELBTN_H

#include <QObject>
#include <QPushButton>

class ModelBtn : public QPushButton
{
    Q_OBJECT
public:
    ModelBtn(int type,QWidget *widget);
    ~ModelBtn();

public:


protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    QWidget *widget_;
    QPixmap *pixmap_;
    int type_;

signals:
    void sig_add_polygon(QPoint,int);
};

#endif // MODELBTN_H
