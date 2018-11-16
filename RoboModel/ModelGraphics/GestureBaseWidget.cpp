#include "GestureBaseWidget.h"

GestureBaseWidget::GestureBaseWidget(QWidget *parent):QWidget(parent),
    m_lastMousePos(QPoint(0,0))
{

}

void GestureBaseWidget::setScrollBar(QScrollBar *bar)
{
    m_scrollBar = bar;
}

QScrollBar *GestureBaseWidget::getScrollBar()
{
    return m_scrollBar;
}

void GestureBaseWidget::mouseMoveEvent(QMouseEvent *event)
{
    QPointF mouseDelta = event->pos() - m_lastMousePos;
    int deltax = mouseDelta.x();
    int deltay = mouseDelta.y();
//    int pos = this->verticalScrollBar()->value();
//    this->verticalScrollBar()->setValue(pos-deltax*5/1);
//    m_lastMousePos = event->pos();
    QWidget::mouseMoveEvent(event);
}

void GestureBaseWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        m_lastMousePos = event->pos();
}
