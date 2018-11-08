#ifndef QWCHARTVIEW_H
#define QWCHARTVIEW_H
#include <QtCharts/QChartView>
QT_CHARTS_USE_NAMESPACE
class QWChartView :public QChartView
{
public:
    QWChartView(QWidget* parent=0);
protected:
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    QPoint beginPoint;
    QPoint endPoint;
signals:
    void mouseMovePoint(QPoint point);

};

#endif // QWCHARTVIEW_H
