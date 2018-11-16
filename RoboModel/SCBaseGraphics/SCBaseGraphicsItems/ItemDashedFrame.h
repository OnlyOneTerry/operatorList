#ifndef ITEMDASHEDFRAME_H
#define ITEMDASHEDFRAME_H

#include <QGraphicsItem>
#include <QPainter>
#include <QObject>


class  ItemDashedFrame :public QObject, public QGraphicsItem
{
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    enum { Type = UserType + 80000 };
    int type() const Q_DECL_OVERRIDE { return Type; }

public:
    ItemDashedFrame(QRectF rect=QRectF(),QGraphicsItem *parent = Q_NULLPTR);
    ~ItemDashedFrame();

    void set_rect(const QRectF& rectf);                         //scene
    void set_pos_list(const QList<QPointF>& pos_list); //scene
    QList<QPointF> get_pos_list();
    void clear_pos();

protected:
    QPainterPath shape() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    QRectF boundingRect() const;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);

private:
    QColor color_brush_{Qt::cyan};
    QPolygonF poly_dash_;
    QPen pen_{QPen(Qt::blue,1,Qt::PenStyle::DashLine, Qt::FlatCap, Qt::SvgMiterJoin)};
    QList<QPointF> pos_list_;

    QPointF press_pos_;
    qreal current_angle_{0.};

signals:
    void sig_select_release();
    void sig_copy();
};

#endif // ITEMDASHEDFRAME_H
