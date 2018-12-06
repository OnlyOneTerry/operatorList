#ifndef WriteThread_H
#define WriteThread_H

#include "BaseThread.h"


class WriteThread : public BaseThread
{
    Q_OBJECT
public:
    explicit WriteThread(QObject *parent = nullptr);

    ~WriteThread();
signals:

public slots:

private:
    void run() override;
};

#endif // WriteThread_H
