#include "GuidFinishedWid.h"
#include "ui_GuidFinishedWid.h"

GuidFinishedWid::GuidFinishedWid(QWidget *parent) :
    GuidBaseSetting(parent),
    ui(new Ui::GuidFinishedWid)
{
    ui->setupUi(this);
    setType(GUID_FINISHED);
    ui->label_warningToolTip->setText(tr("  Depending on your configuration, the device will be added to the workspace. You can use the mouse to drag the device to move, or select the device and type coordinates in the right property box to set it."));
}

GuidFinishedWid::~GuidFinishedWid()
{
    delete ui;
}
