#ifndef GESTUREBASEWIDGET_H
#define GESTUREBASEWIDGET_H

#include<QWidget>
#include<QScrollBar>
#include<QMouseEvent>
class GestureBaseWidget:public QWidget
{
public:
    GestureBaseWidget(QWidget* parent=0);
    void setScrollBar(QScrollBar* bar);
    QScrollBar* getScrollBar();
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
private:
    QScrollBar* m_scrollBar=NULL;
     QPoint m_lastMousePos;
};

#endif // GESTUREBASEWIDGET_H
