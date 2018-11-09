#include "ChartWidget.h"
#include "ChartWidget.h"
#include "ui_ChartWidget.h"
#include "RoLog.h"
#include <QFile>
#include<QDebug>
#include<QFileDialog>
ChartWidget::ChartWidget(QWidget *parent) :
    CustomChildBaseDialog(parent),
    ui(new Ui::ChartWidget)
{
    ui->setupUi(this);
    //加载指定文件数据
//    acquireData(ROBOTMOVEINFO_PATH);
    this->resize(980,600);
    this->d->showMinMaxButton();
    //转换QMap<qreal,qreal> tempMap ;
    //    xspeedChart = new ChartForm(m_xspeedMap,
    //                                getMinvalue(m_xspeedMap),
    //                                getMaxvalue(m_xspeedMap),
    //                                QStringLiteral("Vx速度"),
    //                                QStringLiteral("时间[ms]"),
    //                                QStringLiteral("vxspeed chart"));
    //    yspeedChart = new ChartForm(m_yspeedMap,
    //                                getMinvalue(m_yspeedMap),
    //                                getMaxvalue(m_yspeedMap),
    //                                QStringLiteral("Vy速度"),
    //                                QStringLiteral("时间[ms]"),
    //                                QStringLiteral("vyspeed chart"));
    //    wspeedChart = new ChartForm(m_wspeedMap,
    //                                getMinvalue(m_wspeedMap),
    //                                getMaxvalue(m_wspeedMap),
    //                                QStringLiteral("角速度"),
    //                                QStringLiteral("时间[ms]"),
    //                                QStringLiteral("wspeed chart"));
    //    ui->verticalLayout->addWidget(xspeedChart);
    //    ui->verticalLayout->addWidget(yspeedChart);
    //    ui->verticalLayout->addWidget(wspeedChart);

    //将显示数据
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

bool ChartWidget::acquireData(QString path)
{
    qDebug()<<"path is ----------"<<path;
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        SCDebug<<file.errorString()<<path;
        return false;
    }
    //读取文件
    m_dataMap.clear();
    int i= 0;
    while(!file.atEnd())
    {
        QByteArray line = file.readLine();
        QString lineString(line);
        parseData(lineString);
        parseDataInList(lineString);
    }

    file.close();
    return true;
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

void ChartWidget::parseDataInList(QString str)
{
    if(str.isEmpty())return;
    QStringList tempList = str.split("\t");
    if(tempList.size()>0)
    {
        for(int i= 0;i<tempList.size();i++)
        {
            m_dataMap[i].append(tempList.at(i).toDouble());
        }
    }
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

void ChartWidget::show()
{
//    int n = 0;
    QMap<int,QList<qreal>>::iterator iter = m_dataMap.begin();
    for(;iter!=m_dataMap.end();iter++)
    {
//        if(n>2)return;
        QList<qreal> list = iter.value();
        QMap<qreal,qreal> mapsort;
        QMap<qreal,qreal> map;
        for(int i =0;i<list.size();i++)
        {
            mapsort.insert(list.at(i),i);//用于对值排序
            map.insert(i,list.at(i));
        }
        ChartForm* chart = new ChartForm(map,mapsort.firstKey(),
                                         mapsort.lastKey(),
                                         QStringLiteral("axisY"),
                                         QStringLiteral("axisX"),
                                         QStringLiteral(""));
        ui->verticalLayout->addWidget(chart);
//        n++;
    }
}


void ChartWidget::on_pushButton_load_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Spreadsheet"), ".",
                                                    tr("Spreadsheet files (*.txt)"));
    if (!fileName.isEmpty())
    {
        acquireData(fileName);
        show();
    }
}

void ChartWidget::on_checkBox_time_clicked(bool checked)
{

}
