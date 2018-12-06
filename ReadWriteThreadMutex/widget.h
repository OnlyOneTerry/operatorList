#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include "ReadThread.h"
#include "WriteThread.h"

#include <QtWinExtras>

namespace Ui {
class Widget;
}


class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    void createThumbnailToolBar();
    void createJumpList();
    void createTaskbar();
    void initShow();
    void stylize();
    bool event(QEvent *event);
public slots:
    void slotTimeOut();
private:
    Ui::Widget *ui;
    ReadThread *_pReadThread = Q_NULLPTR;
    WriteThread *_pWriteThread = Q_NULLPTR;

    QWinTaskbarButton *taskbarButton = nullptr;
    QWinTaskbarProgress *taskbarProgress = nullptr;
    QWinThumbnailToolBar *thumbnailToolBar = nullptr;
    QWinThumbnailToolButton *playToolButton = nullptr;
    QWinThumbnailToolButton *forwardToolButton = nullptr;
    QWinThumbnailToolButton *backwardToolButton = nullptr;
    QTimer *pTimer = Q_NULLPTR;
};

#endif // WIDGET_H
