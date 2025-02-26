/*****************************************************************************
* @FileName:dvp2getframe.cpp
* @CreatTime: 2020/8/19 11:58
* @Version: ver 1.0
*****************************************************************************/

#include <iostream>
#include <thread>
using namespace std;

#include "../../../library/Visual C++/include/DVPCamera.h"
#ifdef _M_X64
#pragma comment(lib, "../../../library/Visual C++/lib/x64/DVPCamera64.lib")
#else				  
#pragma comment(lib, "../../../library/Visual C++/lib/x86/DVPCamera.lib")
#endif

#pragma warning(disable:4996)

//#define TEST_TRIG			/* set trigger model */
//#define SOFT_TRIG			/* set software trigger */
#define GRABCOUNT 20		

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

		dvpRegion region;
		double exp;
		float gain;
#ifdef TEST_TRIG
		status = dvpSetTriggerState(h, true);
		if (status == DVP_STATUS_OK)
		{
			dvpSetTriggerSource(h, TRIGGER_SOURCE_SOFTWARE);
			dvpSetTriggerInputType(h, TRIGGER_POS_EDGE);
			dvpSetInputIoFunction(h, INPUT_IO_1, INPUT_FUNCTION_TRIGGER);
			trigMode = true;
		}
		else
		{
			printf("dvpSetTriggerState failed with err:%d\r\n", status);
			break;
		}
#else
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
#endif
		status = dvpGetRoi(h, &region);
		if (status != DVP_STATUS_OK)
		{
			printf("dvpGetRoi failed with err:%d\r\n", status);
			break;
		}
		printf("%s, region: x:%d, y:%d, w:%d, h:%d\r\n", name, region.X, region.Y, region.W, region.H);

		status = dvpGetExposure(h, &exp);
		if (status != DVP_STATUS_OK)
		{
			printf("dvpGetExposure failed with err:%d\r\n", status);
			break;
		}

		status = dvpGetAnalogGain(h, &gain);
		if (status != DVP_STATUS_OK)
		{
			printf("dvpGetAnalogGain failed with err:%d\r\n", status);
			break;
		}

		printf("%s, exposure: %lf, gain: %f\r\n", name, exp, gain);

		dvpFrame frame;

		void* p;

		status = dvpStart(h);
		if (status != DVP_STATUS_OK)
		{
			break;
		}

		for (int j = 0; j < GRABCOUNT; j++)
		{
#ifdef SOFT_TRIG
			if (trigMode)
			{
				// trig a frame
				status = dvpTriggerFire(h);
				if (status != DVP_STATUS_OK)
				{
					printf("Fail to trig a frame\r\n");
				}
			}
#endif
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
					break;
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

			/*char PicName[64];
			sprintf(PicName, "pic/test-%s_pic_%d.jpg", name, j);
			status = dvpSavePicture(&frame, p, PicName, 90);
			if (status == DVP_STATUS_OK)
			{
				printf("Save to %s OK\r\n", PicName);
			}*/

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