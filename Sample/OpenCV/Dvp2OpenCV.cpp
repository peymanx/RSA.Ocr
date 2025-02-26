/*****************************************************************************
* @FileName:dvp2_opencv.cpp
* @Author:
* @Mail:
* @CreatTime: 2020/8/24 14:42
* @Descriptions: ch:编译前请配置好opencv、dvp2 api有关的库路径，避免编译报错；
* @Descriptions: en:Before compiling, please configure the path of libraries related to opencv and dvp2 api to avoid compilation errors;
* @Version: ver 1.0
*****************************************************************************/
#include <iostream>
#include <thread>
using namespace std;

#include "../../library/Visual C++/include/DVPCamera.h"
#ifdef _M_X64
#pragma comment(lib, "../../library/Visual C++/lib/x64/DVPCamera64.lib")
#else				  
#pragma comment(lib, "../../library/Visual C++/lib/x86/DVPCamera.lib")
#endif

/********************************chinese*******************************
* OpenCV依赖一共需要配置4处，内容如下：
* 1.项目属性->VC++目录->包含目录中加入$(ProjectDir)OpenCV-3.49\include;$(ProjectDir)OpenCV-3.49\include\opencv2
* 2.项目属性->VC++目录->库目录中加入$(ProjectDir)OpenCV-3.49\lib\x86\vc15
* 3.项目属性->链接器->输入->附件依赖项中加入opencv_world349d.lib或者opencv_world349.lib。
* 4.项目属性->调试->环境，
*		- 编译64位添加：path=%path%;$(ProjectDir)OpenCV-3.49\bin\x64\vc15，或者直接把dll库拷贝到生成的exe程序目录
***********************************************************************/

/********************************english*******************************
* The OpenCV dependencies need to be configured in a total of 4 places, as follows:
* 1.Project Properties->VC++ directory->Include directory add $(ProjectDir)OpenCV-3.49\include;$(ProjectDir)OpenCV-3.49\include\opencv2
* 2.Project Properties->VC++ directory->Library directory add $(ProjectDir)OpenCV-3.49\lib\x86\vc15
* 3.Project Properties->Linker->Input->Attachment dependencies add opencv_world349d.lib or opencv_world349.lib.
* 4.Project Properties->Debugging->Environment.
* - Compile 64-bit add: path=%path%;$(ProjectDir)OpenCV-3.49\bin\x64\vc15, or directly copy the dll library to the generated exe program directory
***********************************************************************/


#include <opencv2/opencv.hpp>

#pragma warning(disable:4996)

#define GRABCOUNT 50

//RGB to BGR
bool RGB2BGR(unsigned char* pRgbData, unsigned int nWidth, unsigned int nHeight)
{
	if (NULL == pRgbData)
	{
		return false;
	}

	for (unsigned int j = 0; j < nHeight; j++)
	{
		for (unsigned int i = 0; i < nWidth; i++)
		{
			unsigned char red = pRgbData[j * (nWidth * 3) + i * 3];
			pRgbData[j * (nWidth * 3) + i * 3] = pRgbData[j * (nWidth * 3) + i * 3 + 2];
			pRgbData[j * (nWidth * 3) + i * 3 + 2] = red;
		}
	}
	return true;
}

// ch:把图像buffer转成Mat格式 | en:Convert the image buffer to Mat format
bool Convert2Mat(dvpFrame* pFrameInfo, unsigned char* pData,cv::Mat& srcImage)
{
	if (pFrameInfo->format == FORMAT_MONO)
	{
		srcImage = cv::Mat(pFrameInfo->iHeight, pFrameInfo->iWidth, CV_8UC1, pData);
		printf("MONO convert to cv::Mat OK.\n");
	}
	else if (pFrameInfo->format == FORMAT_BGR24)
	{
		srcImage = cv::Mat(pFrameInfo->iHeight, pFrameInfo->iWidth, CV_8UC3, pData);
		printf("BGR24 convert to cv::Mat OK.\n");
	}
	else
	{
		printf("unsupported pixel format\n");
		return false;
	}

	if (NULL == srcImage.data)
	{
		printf("Invalid data!\n");
		return false;
	}

	/* ch:保存图片 | en:save image*/
	try {
		cv::imwrite("MatImage.bmp", srcImage);
	}
	catch (cv::Exception& ex) {
		fprintf(stderr, "Exception saving image to bmp format: %s\n", ex.what());
	}

	return true;
}

void test(void* p)
{
	dvpStatus status;
	dvpHandle h;
	char* name = (char*)p;

	printf("Test start,camera is %s\r\n", name);

	do
	{
		/* ch:打开设备 | en:open camera by device name */
		status = dvpOpenByName(name, OPEN_NORMAL, &h);
		if (status != DVP_STATUS_OK)
		{
			printf("dvpOpenByName failed with err:%d\r\n", status);
			break;
		}

		dvpFrame frame;
		void* pBuffer;

		/* ch:开始视频流 | en:start the camera stream */
		status = dvpStart(h);
		if (status != DVP_STATUS_OK)
		{
			break;
		}

		cv::Mat showImage;
		/* ch:获取帧 en:grab frame */
		for (int j = 0; j < GRABCOUNT; j++)
		{
			status = dvpGetFrame(h, &frame, &pBuffer, 3000);
			if (status != DVP_STATUS_OK)
			{
				printf("Fail to get a frame in continuous mode \r\n");
				break;
			}

			Convert2Mat(&frame, (unsigned char*)pBuffer, showImage);

			cv::namedWindow("ImageShow", 0);
			cv::resizeWindow("ImageShow", 640, 480);
			cv::imshow("ImageShow", showImage);
			/* ch:每张图片显示20ms | en:20 milliseconds per image display */
			cv::waitKey(20);	
		}

		/* ch:停止视频流 | en: stop the camera stream */
		status = dvpStop(h);
		if (status != DVP_STATUS_OK)
		{
			break;
		}
	} while (0);

	status = dvpClose(h);

	printf("test quit, %s, status:%d\r\n", name, status);
}


int main()
{
	printf("start...\r\n");

	dvpUint32 count = 0, num = -1;
	dvpCameraInfo info[8];

	/* ch:枚举相机 | en:Enumerate Cameras */
	dvpRefresh(&count);
	if (count > 8)
		count = 8;

	for (int i = 0; i < (int)count; i++)
	{
		if (dvpEnum(i, &info[i]) == DVP_STATUS_OK)
		{
			printf("[%d]-Camera FriendlyName : %s\r\n", i, info[i].FriendlyName);
		}
	}

	/* ch:没发现设备 | en:no camera found */
	if (count == 0)
	{
		printf("No device found!\n");
		return 0;
	}

	while (num < 0 || num >= count)
	{
		printf("Please enter the number of the camera you want to open: \r\n");
		scanf("%d", &num);
	}

	thread task(test, (void*)info[num].FriendlyName);
	task.join();

	system("pause");
	return 0;
}