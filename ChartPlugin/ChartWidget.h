#ifndef CHARTWIDGET_H
#define CHARTWIDGET_H
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include"FramelessHelper.h"
#include"QWChartView.h"
#include"ChartForm.h"
#include "CustomBaseDialog/CustomChildBaseDialog.h"
namespace Ui {
class ChartWidget;
}
QT_CHARTS_USE_NAMESPACE
struct  chartPosInfo
{
    double vxspeed;
    double vyspeed;
    double w;
    qint64 time;

    chartPosInfo(double vxsp,double vysp,double w,qint64 time)
    {
        this->vxspeed = vxsp;
        this->vyspeed = vysp;
        this->w = w;
        this->time = time;
    }

    chartPosInfo(const chartPosInfo& data)
    {
        this->vxspeed = data.vxspeed;
        this->vyspeed = data.vyspeed;
        this->w = data.w;
        this->time = data.time;
    }
};
#define  ROBOTMOVEINFO_PATH "./appInfo/robots/speeds.txt"
class ChartWidget : public CustomChildBaseDialog
{
    Q_OBJECT

public:
    explicit ChartWidget(QWidget *parent = 0);
    ~ChartWidget();
    //获取文件
    void acquireData(QString path);
    //解析文件
    void parseData(QString str);
    //获取最大value值
    double getMaxvalue(const QMap<qreal,qreal>& map);
    //获取最小value值
    double getMinvalue(const QMap<qreal,qreal>& map);
protected:
    void closeEvent(QCloseEvent *event);
signals:
    void sigClosedWidget();
public slots:
    void on_horizontalSlider_sliderMoved(int position);
private:
    Ui::ChartWidget *ui;
    QMap<qreal,qreal>m_xspeedMap;
    QMap<qreal,qreal>m_yspeedMap;
    QMap<qreal,qreal>m_wspeedMap;
    ChartForm* xspeedChart=NULL;
    ChartForm* yspeedChart=NULL;
    ChartForm* wspeedChart=NULL;
    qreal startTime=0;
    int   m_index=0;
    QList<QPointF> m_xspeedList;
};

#endif // CHARTWIDGET_H
