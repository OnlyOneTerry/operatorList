#ifndef CHARTFORM_H
#define CHARTFORM_H
#include<QWidget>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include"QWChartView.h"
#include <QWidget>

namespace Ui {
class ChartForm;
}

class ChartForm : public QWidget
{
    Q_OBJECT
public:
    ChartForm(QMap<qreal,qreal> map,qreal axisyMin,qreal axisYMax,QString axisYTitle,QString serisName,QString chartName,QWidget* parent = 0);
    ~ChartForm();
public:
    void draw();
private:
    Ui::ChartForm *ui;
    QWChartView *m_chartView;
    QChart *m_chart;
    QValueAxis *m_axisX ;
    QValueAxis *m_axisY ;
    QLineSeries *m_lineseries;
    //y轴范围
    qreal min_axisY = 0;
    qreal max_axisY = 0;
    //linearSeries名称
    QString  m_seriesName;
    //y轴名称
    QString  m_axisYTitle;
    //x轴名称
    QString  m_axisXTitle;
    //chart名称
    QString  m_chartName;

    //x,y对应的数据对
    QMap<qreal,qreal> m_map;
};


#endif // CHARTFORM_H
