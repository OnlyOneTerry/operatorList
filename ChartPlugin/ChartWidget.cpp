#include "ChartWidget.h"
#include "ChartWidget.h"
#include "ui_ChartWidget.h"
#include <QDebug>
#include<QFile>
ChartWidget::ChartWidget(QWidget *parent) :
    CustomChildBaseDialog(parent),
    ui(new Ui::ChartWidget)
{
    ui->setupUi(this);
    //加载指定文件数据
    acquireData(ROBOTMOVEINFO_PATH);
    this->resize(980,600);
    //设置滚动条
    ui->horizontalSlider->setMinimum(0);
    ui->horizontalSlider->setMaximum(m_xspeedMap.count()-20);
    //速度
    QMap<qreal,qreal>::iterator xIter = m_xspeedMap.begin();
    for(;xIter!=m_xspeedMap.end();xIter++)
    {
        m_xspeedList.append(QPointF(xIter.key(),xIter.value()));
    }
    //转换QMap<qreal,qreal> tempMap ;
    xspeedChart = new ChartForm(m_xspeedMap,
                                getMinvalue(m_xspeedMap),
                                getMaxvalue(m_xspeedMap),
                                QStringLiteral("Vx速度"),
                                QStringLiteral("时间[ms]"),
                                QStringLiteral("vxspeed chart"));
    yspeedChart = new ChartForm(m_yspeedMap,
                                getMinvalue(m_yspeedMap),
                                getMaxvalue(m_yspeedMap),
                                QStringLiteral("Vy速度"),
                                QStringLiteral("时间[ms]"),
                                QStringLiteral("vyspeed chart"));
    wspeedChart = new ChartForm(m_wspeedMap,
                                getMinvalue(m_wspeedMap),
                                getMaxvalue(m_wspeedMap),
                                QStringLiteral("角速度"),
                                QStringLiteral("时间[ms]"),
                                QStringLiteral("wspeed chart"));
    ui->verticalLayout->addWidget(xspeedChart);
    ui->verticalLayout->addWidget(yspeedChart);
    ui->verticalLayout->addWidget(wspeedChart);

}

ChartWidget::~ChartWidget()
{
    delete ui;
    qDebug()<<"--------------------------delete ui";
}

void ChartWidget::closeEvent(QCloseEvent *event)
{
    emit sigClosedWidget();
}

void ChartWidget::acquireData(QString path)
{
    qDebug()<<"path is ----------"<<path;
    QFile file(path);
    if(file.open(QIODevice::ReadOnly))
    {
        //读取文件
        while(!file.atEnd())
        {
            QByteArray line = file.readLine();
            QString lineString(line);
            parseData(lineString);
        }
    }
    else
    {

    }

    file.close();
}

void ChartWidget::parseData(QString str)
{
    QStringList tempList = str.split("\t");
    double Vx =tempList.at(0).toDouble();
    double Vy = tempList.at(1).toDouble();
    double W  = tempList.at(2).toDouble();
    double Time = tempList.at(3).toDouble();
    if(m_index==0)
    {
        startTime = Time;
        m_xspeedMap.insert(0,Vx);
        m_yspeedMap.insert(0,Vy);
        m_wspeedMap.insert(0,W);
        qDebug()<<"time is ------1-----"<<Time-startTime;
    }
    else
    {
        m_xspeedMap.insert(Time-startTime,Vx);
        m_yspeedMap.insert(Time-startTime,Vy);
        m_wspeedMap.insert(Time-startTime,W);
        if(Time-startTime<0)
        {
            qDebug()<<"time is ------2-----"<<Time-startTime;
            qDebug()<<"startTime is ------2-----"<<startTime;
            qDebug()<<"time is ------2-----"<<Time;
        }
    }
    m_index++;
}

double ChartWidget::getMaxvalue(const QMap<qreal, qreal> &map)
{
    QMap<qreal,qreal> tempMap ;
    QMap<qreal,qreal>::const_iterator iter = map.begin();
    for(;iter!=map.end();iter++)
    {
        tempMap.insert(iter.value(),iter.key());
    }
    return tempMap.lastKey();
}

double ChartWidget::getMinvalue(const QMap<qreal, qreal> &map)
{
    QMap<qreal,qreal> tempMap ;
    QMap<qreal,qreal>::const_iterator iter = map.begin();
    for(;iter!=map.end();iter++)
    {
        tempMap.insert(iter.value(),iter.key());
    }
    return tempMap.firstKey();
}

void ChartWidget::on_horizontalSlider_sliderMoved(int position)
{
#if 0
    if(position+20>m_xspeedList.count())return;
    QVector<QPointF> templist;
    for(int i = position; i<position+20;i++)
    {
        templist.append(QPointF(m_xspeedList.at(i).x(),m_xspeedList.at(i).y()));
    }
    m_Speedchart->setTitle("xspeed chart");
    m_SpeedaxisX->setRange(templist[0].x(),templist[19].x());
    m_Speedchart->legend()->setVisible(true);
    m_Speedchart->legend()->setAlignment(Qt::AlignBottom);
    m_Speedlineseries->replace(templist);
#endif
}
