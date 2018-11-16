#ifndef SCBaseGraphicsView_H
#define SCBaseGraphicsView_H

#include <QGraphicsView>
#include <QWidget>
#include <QDebug>


class  SCBaseGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    SCBaseGraphicsView(QWidget *parent=NULL);
    Q_PROPERTY(QColor backgroundColor READ getBackgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QColor lineBorderColor READ getLineBorderColor WRITE setLineBorderColor)
    Q_PROPERTY(QColor textColor READ getTextColor WRITE setTextColor)
public:
    enum ReferLine      //参考线
    {
        NO=0,
        Hline,
        Vline,
        V_H_line
    };

    enum CursorType
    {
        NONE = 0,
        SplitHCursor,
        SplitVCursor,
        CrossCursor
    };

    qreal scale_history_{1.0};//缩放记录

public:
    void wheelEvent(QWheelEvent *event);
    void paintEvent(QPaintEvent *event);

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
public:
    void set_scale_size(qreal scale_size);
    QCursor get_viewport_cursor() const;

    //Q_PROPERTY
    void setBackgroundColor(const QColor &value);
    void setLineBorderColor(const QColor &value);
    void setTextColor(const QColor &value);
    QColor getTextColor() const;
    QColor getLineBorderColor() const;
    QColor getBackgroundColor() const;
private:
    QFont rule_font_;
    const int k_rule_width_=25;
    ReferLine referLine=NO;
    CursorType cursorType_;
    QList<QPointF> refer_hline_list_;
    QList<QPointF> refer_vline_list_;
    QList<QPointF> crossPoint_list_ ;
    bool is_operate_refer_line_;
    QPointF pressed_refer_pos_;
    QPoint current_mouse_screen_pos_;
    QPointF crossPos_;
    QList<QPointF > rule_pos_list_;
    bool mouseIsPressed_ = false;//鼠标被按下
    bool crossFlag = false;//当前光标是否为十字
    bool isNeedAddLine = false;//判断是否需要添加线
    //---
    QColor backgroundColor = QColor(240,240,240);   //屏幕尺背景色
    QColor lineBorderColor = QColor(120,120,120);                 //屏幕尺边界线条颜色
    QColor textColor = QColor(60,60,60);                        //字体颜色
};

#endif // SCBaseGraphicsView_H
