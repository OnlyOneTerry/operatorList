#ifndef ZoomIn_H
#define ZoomIn_H

#include "BaseWidget.h"

class ZoomIn : public BaseWidget
{
    Q_OBJECT

public:
    ZoomIn(QWidget *parent = 0);
    ~ZoomIn();
private:
    void showEvent(QShowEvent*e);
    bool eventFilter(QObject *o, QEvent *e);
    void mousePressEvent(QMouseEvent*e);
    void mouseMoveEvent(QMouseEvent*e);
    void mouseReleaseEvent(QMouseEvent*e);
    void paintEvent(QPaintEvent*);

public slots:
    void slotTriggered(QAction *ac);

signals:
    void sigZoomIn(int,const QString&);
    void sigShowMessage(const QString& msg,int type = 0);
protected:
    void resizeEvent(QResizeEvent *event);
protected slots:
    void slotMessage(const QString &msg, int type = 0);
private:
    QPoint m_pos;
    QPixmap m_pix;
    QPixmap m_tempPix;
    bool m_isDis;
    qreal m_O, m_width,m_height;
    int _shapeMode;//形状模式
};

#endif // ZoomIn_H
