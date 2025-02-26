#include "MyGraphicsitem.h"

MyGraphicsItem::MyGraphicsItem()
{
    setAcceptDrops(true);   /* ch: 接受拖拽 |en: accept drag */
    m_scaleValue = 0;
    m_scaleDafault = 0;
    m_isMove = false;
}

void MyGraphicsItem::loadImage(QImage image)
{
    m_pix = image;
    setScale(m_scaleValue);
}

QRectF MyGraphicsItem::boundingRect() const
{
    return QRectF(-m_pix.width() / 2, -m_pix.height() / 2, m_pix.width(), m_pix.height());
}

void MyGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawImage(-m_pix.width() / 2, -m_pix.height() / 2, m_pix);
}

void MyGraphicsItem::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    if((event->delta() > 0)&&(m_scaleValue >= 50))/* ch: 最大放大到原始图像的50倍 |en: Maximum magnification up to 50 times of original image */
    {
        return;
    }
    else if((event->delta() < 0)&&(m_scaleValue <= m_scaleDafault/2)) /* ch: 图像缩小到(自适应大小/2) 之后就不继续缩小 |en: The image is reduced to (adaptive size/2) and then does not continue to be reduced.*/
    {
        //ResetItemPos();   /* ch: 重置图片大小和位置，使之自适应控件窗口大小|en: Reset the size and position of the image so that it adapts to the size of the control window*/
    }
    else
    {
        qreal qrealOriginScale = m_scaleValue;
        /* ch: 鼠标滚轮向前滚动|en: Mouse wheel scrolls forward*/
        if(event->delta() > 0)
        {
            /* ch: 每次放大10%|en: Magnify 10% each time*/
            m_scaleValue*=1.1;
        }
        else
        {
            /* ch: 每次缩小10%|en: Shrink 10% each time*/
            m_scaleValue*=0.9;
        }
        setScale(m_scaleValue);
        if(event->delta() > 0)
        {
            /* ch: 使图片缩放的效果看起来像是以鼠标所在点为中心进行缩放的| en: Make the image zoom effect look like it is zoomed around the mouse point.*/
            moveBy(-event->pos().x()*qrealOriginScale*0.1, -event->pos().y()*qrealOriginScale*0.1);
        }
        else
        {
            /* ch: 使图片缩放的效果看起来像是以鼠标所在点为中心进行缩放的| en: Make the image zoom effect look like it is zoomed around the mouse point.*/
            moveBy(event->pos().x()*qrealOriginScale*0.1, event->pos().y()*qrealOriginScale*0.1);//
        }
    }
}

void MyGraphicsItem::ResetItemPos()
{
    /* ch: 缩放比例回到一开始的自适应比例| en: The scaling ratio returns to the initial adaptive ratio.*/
    m_scaleValue = m_scaleDafault;
    /* ch: 缩放到一开始的自适应大小| en: Scale to initial adaptive size*/
   setScale(m_scaleDafault);
   //setPos(0,0);
}

void MyGraphicsItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()== Qt::LeftButton)
    {
        /* ch: 鼠标左击时，获取当前鼠标在图片中的坐标，| en: When the mouse is left-clicked, get the coordinates of the current mouse in the picture.*/
        m_startPos = event->pos();
        /* ch: 标记鼠标左键被按下| en: Mark left mouse button pressed*/
        m_isMove = true;
    }
    else if(event->button() == Qt::RightButton)
    {
        /* ch: 右击鼠标重置大小| en: Right click mouse to reset size*/
        ResetItemPos();
    }
}

void MyGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_isMove)
    {
        QPointF point = (event->pos() - m_startPos)*m_scaleValue;
        moveBy(point.x(), point.y());
    }
}

void MyGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
    m_isMove = false;/* ch: 标记鼠标左键已经抬起| en: Mark the left mouse button has been raised*/
}

qreal MyGraphicsItem::getScaleValue() const
{
    return m_scaleValue;
}

void MyGraphicsItem::setQGraphicsViewWH(int nwidth, int nheight)
{
    if(m_bFlag==false)
    {
        m_bFlag = true;
    }
    else
    {
        return ;
    }
    int nImgWidth = m_pix.width();
    int nImgHeight = m_pix.height();
    qreal temp1 = nwidth*1.0/nImgWidth;
    qreal temp2 = nheight*1.0/nImgHeight;

    m_scaleDafault = temp1>temp2?temp2:temp1;

    setScale(m_scaleDafault);
    m_scaleValue = m_scaleDafault;
}
