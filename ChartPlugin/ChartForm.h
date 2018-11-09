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
#define NUM 20
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
    void drawEntire(bool isentrie);
private slots:
    void on_horizontalSlider_sliderMoved(int position);

    void on_pushButton_entireOrpartial_clicked();

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
    QList<QPointF> m_List;
    //显示全局或局部
    bool m_isEntire = true;
};


#endif // CHARTFORM_H
