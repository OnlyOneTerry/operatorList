#include "ChartForm.h"
#include "ui_ChartForm.h"
ChartForm::ChartForm(QMap<qreal, qreal> map, qreal axisyMin, qreal axisYMax, QString axisYTitle, QString serisName, QString chartName, QWidget *parent):QWidget(parent),
    m_axisYTitle(axisYTitle),
    m_seriesName(serisName),
    m_chartName(chartName),
    min_axisY(axisyMin),
    max_axisY(axisYMax),
    m_map(map),
    ui(new Ui::ChartForm)
{
    ui->setupUi(this);
    m_chart  = new QChart();
    m_chartView = new  QWChartView;
    m_chartView->setChart(m_chart);
    m_axisX = new QValueAxis();
    m_axisY = new QValueAxis();
    m_lineseries = new QLineSeries();
    m_lineseries->setName(m_seriesName);
    m_chart->addSeries(m_lineseries);
    m_chart->setAxisX(m_axisX,m_lineseries);
    m_chart->setAxisY(m_axisY,m_lineseries);
    m_axisY->setRange(min_axisY-1,max_axisY+1);//y轴范围
    m_axisY->setTitleText(m_axisYTitle);
    ui->verticalLayout->addWidget(m_chartView);
    m_chartView->setRenderHint(QPainter::Antialiasing);
    draw();
}

ChartForm::~ChartForm()
{
    delete ui;
}


void ChartForm::draw()
{
    QMap<qreal,qreal>::iterator xIter = m_map.begin();
    QList<qreal> categories;
    for(;xIter!=m_map.end();xIter++)
    {
        m_lineseries->append(QPointF(xIter.key(),xIter.value()));
        categories.append(xIter.key());
    }
    m_chart->setTitle(m_chartName);
    m_axisX->setRange(0,categories.last());
    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignBottom);
}
