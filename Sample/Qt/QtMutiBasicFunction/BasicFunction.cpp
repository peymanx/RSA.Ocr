#include "BasicFunction.h"
#include "ImageAcquisition.h"
#include "ui_BasicFunction.h"
#include <QFileDialog>
#include <QDate>
#include "DVPCamera.h"


#define CAMERAS  4

QBasicFunction::QBasicFunction(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QBasicFunction)
{
    ui->setupUi(this);

    /* ch:成员变量初始化 | en: Member variable initialization*/ 
	for (int i = 0; i < CAMERAS; i++)
	{
		m_handle[i] = 0;
		scene[i] = NULL;
		loadedPixmapItem[i] = NULL;
		m_pThread[i] = NULL;
		m_AcquireImage[i] = NULL;
	}
	    

    SoftTriggerFlag = false;

    /* ch:预先浏览相机设备 | en: Browse camera equipment in advance*/ 
    on_toolButton_Scan_clicked();

    /* ch:创建显示容器 | en: Create display container*/
	for (int i = 0; i < CAMERAS; i++)
        scene[i] = new QGraphicsScene();

    ui->graphicsView_PreView1->setScene(scene[0]);
	ui->graphicsView_PreView2->setScene(scene[1]);
	ui->graphicsView_PreView3->setScene(scene[2]);
	ui->graphicsView_PreView4->setScene(scene[3]);

	for (int i = 0; i < CAMERAS; i++)
        scene[i]->clear();
}

QBasicFunction::~QBasicFunction()
{
    delete ui;
}

bool QBasicFunction::IsValidHandle(dvpHandle handle)
{
    bool bValidHandle;
    dvpIsValid(handle, &bValidHandle);
    return bValidHandle;
}

//void QBasicFunction::IsMonoCamera(dvpHandle handle)
//{
    /* ch:判断相机是否为彩色相机 | en: Determine whether the camera is a color camera*/
//    dvpSensorInfo sSensorInfo;
//    dvpStatus status = dvpGetSensorInfo(handle,&sSensorInfo);
//    if (status == DVP_STATUS_OK)
//    {
//        if (SENSOR_PIXEL_MONO == sSensorInfo.pixel)
//            bMono = true;
//        else
//            bMono = false;
//    }
//    else if (status != DVP_STATUS_OK)
//    {
//        QMessageBox::about(NULL,"About","Get sensor information fail!");
//    }
//}

void QBasicFunction::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void QBasicFunction::closeEvent(QCloseEvent *)
{
	for (int i = 0; i < CAMERAS; i++)
	{
		if (IsValidHandle(m_handle[i]))
		{
			dvpGetStreamState(m_handle[i], &state);
			if (STATE_STARTED == state)
			{
				on_pushButton_Start_clicked();
			}

			dvpClose(m_handle[i]);
			m_handle[i] = 0;
		}
	}
    
}

void QBasicFunction::UpdateControls()
{
    dvpStatus status;

	bool bOpen = false;
	for (int i = 0; i < CAMERAS; i++)
	{
		if (IsValidHandle(m_handle[i]))
		{
        	/* ch:此时已经打开了一个设备,更新基本功能控件 | en: At this time, a device has been opened and the basic function controls are updated.*/ 
			dvpStreamState state;
			status = dvpGetStreamState(m_handle[i], &state);
			if (status != DVP_STATUS_OK)
			{
				QMessageBox::about(NULL, "About", "Get count fail!");
			}

        /* ch:更新基本控件 | en: Update basic controls*/ 
			ui->pushButton_Start->setText(state == STATE_STARTED ? (tr("Stop")) : (tr("Start")));
			ui->pushButton_Open->setText("Close");
			ui->pushButton_Start->setEnabled(true);

			bOpen = true;
			break;
		}
	}
    

	if (!bOpen)
	{
        /* ch: 此时设备还没有被打开,更新基本功能控件 | en: At this time, the device has not been turned on. Update the basic function controls. */ 
		ui->pushButton_Open->setText("Open");
		ui->pushButton_Start->setText("Start");
		ui->pushButton_Start->setEnabled(false);

		if (ui->comboBox_Devices->count() == 0)
		{
			ui->pushButton_Open->setEnabled(false);
		}
		else
		{
			ui->pushButton_Open->setEnabled(true);
		}


		ui->label_fps->clear();
	}
}




void QBasicFunction::slotDispRate()
{
    dvpStatus status;

    //if (IsValidHandle(m_handle))
    //{
        /* ch: 更新帧率信息 | en: Update frame rate information */ 
    //    status = dvpGetFrameCount(m_handle, &FrameCount);
    //    if (status != DVP_STATUS_OK)
    //    {
    //        QMessageBox::about(NULL,"About","Get frame count fail!");
    //    }

    //    strFrameCount = QString::number(FrameCount.uFrameCount);
    //    strFrameRate = QString::number(FrameCount.fFrameRate);
    //    strFrameInfo = QString::fromLocal8Bit("采集帧数 ") +strFrameCount + QString::fromLocal8Bit(" 采集帧率 ") + strFrameRate +
    //            QString::fromLocal8Bit(" fps 显示帧率 ")+QString::number(m_DisplayCount-m_DisplayCountBackUp)+QString::fromLocal8Bit(" fps");
    //    m_DisplayCountBackUp=m_DisplayCount;
    //    //QWidget::setWindowTitle(strFrameInfo);
    //    ui->label_fps->setText(strFrameInfo);
    //    qDebug()<<strFrameInfo;
    //}
}

void QBasicFunction::slotDispImage(int i)
{
    if (m_AcquireImage[i]!= NULL)
    {
		if (!m_AcquireImage[i]->m_ShowImage.isNull() && m_AcquireImage[i]->m_threadMutex.tryLock()){
			loadedPixmapItem[i]->loadImage(m_AcquireImage[i]->m_ShowImage);
			m_AcquireImage[i]->m_threadMutex.unlock();
        }else{
            return;
        }

        int nwith = ui->graphicsView_PreView1->width(),nheight = ui->graphicsView_PreView1->height();
        /* ch: 将界面控件Graphics View的width和height传进类m_Image中 | 
            en: Pass the width and height of the interface control Graphics View into the class m_Image */
		loadedPixmapItem[i]->setQGraphicsViewWH(nwith, nheight);
        /* ch: 使视窗的大小固定在原始大小，不会随图片的放大而放大（默认状态下图片放大的时候视窗两边会自动出现滚动条，并且视窗内的视野会变大）
            en: Keep the size of the window fixed at the original size and not enlarge as the picture is enlarged 
            (by default, when the picture is enlarged, scroll bars will automatically appear on both sides of the window, 
            and the field of view in the window will become larger),prevent the window from being too large and inconvenient to 
            observe the image when the image is enlarged and then reduced again.*/
        ui->graphicsView_PreView1->setSceneRect(QRectF(-(nwith/2),-(nheight/2),nwith,nheight));
        if(m_DisplayCount>=INT_MAX-1)
        {
            m_DisplayCount=0;
            m_DisplayCountBackUp=0;
        }
        m_DisplayCount++;
    }
}

void QBasicFunction::on_pushButton_Open_clicked()
{
     dvpStatus status;
     QString strName;
     dvpStreamState state;

	 bool bOpen = false;

	 for (int i = 0; i < CAMERAS; i++)
	 {
		 if (IsValidHandle(m_handle[i]))
		 {
			 bOpen = true;
		 }
	 }

	 if (!bOpen)
	 {
		 QStringList strNames = ui->comboBox_Devices->currentText();

		 if (!strNames.isEmpty())
		 {
			 for (int i = 0; i < strNames.length(); i++)
			 {
				 status = dvpOpenByName(strNames[i].toLatin1().data(), OPEN_NORMAL, &m_handle[i]);
				 if (status != DVP_STATUS_OK)
				 {
					 QMessageBox::about(NULL, "About", "Open the camera fail!");
				 }
				 else
				 {
					 loadedPixmapItem[i] = new MyGraphicsItem();
					 scene[i]->addItem(loadedPixmapItem[i]);
				 }

#ifdef           _WIN32
                 _sleep(200);
#else
                 usleep(200*1000);
#endif

				 status = dvpGetStreamState(m_handle[i], &state);
				 if (status != DVP_STATUS_OK)
				 {
					 QMessageBox::about(NULL, "About", "Get the stream state fail!");
				 }

				 if (state == STATE_STARTED)
				 {
					 status = dvpStop(m_handle[i]);
				 }

			 }

		 }
	 }
	 else
	 {		 
		 for (int i = 0; i < CAMERAS; i++)
		 {
			 if (IsValidHandle(m_handle[i]))
			 {
				 status = dvpGetStreamState(m_handle[i], &state);
				 if (state == STATE_STARTED)
				 {
					 status = dvpStop(m_handle[i]);

					
					 m_pThread[i]->requestInterruption();
					 m_pThread[i]->quit();
					 m_pThread[i]->wait();
					 /* ch: 删除线程对象 | en: Deleting thread object */
					 if (m_AcquireImage[i] != NULL)
					 {
						 delete m_AcquireImage[i];
						 m_AcquireImage[i] = NULL;
					 }

					 if (m_pThread[i] != NULL)
					 {
						 delete m_pThread[i];
						 m_pThread[i] = NULL;
					 }
				 }

				 status = dvpClose(m_handle[i]);
				 scene[i]->removeItem(loadedPixmapItem[i]);
				 delete loadedPixmapItem[i];
				 loadedPixmapItem[i] = NULL;
				 m_handle[i] = 0;
			 }	 
		 }	 
	 }
     

     UpdateControls();
}

void QBasicFunction::on_pushButton_Start_clicked()
{
    dvpStatus status;
    dvpStreamState state;
    bool bTrigStatus;


	for (int i = 0; i < CAMERAS; i++)
	{
		if (IsValidHandle(m_handle[i]))
		{
			status = dvpGetStreamState(m_handle[i], &state);
			if (status != DVP_STATUS_OK)
			{
				QMessageBox::about(NULL, "About", "Get the stream state fail!");
			}

			if (state == STATE_STOPED)
			{
				status = dvpGetTriggerState(m_handle[i], &bTrigStatus);
				if (status != DVP_STATUS_FUNCTION_INVALID)
				{
                /* ch: 在启动视频流之前先设置为触发模式 | en: Set to trigger mode before starting video stream */ 
					status = dvpSetTriggerState(m_handle[i], SoftTriggerFlag ? true : false);
					if (status != DVP_STATUS_OK)
					{
						QMessageBox::about(NULL, "About", "Set status of trigger fail!");
					}
				}
				else
				{
					//ui->groupBox_trigger->setEnabled(false);
				}

				status = dvpStart(m_handle[i]);
				if (status != DVP_STATUS_OK)
				{
					QMessageBox::about(NULL, "About", "Start the video stream fail!");
				}
				else
				{
					m_AcquireImage[i] = new QImageAcquisition(m_handle[i],i);
					m_pThread[i] = new QThread();
					m_AcquireImage[i]->moveToThread(m_pThread[i]);
					m_pThread[i]->start();


                /* ch: 建立图像显示信号和槽函数的联系 | en: Establish a connection between image display signals and slot functions */ 
					connect(m_AcquireImage[i], SIGNAL(signalDisplay(int)), this, SLOT(slotDispImage(int)));
				}
			}
			else
			{
				status = dvpStop(m_handle[i]);



				m_pThread[i]->requestInterruption();
				m_pThread[i]->quit();
				m_pThread[i]->wait();
				/* ch: 删除线程对象 | en: Deleting thread object */
				if (m_AcquireImage != NULL)
				{
					delete m_AcquireImage[i];
					m_AcquireImage[i] = NULL;
				}

				if (m_pThread[i] != NULL)
				{
					delete m_pThread[i];
					m_pThread[i] = NULL;
				}

				//ui->graphicsView_PreView->scene()->clear();
			}
		}
	}
    

    UpdateControls();
}


void QBasicFunction::on_toolButton_Scan_clicked()
{
    dvpStatus status;
    dvpUint32 i,n = 0;
    dvpCameraInfo info[16];

    /* ch: 此时，n为成功枚举到的相机数量，将添加到下拉列表中，下拉列表中的内容为每个相机的FriendlyName |
     en: At this time, n is the number of successfully enumerated cameras, which will be added to the drop-down list. 
     The content in the drop-down list is the FriendlyName of each camera. */ 
     ui->comboBox_Devices->clear();

    /* ch: 获得当前能连接的相机数量 | en: Get the number of currently connected cameras */ 
    status = dvpRefresh(&n);
    if (status != DVP_STATUS_OK)
    {
        QMessageBox::about(NULL,"About","Refresh fail!");
    }

    if (status == DVP_STATUS_OK)
    {
        /* ch: 枚举最多16台相机的信息 | en: Enumerate information for up to 16 cameras */ 
        if (n > 16)
        {
            n = 16;
        }

        for (i = 0; i < n; i++)
        {
            /* ch: 逐个枚举出每个相机的信息 | en: Enumerate information about each camera one by one */ 
            status = dvpEnum(i, &info[i]);
            if (status != DVP_STATUS_OK)
            {
                QMessageBox::about(NULL,"About","Enumerate fail!");
            }
            else
            {
                 ui->comboBox_Devices->addItem(tr(info[i].FriendlyName));
            }
        }
        if (i != 0)
        {
            ui->comboBox_Devices->setCurrentIndex(0);
        }

        UpdateControls();
    }
}
