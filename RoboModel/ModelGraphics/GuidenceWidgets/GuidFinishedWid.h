#ifndef GUIDFINISHEDWID_H
#define GUIDFINISHEDWID_H

#include <QWidget>
#include"GuidBaseSetting.h"
namespace Ui {
class GuidFinishedWid;
}

class GuidFinishedWid : public GuidBaseSetting
{
    Q_OBJECT

public:
    explicit GuidFinishedWid(QWidget *parent = 0);
    ~GuidFinishedWid();

private:
    Ui::GuidFinishedWid *ui;
};

#endif // GUIDFINISHEDWID_H
