#include "Test2.h"
#include "ui_Test2.h"

Test2::Test2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Test2)
{
    ui->setupUi(this);
}

Test2::~Test2()
{
    delete ui;
}
