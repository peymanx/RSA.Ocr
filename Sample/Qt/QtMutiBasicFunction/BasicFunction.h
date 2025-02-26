#ifndef BASICFUNCTION_H
#define BASICFUNCTION_H

#include <QWidget>
#include <QTimer>
#include <QMutex>
#include <QWaitCondition>
#include <QTime>
#include <QtGlobal>
#include <QThread>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include "ImageAcquisition.h"
#include "MyGraphicsitem.h"
#include "customcombobox.h"


namespace Ui {
class QBasicFunction;
}

class QBasicFunction : public QWidget
{
    Q_OBJECT

public:
    explicit QBasicFunction(QWidget *parent = 0);
    ~QBasicFunction();

    bool IsValidHandle(dvpHandle handle);
    void IsMonoCamera(dvpHandle handle);
    void InitSpinExpoTime();
    void InitSpinGain();
    void InitAEMode();
    void InitAETarget();
    void InitTrigger();
    void InitROIMode();
    void InitColorSolution();
    void InitTargetFormat();
    void UpdateControls();

protected:
    void changeEvent(QEvent *e);
protected:
    virtual void closeEvent(QCloseEvent *);

private slots:
    void on_pushButton_Open_clicked();
    void on_pushButton_Start_clicked();
    
    
    void on_toolButton_Scan_clicked();
    
   
   

    /* ch: 自定义的槽函数 | en: Custom slot function */  
    void slotDispRate();
    void slotDispImage(int i);

private:
    Ui::QBasicFunction *ui;

    bool                SoftTriggerFlag;
    QString             strFrameCount;              /* ch: 相机获取到的图像帧数 | en: Number of image frames acquired by the camera */  
    QString             strFrameRate;               /* ch: 图像获取帧率 | en: Image acquisition frame rate */  
    QString             strFrameInfo;
    QString             strFilePath;                /* ch: 保存图像时用到的完整路径 | en: The full path to use when saving the image */  
    QString             strDir;                     /* ch: 图像保存的路径 | en: The path where the image is saved */  
    QString             strLoadPath;
	dvpHandle           m_handle[4];
    dvpStreamState      state;
    dvpFrameCount       FrameCount;

    QThread             *m_pThread[4];
              
    QImageAcquisition   *m_AcquireImage[4];         /* ch: 图像采集线程的对象指针 | en: Object pointer to the image acquisition thread */    
    unsigned int        m_DisplayCountBackUp=0;
    unsigned int        m_DisplayCount=0;

    QGraphicsScene      *scene[4];                      /* ch: QGraphicsScene，在graphicsview高效显示图片 | en: QGraphicsScene, efficiently display images in graphicsview */ 
    MyGraphicsItem      *loadedPixmapItem[4];           /* ch: 自定义MyGraphicsItem | en: CustomizeMyGraphicsItem */ 
signals:
//    void                TriggerSignal(bool triFlags);

};

#endif // BASICFUNCTION_H
