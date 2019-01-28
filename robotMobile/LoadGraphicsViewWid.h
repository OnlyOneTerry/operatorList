#ifndef LOADGRAPHICSVIEWWID_H
#define LOADGRAPHICSVIEWWID_H

#include <QWidget>

namespace Ui {
class LoadGraphicsViewWid;
}

class LoadGraphicsViewWid : public QWidget
{
    Q_OBJECT

public:
    explicit LoadGraphicsViewWid(QWidget *parent = 0);
    ~LoadGraphicsViewWid();

private:
    Ui::LoadGraphicsViewWid *ui;
};

#endif // LOADGRAPHICSVIEWWID_H
