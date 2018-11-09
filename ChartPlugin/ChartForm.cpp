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
    m_chart->setTitle(m_chartName);
    m_chart->setAxisX(m_axisX,m_lineseries);
    m_chart->setAxisY(m_axisY,m_lineseries);
    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignBottom);

    m_axisX->setTickCount(20);
    m_axisY->setRange(min_axisY-1,max_axisY+1);//y轴范围
    m_axisY->setTitleText(m_axisYTitle);
    ui->verticalLayout->addWidget(m_chartView);
    m_chartView->setRenderHint(QPainter::Antialiasing);

    //设置滚动条
    if(m_map.count()>NUM)
    {
        ui->horizontalSlider->setMinimum(0);
        ui->horizontalSlider->setMaximum(m_map.count()-NUM);
        QMap<qreal,qreal>::iterator xIter = m_map.begin();
        for(;xIter!=m_map.end();xIter++)
        {
            m_List.append(QPointF(xIter.key(),xIter.value()));
        }
    }
    else
    {
        ui->horizontalSlider->hide();
    }
    draw();
}

ChartForm::~ChartForm()
{
    delete ui;
}


void ChartForm::draw()
{
#if 0
    QMap<qreal,qreal>::iterator xIter = m_map.begin();
    QList<qreal> categories;
    for(;xIter!=m_map.end();xIter++)
    {
        m_lineseries->append(QPointF(xIter.key(),xIter.value()));
        categories.append(xIter.key());
    }
    m_axisX->setRange(0,categories.last());
    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignBottom);
#endif
    if(m_map.count()>NUM)
    {
        QList<qreal> categories;
        for(int i = 0; i<NUM;i++)
        {
            m_lineseries->append(m_List.at(i));
            categories.append(m_List.at(i).x());
        }
        m_axisX->setRange(0,categories.last());
    }
    else
    {
        QMap<qreal,qreal>::iterator xIter = m_map.begin();
        QList<qreal> categories;
        for(;xIter!=m_map.end();xIter++)
        {
            m_lineseries->append(QPointF(xIter.key(),xIter.value()));
            categories.append(xIter.key());
        }
        m_axisX->setRange(0,categories.last());
    }
}

void ChartForm::drawEntire(bool isentrie)
{
    QList<QPointF> templist;
    if(isentrie)
    {
        templist = m_List;
        m_axisX->setRange(0,templist.last().x());
    }
    else
    {
        for(int i = 0; i<NUM;i++)
        {
            templist.append(m_List.at(i));
        }
        m_axisX->setRange(0,templist.last().x());
    }
    m_lineseries->replace(templist);
}

void ChartForm::on_horizontalSlider_sliderMoved(int position)
{
    if(position+NUM>m_List.count())return;
    QVector<QPointF> templist;
    for(int i = position; i<position+NUM;i++)
    {
        templist.append(QPointF(m_List.at(i).x(),m_List.at(i).y()));
    }
    m_axisX->setRange(templist[0].x(),templist[NUM-1].x());
    m_chart->legend()->setVisible(true);
    m_chart->legend()->setAlignment(Qt::AlignBottom);
    m_lineseries->replace(templist);
}

void ChartForm::on_pushButton_entireOrpartial_clicked()
{
    drawEntire(m_isEntire);
    if(m_isEntire)
    {
        ui->pushButton_entireOrpartial->setText(QStringLiteral("显示局部"));
        ui->horizontalSlider->hide();
        m_isEntire = false;
    }
    else
    {
        ui->pushButton_entireOrpartial->setText(QStringLiteral("显示全部"));
        ui->horizontalSlider->show();
        ui->horizontalSlider->setValue(0);
        m_isEntire = true;
    }
}
