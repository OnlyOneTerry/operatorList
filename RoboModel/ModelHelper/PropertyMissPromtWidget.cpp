#include "PropertyMissPromtWidget.h"
#include "ui_propertyMissPromtWidget.h"

PropertyMissPromtWidget::PropertyMissPromtWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PropertyMissPromtWidget)
{
    ui->setupUi(this);
}

PropertyMissPromtWidget::~PropertyMissPromtWidget()
{
    delete ui;
}

void PropertyMissPromtWidget::setPromptInfor(QString missInfor, QString extralInfor,QList<QString> repeatIdlist)
{
     ui->textEdit->clear();
     foreach(auto var,repeatIdlist) {
         ui->textEdit->append(var);
     }
     if(repeatIdlist.size()>0)
     {
         ui->textEdit->append("\n");
     }
     ui->textEdit->append(missInfor);
     ui->textEdit->append(extralInfor);
     QTextCursor cursor = ui->textEdit->textCursor();
     cursor.movePosition(QTextCursor::Start);
     ui->textEdit->setTextCursor(cursor);
}

void PropertyMissPromtWidget::updateWidget()
{
    this->move(this->parentWidget()->width()/2-this->width()/2, this->parentWidget()->height()/2-this->height()/2);
}

void PropertyMissPromtWidget::on_pushButton_close_clicked()
{
    this->hide();
    emit sig_promptWid_close();
}
