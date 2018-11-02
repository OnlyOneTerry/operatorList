#ifndef SAMPLE_H
#define SAMPLE_H

#include <QWidget>

namespace Ui {
class Sample;
}

class Sample : public QWidget
{
    Q_OBJECT

public:
    explicit Sample(QWidget *parent = 0);
    ~Sample();

private:
    Ui::Sample *ui;
};

#endif // SAMPLE_H
