#include "GuidModelName.h"
#include "ui_GuidModelName.h"

GuidModelName::GuidModelName(QWidget *parent) :
    GuidBaseSetting(parent),
    ui(new Ui::GuidModelName)
{
    ui->setupUi(this);
    setType(GUID_MODEL_NAME);
}

GuidModelName::~GuidModelName()
{
    delete ui;
}

void GuidModelName::enableNextStep()
{
   if(ui->lineEdi_model->text().simplified().isEmpty())
       emit sig_enableNextStep(false);
   else
       emit sig_enableNextStep(true);
}

void GuidModelName::on_lineEdi_model_textEdited(const QString &arg1)
{
   basicData_.model_name_ = arg1;
   enableNextStep();
}

void GuidModelName::on_lineEdi_model_textChanged(const QString &arg1)
{
    basicData_.model_name_ = arg1;
    enableNextStep();
}
