/*****************************************************************************
* @FileName:Dvp2EventCallback.cpp
* @CreatTime: 2020/9/24 9:53
* @Version: ver 1.0
*****************************************************************************/

#include <iostream>
#include <thread>
using namespace std;

// DVP API Depends
#include "../../../library/Visual C++/include/DVPCamera.h"
#ifdef _M_X64
#pragma comment(lib, "../../../library/Visual C++/lib/x64/DVPCamera64.lib")
#else				  
#pragma comment(lib, "../../../library/Visual C++/lib/x86/DVPCamera.lib")
#endif

#pragma warning(disable:4996)

#define GRABCOUNT 2000		


static dvpInt32 OnEventStreamStarted(dvpHandle handle, dvpEvent event, void* pContext, dvpInt32 param, struct dvpVariant* pVariant)
{
	
	printf("OnEventStreamStarted \n");
	return 0;
}


static dvpInt32 OnEventStreamStoped(dvpHandle handle, dvpEvent event, void* pContext, dvpInt32 param, struct dvpVariant* pVariant)
{
	printf("OnEventStreamStoped \n");
	return 0;
}


static dvpInt32 OnEventFrameLost(dvpHandle handle, dvpEvent event, void* pContext, dvpInt32 param, struct dvpVariant* pVariant)
{
	printf("OnEventFrameLost \n");
	return 0;
}


static dvpInt32 OnEventFrameTimeout(dvpHandle handle, dvpEvent event, void* pContext, dvpInt32 param, struct dvpVariant* pVariant)
{
	printf("OnEventFrameTimeout \n");
	return 0;
}


static dvpInt32 OnEventReconnected(dvpHandle handle, dvpEvent event, void* pContext, dvpInt32 param, struct dvpVariant* pVariant)
{
	printf("OnEventReconnected \n");
	return 0;
}


static dvpInt32 OnEventFrameStart(dvpHandle handle, dvpEvent event, void* pContext, dvpInt32 param, struct dvpVariant* pVariant)
{
	printf("OnEventFrameStart \n");
	return 0;
}


static dvpInt32 OnEventFrameEnd(dvpHandle handle, dvpEvent event, void* pContext, dvpInt32 param, struct dvpVariant* pVariant)
{
	printf("OnEventFrameEnd \n");
	return 0;
}

void test(void* p)
{
	dvpStatus status;
	dvpHandle h;
	bool trigMode = false;

	char* name = (char*)p;

	printf("Test start,camera is %s\r\n", name);
	do
	{
		status = dvpOpenByName(name, OPEN_NORMAL, &h);
		if (status != DVP_STATUS_OK)
		{
			printf("dvpOpenByName failed with err:%d\r\n", status);
			break;
		}

		status = dvpSetTriggerState(h, false);
		if (status == DVP_STATUS_OK)
		{
			trigMode = false;
		}
		else
		{
			printf("dvpSetTriggerState failed with err:%d\r\n", status);
			break;
		}

		status = dvpRegisterEventCallback(h, OnEventStreamStarted, EVENT_STREAM_STARTRD, NULL);
		if (status != DVP_STATUS_OK)
		{
			printf("dvpRegisterEventCallback OnEventStreamStarted failed with err:%d\r\n", status);
			break;
		}
		status = dvpRegisterEventCallback(h, OnEventStreamStoped, EVENT_STREAM_STOPPED, NULL);
		if (status != DVP_STATUS_OK)
		{
			printf("dvpRegisterEventCallback OnEventStreamStoped failed with err:%d\r\n", status);
			break;
		}
		status = dvpRegisterEventCallback(h, OnEventFrameLost, EVENT_FRAME_LOST, NULL);
		if (status != DVP_STATUS_OK)
		{
			printf("dvpRegisterEventCallback OnEventFrameLost failed with err:%d\r\n", status);
			break;
		}
		status = dvpRegisterEventCallback(h, OnEventFrameTimeout, EVENT_FRAME_TIMEOUT, NULL);
		if (status != DVP_STATUS_OK)
		{
			printf("dvpRegisterEventCallback OnEventFrameTimeout failed with err:%d\r\n", status);
			break;
		}
		status = dvpRegisterEventCallback(h, OnEventReconnected, EVENT_RECONNECTED, NULL);
		if (status != DVP_STATUS_OK)
		{
			printf("dvpRegisterEventCallback OnEventReconnected failed with err:%d\r\n", status);
			break;
		}
		status = dvpRegisterEventCallback(h, OnEventFrameStart, EVENT_FRAME_START, NULL);
		if (status != DVP_STATUS_OK)
		{
			printf("dvpRegisterEventCallback OnEventFrameStart failed with err:%d\r\n", status);
			break;
		}
		status = dvpRegisterEventCallback(h, OnEventFrameEnd, EVENT_FRAME_END, NULL);
		if (status != DVP_STATUS_OK)
		{
			printf("dvpRegisterEventCallback OnEventFrameEnd failed with err:%d\r\n", status);
			break;
		}

		dvpFrame frame;

		void* p;

		status = dvpStart(h);
		if (status != DVP_STATUS_OK)
		{
			break;
		}

		for (int j = 0; j < GRABCOUNT; j++)
		{
			status = dvpGetFrame(h, &frame, &p, 3000);
			if (status != DVP_STATUS_OK)
			{
				if (trigMode)
				{
					printf("Fail to get a frame in trigger mode \r\n");
					continue;
				}
				else
				{
					printf("Fail to get a frame in continuous mode \r\n");
					continue;
				}
			}

			dvpFrameCount framecount;
			status = dvpGetFrameCount(h, &framecount);
			if (status != DVP_STATUS_OK)
			{
				printf("get framecount failed\n");
			}
			printf("framecount: %d, framerate: %f\n", framecount.uFrameCount, framecount.fFrameRate);

			printf("%s, frame:%lld, timestamp:%lld, %d*%d, %dbytes, format:%d\r\n",
				name,
				frame.uFrameID,
				frame.uTimestamp,
				frame.iWidth,
				frame.iHeight,
				frame.uBytes,
				frame.format);
		}

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