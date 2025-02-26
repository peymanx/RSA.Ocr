//============================================================================================
//cn:
//用QT 4.8之后推荐的方式创建线程，只需要将一个继承于QObject的类通过moveToThread移到QThread的一个对象中
//需要注意的是：
//只有在槽中执行的操作才是在线程中执行的，所以需要通过连接信号槽的方式来实现
//如果object对象存在父对象，不能将其移到子线程中执行。
//en:
//To create a thread in the way recommended after QT 4.8, you only need to move a class inherited from QObject to an object of QThread through moveToThread.
//have to be aware of is:
//Only operations performed in the slot are performed in the thread, so it needs to be implemented by connecting to the signal slot.
//If the object object has a parent object, it cannot be moved to a child thread for execution.
//============================================================================================

#ifndef IMAGEACQUISITION_H
#define IMAGEACQUISITION_H

#include <QWaitCondition>
#include <QMessageBox>
#include <QThread>
#include <QMutex>
#include <QObject>
#include <QDebug>
#include <QTimer>
#include "DVPCamera.h"

#define GRABTIMEOUT   3000                /* ch: 图像获取的超时时间 ms | en: Image acquisition timeout ms */ 

class QImageAcquisition : public QObject
{
    Q_OBJECT
public:
     QImageAcquisition(dvpHandle &handle,int index,QObject *parent = nullptr);
    ~QImageAcquisition();
    bool IsValidHandle(dvpHandle handle);    /* ch: 判断句柄是否有效 | en: Determine whether the handle is valid */ 
    dvpHandle    m_handle;
    QImage       m_ShowImage;

    dvpFrame     m_pFrame;                   /* ch: 采集到的图像的结构体指针 | en: Structure pointer of the collected image */ 
    void *       pBuffer;                    /* ch: 采集到的图像的内存首地址 | en: The first memory address of the collected image */ 

    QMutex       m_threadMutex;              
    QTimer       *m_timer;

	int m_nIndex;
private slots:
    void         slotGrabFrames();                /* ch: 抓帧函数 | en: frame grab function */ 

signals:
    void         signalDisplay(int i);            /* ch: 显示图像信号 | en: display image signal */ 
};

#endif // IMAGEACQUISITION_H

