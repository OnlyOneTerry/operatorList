#ifndef ReadThread_H
#define ReadThread_H

#include "BaseThread.h"

class ReadThread : public BaseThread
{
    Q_OBJECT
public:
    explicit ReadThread(QObject *parent = nullptr);

    ~ReadThread();
signals:

public slots:

private:
    void run() override;

};

#endif // ReadThread_H
