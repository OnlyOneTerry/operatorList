#include "WidgetModelSetting.h"
#include "ui_WidgetModelSetting.h"
#include "ModelGraphicsView.h"
WidgetModelSetting::WidgetModelSetting(QWidget *parent) :
    WidgetSetting(parent),
    ui(new Ui::WidgetModelSetting),
    model_scene_(NULL)

{
    ui->setupUi(this);
    ui->groupBox->hide();
}

WidgetModelSetting::~WidgetModelSetting()
{
    delete ui;
}

void WidgetModelSetting::update_value (){}
