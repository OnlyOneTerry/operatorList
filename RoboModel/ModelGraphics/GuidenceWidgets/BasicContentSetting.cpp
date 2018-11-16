#include "BasicContentSetting.h"
#include "ui_BasicContentSetting.h"
#include"UiClass.h"
#include<QMessageBox>

BasicContentSetting::BasicContentSetting(QWidget *parent) :
    CustomChildBaseDialog(parent),
    ui(new Ui::BasicContentSetting)
{
    ui->setupUi(this);

    this->setModal(true);
    this->d->setHideButton(CustomBaseDialog::E_CloseButton);
    this->d->setAttribute(Qt::WA_DeleteOnClose);
    //    this->setWindowFlags(Qt::WindowStaysOnTopHint|Qt::WindowCloseButtonHint);
    //    setWindowFlags(this->windowFlags() & ~Qt::WindowCloseButtonHint);
}

BasicContentSetting::~BasicContentSetting()
{
    delete ui;
}

void BasicContentSetting::setId(int id)
{
    id_ = id;
}

int BasicContentSetting::getId()
{
    return id_;
}

