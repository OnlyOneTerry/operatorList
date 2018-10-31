#include "Sample.h"
#include "ui_Sample.h"

Sample::Sample(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Sample)
{
    ui->setupUi(this);
}

Sample::~Sample()
{
    delete ui;
}
