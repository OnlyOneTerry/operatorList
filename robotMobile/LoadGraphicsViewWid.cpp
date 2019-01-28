#include "LoadGraphicsViewWid.h"
#include "ui_LoadGraphicsViewWid.h"

LoadGraphicsViewWid::LoadGraphicsViewWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoadGraphicsViewWid)
{
    ui->setupUi(this);
}

LoadGraphicsViewWid::~LoadGraphicsViewWid()
{
    delete ui;
}
