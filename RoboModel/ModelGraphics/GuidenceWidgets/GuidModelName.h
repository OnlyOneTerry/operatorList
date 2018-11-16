#ifndef GUIDMODELNAME_H
#define GUIDMODELNAME_H

#include"GuidBaseSetting.h"
namespace Ui {
class GuidModelName;
}

class GuidModelName : public GuidBaseSetting
{
    Q_OBJECT

public:
    explicit GuidModelName(QWidget *parent = 0);
    ~GuidModelName();
 void enableNextStep();
private slots:
 void on_lineEdi_model_textEdited(const QString &arg1);

 void on_lineEdi_model_textChanged(const QString &arg1);

private:
    Ui::GuidModelName *ui;

public:
    ModelBaseData basicData_;
};

#endif // GUIDMODELNAME_H
