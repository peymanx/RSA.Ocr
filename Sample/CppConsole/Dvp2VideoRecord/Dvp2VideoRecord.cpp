/*****************************************************************************
* @FileName:Dvp2VideoRecord.cpp
* @CreatTime: 2020/9/25 15:31
* @Version: ver 1.0
*****************************************************************************/

#include <iostream>
#include <conio.h>
#include <thread>
using namespace std;

#define FILENAME "video.wmv"

#include "../../../library/Visual C++/include/DVPCamera.h"
#ifdef _M_X64
#pragma comment(lib, "../../../library/Visual C++/lib/x64/DVPCamera64.lib")
#else				  
#pragma comment(lib, "../../../library/Visual C++/lib/x86/DVPCamera.lib")
#endif

#pragma warning(disable:4996)

static dvpRecordHandle videohandle;	

static dvpInt32 OnVideoRecordWrite(dvpHandle handle, dvpStreamEvent event, void* pContext, dvpFrame* pFrame, void* pBuffer)
{
	dvpStatus status;

	printf("frame:%lld, timestamp:%lld, %d*%d, %dbytes, format:%d\r",
		pFrame->uFrameID, pFrame->uTimestamp, pFrame->iWidth,
		pFrame->iHeight, pFrame->uBytes, pFrame->format);

	bool isVideoRecorderValid;
	status = dvpIsVideoRecorderValid(videohandle, &isVideoRecorderValid);
	if (status != DVP_STATUS_OK)
	{
		printf("dvpIsVideoRecorderValid failed with err:%d\r\n", status);
	}

	if (isVideoRecorderValid)
	{
		status = dvpVideoRecordWrite(videohandle, pFrame, pBuffer);
		if (status != DVP_STATUS_OK)
		{
			printf("dvpVideoRecordWrite failed with err:%d\r\n", status);
		}
	}
	else {
		dvpStartVideoRecord(FILENAME, 0, 0, 100, &videohandle);
	}


	return 0;
}

void test(void* p)
{
	dvpStatus status;
	dvpHandle h;

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

		dvpRegion region;

		status = dvpSetTriggerState(h, false);
		if (status != DVP_STATUS_OK)
		{
			printf("dvpSetTriggerState failed with err:%d\r\n", status);
			break;
		}

		status = dvpGetRoi(h, &region);
		if (status != DVP_STATUS_OK)
		{
			printf("dvpGetRoi failed with err:%d\r\n", status);
			break;
		}
		printf("%s, region: x:%d, y:%d, w:%d, h:%d\r\n", name, region.X, region.Y, region.W, region.H);

		status = dvpRegisterStreamCallback(h, OnVideoRecordWrite, STREAM_EVENT_PROCESSED, NULL);
		if (status != DVP_STATUS_OK)
		{
			printf("dvpRegisterStreamCallback failed with err:%d\r\n", status);
			break;
		}

		status = dvpStart(h);
		if (status != DVP_STATUS_OK)
		{
			break;
		}

		status = dvpStartVideoRecord(FILENAME, 0, 0, 100, &videohandle);
		if (status != DVP_STATUS_OK)
		{
			printf("dvpStartVideoRecord failed with err:%d\r\n", status);
			break;
		}

		int key;
		printf("\nPress ESC to exit!\n");
		while (1)
		{
			key = getch();
			if (key == 27)
			{
				status = dvpStopVideoRecord(videohandle);
				if (status != DVP_STATUS_OK)
				{
					break;
				}

				status = dvpStop(h);
				if (status != DVP_STATUS_OK)
				{
					break;
				}
				break;
			}
		}
	} while (0);

	status = dvpClose(h);

	printf("\ntest quit, %s, status:%d\n", name, status);
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