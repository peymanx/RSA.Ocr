#include "ImageAcquisition.h"
#include "DVPCamera.h"

QImageAcquisition::QImageAcquisition(dvpHandle &handle, int index, QObject *parent) :
    QObject(parent)
{
    m_handle = handle;
    pBuffer = NULL;

    m_timer = new QTimer();

    /* ch: 将定时器超时信号与槽(功能函数)联系起来 | en: Connect the timer timeout signal to the slot (functional function) */ 
    connect(m_timer, SIGNAL(timeout()), this, SLOT(slotGrabFrames()));

    /* ch: 定时器每10毫秒采集一次 | en: The timer collects data every 10 milliseconds.*/
    m_timer->start(10);

	m_nIndex = index;
    //qDebug()<< __FUNCTION__ << " id = "<< QThread::currentThreadId();
}

QImageAcquisition::~QImageAcquisition()
{
    if(m_timer->isActive())
    {
        m_timer->stop();
        delete m_timer;
    }
}

bool QImageAcquisition::IsValidHandle(dvpHandle handle)
{
    bool bValidHandle = false;
    dvpIsValid(handle, &bValidHandle);

    return bValidHandle;
}

void QImageAcquisition::slotGrabFrames()
{
    dvpStatus status;
    status = dvpGetFrame(m_handle, &m_pFrame, &pBuffer, GRABTIMEOUT);

    if (status == DVP_STATUS_OK)
    {
        if(m_threadMutex.tryLock())
		{
            if(m_pFrame.format==FORMAT_BGR24)
			{
                /* ch: 其他版本先把BGR数据转成RGB数据，再用RGB数据转QImage |
                    en: Other versions first convert BGR data into RGB data, and then use the RGB data to convert into QImage*/
                m_ShowImage = QImage((uchar*)pBuffer,m_pFrame.iWidth, m_pFrame.iHeight, QImage::Format_RGB888); //5.7
                m_ShowImage = m_ShowImage.rgbSwapped();
            }
			else
			{
                m_ShowImage = QImage((uchar*)pBuffer,m_pFrame.iWidth, m_pFrame.iHeight,QImage::Format_Grayscale8); //5.7
            }
            m_threadMutex.unlock();
        }

		emit signalDisplay(m_nIndex);
    }
}

