#ifndef MYGRAPHICSITEM_H
#define MYGRAPHICSITEM_H



#include <QWidget>
#include <QtGui>
#include <QPixmap>
#include <QPainter>
#include <QRectF>
#include <QMouseEvent>
#include <QPointF>
#include <QDragEnterEvent>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsItem>

class MyGraphicsItem : public QGraphicsItem
{
        //Q_OBJECT
public:
    MyGraphicsItem();
    //void    loadPixmap(QPixmap *pixmap);
    void    loadImage(QImage pixmap);
    QRectF  boundingRect() const;
    void    paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void    wheelEvent(QGraphicsSceneWheelEvent *event);
    void    ResetItemPos();
    void    mousePressEvent(QGraphicsSceneMouseEvent *event);
    void    mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void    mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    qreal   getScaleValue() const;
    void    setQGraphicsViewWH(int nwidth,int nheight);
private:
    qreal       m_scaleValue;
    qreal       m_scaleDafault;
    //QPixmap     m_pix;
    QImage      m_pix;
    int         m_zoomState;
    bool        m_isMove;
    QPointF     m_startPos;

    bool        m_bFlag = false;
};

#endif // MYGRAPHICSITEM_H
