#include "DialogSaveAsTemplate.h"
#include "ui_DialogSaveAsTemplate.h"
#include"PublicClass.h"
#include<QDir>
DialogSaveAsTemplate::DialogSaveAsTemplate(QWidget *parent) :
    CustomChildBaseDialog(parent),
    ui(new Ui::DialogSaveAsTemplate)
{
    ui->setupUi(this);
    ui->label_warningToolTip->hide();
    //若不存在默认文件夹则先创建
    QDir dir;
    dir.cd("./appInfo/model");
    if(!dir.exists("template"))
    {
        bool ok = dir.mkdir("template");//只创建一级子目录，即必须保证上级目录存在
    }
    dir.setCurrent(qApp->applicationDirPath());
    //    setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowCloseButtonHint);
}

DialogSaveAsTemplate::~DialogSaveAsTemplate()
{
    delete ui;
}

QStringList DialogSaveAsTemplate::getTmpNames()
{
    QStringList namelist;

    QDir dir("./appInfo/model/template");
    QStringList filter;
    filter<<"*.tmp";
    dir.setNameFilters(filter);
    QFileInfoList fileInfoList=dir.entryInfoList(filter);
    dir.setCurrent(qApp->applicationDirPath());
    for (int i=0;i<fileInfoList.count();i++)
    {
        namelist<<fileInfoList[i].fileName();
    }
    return namelist;
}

void DialogSaveAsTemplate::on_pushButton_confirm_clicked()
{
    QString name = ui->lineEdit->text();
    if(name.simplified().isEmpty())
    {
        ui->label_warningToolTip->show();
        ui->label_warningToolTip->setText(tr("Warning: name cannot be empty"));
        return;
    }
    QStringList namelist = getTmpNames();
    for(int i = 0;i<namelist.size();i++)
    {
        qDebug()<<"i is "<<i<<namelist.at(i);
    }
    if(namelist.contains(name.simplified()+".tmp"))
    {
        ui->label_warningToolTip->show();
        ui->label_warningToolTip->setText(tr("Warning: template already exists"));
        return;
    }
    emit sig_confirm(ui->lineEdit->text());
}

void DialogSaveAsTemplate::on_pushButton_cancel_clicked()
{
    emit sig_cancel();
}

void DialogSaveAsTemplate::on_lineEdit_textChanged(const QString &arg1)
{
    if(arg1.simplified().isEmpty())
    {
        ui->label_warningToolTip->show();
        ui->label_warningToolTip->setText(tr("Warning: name cannot be empty"));
        return;
    }
    else
    {
        ui->label_warningToolTip->hide();
    }

    QStringList namelist = getTmpNames();
    if(namelist.contains(arg1.simplified()+".tmp"))
    {
        ui->label_warningToolTip->show();
        ui->label_warningToolTip->setText(tr("Warning: template already exists"));
        return;
    }

}
