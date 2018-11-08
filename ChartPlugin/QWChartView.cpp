#include "QWChartView.h"

QWChartView::QWChartView(QWidget *parent):QChartView(parent)
{

}

void QWChartView::mouseMoveEvent(QMouseEvent *event)
{
    QPoint point = event->pos();

    QChartView::mouseMoveEvent(event);
}

void QWChartView::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton)
        beginPoint = event->pos();
    QChartView::mousePressEvent(event);
}

void QWChartView::mouseReleaseEvent(QMouseEvent *event)
{
//    if(event->button()==Qt::LeftButton)
//    {
//        endPoint = event->pos();
//        QRectF rectf;
//        rectf.setTopLeft(this->beginPoint);
//        rectf.setBottomRight(this->endPoint);
//        this->chart()->zoom(1.5);
//    }
//    else if(event->button()==Qt::RightButton)
//    {
//        this->chart()->zoomReset();
//    }
    QChartView::mouseReleaseEvent(event);
}
