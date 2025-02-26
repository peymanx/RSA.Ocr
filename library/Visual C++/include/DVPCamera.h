/**@mainpage Overview
*
*@section Design Design Philosophy
*-# Simple, easy, stable, and reliable
*-# The API is refined to try to avoid multiple uses of one function, and give full play to the characteristics of C/C++as a static language and powerful error correction during compilation
*-# Although there are many API, they are highly ordered. If necessary, all functions with similar declarations can be placed in a map corresponding to the name and function pointer to achieve more flexible dynamic calls
*
*@section Development Development suggestions
*-# Under predictable circumstances @link #DVP_INFORMATION @endlink, please ensure that all standard APIs return DVP_STATUS_OK @link #dvpStatus::DVP_STATUS_OK @endlink(For example, avoiding calling features that are only available to color cameras for mono cameras). This helps improve the robustness of the program
*-# Make good use of the "developer mode" of the official demo program. This helps to deepen the understanding of the functions and applications of each API interface<a href="C:/Program Files(x86)/Do3think/BasedCam2">Basedcam2</a>
*-# The most basic API are organized under the Basic Functions grouping @link #DVP2_API_BASE @endlink. This helps to quickly grasp the core functions of the camera
*-# Refer to CaptureImage Example @link #CaptureImage @endlink.This helps get started quickly
*-# The dvpHandle Value should be as random number @link #dvpHandle @endlink 
*-# You don't need to read the header file directly. You will get a more friendly experience by referring to the chm document
*
*
*/


#ifndef __DVPCAMERA_H__
#define __DVPCAMERA_H__

#define  dvpPtr(ptr) ptr*

#ifndef __cplusplus
#define  bool unsigned char
#define  false(0)
#define  true(1)
#endif

#ifndef _WIN32
#ifndef __cplusplus
#define  dvp2api extern
#else
#define  dvp2api extern "C" 
#endif
#else
#include<windows.h>
#ifndef __cplusplus
#define  dvp2api extern __declspec(dllimport)
#else
#define  dvp2api extern "C" __declspec(dllimport)
#endif
#endif

dvp2api const unsigned int dvpVersion[4]; 


/**
 * @addtogroup EnumGroup Enum define
 * @{
 */

/**
 * @brief image format
 * @see dvpGetFrame
*/
typedef enum dvpImageFormat
{
	/** @brief Mono image*/
	FORMAT_MONO = 0, 
	
	/** @brief BGGR bayer image*/
	FORMAT_BAYER_BG = 1, 
	
	/** @brief GBRG bayer image*/
	FORMAT_BAYER_GB = 2, 
	
	/** @brief GRBG bayer image*/
	FORMAT_BAYER_GR = 3, 
	
	/** @brief RGGB bayer image*/
	FORMAT_BAYER_RG = 4, 
	
	/** @brief each pixel is represented by 24bits, with 8bits allocated for each color channel(blue,green,and red)*/
	FORMAT_BGR24 = 10, 
	
	/** @brief each pixel is represented by 32bits, with 8bits allocated for each color channel(blue,green,red and alpha)*/
	FORMAT_BGR32 = 11, 
	
	/** @brief each pixel is represented by 48bits, with 16bits allocated for each color channel(blue,green,red)*/
	FORMAT_BGR48 = 12, 
	
	/** @brief each pixel is represented by 64bits, with 16bits allocated for each color channel(blue,green,red and alpha)*/
	FORMAT_BGR64 = 13, 
	
	/** @brief each pixel is represented by 24bits, with 8bits allocated for each color channel(red,green,blue)*/
	FORMAT_RGB24 = 14, 
	
	/** @brief each pixel is represented by 32bits, with 8bits allocated for each color channel(red,green,blue and alpha)*/
	FORMAT_RGB32 = 15, 
	
	/** @brief each pixel is represented by 48bits, with 16bits allocated for each color channel(red,green,blue)*/
	FORMAT_RGB48 = 16, 
	
	/** @brief YUV411*/
	FORMAT_YUV411 = 20, 
	
	/** @brief YUV422*/
	FORMAT_YUV422 = 21, 
	
	/** @brief YUV444*/
	FORMAT_YUV444 = 22, 
	
	/** @brief YUV420*/
	FORMAT_YUV420 = 23, 
	
    /** @brief One pixel occupies one bit */
    FORMAT_MONO1 = 27,
	
	/** @brief BGR 3-channel 8-bit split image*/
	FORMAT_B8_G8_R8 = 40, 
	
	/** @brief BGR 3-channel 16 bit split image*/
	FORMAT_B16_G16_R16 = 44, 
	
}dvpImageFormat; 

/** @brief image data stream format
*@see dvpSetSourceFormat
*@see dvpSetTargetFormat
*@warning may not be supported in all formats
*@note In special cases, #dvpSetSourceFormatSel and #dvpSetTargetFormatSel series API are recommended*/
typedef enum dvpStreamFormat
{
	/** @brief 8-bit image*/
	S_RAW8 = 0, 
	
	/** @brief 10-bit image*/
	S_RAW10 = 1, 
	
	/** @brief 12-bit image */
	S_RAW12 = 2, 
	
	/** @brief 14-bit image */
	S_RAW14 = 3, 
	
	/** @brief 16-bit image */
	S_RAW16 = 4, 
	
	/** @brief each pixel is represented by 24bits, with 8bits allocated for each color channel(blue,green,and red)*/
	S_BGR24 = 10, 
	
	/** @brief each pixel is represented by 32bits, with 8bits allocated for each color channel(blue,green,red and alpha) */
	S_BGR32 = 11, 
	
	/** @brief each pixel is represented by 48bits, with 12bits allocated for each color channel(blue,green,red and alpha) */
	S_BGR48 = 12, 
	
	/** @brief each pixel is represented by 64bits, with 16bits allocated for each color channel(blue,green,red and alpha) */
	S_BGR64 = 13, 
	
	/** @brief each pixel is represented by 24bits, with 8bits allocated for each color channel(red,green,blue) */
	S_RGB24 = 14, 
	
	/** @brief each pixel is represented by 32bits, with 8bits allocated for each color channel(red,green,blue and alpha) */
	S_RGB32 = 15, 
	
	/** @brief each pixel is represented by 48bits, with 12bits allocated for each color channel(red,green,blue and alpha) */
	S_RGB48 = 16, 
	
	/** @brief each pixel is represented by 64bits, with 16bits allocated for each color channel(red,green,blue and alpha) */
	S_RGB64 = 17, 
	
	/** @brief YUV411 */
	S_YUV411 = 20, 
	
	/** @brief YUV422 */
	S_YUV422 = 21, 
	
	/** @brief YUV444 */
	S_YUV444 = 22, 
	
	/** @brief YUV444 */
	S_YUV420 = 23, 
	
    /** @brief 1 bit gray image */
    S_MONO1 = 27,

    /** @brief 2 bit gray image */
    S_MONO2 = 28,

    /** @brief 4 bit gray image */
    S_MONO4 = 29,
	
	/** @brief 8-bit gray image */
	S_MONO8 = 30, 
	
	/** @brief 10-bit gray image */
	S_MONO10 = 31, 
	
	/** @brief 12-bit gray image */
	S_MONO12 = 32, 
	
	/** @brief 14-bit gray image */
	S_MONO14 = 33, 
	
	/** @brief 16-bit gray image */
	S_MONO16 = 34, 
	
	/** @brief BGR 3-channel 8-bit split image */
	S_B8_G8_R8 = 40, 
	
	/** @brief BGR 3-channel 16 bit split image */
	S_B16_G16_R16 = 44, 
	
}dvpStreamFormat; 


/** @brief Image bit depth
*@see dvpGetFrame */
typedef enum dvpBits
{
	/** @brief 8-bit */
	BITS_8 = 0, 
	
	/** @brief 10-bit */
	BITS_10 = 1, 
	
	/** @brief 12-bit */
	BITS_12 = 2, 
	
	/** @brief 14-bit */
	BITS_14 = 3, 
	
	/** @brief 16-bit */
	BITS_16 = 4, 
	
}dvpBits; 


/** @brief AE Mode */
typedef enum dvpAeMode
{
	/** @brief AE and AG are both enabled,  with a priority on adjusting the exposure time. */
	AE_MODE_AE_AG = 0, 
	
	/** @brief AE and AG are both enabled,  with a priority on adjusting the gain. */
	AE_MODE_AG_AE = 1, 
	
	/** @brief only auto exposure time */
	AE_MODE_AE_ONLY = 2, 
	
	/** @brief only auto gain */
	AE_MODE_AG_ONLY = 3, 
	
}dvpAeMode; 


/** @brief AE operation */
typedef enum dvpAeOperation
{
	/** @brief disable operation */
	AE_OP_OFF = 0, 
	
	/** @brief only once operation */
	AE_OP_ONCE = 1, 
	
	/** @brief continuous operation */
	AE_OP_CONTINUOUS = 2, 
	
}dvpAeOperation; 


/** @brief AWB operation */
typedef enum dvpAwbOperation
{
	/** @brief disable operation */
	AWB_OP_OFF = 0, 
	
	/** @brief only once operation */
	AWB_OP_ONCE = 1, 
	
	/** @brief continuous operation */
	AWB_OP_CONTINUOUS = 2, 
	
}dvpAwbOperation; 


/** @brief AntiFlick */
typedef enum dvpAntiFlick
{
	/** @brief disable */
	ANTIFLICK_DISABLE = 0, 
	
	/** @brief enable 50hz AntiFlick */
	ANTIFLICK_50HZ = 1, 
	
	/** @brief enable 60hz AntiFlick */
	ANTIFLICK_60HZ = 2, 
	
}dvpAntiFlick; 


/** @brief image sensor type */
typedef enum dvpSensorType
{
	/** @brief CMOS image sensor */
	SENSOR_TYPE_CMOS = 0, 
	
	/** @brief CCD image sensor */
	SENSOR_TYPE_CCD = 1, 
	
}dvpSensorType; 


/** @brief image sensor pixel type */
typedef enum dvpSensorPixel
{
	/** @brief mono pixel */
	SENSOR_PIXEL_MONO = 0, 
	
	/** @brief RGGB color pixel */
	SENSOR_PIXEL_BAYER_RG = 1, 
	
	/** @brief GBRG color pixel */
	SENSOR_PIXEL_BAYER_GB = 2, 
	
	/** @brief GRBG color pixel */
	SENSOR_PIXEL_BAYER_GR = 3, 
	
	/** @brief BGGR color pixel */
	SENSOR_PIXEL_BAYER_BG = 4, 
	
}dvpSensorPixel; 


/** @brief video stream callback event */
typedef enum dvpStreamEvent
{
	/** @brief After the image reaches */
	STREAM_EVENT_ARRIVED = 0, 
	
	/** @brief After image correction */
	STREAM_EVENT_CORRECTED = 1, 
	
	/** @brief After image processing */
	STREAM_EVENT_PROCESSED = 2, 
	
	/** @brief Start a thread to grab frame in a @link #dvpGetFrame dvpGetFrame @endlink way, and call other user processing procedures
	*@warning must be set before @link #dvpStart starts the video stream @endlink
	*@see dvpProcessImage */
	STREAM_EVENT_FRAME_THREAD = 3, 
	
}dvpStreamEvent; 


/** @brief normal event */
typedef enum dvpEvent
{
	/** @brief unknown */
	EVENT_UNKNOW = 0, 
	
	/** @brief successfully connected */
	EVENT_CONNECTED = 1, 
	
	/** @brief disconnect */
	EVENT_DISCONNECTED = 2, 
	
	/** @brief frame stream has been started */
	EVENT_STREAM_STARTRD = 3, 
	
	/** @brief frame stream has stopped */
	EVENT_STREAM_STOPPED = 4, 
	
	/** @brief frame loss(error frame) */
	EVENT_FRAME_LOST = 5, 
	
	/** @brief timeout waiting for frame */
	EVENT_FRAME_TIMEOUT = 6, 
	
	/** @brief lost connection */
	EVENT_LOST_CONNECTION = 7, 
	
	/** @brief reconnect */
	EVENT_RECONNECTED = 8, 
	
	/** @brief  start frame transmission */
	EVENT_FRAME_START = 9, 
	
	/** @brief end frame transmission */
	EVENT_FRAME_END = 10, 
	
	/** @brief The trigger signal has been sent out */
	EVENT_TRIGGER_LAUNCH,
}dvpEvent; 


/** @brief Trigger input signal type */
typedef enum dvpTriggerInputType
{
	/** @brief trigger input off */
	TRIGGER_IN_OFF = 0, 
	
	/** @brief falling edge trigger */
	TRIGGER_NEG_EDGE = 1, 
	
	/** @brief low level trigger */
	TRIGGER_LOW_LEVEL = 2, 
	
	/** @brief rising edge trigger */
	TRIGGER_POS_EDGE = 3, 
	
	/** @brief high level trigger */
	TRIGGER_HIGH_LEVEL = 4, 
	
}dvpTriggerInputType; 


/** @brief Strobe output signal type */
typedef enum dvpStrobeOutputType
{
	/** @brief Strobe output is closed */
	STROBE_OUT_OFF = 0, 
	
	/** @brief output low level */
	STROBE_OUT_LOW = 1, 
	
	/** @brief output high level */
	STROBE_OUT_HIGH = 2, 
	
}dvpStrobeOutputType; 


/** @brief Strobe signal driving mode */
typedef enum dvpStrobeDriver
{
	/** @brief The whole frame cycle is valid */
	FRAME_DURATION = 0, 
	
	/** @brief driven by timer */
	TIMER_LOGIC = 1, 
	
	/** @brief follow the Strobe signal of sensor */
	SENSOR_STROBE = 2, 
	
	/** @brief effective during the entire trigger process, include trigger delay and check */
	TRIG_DURATION = 4, 
	
	/** @brief TriggerStartActive, activated by the edge of trigger signal */
	TRIG_START = 5, 
	
	/** @brief TriggerEndActive, activated by the edge of trigger signal */
	TRIG_END = 6, 
	
	/** @brief follow the Strobe signal of SENSOR, or the exposure control signal exp_ctrl generated by FPGA */
	EXPOSURE_DURATION = 8, 
	
	/** @brief start exposure */
	EXPOSURE_START = 9, 
	
	/** @brief end exposure */
	EXPOSURE_END = 10, 
	
	/** @brief frame start */
	FRAME_START = 13, 
	
	/** @brief frame end */
	FRAME_END = 14, 
	
	/** @brief The process indication that a group of frames trigger multiple frames */
	BURST_FRAME_DURATION = 16, 
	
	/** @brief A group of frames triggers the start of multiple frames */
	BURST_FRAME_START = 17, 
	
	/** @brief A group of frames triggers the end of multiple frames */
	BURST_FRAME_END = 18, 
	
    /** @brief After the start of the frame is activated, a fixed delay of how many lines are output before strobe */
    FRAME_START_DELAY_LINES = 20,

    /** @brief reserve */
    FRAME_END_DELAY_LINES = 21,
}dvpStrobeDriver; 


/** @brief output mode
*@deprecated */
typedef enum dvpOutputIoFunction
{
	/** @brief normal output */
	OUTPUT_FUNCTION_NORMAL = 0x0000, 
	
	/** @brief strobe output */
	OUTPUT_FUNCTION_STROBE = 0x0001, 
	
}dvpOutputIoFunction; 


/** @brief input mode
*@deprecated */
typedef enum dvpInputIoFunction
{
	/** @brief normal input */
	INPUT_FUNCTION_NORMAL = 0x8001, 
	
	/** @brief trigger input */
	INPUT_FUNCTION_TRIGGER = 0x8002, 
	
}dvpInputIoFunction; 


/** @brief Output IO
*@deprecated */
typedef enum dvpOutputIo
{
	/** @brief output IO1 */
	OUTPUT_IO_1 = 0x0001, 
	
	/** @brief output IO2 */
	OUTPUT_IO_2 = 0x0002, 
	
	/** @brief output IO3 */
	OUTPUT_IO_3 = 0x0003, 
	
	/** @brief output IO4 */
	OUTPUT_IO_4 = 0x0004, 
	
	/** @brief output IO5 */
	OUTPUT_IO_5 = 0x0005, 
	
	/** @brief output IO6 */
	OUTPUT_IO_6 = 0x0006, 
	
	/** @brief output IO7 */
	OUTPUT_IO_7 = 0x0007, 
	
	/** @brief output IO8 */
	OUTPUT_IO_8 = 0x0008, 
	
	/** @brief output IO9 */
	OUTPUT_IO_9 = 0x0009, 
	
	/** @brief output IO10 */
	OUTPUT_IO_10 = 0x000a, 
	
	/** @brief output IO11 */
	OUTPUT_IO_11 = 0x000b, 
	
	/** @brief output IO12 */
	OUTPUT_IO_12 = 0x000c, 
	
	/** @brief output IO13 */
	OUTPUT_IO_13 = 0x000d, 
	
	/** @brief output IO14 */
	OUTPUT_IO_14 = 0x000e, 
	
	/** @brief output IO15 */
	OUTPUT_IO_15 = 0x000f, 
	
	/** @brief output IO16 */
	OUTPUT_IO_16 = 0x0010, 
	
}dvpOutputIo; 


/** @brief Input IO
*@deprecated */
typedef enum dvpInputIo
{
	/** @brief input IO1 */
	INPUT_IO_1 = 0x8001, 
	
	/** @brief input IO2 */
	INPUT_IO_2 = 0x8002, 
	
	/** @brief input IO3 */
	INPUT_IO_3 = 0x8003, 
	
	/** @brief input IO4 */
	INPUT_IO_4 = 0x8004, 
	
	/** @brief input IO5 */
	INPUT_IO_5 = 0x8005, 
	
	/** @brief input IO6 */
	INPUT_IO_6 = 0x8006, 
	
	/** @brief input IO7 */
	INPUT_IO_7 = 0x8007, 
	
	/** @brief input IO8 */
	INPUT_IO_8 = 0x8008, 
	
}dvpInputIo; 


/** @brief device pin */
typedef enum dvpLine
{
	/** @brief pin1 */
	LINE_1 = 0x10001, 
	
	/** @brief pin2 */
	LINE_2 = 0x10002, 
	
	/** @brief pin3 */
	LINE_3 = 0x10003, 
	
	/** @brief pin4 */
	LINE_4 = 0x10004, 
	
	/** @brief pin5 */
	LINE_5 = 0x10005, 
	
	/** @brief pin6 */
	LINE_6 = 0x10006, 
	
	/** @brief pin7 */
	LINE_7 = 0x10007, 
	
	/** @brief pin8 */
	LINE_8 = 0x10008, 
	
	/** @brief pin9 */
	LINE_9 = 0x10009, 
	
	/** @brief pin10 */
	LINE_10 = 0x1000a, 
	
	/** @brief pin11 */
	LINE_11 = 0x1000b, 
	
	/** @brief pin12 */
	LINE_12 = 0x1000c, 
	
	/** @brief pin13 */
	LINE_13 = 0x1000d, 
	
	/** @brief pin14 */
	LINE_14 = 0x1000e, 
	
	/** @brief Pin15 */
	LINE_15 = 0x1000f, 
	
	/** @brief pin16 */
	LINE_16 = 0x10010, 
	
}dvpLine; 


/** @brief pin mode */
typedef enum dvpLineMode
{
	/** @brief input signal pin */
	LINE_MODE_INPUT = 0, 
	
	/** @brief output signal pin */
	LINE_MODE_OUTPUT = 1, 
	
}dvpLineMode; 


/** @brief signal source of pin output */
typedef enum dvpLineSource
{
	/** @brief user control level */
	OUTPUT_SOURCE_NORMAL = 0, 
	
	/** @brief strobe signal */
	OUTPUT_SOURCE_STROBE = 1, 
	
	/** @brief PWM signal */
	OUTPUT_SOURCE_PWM = 2, 
	
    /** @brief TX signal */
    OUTPUT_SOURCE_UART_TX = 0x0a,
	/** @brief PULSE signal */
	OUTPUT_SOURCE_PULSE = 0x10, 
	
    /** @brief sequence strobe 1 */
    OUTPUT_SOURCE_SEQUENCE_STROBE_1 = 0x20,

    /** @brief sequence strobe 2 */
    OUTPUT_SOURCE_SEQUENCE_STROBE_2 = 0x21,

    /** @brief sequence strobe 3 */
    OUTPUT_SOURCE_SEQUENCE_STROBE_3 = 0x22,

    /** @brief sequence strobe 4 */
    OUTPUT_SOURCE_SEQUENCE_STROBE_4 = 0x23,
}dvpLineSource; 


/** @brief pin trigger mode */
typedef enum dvpTriggerLineMode
{
	/** @brief normal */
	TRIGGER_LINE_MODE_NORMAL = 0, 
	
	/** @brief exposure time is determined by the pulse width */
	TRIGGER_LINE_MODE_BULB = 1, 
	
}dvpTriggerLineMode; 


/** @brief Trigger source */
typedef enum dvpTriggerSource
{
	/** @brief @link #dvpTriggerFire software @endlink trigger */
	TRIGGER_SOURCE_SOFTWARE = 0, 
	
	/** @brief @link #dvpLine::LINE_1 pin1 @endlink trigger */
	TRIGGER_SOURCE_LINE1 = 1, 
	
	/** @brief @link #dvpLine::LINE_2 pin2 @endlink trigger */
	TRIGGER_SOURCE_LINE2 = 2, 
	
	/** @brief @link #dvpLine::LINE_3 pin3 @endlink trigger */
	TRIGGER_SOURCE_LINE3 = 3, 
	
	/** @brief @link #dvpLine::LINE_4 pin4 @endlink trigger */
	TRIGGER_SOURCE_LINE4 = 4, 
	
	/** @brief @link #dvpLine::LINE_5 pin5 @endlink trigger */
	TRIGGER_SOURCE_LINE5 = 5, 
	
	/** @brief @link #dvpLine::LINE_6 pin6 @endlink trigger */
	TRIGGER_SOURCE_LINE6 = 6, 
	
	/** @brief @link #dvpLine::LINE_7 pin7 @endlink trigger */
	TRIGGER_SOURCE_LINE7 = 7, 
	
	/** @brief @link #dvpLine::LINE_8 pin8 @endlink trigger */
	TRIGGER_SOURCE_LINE8 = 8, 
	
}dvpTriggerSource; 


/** @brief The position of the first pixel */
typedef enum dvpFirstPosition
{
	/** @brief upper left corner */
	UP_LEFT = 0, 
	
	/** @brief upper right corner */
	UP_RIGHT = 1, 
	
	/** @brief lower left corner */
	BOTTOM_LEFT = 2, 
	
	/** @brief lower right corner */
	BOTTOM_RIGHT = 3, 
	
}dvpFirstPosition; 


/** @brief open mode of device
*@note supports "or" operation, such as: @code dvpOpen(0, dvpOpenMode(OPEN_NORMAL | OPEN_DEBUG), &handle)@endcode */
typedef enum dvpOpenMode
{
	/** @brief Open offline */
	OPEN_OFFLINE = 0, 
	
	/** @brief Open the actual device */
	OPEN_NORMAL = 1<<0, 
	
	/** @brief Open the device in debugging mode: for GigE cameras, the heartbeat packet timeout caused by single step debugging or breakpoints can be avoided */
	OPEN_DEBUG = 1<<3, 
	
	/** @brief grab frame and processing threads use higher priority */
	HIGH_PRIORITY = 1<<4, 
	
	/** @brief Auto configure IP */
	OPEN_AUTOIP = 1<<5, 
	
}dvpOpenMode; 


/** @brief frame stream state@see dvpGetStreamState */
typedef enum dvpStreamState
{
	/** @brief has stopped@see dvpStop */
	STATE_STOPED = 0, 
	
	/** @brief has started@see dvpStart */
	STATE_STARTED = 2, 
	
}dvpStreamState; 


/** @brief Curve fitting type */
typedef enum dvpCurveStyle
{
	/** @brief Linear curve fitting */
	CURVE_STYLE_LINE = 0, 
	
	/** @brief Smooth curve fitting */
	CURVE_STYLE_WAVE = 1, 
	
}dvpCurveStyle; 


/** @brief User Settings
*@note are usually stored in camera memory */
typedef enum dvpUserSet
{
	/** @brief default read-only setting
	*@note can only be read and cannot be written */
	USER_SET_DEFAULT = 0, 
	
	/** @brief User Setting 1 */
	USER_SET_1 = 1, 
	
	/** @brief User Setting 2 */
	USER_SET_2 = 2, 
	
}dvpUserSet; 


/** @brief frame buffer mode */
typedef enum dvpBufferMode
{
	/** @brief output the latest frame, and the old frame will be overwritten */
	BUFFER_MODE_NEWEST = 0, 
	
	/** @brief FIFO queue */
	BUFFER_MODE_FIFO = 1, 
	
}dvpBufferMode; 


/** @brief Device type */
typedef enum dvpDeviceType
{
	/** @brief unknown type */
	DEVICE_TYPE_UNKNOWN = 0, 
	
	/** @brief USB2.0 camera */
	DEVICE_TYPE_USB2_CAMERA = 10, 
	
	/** @brief USB3.0 camera */
	DEVICE_TYPE_USB3_CAMERA = 20, 
	
	/** @brief GigE camera */
	DEVICE_TYPE_GLAN_CAMERA = 30, 
	
	/** @brief 10 GigE camera */
	DEVICE_TYPE_XGIGE_CAMERA = 40, 
	
	/** @brief 10 GigE GrabCard */
	DEVICE_TYPE_XGIGE_GRABBER = 100, 
	
}dvpDeviceType; 


/** @brief Debug report, function module classification */
typedef enum dvpReportPart
{
	/** @brief Default or no function classification */
	PART_DEFAULT = 0x00, 
	
	/** @brief Link related */
	PART_LINK = 0x10, 
	
	/** @brief control related */
	PART_CONTROL = 0x20, 
	
	/** @brief frame stream or grab frame related */
	PART_STREAM = 0x30, 
	
	/** @brief Trigger function related */
	PART_TRIG = 0x40, 
	
	/** @brief GPIO function related */
	PART_GPIO = 0x50, 
	
	/** @brief Image effect related */
	PART_IMAGE = 0x60, 
	
}dvpReportPart; 


/** @brief Level of debugging report */
typedef enum dvpReportLevel
{
	/** @brief information or tips */
	LEVEL_INFO = 0x10, 
	
	/** @brief Problems or faults */
	LEVEL_PROBLEM = 0x40, 
	
}dvpReportLevel; 


/** @brief Status codes of all standard API
*@note equals DVP_STATUS_OK indicates normal
*@note greater than DVP_STATUS_OK indicates a warning
*@warning less than DVP_STATUS_OK indicates an error */
typedef enum dvpStatus
{
	/** @brief operation is ignored. No action is required */
	DVP_STATUS_IGNORED = 7, 
	
	/** @brief needs other data and operations */
	DVP_STATUS_NEED_OTHER = 6, 
	
	/** @brief has to go to the next stage, only part of the action has been completed */
	DVP_STATUS_NEXT_STAGE = 5, 
	
	/** @brief is busy, this operation cannot be carried out */
	DVP_STATUS_BUSY = 4, 
	
	/** @brief needs to wait, you can try again */
	DVP_STATUS_WAIT = 3, 
	
	/** @brief is in progress and has been operated */
	DVP_STATUS_IN_PROCESS = 2, 
	
	/** @brief operation succeeded */
	DVP_STATUS_OK = 1, 
	
	/** @brief operation failed */
	DVP_STATUS_FAILED = 0, 
	
	/** @brief Unknown error */
	DVP_STATUS_UNKNOW = -1, 
	
	/** @brief does not support this function */
	DVP_STATUS_NOT_SUPPORTED = -2, 
	
	/** @brief initialization is incomplete */
	DVP_STATUS_NOT_INITIALIZED = -3, 
	
	/** @brief parameter is invalid */
	DVP_STATUS_PARAMETER_INVALID = -4, 
	
	/** @brief parameter out of range */
	DVP_STATUS_PARAMETER_OUT_OF_BOUND = -5, 
	
	/** @brief Feature not enabled */
	DVP_STATUS_UNENABLED = -6, 
	
	/** @brief unconnected to the device */
	DVP_STATUS_UNCONNECTED = -7, 
	
	/** @brief function is invalid */
	DVP_STATUS_NOT_VALID = -8, 
	
	/** @brief device is not opened */
	DVP_STATUS_UNPLAY = -9, 
	
	/** @brief device is not started */
	DVP_STATUS_NOT_STARTED = -10, 
	
	/** @brief device didn't stop */
	DVP_STATUS_NOT_STOPPED = -11, 
	
	/** @brief device is not ready */
	DVP_STATUS_NOT_READY = -12, 
	
	/** @brief Invalid handle(empty handle or wild handle), usually caused by the camera not being opened */
	DVP_STATUS_INVALID_HANDLE = -13, 
	
	/** @brief error description */
	DVP_STATUS_DESCR_FAULT = -20, 
	
	/** @brief error name */
	DVP_STATUS_NAME_FAULT = -21, 
	
	/** @brief error value */
	DVP_STATUS_VALUE_FAULT = -22, 
	
	/** @brief is restricted */
	DVP_STATUS_LIMITED = -28, 
	
	/** @brief function is invalid */
	DVP_STATUS_FUNCTION_INVALID = -29, 
	
	/** @brief is in automatic progress, manual mode is invalid */
	DVP_STATUS_IN_AUTO = -30, 
	
	/** @brief operation rejected */
	DVP_STATUS_DENIED = -31, 
	
	/** @brief offset or address misalignment */
	DVP_STATUS_BAD_ALIGNMENT = -40, 
	
	/** @brief address is invalid */
	DVP_STATUS_ADDRESS_INVALID = -41, 
	
	/** @brief data block size is invalid */
	DVP_STATUS_SIZE_INVALID = -42, 
	
	/** @brief data overload */
	DVP_STATUS_OVER_LOAD = -43, 
	
	/** @brief data is insufficient */
	DVP_STATUS_UNDER_LOAD = -44, 
	
	/** @brief check, verification failed */
	DVP_STATUS_CHECKED_FAILED = -50, 
	
	/** @brief is unavailable */
	DVP_STATUS_UNUSABLE = -51, 
	
	/** @brief Business ID is invalid or does not match */
	DVP_STATUS_BID_INVALID = -52, 
	
	/** @brief is used improperly */
	DVP_STATUS_IMPROPER = -53, 
	
	/** @brief is read-only and cannot be written */
	DVP_STATUS_READ_ONLY = -54, 
	
	/** @brief Unrecognized data */
	DVP_STATUS_UNRECOGNIZED_DATA = -55, 
	
	/** @brief timeout error */
	DVP_STATUS_TIME_OUT = -1000, 
	
	/** @brief Hardware IO error */
	DVP_STATUS_IO_ERROR = -1001, 
	
	/** @brief communication error */
	DVP_STATUS_COMM_ERROR = -1002, 
	
	/** @brief bus error */
	DVP_STATUS_BUS_ERROR = -1003, 
	
	/** @brief format error */
	DVP_STATUS_FORMAT_INVALID = -1004, 
	
	/** @brief content is invalid */
	DVP_STATUS_CONTENT_INVALID = -1005, 
	
	/** @brief I2C bus error */
	DVP_STATUS_I2C_FAULT = -1010, 
	
	/** @brief I2C timeout waiting for reply */
	DVP_STATUS_I2C_ACK_TIMEOUT = -1011, 
	
	/** @brief I2C waits for bus action timeout, for example, SCL is pulled to low level by external devices */
	DVP_STATUS_I2C_BUS_TIMEOUT = -1012, 
	
	/** @brief SPI bus error */
	DVP_STATUS_SPI_FAULT = -1020, 
	
	/** @brief UART bus error */
	DVP_STATUS_UART_FAULT = -1030, 
	
	/** @brief GPIO bus error */
	DVP_STATUS_GPIO_FAULT = -1040, 
	
	/** @brief USB bus error */
	DVP_STATUS_USB_FAULT = -1050, 
	
	/** @brief PCI bus error */
	DVP_STATUS_PCI_FAULT = -1060, 
	
	/** @brief Physical layer error */
	DVP_STATUS_PHY_FAULT = -1070, 
	
	/** @brief Link layer error */
	DVP_STATUS_LINK_FAULT = -1080, 
	
	/** @brief Transport Layer Error */
	DVP_STATUS_TRANS_FAULT = -1090, 
	
	/** @brief No device found */
	DVP_STATUS_NO_DEVICE_FOUND = -1100, 
	
	/** @brief No logical device found */
	DVP_STATUS_NO_LOGIC_DEVICE_FOUND = -1101, 
	
	/** @brief device has been opened */
	DVP_STATUS_DEVICE_IS_OPENED = -1102, 
	
	/** @brief The device has been closed */
	DVP_STATUS_DEVICE_IS_CLOSED = -1103, 
	
	/** @brief The device has been disconnected */
	DVP_STATUS_DEVICE_IS_DISCONNECTED = -1104, 
	
	/** @brief The device has been opened by another host */
	DVP_STATUS_DEVICE_IS_OPENED_BY_ANOTHER = -1105, 
	
	/** @brief device has been started */
	DVP_STATUS_DEVICE_IS_STARTED = -1106, 
	
	/** @brief The device has been stopped */
	DVP_STATUS_DEVICE_IS_STOPPED = -1107, 
	
	/** @brief does not have enough system memory */
	DVP_STATUS_INSUFFICIENT_MEMORY = -1200, 
	
	/** @brief The memory reads and writes incorrectly or cannot be read and written normally */
	DVP_STATUS_MEMORY_FAULT = -1201, 
	
	/** @brief is write protected and cannot be written */
	DVP_STATUS_WRITE_PROTECTED = -1202, 
	
	/** @brief failed to create file */
	DVP_STATUS_FILE_CREATE_FAILED = -1300, 
	
	/** @brief file format is invalid */
	DVP_STATUS_FILE_INVALID = -1301, 
	
	/** @brief failed to read the file */
	DVP_STATUS_FILE_READ_FAILED = -1302, 
	
	/** @brief failed to write file */
	DVP_STATUS_FILE_WRITE_FAILED = -1303, 
	
	/** @brief failed to open the file */
	DVP_STATUS_FILE_OPEN_FAILED = -1304, 
	
	/** @brief Failed to read data comparison */
	DVP_STATUS_FILE_CHECKSUM_FAILED = -1305, 
	
	/** @brief grab frame failed */
	DVP_STATUS_GRAB_FAILED = -1600, 
	
	/** @brief frame data is lost and incomplete */
	DVP_STATUS_LOST_DATA = -1601, 
	
	/** @brief did not receive the frame EOF */
	DVP_STATUS_EOF_ERROR = -1602, 
	
	/** @brief The grab frame function has been opened */
	DVP_STATUS_GRAB_IS_OPENED = -1603, 
	
	/** @brief The grab frame function has been closed */
	DVP_STATUS_GRAB_IS_CLOSED = -1604, 
	
	/** @brief The grab frame function has been started */
	DVP_STATUS_GRAB_IS_STARTED = -1605, 
	
	/** @brief The grab frame function has been stopped */
	DVP_STATUS_GRAB_IS_STOPPED = -1606, 
	
	/** @brief The grab frame function is restarting */
	DVP_STATUS_GRAB_IS_RESTARTING = -1607, 
	
	/** @brief Invalid ROI parameter */
	DVP_STATUS_ROI_PARAM_INVALID = -1610, 
	
	/** @brief ROI function is not supported */
	DVP_STATUS_ROI_NOT_SUPPORTED = -1611, 
	
}dvpStatus; 


/** @brief XML API type */
typedef enum dvpXMLInterfaceType
{
	/** @brief IValue interface */
	INTERFACE_TYPE_IVALUE = 0, 
	
	/** @brief IBase interface */
	INTERFACE_TYPE_IBASE = 1, 
	
	/** @brief IInteger interface */
	INTERFACE_TYPE_IInteger = 2, 
	
	/** @brief IBoolean interface */
	INTERFACE_TYPE_IBoolean = 3, 
	
	/** @brief ICommand interface */
	INTERFACE_TYPE_ICommand = 4, 
	
	/** @brief IFloat interface */
	INTERFACE_TYPE_IFloat = 5, 
	
	/** @brief IString interface */
	INTERFACE_TYPE_IString = 6, 
	
	/** @brief IRegister interface */
	INTERFACE_TYPE_IRegister = 7, 
	
	/** @brief ICategory interface */
	INTERFACE_TYPE_ICategory = 8, 
	
	/** @brief IEnumeration interface */
	INTERFACE_TYPE_IEnumeration = 9, 
	
	/** @brief IEnumEntry interface */
	INTERFACE_TYPE_IEnumEntry = 10, 
	
	/** @brief IPort interface */
	INTERFACE_TYPE_IPort = 11, 
	
}dvpXMLInterfaceType; 

/**
 * @}
 */

#ifndef _WIN32

#include<stdint.h>

/**
 * @addtogroup MacroGroup Macro define
 * @{
 */

/** @brief 8-bit unsigned integer */
typedef uint8_t dvpByte; 

/** @brief 32-bit signed integer */
typedef int32_t dvpInt32; 

/** @brief 32-bit unsigned integer */
typedef uint32_t dvpUint32; 

/** @brief 64-bit signed integer */
typedef int64_t dvpInt64; 

/** @brief 64-bit unsigned integer */
typedef uint64_t dvpUint64; 

#else
/** @brief 8-bit unsigned integer */
typedef BYTE dvpByte; 

/** @brief 32-bit signed integer */
typedef INT32 dvpInt32; 

/** @brief 32-bit unsigned integer */
typedef UINT32 dvpUint32; 

/** @brief 64-bit signed integer */
typedef INT64 dvpInt64; 

/** @brief 64-bit unsigned integer */
typedef UINT64 dvpUint64; 

#endif//_WIN32
/** @brief 128byte reserved */
typedef dvpUint32 dvpReserved[32]; 

/** @brief 64byte string */
typedef char dvpString64[64]; 

/** @brief 128byte string */
typedef char dvpString128[128]; 

/** @brief 32byte bool array */
typedef bool dvpBool32[32]; 

/** @brief 4 rows and 3 columns simple matrix */
typedef float dvpMatrix[4][3]; 

/** @brief define a type dvpFloat3c consisting of 3 float values. */
typedef float dvpFloat3c[3]; 

/** @brief define a type dvpSize3c consisting of 3 dvpUint32 values. */
typedef dvpUint32 dvpSize3c[3]; 

/** @brief Camera handle
*@see dvpOpen
*@see dvpOpenByName */
typedef dvpUint32 dvpHandle; 

/** @brief video encoder handle
*@see dvpStartVideoRecord */
typedef dvpUint32 dvpRecordHandle; 

/** @brief string pointer */
typedef const char* dvpStr; 

/**
 * @}
 */

/** @brief Color Matrix */
typedef struct dvpColorMatrix
{
	/** @brief matrix */
	dvpMatrix matrix; 
	
	/** @brief reserved bytes */
	dvpReserved reserved; 
}dvpColorMatrix; 


/** @brief Color correction information */
typedef struct dvpColorCorrection
{
	/** @brief BGR correction coefficient */
	dvpFloat3c bgr; 
	
	/** @brief reserved bytes */
	dvpReserved reserved; 
}dvpColorCorrection; 


/** @brief 2D float point */
typedef struct dvpPoint2f
{
	/** @brief abscissa */
	float X; 
	
	/** @brief ordinate */
	float Y; 
	
}dvpPoint2f; 


/** @brief */
typedef struct dvpPoint2f dvpPoint3c[3][256]; 

/** @brief BGR curve array */
typedef struct dvpCurveArray
{
	/** @brief BGR curve array space */
	dvpPoint3c point; 
	
	/** @brief BGR effective channels, up to 3 */
	dvpUint32 rows; 
	
	/** @brief Effective points of BGR three channels, each channel can have up to 256 */
	dvpSize3c cols; 
	
}dvpCurveArray; 


/** @brief LUT defined by curve
*@warning This structure has a large number of bytes, which is easy to cause stack overflow. It is recommended to use the new dvpCurveLut() method to construct on the heap */
typedef struct dvpCurveLut
{
	/** @brief enable status */
	bool enable; 
	
	/** @brief Curve style */
	dvpCurveStyle style; 
	
	/** @brief anchor array, a complete LUT can be determined by a few anchors */
	dvpCurveArray array; 
	
	/** @brief reserved bytes */
	dvpReserved reserved; 
}dvpCurveLut; 


/** @brief Camera information
*@see dvpGetCameraInfo
*@see dvpEnum */
typedef struct dvpCameraInfo
{
	/** @brief Design manufacturer */
	dvpString64 Vendor; 
	
	/** @brief Production manufacturer */
	dvpString64 Manufacturer; 
	
	/** @brief model */
	dvpString64 Model; 
	
	/** @brief series */
	dvpString64 Family; 
	
	/** @brief link name */
	dvpString64 LinkName; 
	
	/** @brief image sensor description */
	dvpString64 SensorInfo; 
	
	/** @brief hardware version */
	dvpString64 HardwareVersion; 
	
	/** @brief firmware version */
	dvpString64 FirmwareVersion; 
	
	/** @brief kernel driver version */
	dvpString64 KernelVersion; 
	
	/** @brief Device driver version */
	dvpString64 DscamVersion; 
	
	/** @brief Friendly device name */
	dvpString64 FriendlyName; 
	
	/** @brief port description */
	dvpString64 PortInfo; 
	
	/** @brief serial number
	*@note can be changed generally */
	dvpString64 SerialNumber; 
	
	/** @brief Camera description */
	dvpString128 CameraInfo; 
	
	/** @brief User Name */
	dvpString128 UserID; 
	
	/** @brief Original serial number */
	dvpString64 OriginalSerialNumber; 
	
	/** @brief reserved bytes */
	dvpString64 reserved; 
}dvpCameraInfo; 


/** @brief Region Description */
typedef struct dvpRegionDescr
{
	/** @brief Minimum width */
	dvpInt32 iMinW; 
	
	/** @brief Minimum height */
	dvpInt32 iMinH; 
	
	/** @brief maximum width */
	dvpInt32 iMaxW; 
	
	/** @brief Maximum height */
	dvpInt32 iMaxH; 
	
	/** @brief step size in width direction */
	dvpInt32 iStepW; 
	
	/** @brief step size in height direction */
	dvpInt32 iStepH; 
	
	/** @brief reserved bytes */
	dvpReserved reserved; 
}dvpRegionDescr; 


/** @brief
*@see dvpGetRoi
*@see dvpGetAwbRoi
*@see dvpGetAeRoi
*@warning data members are usually even, please refer to the user manual for details */
typedef struct dvpRegion
{
	/** @brief Horizontal starting position */
	dvpInt32 X; 
	
	/** @brief vertical starting position */
	dvpInt32 Y; 
	
	/** @brief width(greater than 0) */
	dvpInt32 W; 
	
	/** @brief height(greater than 0) */
	dvpInt32 H; 
	
	/** @brief reserved bytes */
	dvpReserved reserved; 
}dvpRegion; 


/** @brief frame information
*@see dvpGetFrame */
typedef struct dvpFrame
{
	/** @brief format */
	enum dvpImageFormat format; 
	
	/** @brief bit width */
	enum dvpBits bits; 
	
	/** @brief frame data size,in bytes */
	dvpUint32 uBytes; 
	
	/** @brief width */
	dvpInt32 iWidth; 
	
	/** @brief height */
	dvpInt32 iHeight; 
	
	/** @brief frame ID */
	dvpUint64 uFrameID; 
	
	/** @brief timestamp */
	dvpUint64 uTimestamp; 
	
	/** @brief Exposure time(in microseconds)
	*@see dvpGetExposure */
	double fExposure; 
	
	/** @brief Analog gain
	*@see dvpGetAnalogGain */
	float fAGain; 
	
	/** @brief The position of the first pixel
	*@see dvpGetFirstPosition */
	enum dvpFirstPosition position; 
	
	/** @brief Whether to flip horizontally
	*@see dvpGetFlipHorizontalState */
	bool bFlipHorizontalState; 
	
	/** @brief Whether to flip vertically
	*@see dvpGetFlipVerticalState */
	bool bFlipVerticalState; 
	
	/** @brief Whether to rotate 90 degrees
	*@see dvpGetRotateState */
	bool bRotateState; 
	
	/** @brief Whether it rotates counterclockwise
	*@see dvpGetRotateOpposite */
	bool bRotateOpposite; 
	
	/** @brief internal flag */
	dvpUint32 internalFlags; 
	
	/** @brief Internal information */
	dvpUint32 internalValue; 
	
	/** @brief Trigger ID of one or more frames corresponding to each trigger
	*@see dvpGetFramesPerTrigger */
	dvpUint64 uTriggerId; 
	
	/** @brief User customized data, which is value or pointer */
	dvpUint64 userValue; 
    /** @brief Attached information in the form of a linked list, which can concatenate multiple types of information */
	dvpUint64 pExtra;
	
	/** @brief reserved bytes */
	dvpUint32 reserved[22]; 
}dvpFrame; 


/** @brief Frame information and image data
*@see dvpGetFrameBuffer */
typedef struct dvpFrameBuffer
{
	/** @brief frame information */
	struct dvpFrame frame; 
	
	/** @brief First address of image data
	*@warning Do not release buffer manually */
	dvpPtr(dvpByte) pBuffer; 
	
}dvpFrameBuffer; 


/** @brief General Data
*@see dvpEventCallback */
typedef struct dvpVariant
{
	/** @brief Data First Address */
	dvpPtr(dvpByte) pData; 
	
	/** @brief Data Bytes */
	dvpUint32 uSize; 
	
	/** @brief Data type name */
	dvpString64 name; 
	
}dvpVariant; 


/** @brief 64-bit integer data description */
typedef struct dvpInt64Descr
{
	/** @brief step length */
	dvpUint64 iStep; 
	
	/** @brief minimum */
	dvpUint64 iMin; 
	
	/** @brief max */
	dvpUint64 iMax; 
	
	/** @brief default value */
	dvpUint64 iDefault; 
	
	/** @brief reserved bytes */
	dvpReserved reserved; 
}dvpInt64Descr; 


/** @brief Integer data description */
typedef struct dvpIntDescr
{
	/** @brief step length */
	dvpInt32 iStep; 
	
	/** @brief minimum */
	dvpInt32 iMin; 
	
	/** @brief max */
	dvpInt32 iMax; 
	
	/** @brief default value */
	dvpInt32 iDefault; 
	
	/** @brief reserved bytes */
	dvpReserved reserved; 
}dvpIntDescr; 


/** @brief unsigned integer data description */
typedef struct dvpUintDescr
{
	/** @brief step length */
	dvpUint32 uStep; 
	
	/** @brief minimum */
	dvpUint32 uMin; 
	
	/** @brief max */
	dvpUint32 uMax; 
	
	/** @brief default value */
	dvpUint32 uDefault; 
	
	/** @brief reserved bytes */
	dvpReserved reserved; 
}dvpUintDescr; 


/** @brief float data description */
typedef struct dvpFloatDescr
{
	/** @brief step length */
	float fStep; 
	
	/** @brief minimum */
	float fMin; 
	
	/** @brief max */
	float fMax; 
	
	/** @brief default value */
	float fDefault; 
	
	/** @brief reserved bytes */
	dvpReserved reserved; 
}dvpFloatDescr; 


/** @brief double precision data description */
typedef struct dvpDoubleDescr
{
	/** @brief step length */
	double fStep; 
	
	/** @brief minimum */
	double fMin; 
	
	/** @brief max */
	double fMax; 
	
	/** @brief default value */
	double fDefault; 
	
	/** @brief reserved bytes */
	dvpReserved reserved; 
}dvpDoubleDescr; 


/** @brief 64-bit unsigned integer data description */
typedef struct dvpUint64Descr
{
	/** @brief step length */
	dvpUint64 uStep; 
	
	/** @brief minimum */
	dvpUint64 uMin; 
	
	/** @brief max */
	dvpUint64 uMax; 
	
	/** @brief default value */
	dvpUint64 uDefault; 
	
	/** @brief reserved bytes */
	dvpReserved reserved; 
}dvpUint64Descr; 


/** @brief selection description */
typedef struct dvpSelectionDescr
{
	/** @brief Default Index */
	dvpUint32 uDefault; 
	
	/** @brief Number of indexes */
	dvpUint32 uCount; 
	
	/** @brief Whether the video stream should be stopped */
	bool bNeedStop; 
	
	/** @brief reserved bytes */
	dvpReserved reserved; 
}dvpSelectionDescr; 


/** @brief Selection description */
typedef struct dvpEnumDescr
{
	/** @brief enumeration value */
	dvpInt32 iEnumValue; 
	
	/** @brief Enumerate name */
	dvpString64 szEnumName; 
	
}dvpEnumDescr; 


/** @brief option */
typedef struct dvpSelection
{
	/** @brief string description */
	dvpString64 string; 
	
	/** @brief index */
	dvpInt32 iIndex; 
	
	/** @brief reserved bytes */
	dvpReserved reserved; 
}dvpSelection; 


/** @brief resolution mode
*@see dvpGetTriggerModeSelDetail */
typedef struct dvpResolutionMode
{
	/** @brief resolution mode option */
	struct dvpSelection selection; 
	
	/** @brief Region */
	struct dvpRegionDescr region; 
	
	/** @brief reserved bytes */
	dvpReserved reserved; 
}dvpResolutionMode; 


/** @brief Quick Roi
*@see dvpGetQuickRoiSelDetail */
typedef struct dvpQuickRoi
{
	/** @brief option */
	struct dvpSelection selection; 
	
	/** @brief Region of interest */
	struct dvpRegion roi; 
	
	/** @brief resolution mode */
	struct dvpResolutionMode mode; 
	
	/** @brief reserved bytes */
	dvpReserved reserved; 
}dvpQuickRoi; 


/** @brief image format */
typedef struct dvpFormatSelection
{
	/** @brief resolution mode option */
	struct dvpSelection selection; 
	
	/** @brief Enumeration type corresponding to */
	enum dvpStreamFormat format; 
	
	/** @brief reserved bytes */
	dvpReserved reserved; 
}dvpFormatSelection; 


/** @brief Frame Count
*@see dvpGetFrameCount */
typedef struct dvpFrameCount
{
	/** @brief The number of frames received, usually transmitted to the computer */
	dvpUint32 uFrameCount; 
	
	/** @brief The Drop frames, cannot be grabbed in time */
	dvpUint32 uFrameDrop; 
	
	/** @brief Ignore frames, lose after grabbed */
	dvpUint32 uFrameIgnore; 
	
	/** @brief Error Frame Count */
	dvpUint32 uFrameError; 
	
	/** @brief The correct frame grabbed */
	dvpUint32 uFrameOK; 
	
	/** @brief Valid frames submitted/output */
	dvpUint32 uFrameOut; 
	
	/** @brief retransmit frames */
	dvpUint32 uFrameResend; 
	
	/** @brief number of frames through image processing */
	dvpUint32 uFrameProc; 
	
	/** @brief grab frame rate */
	float fFrameRate; 
	
	/** @brief Image processing frame rate */
	float fProcRate; 
	
	/** @brief reserved bytes */
	dvpReserved reserved; 
}dvpFrameCount; 


/** @brief User IO Information
*@see dvpGetUserIoInfo */
typedef struct dvpUserIoInfo
{
	/** @brief Available input IO */
	dvpBool32 inputValid; 
	
	/** @brief Available output IO */
	dvpBool32 outputValid; 
	
	/** @brief reserved bytes */
	dvpReserved reserved; 
}dvpUserIoInfo; 


/** @brief User IO Information
*@see dvpGetLineInfo */
typedef struct dvpLineInfo
{
	/** @brief Available IO */
	dvpBool32 valid; 
	
	/** @brief Bidirectional IO */
	dvpBool32 bidir; 
	
	/** @brief Whether the inverter is supported */
	dvpBool32 inverter; 
	
	/** @brief Whether the software can set the output level */
	dvpBool32 user; 
	
	/** @brief Whether the strobe signal is supported */
	dvpBool32 strobe; 
	
	/** @brief Whether the pwm signal is supported */
	dvpBool32 pwm; 
	
	/** @brief Whether the timer signal is supported */
	dvpBool32 timer; 
	
	/** @brief Whether uart signal is supported */
	dvpBool32 uart; 
	
	/** @brief Whether pulse signal is supported */
	dvpBool32 pulse; 
	
    /** @brief Whether triggering input is support */
    dvpBool32 trigger;

    /** @brief Whether sequence strobe is support */
    dvpBool32 sequence_strobe;
	
	/** @brief reserved bytes */
	dvpUint32 reserved[8]; 
}dvpLineInfo; 


/** @brief AE Configuration
*@see dvpGetAeConfig */
typedef struct dvpAeConfig
{
	/** @brief Minimum auto exposure time(in microseconds) */
	double fExposureMin; 
	
	/** @brief Maximum auto exposure time(in microseconds) */
	double fExposureMax; 
	
	/** @brief reserve value 1 */
	double reserved1; 
	
	/** @brief Minimum auto gain */
	float fGainMin; 
	
	/** @brief Maximum autom gain */
	float fGainMax; 
	
	/** @brief reserve value 2 */
	float reserved2; 
	
	/** @brief reserved bytes */
	dvpReserved reserved; 
}dvpAeConfig; 


/** @brief image sensor information
*@see dvpGetSensorInfo */
typedef struct dvpSensorInfo
{
	/** @brief string description */
	dvpString64 descr; 
	
	/** @brief Sensor type */
	enum dvpSensorType sensor; 
	
	/** @brief Original pixel type */
	enum dvpSensorPixel pixel; 
	
	/** @brief Region */
	struct dvpRegionDescr region; 
	
	/** @brief reserved bytes */
	dvpReserved reserved; 
}dvpSensorInfo; 


/** @brief Camera function description information
*@see dvpGetFunctionInfo */
typedef struct dvpFunctionInfo
{
	/** @brief Whether the trigger function is supported */
	bool bTrigger; 
	
	/** @brief Whether the hardware ISP function is supported */
	bool bHardwareIsp; 
	
	/** @brief Whether the UserSet function is supported */
	bool bUserSet; 
	
	/** @brief Whether the horizontal flip function is supported */
	bool bHflip; 
	
	/** @brief Whether the vertical flip function is supported */
	bool bVflip; 
	
	/** @brief Whether the negative film function is supported */
	bool bInverse; 
	
	/** @brief Whether the function with gige standard is supported */
	bool bLine; 
	
	/** @brief Whether the cooler is supported
	*@see dvpGetCoolerState */
	bool bCooler; 
	
	/** @brief Whether the thermometer function is supported */
	bool bTemperature; 
	
	/** @brief Reserved */
	bool bReserved3; 
	
	/** @brief reserved bytes */
	dvpUint32 reserved[30]; 
}dvpFunctionInfo; 


/** @brief device temperature
*@see dvpGetTemperatureInfo */
typedef struct dvpTemperatureInfo
{
	/** @brief The temperature of the device */
	float fDevice; 
	
	/** @brief Sensor temperature */
	float fSensor; 
	
	/** @brief Temperature of chip 1 */
	float fChip1; 
	
	/** @brief Temperature of chip 2 */
	float fChip2; 
	
	/** @brief reserved bytes */
	dvpReserved reserved; 
}dvpTemperatureInfo; 


/** @brief Frame buffer Configuration
*@see dvpGetBufferConfig */
typedef struct dvpBufferConfig
{
	/** @brief buffer working mode */
	enum dvpBufferMode mode; 
	
	/** @brief buffer queue size
	*@see dvpGetBufferQueueSizeDescr
	*@warning may consume some memory space. Please set it according to the available memory of the system */
	dvpUint32 uQueueSize; 
	
	/** @brief Whether to discard the lastest frame when the buffer queue is full
	*@note true is to discard the lastest frame, false is to overwrite the oldest frame in the buffer queue */
	bool bDropNew; 
	
	/** @brief save buffer memory
	*@note whether to allocate memory according to the current resolution size and pixel bit width, rather than using the sensor's maximum resolution and maximum bit width */
	bool bLite; 
	
	/** @brief reserved bytes */
	dvpReserved reserved; 
}dvpBufferConfig; 


/** @brief line trigger configuration
*@see dvpGetLineTriggerConfig
*@note is generally valid only for line scan camera */
typedef struct dvpLineTriggerConfig
{
	/** @brief Pre frequency division */
	dvpUint32 uPreDiv; 
	
	/** @brief frequency multiplication */
	dvpUint32 uMult; 
	
	/** @brief frequency division */
	dvpUint32 uDiv; 
	
	/** @brief reserved bytes */
	dvpReserved reserved; 
}dvpLineTriggerConfig; 


/**
*@brief Get camera information
*@param[in] handle Camera handle
*@param[out] pCameraInfo Camera information
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_INFORMATION
*@see dvpGetDeviceType
*/
dvp2api dvpStatus dvpGetCameraInfo(dvpHandle handle, dvpCameraInfo* pCameraInfo); 


/**
*@brief Get sensor information
*@param[in] handle Camera handle
*@param[out] pSensorInfo sensor information
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_INFORMATION
*/
dvp2api dvpStatus dvpGetSensorInfo(dvpHandle handle, dvpSensorInfo* pSensorInfo); 


/**
*@brief Get frame count
*@param[in] handle Camera handle
*@param[out] pFrameCount Frame Count
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_INFORMATION
*/
dvp2api dvpStatus dvpGetFrameCount(dvpHandle handle, dvpFrameCount* pFrameCount); 


/**
*@brief Get user IO valid flag
*@param[in] handle Camera handle
*@param[out] pUserIoInfo User IO Valid Flag
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_INFORMATION
*@deprecated
*/
dvp2api dvpStatus dvpGetUserIoInfo(dvpHandle handle, dvpUserIoInfo* pUserIoInfo); 


/**
*@brief Get pin description flag
*@param[in] handle Camera handle
*@param[out] pLineInfo pin description flag
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_LINE
*/
dvp2api dvpStatus dvpGetLineInfo(dvpHandle handle, dvpLineInfo* pLineInfo); 


/**
*@brief Get the position of the first pixel
*@param[in] handle Camera handle
*@param[out] pFirstPosition the position of the first pixel
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_INFORMATION
*/
dvp2api dvpStatus dvpGetFirstPosition(dvpHandle handle, dvpFirstPosition* pFirstPosition); 


/**
*@brief Get function information
*@param[in] handle Camera handle
*@param[out] pFunctionInfo function information
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_INFORMATION
*@note Please call the corresponding API according to the function description. For example, do not use the unique features of color cameras for black and white cameras to avoid unnecessary error status codes
*/
dvp2api dvpStatus dvpGetFunctionInfo(dvpHandle handle, dvpFunctionInfo* pFunctionInfo); 


/**
*@brief Get the device type
*@param[in] handle Camera handle
*@param[out] pDeviceType Device Type
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_INFORMATION
*@see dvpGetCameraInfo
*/
dvp2api dvpStatus dvpGetDeviceType(dvpHandle handle, dvpDeviceType* pDeviceType); 


/**
*@brief Get device temperature information
*@param[in] handle Camera handle
*@param[out] pTemperatureInfo Device temperature information
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_INFORMATION
*@see dvpGetCoolerState
*/
dvp2api dvpStatus dvpGetTemperatureInfo(dvpHandle handle, dvpTemperatureInfo* pTemperatureInfo); 


/**
*@brief Send software trigger signal
*@param[in] handle Camera handle
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@note You must first set well trigger source@link #dvpSetTriggerSource @endlink @link #dvpTriggerSource::TRIGGER_SOURCE_SOFTWARE @endlink
*/
dvp2api dvpStatus dvpTriggerFire(dvpHandle handle); 


/**
*@brief Create defect pixel correction information
*@param[in] handle Camera handle
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_CORRECTION
*@see dvpSetDefectFixState
*@see dvpSetAutoDefectFixState
*/
dvp2api dvpStatus dvpCreateDefectFixInfo(dvpHandle handle); 


/**
*@brief Create flat field correction information
*@param[in] handle Camera handle
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_CORRECTION
*@note The user creates flat field information in specific scenes and scenes
*/
dvp2api dvpStatus dvpCreateFlatFieldInfo(dvpHandle handle); 


/**
*@brief Reset device
*@param[in] handle Camera handle
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP2_API_BASE
*@note After the operation is successful, it will automatically return to the device status before reset
*/
dvp2api dvpStatus dvpResetDevice(dvpHandle handle); 


/**
*@brief Quickly restart frame stream
*@param[in] handle Camera handle
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP2_API_BASE
*@see dvpHold
*must hold frame stream before operation @link #dvpHold @endlink
*/
dvp2api dvpStatus dvpRestart(dvpHandle handle); 


/**
*@brief Quickly pause the frame stream
*@param[in] handle Camera handle
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP2_API_BASE
*@see dvpRestart
*@see dvpIsHold
*@note user can perform this action before starting the video stream. after the video stream is started, execute dvpRestart to restore the video stream and immediately output image data.@link #dvpStart @endlink @link #dvpRestart @endlink
*/
dvp2api dvpStatus dvpHold(dvpHandle handle); 


/**
*@brief Get gamma value
*@param[in] handle Camera handle
*@param[out] pGamma gamma value
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetColorSolutionSel
*@see dvpSetGammaState
*@see dvpGetGammaState
*@see dvpGetGammaDescr
*@see dvpSetGamma
*/
dvp2api dvpStatus dvpGetGamma(dvpHandle handle, dvpInt32* pGamma); 


/**
*@brief Set gamma value
*@param[in] handle Camera handle
*@param[in] Gamma Gamma value
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetColorSolutionSel
*@see dvpSetGammaState
*@see dvpGetGammaState
*@see dvpGetGammaDescr
*@see dvpGetGamma
*@note can make the image brightness match the normal vision of human eyes
*/
dvp2api dvpStatus dvpSetGamma(dvpHandle handle, dvpInt32 Gamma); 


/**
*@brief Get gamma description information
*@param[in] handle Camera handle
*@param[out] pGammaDescr Gamma description information
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetColorSolutionSel
*@see dvpSetGammaState
*@see dvpGetGammaState
*@see dvpSetGamma
*@see dvpGetGamma
*/
dvp2api dvpStatus dvpGetGammaDescr(dvpHandle handle, dvpIntDescr* pGammaDescr); 


/**
*@brief Get the enabling status of gamma function
*@param[in] handle Camera handle
*@param[out] pGammaState The enabling state of the gamma function
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetColorSolutionSel
*@see dvpSetGamma
*@see dvpGetGamma
*@see dvpGetGammaDescr
*@see dvpSetGammaState
*/
dvp2api dvpStatus dvpGetGammaState(dvpHandle handle, bool* pGammaState); 


/**
*@brief Set the enabling status of gamma function
*@param[in] handle Camera handle
*@param[in] GammaState The enabling state of the gamma function
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetColorSolutionSel
*@see dvpSetGamma
*@see dvpGetGamma
*@see dvpGetGammaDescr
*@see dvpGetGammaState
*/
dvp2api dvpStatus dvpSetGammaState(dvpHandle handle, bool GammaState); 


/**
*@brief Get the contrast value
*@param[in] handle Camera handle
*@param[out] pContrast
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetBlackLevel
*@see dvpSetContrastState
*@see dvpGetContrastState
*@see dvpGetContrastDescr
*@see dvpSetContrast
*/
dvp2api dvpStatus dvpGetContrast(dvpHandle handle, dvpInt32* pContrast); 


/**
*@brief Set the contrast value
*@param[in] handle Camera handle
*@param[in] Contrast
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetBlackLevel
*@see dvpSetContrastState
*@see dvpGetContrastState
*@see dvpGetContrastDescr
*@see dvpGetContrast
*@note set image contrast
*/
dvp2api dvpStatus dvpSetContrast(dvpHandle handle, dvpInt32 Contrast); 


/**
*@brief Get the description of contrast
*@param[in] handle Camera handle
*@param[out] pContrastDescr Description of contrast
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetBlackLevel
*@see dvpSetContrastState
*@see dvpGetContrastState
*@see dvpSetContrast
*@see dvpGetContrast
*/
dvp2api dvpStatus dvpGetContrastDescr(dvpHandle handle, dvpIntDescr* pContrastDescr); 


/**
*@brief Get the enabling status of the contrast function
*@param[in] handle Camera handle
*@param[out] pContrastState Enable state of contrast function
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetBlackLevel
*@see dvpSetContrast
*@see dvpGetContrast
*@see dvpGetContrastDescr
*@see dvpSetContrastState
*/
dvp2api dvpStatus dvpGetContrastState(dvpHandle handle, bool* pContrastState); 


/**
*@brief Set the enabling status of the contrast function
*@param[in] handle Camera handle
*@param[in] ContrastState Enable state of contrast function
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetBlackLevel
*@see dvpSetContrast
*@see dvpGetContrast
*@see dvpGetContrastDescr
*@see dvpGetContrastState
*/
dvp2api dvpStatus dvpSetContrastState(dvpHandle handle, bool ContrastState); 


/**
*@brief Get image Roi(relative to the whole sensor region)
*@param[in] handle Camera handle
*@param[out] pRoi Image Roi(relative to the entire sensor region)
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_SIZE
*@see dvpGetResolutionModeSel
*@see dvpGetQuickRoiSel
*@see dvpSetRoiState
*@see dvpGetRoiState
*@see dvpGetRoiDescr
*@see dvpSetRoi
*/
dvp2api dvpStatus dvpGetRoi(dvpHandle handle, dvpRegion* pRoi); 


/**
*@brief Set image Roi(relative to the entire sensor region)
*@param[in] handle Camera handle
*@param[in] Roi image Roi(relative to the whole sensor region)
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_SIZE
*@see dvpGetResolutionModeSel
*@see dvpGetQuickRoiSel
*@see dvpSetRoiState
*@see dvpGetRoiState
*@see dvpGetRoiDescr
*@see dvpGetRoi
*/
dvp2api dvpStatus dvpSetRoi(dvpHandle handle, dvpRegion Roi); 


/**
*@brief Obtain the description information of the image Roi(relative to the entire sensor region)
*@param[in] handle Camera handle
*@param[out] pRoiDescr Description information of image Roi(relative to the whole sensor region)
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_SIZE
*@see dvpGetResolutionModeSel
*@see dvpGetQuickRoiSel
*@see dvpSetRoiState
*@see dvpGetRoiState
*@see dvpSetRoi
*@see dvpGetRoi
*/
dvp2api dvpStatus dvpGetRoiDescr(dvpHandle handle, dvpRegionDescr* pRoiDescr); 


/**
*@brief Get the enabling status of the image Roi(relative to the entire sensor region) function
*@param[in] handle Camera handle
*@param[out] pRoiState Image Roi(relative to the whole sensor region) function enable state
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_SIZE
*@see dvpGetResolutionModeSel
*@see dvpGetQuickRoiSel
*@see dvpSetRoi
*@see dvpGetRoi
*@see dvpGetRoiDescr
*@see dvpSetRoiState
*/
dvp2api dvpStatus dvpGetRoiState(dvpHandle handle, bool* pRoiState); 


/**
*@brief Set the enabling status of the image Roi(relative to the entire sensor region) function
*@param[in] handle Camera handle
*@param[in] RoiState Image Roi(relative to the entire sensor region) function enable state
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_SIZE
*@see dvpGetResolutionModeSel
*@see dvpGetQuickRoiSel
*@see dvpSetRoi
*@see dvpGetRoi
*@see dvpGetRoiDescr
*@see dvpGetRoiState
*/
dvp2api dvpStatus dvpSetRoiState(dvpHandle handle, bool RoiState); 


/**
*@brief Get the value of color temperature(unit:K)
*@param[in] handle Camera handle
*@param[out] pColorTemperature
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_COLOR
*@see dvpSetColorTemperatureState
*@see dvpGetColorTemperatureState
*@see dvpGetColorTemperatureDescr
*@see dvpSetColorTemperature
*@note Adjust the warm and cool colors
*/
dvp2api dvpStatus dvpGetColorTemperature(dvpHandle handle, dvpInt32* pColorTemperature); 


/**
*@brief Set the value of color temperature(in K)
*@param[in] handle Camera handle
*@param[in] ColorTemperature
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_COLOR
*@see dvpSetColorTemperatureState
*@see dvpGetColorTemperatureState
*@see dvpGetColorTemperatureDescr
*@see dvpGetColorTemperature
*@note Adjust the warm and cool colors
*/
dvp2api dvpStatus dvpSetColorTemperature(dvpHandle handle, dvpInt32 ColorTemperature); 


/**
*@brief Get description information of color temperature
*@param[in] handle Camera handle
*@param[out] pColorTemperatureDescr Description of color temperature
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_COLOR
*@see dvpSetColorTemperatureState
*@see dvpGetColorTemperatureState
*@see dvpSetColorTemperature
*@see dvpGetColorTemperature
*/
dvp2api dvpStatus dvpGetColorTemperatureDescr(dvpHandle handle, dvpIntDescr* pColorTemperatureDescr); 


/**
*@brief Get the enabling status of the color temperature function
*@param[in] handle Camera handle
*@param[out] pColorTemperatureState enabling state of color temperature function
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_COLOR
*@see dvpSetColorTemperature
*@see dvpGetColorTemperature
*@see dvpGetColorTemperatureDescr
*@see dvpSetColorTemperatureState
*/
dvp2api dvpStatus dvpGetColorTemperatureState(dvpHandle handle, bool* pColorTemperatureState); 


/**
*@brief Set the enabling state of the color temperature function
*@param[in] handle Camera handle
*@param[in] ColorTemperatureState Enable state of color temperature function
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_COLOR
*@see dvpSetColorTemperature
*@see dvpGetColorTemperature
*@see dvpGetColorTemperatureDescr
*@see dvpGetColorTemperatureState
*/
dvp2api dvpStatus dvpSetColorTemperatureState(dvpHandle handle, bool ColorTemperatureState); 


/**
*@brief Get the sharpness value
*@param[in] handle Camera handle
*@param[out] pSharpness
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetNoiseReduct2d
*@see dvpSetSharpnessState
*@see dvpGetSharpnessState
*@see dvpGetSharpnessDescr
*@see dvpSetSharpness
*@note Adjust sharpness
*/
dvp2api dvpStatus dvpGetSharpness(dvpHandle handle, dvpInt32* pSharpness); 


/**
*@brief Set the sharpness value
*@param[in] handle Camera handle
*@param[in] Sharpness value
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetNoiseReduct2d
*@see dvpSetSharpnessState
*@see dvpGetSharpnessState
*@see dvpGetSharpnessDescr
*@see dvpGetSharpness
*@note Adjust sharpness
*/
dvp2api dvpStatus dvpSetSharpness(dvpHandle handle, dvpInt32 Sharpness); 


/**
*@brief Get the description information of sharpness
*@param[in] handle Camera handle
*@param[out] pSharpnessDescr Description of sharpness
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetNoiseReduct2d
*@see dvpSetSharpnessState
*@see dvpGetSharpnessState
*@see dvpSetSharpness
*@see dvpGetSharpness
*/
dvp2api dvpStatus dvpGetSharpnessDescr(dvpHandle handle, dvpIntDescr* pSharpnessDescr); 


/**
*@brief Get the enabling state of sharpness function
*@param[in] handle Camera handle
*@param[out] pSharpnessState enabling state of sharpness function
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetNoiseReduct2d
*@see dvpSetSharpness
*@see dvpGetSharpness
*@see dvpGetSharpnessDescr
*@see dvpSetSharpnessState
*/
dvp2api dvpStatus dvpGetSharpnessState(dvpHandle handle, bool* pSharpnessState); 


/**
*@brief Set the enabling status of sharpness function
*@param[in] handle Camera handle
*@param[in] SharpnessState enabling state of sharpness function
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetNoiseReduct2d
*@see dvpSetSharpness
*@see dvpGetSharpness
*@see dvpGetSharpnessDescr
*@see dvpGetSharpnessState
*/
dvp2api dvpStatus dvpSetSharpnessState(dvpHandle handle, bool SharpnessState); 


/**
*@brief Get the value of saturation
*@param[in] handle Camera handle
*@param[out] pSaturation
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_COLOR
*@see dvpSetSaturationState
*@see dvpGetSaturationState
*@see dvpGetSaturationDescr
*@see dvpSetSaturation
*@note Adjust image saturation
*/
dvp2api dvpStatus dvpGetSaturation(dvpHandle handle, dvpInt32* pSaturation); 


/**
*@brief Set the value of saturation
*@param[in] handle Camera handle
*@param[in] Saturation
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_COLOR
*@see dvpSetSaturationState
*@see dvpGetSaturationState
*@see dvpGetSaturationDescr
*@see dvpGetSaturation
*@note Adjust image saturation
*/
dvp2api dvpStatus dvpSetSaturation(dvpHandle handle, dvpInt32 Saturation); 


/**
*@brief Get the description information of saturation
*@param[in] handle Camera handle
*@param[out] pSaturationDescr saturation description information
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_COLOR
*@see dvpSetSaturationState
*@see dvpGetSaturationState
*@see dvpSetSaturation
*@see dvpGetSaturation
*/
dvp2api dvpStatus dvpGetSaturationDescr(dvpHandle handle, dvpIntDescr* pSaturationDescr); 


/**
*@brief Get the enabling status of saturation function
*@param[in] handle Camera handle
*@param[out] pSaturationState enabling state of saturation function
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_COLOR
*@see dvpSetSaturation
*@see dvpGetSaturation
*@see dvpGetSaturationDescr
*@see dvpSetSaturationState
*/
dvp2api dvpStatus dvpGetSaturationState(dvpHandle handle, bool* pSaturationState); 


/**
*@brief Set the enabling status of saturation function
*@param[in] handle Camera handle
*@param[in] SaturationState Enable state of saturation function
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_COLOR
*@see dvpSetSaturation
*@see dvpGetSaturation
*@see dvpGetSaturationDescr
*@see dvpGetSaturationState
*/
dvp2api dvpStatus dvpSetSaturationState(dvpHandle handle, bool SaturationState); 


/**
*@brief Get 2D noise reduction value
*@param[in] handle Camera handle
*@param[out] pNoiseReduct2d 2D noise reduction value
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetSharpness
*@see dvpGetNoiseReduct3d
*@see dvpSetNoiseReduct2dState
*@see dvpGetNoiseReduct2dState
*@see dvpGetNoiseReduct2dDescr
*@see dvpSetNoiseReduct2d
*@note smoothes the image, but affects the sharpness
*/
dvp2api dvpStatus dvpGetNoiseReduct2d(dvpHandle handle, dvpInt32* pNoiseReduct2d); 


/**
*@brief Set 2D noise reduction value
*@param[in] handle Camera handle
*@param[in] NoiseReduct2d 2D noise reduction value
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetSharpness
*@see dvpGetNoiseReduct3d
*@see dvpSetNoiseReduct2dState
*@see dvpGetNoiseReduct2dState
*@see dvpGetNoiseReduct2dDescr
*@see dvpGetNoiseReduct2d
*@note smoothes the image, but affects the sharpness
*/
dvp2api dvpStatus dvpSetNoiseReduct2d(dvpHandle handle, dvpInt32 NoiseReduct2d); 


/**
*@brief Get the description information of 2D noise reduction
*@param[in] handle Camera handle
*@param[out] pNoiseReduct2dDescr Description of 2D noise reduction
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetSharpness
*@see dvpGetNoiseReduct3d
*@see dvpSetNoiseReduct2dState
*@see dvpGetNoiseReduct2dState
*@see dvpSetNoiseReduct2d
*@see dvpGetNoiseReduct2d
*/
dvp2api dvpStatus dvpGetNoiseReduct2dDescr(dvpHandle handle, dvpIntDescr* pNoiseReduct2dDescr); 


/**
*@brief Get the enabling status of 2D noise reduction function
*@param[in] handle Camera handle
*@param[out] pNoiseReduct2dState Enable state of 2D noise reduction function
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetSharpness
*@see dvpGetNoiseReduct3d
*@see dvpSetNoiseReduct2d
*@see dvpGetNoiseReduct2d
*@see dvpGetNoiseReduct2dDescr
*@see dvpSetNoiseReduct2dState
*/
dvp2api dvpStatus dvpGetNoiseReduct2dState(dvpHandle handle, bool* pNoiseReduct2dState); 


/**
*@brief Set the enabling status of 2D noise reduction function
*@param[in] handle Camera handle
*@param[in] NoiseReduct2dState Enable state of 2D noise reduction function
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetSharpness
*@see dvpGetNoiseReduct3d
*@see dvpSetNoiseReduct2d
*@see dvpGetNoiseReduct2d
*@see dvpGetNoiseReduct2dDescr
*@see dvpGetNoiseReduct2dState
*/
dvp2api dvpStatus dvpSetNoiseReduct2dState(dvpHandle handle, bool NoiseReduct2dState); 


/**
*@brief Get the value of 3D noise reduction
*@param[in] handle Camera handle
*@param[out] pNoiseReduct3d 3D noise reduction value
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetNoiseReduct2d
*@see dvpSetNoiseReduct3dState
*@see dvpGetNoiseReduct3dState
*@see dvpGetNoiseReduct3dDescr
*@see dvpSetNoiseReduct3d
*@note has good noise reduction effect, but may cause motion blur.
*/
dvp2api dvpStatus dvpGetNoiseReduct3d(dvpHandle handle, dvpInt32* pNoiseReduct3d); 


/**
*@brief Set the value of 3D noise reduction
*@param[in] handle Camera handle
*@param[in] NoiseReduct3d 3D noise reduction value
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetNoiseReduct2d
*@see dvpSetNoiseReduct3dState
*@see dvpGetNoiseReduct3dState
*@see dvpGetNoiseReduct3dDescr
*@see dvpGetNoiseReduct3d
*@note has good noise reduction effect, but may cause motion blur.
*/
dvp2api dvpStatus dvpSetNoiseReduct3d(dvpHandle handle, dvpInt32 NoiseReduct3d); 


/**
*@brief Get the description information of 3D noise reduction
*@param[in] handle Camera handle
*@param[out] pNoiseReduct3dDescr Description of 3D noise reduction
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetNoiseReduct2d
*@see dvpSetNoiseReduct3dState
*@see dvpGetNoiseReduct3dState
*@see dvpSetNoiseReduct3d
*@see dvpGetNoiseReduct3d
*/
dvp2api dvpStatus dvpGetNoiseReduct3dDescr(dvpHandle handle, dvpIntDescr* pNoiseReduct3dDescr); 


/**
*@brief Get the enabling status of 3D noise reduction function
*@param[in] handle Camera handle
*@param[out] pNoiseReduct3dState Enable state of 3D noise reduction function
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetNoiseReduct2d
*@see dvpSetNoiseReduct3d
*@see dvpGetNoiseReduct3d
*@see dvpGetNoiseReduct3dDescr
*@see dvpSetNoiseReduct3dState
*/
dvp2api dvpStatus dvpGetNoiseReduct3dState(dvpHandle handle, bool* pNoiseReduct3dState); 


/**
*@brief Set the enabling status of 3D noise reduction function
*@param[in] handle Camera handle
*@param[in] NoiseReduct3dState Enable state of 3D noise reduction function
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetNoiseReduct2d
*@see dvpSetNoiseReduct3d
*@see dvpGetNoiseReduct3d
*@see dvpGetNoiseReduct3dDescr
*@see dvpGetNoiseReduct3dState
*/
dvp2api dvpStatus dvpSetNoiseReduct3dState(dvpHandle handle, bool NoiseReduct3dState); 


/**
*@brief Get the value of the black level
*@param[in] handle Camera handle
*@param[out] pBlackLevel black field value
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetContrast
*@see dvpSetBlackLevelState
*@see dvpGetBlackLevelState
*@see dvpGetBlackLevelDescr
*@see dvpSetBlackLevel
*@note By combining contrast, transparency can be adjusted more effectively.
*/
dvp2api dvpStatus dvpGetBlackLevel(dvpHandle handle, float* pBlackLevel); 


/**
*@brief Set the value of the black level
*@param[in] handle Camera handle
*@param[in] BlackLevel
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetContrast
*@see dvpSetBlackLevelState
*@see dvpGetBlackLevelState
*@see dvpGetBlackLevelDescr
*@see dvpGetBlackLevel
*@note By combining contrast, transparency can be adjusted more effectively.
*/
dvp2api dvpStatus dvpSetBlackLevel(dvpHandle handle, float BlackLevel); 


/**
*@brief Get the description information of the black level
*@param[in] handle Camera handle
*@param[out] pBlackLevelDescr Description of the black level
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetContrast
*@see dvpSetBlackLevelState
*@see dvpGetBlackLevelState
*@see dvpSetBlackLevel
*@see dvpGetBlackLevel
*/
dvp2api dvpStatus dvpGetBlackLevelDescr(dvpHandle handle, dvpFloatDescr* pBlackLevelDescr); 


/**
*@brief Get the enabling status of the black level function
*@param[in] handle Camera handle
*@param[out] pBlackLevelState The enabling state of the black level function
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetContrast
*@see dvpSetBlackLevel
*@see dvpGetBlackLevel
*@see dvpGetBlackLevelDescr
*@see dvpSetBlackLevelState
*/
dvp2api dvpStatus dvpGetBlackLevelState(dvpHandle handle, bool* pBlackLevelState); 


/**
*@brief Set the enabling status of the black level function
*@param[in] handle Camera handle
*@param[in] BlackLevelState The enabling state of the black level function
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetContrast
*@see dvpSetBlackLevel
*@see dvpGetBlackLevel
*@see dvpGetBlackLevelDescr
*@see dvpGetBlackLevelState
*/
dvp2api dvpStatus dvpSetBlackLevelState(dvpHandle handle, bool BlackLevelState); 


/**
*@brief Get the cycle value of the loop trigger(unit:microseconds)
*@param[in] handle Camera handle
*@param[out] pSoftTriggerLoop Cycle triggered cycle value(unit:microseconds)
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpSetSoftTriggerLoopState
*@see dvpGetSoftTriggerLoopState
*@see dvpGetSoftTriggerLoopDescr
*@see dvpSetSoftTriggerLoop
*/
dvp2api dvpStatus dvpGetSoftTriggerLoop(dvpHandle handle, double* pSoftTriggerLoop); 


/**
*@brief Set the cycle value of loop trigger(unit:microseconds)
*@param[in] handle Camera handle
*@param[in] SoftTriggerLoop Cycle value triggered by SoftTriggerLoop cycle(unit:microseconds)
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpSetSoftTriggerLoopState
*@see dvpGetSoftTriggerLoopState
*@see dvpGetSoftTriggerLoopDescr
*@see dvpGetSoftTriggerLoop
*/
dvp2api dvpStatus dvpSetSoftTriggerLoop(dvpHandle handle, double SoftTriggerLoop); 


/**
*@brief Get the description of the loop trigger
*@param[in] handle Camera handle
*@param[out] pSoftTriggerLoopDescr Description of the loop trigger
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpSetSoftTriggerLoopState
*@see dvpGetSoftTriggerLoopState
*@see dvpSetSoftTriggerLoop
*@see dvpGetSoftTriggerLoop
*/
dvp2api dvpStatus dvpGetSoftTriggerLoopDescr(dvpHandle handle, dvpDoubleDescr* pSoftTriggerLoopDescr); 


/**
*@brief Get the enabling status of the loop trigger function
*@param[in] handle Camera handle
*@param[out] pSoftTriggerLoopState Enable state of loop trigger function
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpSetSoftTriggerLoop
*@see dvpGetSoftTriggerLoop
*@see dvpGetSoftTriggerLoopDescr
*@see dvpSetSoftTriggerLoopState
*/
dvp2api dvpStatus dvpGetSoftTriggerLoopState(dvpHandle handle, bool* pSoftTriggerLoopState); 


/**
*@brief Set the enabling status of the loop trigger function
*@param[in] handle Camera handle
*@param[in] SoftTriggerLoopState Enable state of loop trigger function
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpSetSoftTriggerLoop
*@see dvpGetSoftTriggerLoop
*@see dvpGetSoftTriggerLoopDescr
*@see dvpGetSoftTriggerLoopState
*/
dvp2api dvpStatus dvpSetSoftTriggerLoopState(dvpHandle handle, bool SoftTriggerLoopState); 


/**
*@brief Get the number of grabbed multi frames
*@param[in] handle Camera handle
*@param[out] pMultiFrames
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ADVANCED
*@see dvpSetMultiFramesState
*@see dvpGetMultiFramesState
*@see dvpGetMultiFramesDescr
*@see dvpSetMultiFrames
*@note When MultiFrames is 1, it is equivalent to grab a single frame
*/
dvp2api dvpStatus dvpGetMultiFrames(dvpHandle handle, dvpUint64* pMultiFrames); 


/**
*@brief Set the number of grabbed multi frames
*@param[in] handle Camera handle
*@param[in] MultiFrames
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ADVANCED
*@see dvpSetMultiFramesState
*@see dvpGetMultiFramesState
*@see dvpGetMultiFramesDescr
*@see dvpGetMultiFrames
*@note When MultiFrames is 1, it is equivalent to grab a single frame
*/
dvp2api dvpStatus dvpSetMultiFrames(dvpHandle handle, dvpUint64 MultiFrames); 


/**
*@brief Get the description information of grabbed multi frames
*@param[in] handle Camera handle
*@param[out] pMultiFramesDescr Description of multi frame acquisition
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ADVANCED
*@see dvpSetMultiFramesState
*@see dvpGetMultiFramesState
*@see dvpSetMultiFrames
*@see dvpGetMultiFrames
*/
dvp2api dvpStatus dvpGetMultiFramesDescr(dvpHandle handle, dvpUint64Descr* pMultiFramesDescr); 


/**
*@brief Get the enabling status of the multi frames grabbed function
*@param[in] handle Camera handle
*@param[out] pMultiFramesState The enabling state of the multi frame grab function
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ADVANCED
*@see dvpSetMultiFrames
*@see dvpGetMultiFrames
*@see dvpGetMultiFramesDescr
*@see dvpSetMultiFramesState
*/
dvp2api dvpStatus dvpGetMultiFramesState(dvpHandle handle, bool* pMultiFramesState); 


/**
*@brief Set the enabling status of the multi frames grabbed function
*@param[in] handle Camera handle
*@param[in] MultiFramesState The enabling state of the multi frame acquisition function
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ADVANCED
*@see dvpSetMultiFrames
*@see dvpGetMultiFrames
*@see dvpGetMultiFramesDescr
*@see dvpGetMultiFramesState
*/
dvp2api dvpStatus dvpSetMultiFramesState(dvpHandle handle, bool MultiFramesState); 


/**
*@brief  get the transmission interval time of the data packets(unit:microseconds)
*@param[in] handle Camera handle
*@param[out] pStreamPackInterval The value of the data transmission packet interval(in microseconds)
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_PERFORMANCE
*@see dvpGetStreamPackIntervalDescr
*@see dvpSetStreamPackInterval
*@note Large interval can save network bandwidth
*/
dvp2api dvpStatus dvpGetStreamPackInterval(dvpHandle handle, dvpInt32* pStreamPackInterval); 


/**
*@brief Set the transmission interval time of the data packets(unit:microseconds)
*@param[in] handle Camera handle
*@param[in] StreamPackInterval The value of the data transmission packet interval(in microseconds)
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_PERFORMANCE
*@see dvpGetStreamPackIntervalDescr
*@see dvpGetStreamPackInterval
*@note Large interval can save network bandwidth
*/
dvp2api dvpStatus dvpSetStreamPackInterval(dvpHandle handle, dvpInt32 StreamPackInterval); 


/**
*@brief Get the description information of the transmission interval time of the data packets
*@param[in] handle Camera handle
*@param[out] pStreamPackIntervalDescr Description of transmission interval time of the data packets
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_PERFORMANCE
*@see dvpSetStreamPackInterval
*@see dvpGetStreamPackInterval
*/
dvp2api dvpStatus dvpGetStreamPackIntervalDescr(dvpHandle handle, dvpIntDescr* pStreamPackIntervalDescr); 


/**
*@brief Get the value of data transmission packet length(in bytes)
*@param[in] handle Camera handle
*@param[out] pStreamPackSize The value of data transmission packet length(in bytes)
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_PERFORMANCE
*@see dvpGetStreamPackSizeDescr
*@see dvpSetStreamPackSize
*@note If supported by computer hardware, data transmission efficiency can be improved
*@note It is recommended to use typical transmission packet length @link #dvpGetPixelRateSel @endlink
*/
dvp2api dvpStatus dvpGetStreamPackSize(dvpHandle handle, dvpInt32* pStreamPackSize); 


/**
*@brief Set the value of data transmission packet length(in bytes)
*@param[in] handle Camera handle
*@param[in] StreamPackSize The value of the data transmission packet length(in bytes)
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_PERFORMANCE
*@see dvpGetStreamPackSizeDescr
*@see dvpGetStreamPackSize
*@note If supported by computer hardware, data transmission efficiency can be improved
*@note It is recommended to use typical transmission packet length@link #dvpGetPixelRateSel @endlink
*/
dvp2api dvpStatus dvpSetStreamPackSize(dvpHandle handle, dvpInt32 StreamPackSize); 


/**
*@brief Get the description information of data transmission packet length
*@param[in] handle Camera handle
*@param[out] pStreamPackSizeDescr Description of data transmission packet length
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_PERFORMANCE
*@see dvpSetStreamPackSize
*@see dvpGetStreamPackSize
*/
dvp2api dvpStatus dvpGetStreamPackSizeDescr(dvpHandle handle, dvpIntDescr* pStreamPackSizeDescr); 


/**
*@brief Get the value of AE target
*@param[in] handle Camera handle
*@param[out] pAeTarget
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_EXPOSURE
*@see dvpGetAeOperation
*@see dvpGetAeTargetDescr
*@see dvpSetAeTarget
*@note The final brightness to be achieved by adjusting the exposure time and analog gain @link #dvpGetExposure @endlink和@link #dvpGetAnalogGain @endlink
*/
dvp2api dvpStatus dvpGetAeTarget(dvpHandle handle, dvpInt32* pAeTarget); 


/**
*@brief Set the value of AE target
*@param[in] handle Camera handle
*@param[in] AeTarget
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_EXPOSURE
*@see dvpGetAeOperation
*@see dvpGetAeTargetDescr
*@see dvpGetAeTarget
*@note The final brightness to be achieved by adjusting the exposure time and analog gain @link #dvpGetExposure @endlink和@link #dvpGetAnalogGain @endlink
*/
dvp2api dvpStatus dvpSetAeTarget(dvpHandle handle, dvpInt32 AeTarget); 


/**
*@brief Get the description information of the AE target
*@param[in] handle Camera handle
*@param[out] pAeTargetDescr Description information of AE target
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_EXPOSURE
*@see dvpGetAeOperation
*@see dvpSetAeTarget
*@see dvpGetAeTarget
*/
dvp2api dvpStatus dvpGetAeTargetDescr(dvpHandle handle, dvpIntDescr* pAeTargetDescr); 


/**
*@brief Get the value of analog gain
*@param[in] handle Camera handle
*@param[out] pAnalogGain
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_EXPOSURE
*@see dvpGetExposure
*@see dvpGetAnalogGainDescr
*@see dvpSetAnalogGain
*@note The greater the analog gain, the greater the noise
*/
dvp2api dvpStatus dvpGetAnalogGain(dvpHandle handle, float* pAnalogGain); 


/**
*@brief Set the value of analog gain
*@param[in] handle Camera handle
*@param[in] AnalogGain
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_EXPOSURE
*@see dvpGetExposure
*@see dvpGetAnalogGainDescr
*@see dvpGetAnalogGain
*@note The greater the analog gain, the greater the noise
*/
dvp2api dvpStatus dvpSetAnalogGain(dvpHandle handle, float AnalogGain); 


/**
*@brief Get description information of analog gain
*@param[in] handle Camera handle
*@param[out] pAnalogGainDescr Description of analog gain
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_EXPOSURE
*@see dvpGetExposure
*@see dvpSetAnalogGain
*@see dvpGetAnalogGain
*/
dvp2api dvpStatus dvpGetAnalogGainDescr(dvpHandle handle, dvpFloatDescr* pAnalogGainDescr); 


/**
*@brief Get the value of exposure time(in microseconds)
*@param[in] handle Camera handle
*@param[out] pExposure value(in microseconds)
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_EXPOSURE
*@see dvpGetAnalogGain
*@see dvpGetExposureDescr
*@see dvpSetExposure
*@note The longer the exposure time is, the lower the frame rate may be
*/
dvp2api dvpStatus dvpGetExposure(dvpHandle handle, double* pExposure); 


/**
*@brief Set the value of exposure time(in microseconds)
*@param[in] handle Camera handle
*@param[in] Exposure value(in microseconds)
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_EXPOSURE
*@see dvpGetAnalogGain
*@see dvpGetExposureDescr
*@see dvpGetExposure
*@note The longer the exposure time is, the lower the frame rate may be
*/
dvp2api dvpStatus dvpSetExposure(dvpHandle handle, double Exposure); 


/**
*@brief Get the description information of the exposure time
*@param[in] handle Camera handle
*@param[out] pExposureDescr Description of exposure time
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_EXPOSURE
*@see dvpGetAnalogGain
*@see dvpSetExposure
*@see dvpGetExposure
*/
dvp2api dvpStatus dvpGetExposureDescr(dvpHandle handle, dvpDoubleDescr* pExposureDescr); 


/**
*@brief Get the value of the external trigger signal debounce time(unit:microseconds)
*@param[in] handle Camera handle
*@param[out] pTriggerJitterFilter The value of the external trigger signal debounce time(in microseconds)
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpGetTriggerJitterFilterDescr
*@see dvpSetTriggerJitterFilter
*@note There may be a certain voltage fluctuation at the moment when the external electrical signal is triggered, so it is necessary to set an appropriate debounce time to ensure that the signal is stable
*/
dvp2api dvpStatus dvpGetTriggerJitterFilter(dvpHandle handle, double* pTriggerJitterFilter); 


/**
*@brief Set the value of the external trigger signal's debounce time(unit:microseconds)
*@param[in] handle Camera handle
*@param[in] TriggerJitterFilter The value of the external trigger signal's debounce time(in microseconds)
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpGetTriggerJitterFilterDescr
*@see dvpGetTriggerJitterFilter
*@note There may be a certain voltage fluctuation at the moment when the external electrical signal is triggered, so it is necessary to set an appropriate debounce time to ensure that the signal is stable
*/
dvp2api dvpStatus dvpSetTriggerJitterFilter(dvpHandle handle, double TriggerJitterFilter); 


/**
*@brief Get the description information of the external trigger signal's debounce time
*@param[in] handle Camera handle
*@param[out] pTriggerJitterFilterDescr Description information of external trigger signal's debounce time
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpSetTriggerJitterFilter
*@see dvpGetTriggerJitterFilter
*/
dvp2api dvpStatus dvpGetTriggerJitterFilterDescr(dvpHandle handle, dvpDoubleDescr* pTriggerJitterFilterDescr); 


/**
*@brief Get the value of trigger delay(unit:microseconds)
*@param[in] handle Camera handle
*@param[out] pTriggerDelay
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpGetTriggerDelayDescr
*@see dvpSetTriggerDelay
*@note After receiving a valid trigger signal, delay the response
*/
dvp2api dvpStatus dvpGetTriggerDelay(dvpHandle handle, double* pTriggerDelay); 


/**
*@brief Set the value of trigger delay(unit:microseconds)
*@param[in] handle Camera handle
*@param[in] TriggerDelay
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpGetTriggerDelayDescr
*@see dvpGetTriggerDelay
*@note After receiving a valid trigger signal, delay the response
*/
dvp2api dvpStatus dvpSetTriggerDelay(dvpHandle handle, double TriggerDelay); 


/**
*@brief Get the description of the trigger delay
*@param[in] handle Camera handle
*@param[out] pTriggerDelayDescr Description of trigger delay
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpSetTriggerDelay
*@see dvpGetTriggerDelay
*/
dvp2api dvpStatus dvpGetTriggerDelayDescr(dvpHandle handle, dvpDoubleDescr* pTriggerDelayDescr); 


/**
*@brief Get the delay value of Strobe signal(unit:microseconds)
*@param[in] handle Camera handle
*@param[out] pStrobeDelay The delay value of Strobe signal(unit:microseconds)
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpGetStrobeDelayDescr
*@see dvpSetStrobeDelay
*@note Strobe delay response
*/
dvp2api dvpStatus dvpGetStrobeDelay(dvpHandle handle, double* pStrobeDelay); 


/**
*@brief Set the delay value of Strobe signal(unit:microseconds)
*@param[in] handle Camera handle
*@param[in] StrobeDelay The delay value of Strobe signal(unit: microseconds)
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpGetStrobeDelayDescr
*@see dvpGetStrobeDelay
*@note Strobe delay response
*/
dvp2api dvpStatus dvpSetStrobeDelay(dvpHandle handle, double StrobeDelay); 


/**
*@brief Get the description information of Strobe signal delay
*@param[in] handle Camera handle
*@param[out] pStrobeDelayDescr Description of delay of Strobe signal
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpSetStrobeDelay
*@see dvpGetStrobeDelay
*/
dvp2api dvpStatus dvpGetStrobeDelayDescr(dvpHandle handle, dvpDoubleDescr* pStrobeDelayDescr); 


/**
*@brief Get the duration value of Strobe signal(unit: microseconds)
*@param[in] handle Camera handle
*@param[out] pStrobeDuration value of Strobe signal duration(unit: microseconds)
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpGetStrobeDurationDescr
*@see dvpSetStrobeDuration
*@note Strobe action time
*/
dvp2api dvpStatus dvpGetStrobeDuration(dvpHandle handle, double* pStrobeDuration); 


/**
*@brief Set the value of Strobe signal duration(unit: microseconds)
*@param[in] handle Camera handle
*@param[in] StrobeDuration value of Strobe signal duration(unit: microseconds)
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpGetStrobeDurationDescr
*@see dvpGetStrobeDuration
*@note Strobe action time
*/
dvp2api dvpStatus dvpSetStrobeDuration(dvpHandle handle, double StrobeDuration); 


/**
*@brief Get the description of the duration of Strobe signal
*@param[in] handle Camera handle
*@param[out] pStrobeDurationDescr Description of duration of Strobe signal
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpSetStrobeDuration
*@see dvpGetStrobeDuration
*/
dvp2api dvpStatus dvpGetStrobeDurationDescr(dvpHandle handle, dvpDoubleDescr* pStrobeDurationDescr); 


/**
*@brief Get the number of output frames for once trigger
*@param[in] handle Camera handle
*@param[out] pFramesPerTrigger the number of output frames for once trigger
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpGetFramesPerTriggerDescr
*@see dvpSetFramesPerTrigger
*@note uses a trigger signal to output multiple frame
*/
dvp2api dvpStatus dvpGetFramesPerTrigger(dvpHandle handle, dvpInt32* pFramesPerTrigger); 


/**
*@brief Set the number of output frames for once trigger
*@param[in] handle Camera handle
*@param[in] FramesPerTrigger the number of output frames for once trigger
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpGetFramesPerTriggerDescr
*@see dvpGetFramesPerTrigger
*@note uses a trigger signal to output multiple images
*/
dvp2api dvpStatus dvpSetFramesPerTrigger(dvpHandle handle, dvpInt32 FramesPerTrigger); 


/**
*@brief Get the description of the output frames for once trigger
*@param[in] handle Camera handle
*@param[out] pFramesPerTriggerDescr Description of the output frames for once trigger
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpSetFramesPerTrigger
*@see dvpGetFramesPerTrigger
*/
dvp2api dvpStatus dvpGetFramesPerTriggerDescr(dvpHandle handle, dvpIntDescr* pFramesPerTriggerDescr); 


/**
*@brief Get the value of the timer in the camera(in microseconds)
*@param[in] handle Camera handle
*@param[out] pTimerValue The value of the timer in the camera(in microseconds)
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ADVANCED
*@see dvpGetTimerValueDescr
*@see dvpSetTimerValue
*/
dvp2api dvpStatus dvpGetTimerValue(dvpHandle handle, double* pTimerValue); 


/**
*@brief Set the value of the timer in the camera(in microseconds)
*@param[in] handle Camera handle
*@param[in] TimerValue The value of the timer in the camera(in microseconds)
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ADVANCED
*@see dvpGetTimerValueDescr
*@see dvpGetTimerValue
*/
dvp2api dvpStatus dvpSetTimerValue(dvpHandle handle, double TimerValue); 


/**
*@brief Get the description information of the timer in the camera
*@param[in] handle Camera handle
*@param[out] pTimerValueDescr Description of the timer in the camera
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ADVANCED
*@see dvpSetTimerValue
*@see dvpGetTimerValue
*/
dvp2api dvpStatus dvpGetTimerValueDescr(dvpHandle handle, dvpDoubleDescr* pTimerValueDescr); 


/**
*@brief Get the buffer queue size
*@param[in] handle Camera handle
*@param[out] pBufferQueueSize buffer queue size
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ADVANCED
*@see dvpGetBufferQueueSizeDescr
*@see dvpSetBufferQueueSize
*@see dvpGetBufferConfig
*@warning may consume some memory space. Please set it according to the available memory of the system
*@warning This operation is recommended to be set before starts the video stream@link #dvpStart @endlink
*/
dvp2api dvpStatus dvpGetBufferQueueSize(dvpHandle handle, dvpInt32* pBufferQueueSize); 


/**
*@brief Set buffer queue size
*@param[in] handle Camera handle
*@param[in] BufferQueueSize buffer queue Size
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ADVANCED
*@see dvpGetBufferQueueSizeDescr
*@see dvpGetBufferQueueSize
*@see dvpGetBufferConfig
*@warning may consume some memory space. Please set it according to the available memory of the system
*@warning This operation is recommended to be set before starts the video stream@link #dvpStart @endlink
*/
dvp2api dvpStatus dvpSetBufferQueueSize(dvpHandle handle, dvpInt32 BufferQueueSize); 


/**
*@brief Get the description of the buffer queue size
*@param[in] handle Camera handle
*@param[out] pBufferQueueSizeDescr Description of buffer queue size
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ADVANCED
*@see dvpSetBufferQueueSize
*@see dvpGetBufferQueueSize
*/
dvp2api dvpStatus dvpGetBufferQueueSizeDescr(dvpHandle handle, dvpIntDescr* pBufferQueueSizeDescr); 


/**
*@brief get line frequency of line scan camera
*@param[in] handle Camera handle
*@param[out] pLineRate line scan camera's line frequency
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_LINESCAN
*@see dvpGetLineRateDescr
*@see dvpSetLineRate
*/
dvp2api dvpStatus dvpGetLineRate(dvpHandle handle, double* pLineRate); 


/**
*@brief Set line frequency of line scan camera
*@param[in] handle Camera handle
*@param[in] LineRate line scan camera's line frequency
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_LINESCAN
*@see dvpGetLineRateDescr
*@see dvpGetLineRate
*/
dvp2api dvpStatus dvpSetLineRate(dvpHandle handle, double LineRate); 


/**
*@brief get the description information of line scan camera's line frequency
*@param[in] handle Camera handle
*@param[out] pLineRateDescr the description information of line scan camera's line frequency
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_LINESCAN
*@see dvpSetLineRate
*@see dvpGetLineRate
*/
dvp2api dvpStatus dvpGetLineRateDescr(dvpHandle handle, dvpDoubleDescr* pLineRateDescr); 


/**
*@brief Get the trigger source of the camera
*@param[in] handle Camera handle
*@param[out] pTriggerSource Trigger source of camera
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpSetTriggerSource
*/
dvp2api dvpStatus dvpGetTriggerSource(dvpHandle handle, dvpTriggerSource* pTriggerSource); 


/**
*@brief Set the trigger source of the camera
*@param[in] handle Camera handle
*@param[in] TriggerSource Trigger Source
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpGetTriggerSource
*/
dvp2api dvpStatus dvpSetTriggerSource(dvpHandle handle, dvpTriggerSource TriggerSource); 


/**
*@brief Get LUT curve
*@param[in] handle Camera handle
*@param[out] pCurveLut LUT curve
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpSetCurveLut
*@note Fit a continuous mapping curve based on a few anchor points
*/
dvp2api dvpStatus dvpGetCurveLut(dvpHandle handle, dvpCurveLut* pCurveLut); 


/**
*@brief Set LUT curve
*@param[in] handle Camera handle
*@param[in] CurveLut LUT curve
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetCurveLut
*@note Fit a continuous mapping curve based on a few anchor points
*/
dvp2api dvpStatus dvpSetCurveLut(dvpHandle handle, dvpCurveLut CurveLut); 


/**
*@brief Get color correction data
*@param[in] handle Camera handle
*@param[out] pColorCorrection Color correction data
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_CORRECTION
*@see dvpSetColorCorrection
*@note The consistency of color correction data is a necessary condition to ensure the consistency of color camera color restoration effect
*/
dvp2api dvpStatus dvpGetColorCorrection(dvpHandle handle, dvpColorCorrection* pColorCorrection); 


/**
*@brief Set color correction data
*@param[in] handle Camera handle
*@param[in] ColorCorrection Color correction data
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_CORRECTION
*@see dvpGetColorCorrection
*@note The consistency of color correction data is a necessary condition to ensure the consistency of color camera color restoration effect
*/
dvp2api dvpStatus dvpSetColorCorrection(dvpHandle handle, dvpColorCorrection ColorCorrection); 


/**
*@brief Get frame stream status
*@param[in] handle Camera handle
*@param[out] pStreamState frame stream status
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ADVANCED
*@see dvpStart
*@see dvpStop
*@see dvpSetStreamState
*/
dvp2api dvpStatus dvpGetStreamState(dvpHandle handle, dvpStreamState* pStreamState); 


/**
*@brief Set the frame stream status
*@param[in] handle Camera handle
*@param[in] StreamState frame stream status
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ADVANCED
*@see dvpStart
*@see dvpStop
*@see dvpGetStreamState
*/
dvp2api dvpStatus dvpSetStreamState(dvpHandle handle, dvpStreamState StreamState); 


/**
*@brief Get the enabling status of the camera's built-in hardware ISP
*@param[in] handle Camera handle
*@param[out] pHardwareIspState The enabling state of the camera's built-in hardware ISP
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_PERFORMANCE
*@see dvpGetFunctionInfo
*@see dvpSetHardwareIspState
*@note hardware ISP can save performance cost of computer
*/
dvp2api dvpStatus dvpGetHardwareIspState(dvpHandle handle, bool* pHardwareIspState); 


/**
*@brief Set the enabling status of the camera's built-in hardware ISP
*@param[in] handle Camera handle
*@param[in] HardwareIspState The enabling state of the camera's built-in hardware ISP
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_PERFORMANCE
*@see dvpGetFunctionInfo
*@see dvpGetHardwareIspState
*@note hardware ISP can save performance cost of computer
*/
dvp2api dvpStatus dvpSetHardwareIspState(dvpHandle handle, bool HardwareIspState); 


/**
*@brief Get the enabling status of the trigger mode
*@param[in] handle Camera handle
*@param[out] pTriggerState enabling state of trigger mode
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpGetFunctionInfo
*@see dvpSetTriggerState
*@note Switch between trigger and continuous mode
*/
dvp2api dvpStatus dvpGetTriggerState(dvpHandle handle, bool* pTriggerState); 


/**
*@brief Set the enabling status of the trigger mode
*@param[in] handle Camera handle
*@param[in] TriggerState Enable state of trigger mode
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpGetFunctionInfo
*@see dvpGetTriggerState
*@note Switch between trigger and continuous mode
*/
dvp2api dvpStatus dvpSetTriggerState(dvpHandle handle, bool TriggerState); 


/**
*@brief Get the enabling status of Mono
*@param[in] handle Camera handle
*@param[out] pMonoState enabling state of Mono
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpSetMonoState
*@note usually YUV transformation is performed on color image and extracts luminance(Y) component
*/
dvp2api dvpStatus dvpGetMonoState(dvpHandle handle, bool* pMonoState); 


/**
*@brief Set the enabling status of Mono
*@param[in] handle Camera handle
*@param[in] MonoState enabling state of Mono
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetMonoState
*@note usually YUV transformation is performed on color image and extracts luminance(Y) component
*/
dvp2api dvpStatus dvpSetMonoState(dvpHandle handle, bool MonoState); 


/**
*@brief Get the enabling status of the negative film
*@param[in] handle Camera handle
*@param[out] pInverseState enabling state of the negative film
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpSetInverseState
*@note realizes the reverse color effect similar to camera negative
*/
dvp2api dvpStatus dvpGetInverseState(dvpHandle handle, bool* pInverseState); 


/**
*@brief Set the enabling status of the negative film
*@param[in] handle Camera handle
*@param[in] InverseState enabling state of the negative film
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetInverseState
*@note realizes the reverse color effect similar to camera negative
*/
dvp2api dvpStatus dvpSetInverseState(dvpHandle handle, bool InverseState); 


/**
*@brief Get the enabling status of horizontal mirror
*@param[in] handle Camera handle
*@param[out] pFlipHorizontalState Horizontal mirror Enable State
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetFlipVerticalState
*@see dvpGetRotateOpposite
*@see dvpSetFlipHorizontalState
*/
dvp2api dvpStatus dvpGetFlipHorizontalState(dvpHandle handle, bool* pFlipHorizontalState); 


/**
*@brief Set the enabling status of horizontal mirror
*@param[in] handle Camera handle
*@param[in] FlipHorizontalState Horizontal mirror Enable State
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetFlipVerticalState
*@see dvpGetRotateOpposite
*@see dvpGetFlipHorizontalState
*/
dvp2api dvpStatus dvpSetFlipHorizontalState(dvpHandle handle, bool FlipHorizontalState); 


/**
*@brief Get the enabling status of vertical flip
*@param[in] handle Camera handle
*@param[out] pFlipVerticalState Vertical Flip Enable State
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetFlipHorizontalState
*@see dvpGetRotateOpposite
*@see dvpSetFlipVerticalState
*/
dvp2api dvpStatus dvpGetFlipVerticalState(dvpHandle handle, bool* pFlipVerticalState); 


/**
*@brief Set the enabling status of vertical flip
*@param[in] handle Camera handle
*@param[in] FlipVerticalState Vertical Flip Enable State
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetFlipHorizontalState
*@see dvpGetRotateOpposite
*@see dvpGetFlipVerticalState
*/
dvp2api dvpStatus dvpSetFlipVerticalState(dvpHandle handle, bool FlipVerticalState); 


/**
*@brief Get the enable state of the flat-field correction
*@param[in] handle Camera handle
*@param[out] pFlatFieldState Enable state of flat-field correction
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_CORRECTION
*@see dvpCreateFlatFieldInfo
*@see dvpSetFlatFieldState
*@note is usually used to compensate for the shading caused by the lens
*/
dvp2api dvpStatus dvpGetFlatFieldState(dvpHandle handle, bool* pFlatFieldState); 


/**
*@brief Set the enabling state of the flat-field correction
*@param[in] handle Camera handle
*@param[in] FlatFieldState Enable state of flat-field correction
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_CORRECTION
*@see dvpCreateFlatFieldInfo
*@see dvpGetFlatFieldState
*@note is usually used to compensate for the shading caused by the lens
*/
dvp2api dvpStatus dvpSetFlatFieldState(dvpHandle handle, bool FlatFieldState); 


/**
*@brief Get the enabling status of defect pixel correction
*@param[in] handle Camera handle
*@param[out] pDefectFixState enabling status of defect pixel correction
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_CORRECTION
*@see dvpCreateDefectFixInfo
*@see dvpSetAutoDefectFixState
*@see dvpSetDefectFixState
*/
dvp2api dvpStatus dvpGetDefectFixState(dvpHandle handle, bool* pDefectFixState); 


/**
*@brief Set the enabling status of defect pixel correction
*@param[in] handle Camera handle
*@param[in] DefectFixState enabling status of defect pixel correction
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_CORRECTION
*@see dvpCreateDefectFixInfo
*@see dvpSetAutoDefectFixState
*@see dvpGetDefectFixState
*/
dvp2api dvpStatus dvpSetDefectFixState(dvpHandle handle, bool DefectFixState); 


/**
*@brief Get the enabling status of auto defect pixel correction
*@param[in] handle Camera handle
*@param[out] pAutoDefectFixState enabling status of aotu defect pixel correction
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_CORRECTION
*@see dvpCreateDefectFixInfo
*@see dvpSetDefectFixState
*@see dvpSetAutoDefectFixState
*@note true is automatic, false is manual
*/
dvp2api dvpStatus dvpGetAutoDefectFixState(dvpHandle handle, bool* pAutoDefectFixState); 


/**
*@brief Set the enabling status of auto defect pixel correction
*@param[in] handle Camera handle
*@param[in] AutoDefectFixState enabling status of auto defect pixel correction
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_CORRECTION
*@see dvpCreateDefectFixInfo
*@see dvpSetDefectFixState
*@see dvpGetAutoDefectFixState
*@note true is automatic, false is manual
*/
dvp2api dvpStatus dvpSetAutoDefectFixState(dvpHandle handle, bool AutoDefectFixState); 


/**
*@brief Get the enabling status of image rotation 
*@param[in] handle Camera handle
*@param[out] pRotateState the enabling status of image rotation 
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetRotateOpposite
*@see dvpSetRotateState
*/
dvp2api dvpStatus dvpGetRotateState(dvpHandle handle, bool* pRotateState); 


/**
*@brief Set the enabling status of image rotation 
*@param[in] handle Camera handle
*@param[in] RotateState the enabling status of image rotation 
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetRotateOpposite
*@see dvpGetRotateState
*/
dvp2api dvpStatus dvpSetRotateState(dvpHandle handle, bool RotateState); 


/**
*@brief Get image rotation direction
*@see dvpGetRotateState
*@param[in] handle Camera handle
*@param[out] pRotateOpposite image rotation direction
*@see dvpGetRotateState
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpSetRotateOpposite
*@note 90 degrees clockwise(true) or counterclockwise(false)
*/
dvp2api dvpStatus dvpGetRotateOpposite(dvpHandle handle, bool* pRotateOpposite); 


/**
*@brief Set image rotation direction
*@see dvpGetRotateState
*@param[in] handle Camera handle
*@param[in] RotateOpposite image rotation direction
*@see dvpGetRotateState
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ENHANCE
*@see dvpGetRotateOpposite
*@note 90 degrees clockwise(true) or counterclockwise(false)
*/
dvp2api dvpStatus dvpSetRotateOpposite(dvpHandle handle, bool RotateOpposite); 


/**
*@brief Get the enabling status of the cooler
*@param[in] handle Camera handle
*@param[out] pCoolerState enabling state of the cooler
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ADVANCED
*@see dvpGetTemperatureInfo
*@see dvpSetCoolerState
*@note generally refers to the cooling fan
*/
dvp2api dvpStatus dvpGetCoolerState(dvpHandle handle, bool* pCoolerState); 


/**
*@brief Set the enabling status of the cooler
*@param[in] handle Camera handle
*@param[in] CoolerState enabling state of the cooler
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ADVANCED
*@see dvpGetTemperatureInfo
*@see dvpGetCoolerState
*@note generally refers to the cooling fan
*/
dvp2api dvpStatus dvpSetCoolerState(dvpHandle handle, bool CoolerState); 


/**
*@brief Get the AE statistics region(relative to the entire visible region)
*@param[in] handle Camera handle
*@param[out] pAeRoi AE Statistics region(relative to the entire visible region)
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_EXPOSURE
*@see dvpSetAeOperation
*@see dvpSetAeRoi
*@note Brightness statistical region of AE algorithm
*/
dvp2api dvpStatus dvpGetAeRoi(dvpHandle handle, dvpRegion* pAeRoi); 


/**
*@brief Set the AE statistics region(relative to the entire visible region)
*@param[in] handle Camera handle
*@param[in] AeRoi AE statistics region(relative to the entire visible region)
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_EXPOSURE
*@see dvpSetAeOperation
*@see dvpGetAeRoi
*@note Brightness statistical region of AE algorithm
*/
dvp2api dvpStatus dvpSetAeRoi(dvpHandle handle, dvpRegion AeRoi); 


/**
*@brief Get the AWB statistical region(relative to the entire visible region)
*@param[in] handle Camera handle
*@param[out] pAwbRoi AWB statistics region(relative to the entire visible region)
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_COLOR
*@see dvpSetAwbOperation
*@see dvpSetAwbRoi
*@note Color statistical region of AWB algorithm
*/
dvp2api dvpStatus dvpGetAwbRoi(dvpHandle handle, dvpRegion* pAwbRoi); 


/**
*@brief Set the AWB statistical region(relative to the entire visible region)
*@param[in] handle Camera handle
*@param[in] AwbRoi AWB statistical region(relative to the entire visible region)
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_COLOR
*@see dvpSetAwbOperation
*@see dvpGetAwbRoi
*@note Color statistical region of AWB algorithm
*/
dvp2api dvpStatus dvpSetAwbRoi(dvpHandle handle, dvpRegion AwbRoi); 


/**
*@brief Get AE Mode
*@param[in] handle Camera handle
*@param[out] pAeMode AE Mode
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_EXPOSURE
*@see dvpGetAwbRoi
*@see dvpGetAwbOperation
*@see dvpSetAeMode
*@note Basic strategy of AE algorithm
*/
dvp2api dvpStatus dvpGetAeMode(dvpHandle handle, dvpAeMode* pAeMode); 


/**
*@brief Set AE Mode
*@param[in] handle Camera handle
*@param[in] AeMode
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_EXPOSURE
*@see dvpGetAwbRoi
*@see dvpGetAwbOperation
*@see dvpGetAeMode
*/
dvp2api dvpStatus dvpSetAeMode(dvpHandle handle, dvpAeMode AeMode); 


/**
*@brief Get anti-flicker status
*@param[in] handle Camera handle
*@param[out] pAntiFlick anti-flicker status
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_EXPOSURE
*@see dvpGetAeRoi
*@see dvpGetAeOperation
*@see dvpSetAntiFlick
*@note eliminate flickering issues caused by AC frequency
*/
dvp2api dvpStatus dvpGetAntiFlick(dvpHandle handle, dvpAntiFlick* pAntiFlick); 


/**
*@brief Set anti-flicker status
*@param[in] handle Camera handle
*@param[in] AntiFlick anti-flicker status
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_EXPOSURE
*@see dvpGetAeRoi
*@see dvpGetAeOperation
*@see dvpGetAntiFlick
*@note eliminate flickering issues caused by AC frequency
*/
dvp2api dvpStatus dvpSetAntiFlick(dvpHandle handle, dvpAntiFlick AntiFlick); 


/**
*@brief Get operation mode of AE
*@param[in] handle Camera handle
*@param[out] pAeOperation Operation mode of AE
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_EXPOSURE
*@see dvpGetAeConfig
*@see dvpGetAeTarget
*@see dvpGetAeMode
*@see dvpSetAeOperation
*@note switching automatic or manual exposure mode
*/
dvp2api dvpStatus dvpGetAeOperation(dvpHandle handle, dvpAeOperation* pAeOperation); 


/**
*@brief Set the operation mode of automatic exposure
*@param[in] handle Camera handle
*@param[in] AeOperation Operation mode of automatic exposure
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_EXPOSURE
*@see dvpGetAeConfig
*@see dvpGetAeTarget
*@see dvpGetAeMode
*@see dvpGetAeOperation
*@note switching automatic or manual exposure mode
*/
dvp2api dvpStatus dvpSetAeOperation(dvpHandle handle, dvpAeOperation AeOperation); 


/**
*@brief Get operation mode of AWB
*@param[in] handle Camera handle
*@param[out] pAwbOperation Operation mode of AWB
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_COLOR
*@see dvpSetAwbOperation
*@note After performing AWB, it is necessary to manually set the RGB gains to 1.0 and disable color temperature effects in order to achieve pure white balance. @link #dvpSetRgbGain @endlink @link dvpSetColorTemperatureState @endlink
*/
dvp2api dvpStatus dvpGetAwbOperation(dvpHandle handle, dvpAwbOperation* pAwbOperation); 


/**
*@brief Set the operation mode of AWB
*@param[in] handle Camera handle
*@param[in] AwbOperation Operation mode of AWB
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_COLOR
*@see dvpGetAwbOperation
*@note After performing AWB, it is necessary to manually set the RGB gains to 1.0 and disable color temperature effects in order to achieve pure white balance. @link #dvpSetRgbGain @endlink @link dvpSetColorTemperatureState @endlink
*/
dvp2api dvpStatus dvpSetAwbOperation(dvpHandle handle, dvpAwbOperation AwbOperation); 


/**
*@brief Get Strobe signal driving mode
*@param[in] handle Camera handle
*@param[out] pStrobeDriver Strobe signal driving mode
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpGetStrobeOutputType
*@see dvpSetStrobeDriver
*/
dvp2api dvpStatus dvpGetStrobeDriver(dvpHandle handle, dvpStrobeDriver* pStrobeDriver); 


/**
*@brief Set Strobe signal drive mode
*@param[in] handle Camera handle
*@param[in] StrobeDriver Strobe signal driving mode
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpGetStrobeOutputType
*@see dvpGetStrobeDriver
*/
dvp2api dvpStatus dvpSetStrobeDriver(dvpHandle handle, dvpStrobeDriver StrobeDriver); 


/**
*@brief Get Strobe signal type
*@param[in] handle Camera handle
*@param[out] pStrobeOutputType Strobe signal type
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpGetStrobeDriver
*@see dvpSetStrobeOutputType
*/
dvp2api dvpStatus dvpGetStrobeOutputType(dvpHandle handle, dvpStrobeOutputType* pStrobeOutputType); 


/**
*@brief Set Strobe signal type
*@param[in] handle Camera handle
*@param[in] StrobeOutputType Strobe signal type
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpGetStrobeDriver
*@see dvpGetStrobeOutputType
*/
dvp2api dvpStatus dvpSetStrobeOutputType(dvpHandle handle, dvpStrobeOutputType StrobeOutputType); 


/**
*@brief Get trigger input signal type
*@param[in] handle Camera handle
*@param[out] pTriggerInputType Trigger input signal type
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpGetTriggerLineMode
*@see dvpSetTriggerInputType
*/
dvp2api dvpStatus dvpGetTriggerInputType(dvpHandle handle, dvpTriggerInputType* pTriggerInputType); 


/**
*@brief Set the trigger input signal type
*@param[in] handle Camera handle
*@param[in] TriggerInputType Trigger input signal type
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_TRIGGER
*@see dvpGetTriggerLineMode
*@see dvpGetTriggerInputType
*/
dvp2api dvpStatus dvpSetTriggerInputType(dvpHandle handle, dvpTriggerInputType TriggerInputType); 


/**
*@brief Get source image format
*@param[in] handle Camera handle
*@param[out] pSourceFormat Source image format
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_FORMAT
*@see dvpSetSourceFormatSel
*@see dvpSetSourceFormat
*@note is usually the final image format processed by software
*/
dvp2api dvpStatus dvpGetSourceFormat(dvpHandle handle, dvpStreamFormat* pSourceFormat); 


/**
*@brief Set source image format
*@param[in] handle Camera handle
*@param[in] SourceFormat
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_FORMAT
*@see dvpSetSourceFormatSel
*@see dvpGetSourceFormat
*@note is usually the final image format processed by software
*/
dvp2api dvpStatus dvpSetSourceFormat(dvpHandle handle, dvpStreamFormat SourceFormat); 


/**
*@brief Get target image format
*@param[in] handle Camera handle
*@param[out] pTargetFormat target image format
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_FORMAT
*@see dvpSetTargetFormatSel
*@see dvpSetTargetFormat
*@note is usually the original format output directly from the camera
*/
dvp2api dvpStatus dvpGetTargetFormat(dvpHandle handle, dvpStreamFormat* pTargetFormat); 


/**
*@brief Set target image format
*@param[in] handle Camera handle
*@param[in] TargetFormat
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_FORMAT
*@see dvpSetTargetFormatSel
*@see dvpGetTargetFormat
*@note is usually the original format output directly from the camera
*/
dvp2api dvpStatus dvpSetTargetFormat(dvpHandle handle, dvpStreamFormat TargetFormat); 


/**
*@brief Get user color matrix
*@param[in] handle Camera handle
*@param[out] pUserColorMatrix user color matrix
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_COLOR
*@see dvpSetUserColorMatrix
*@note MAT[0][0]+MAT[1][0]+MAT[2][0] is identical to 1.0
*@note MAT[0][1]+MAT[1][1]+MAT[2][1] is identical to 1.0
*@note MAT[0][2]+MAT[1][2]+MAT[2][2] is identical to 1.0
*@note Input color[B0. G0. R0]
*@note Output color[B1. G1. R1]
*@note B1=B0 * MAT[0][0]+G0 * MAT[1][0]+R0 * MAT[2][0]+MAT[3][0]
*@note G1=B0 * MAT[0][1]+G0 * MAT[1][1]+R0 * MAT[2][1]+MAT[3][1]
*@note R1=B0 * MAT[0][2]+G0 * MAT[1][2]+R0 * MAT[2][2]+MAT[3][2]
*/
dvp2api dvpStatus dvpGetUserColorMatrix(dvpHandle handle, dvpColorMatrix* pUserColorMatrix); 


/**
*@brief Set user color matrix
*@param[in] handle Camera handle
*@param[in] UserColorMatrix user color matrix
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_COLOR
*@see dvpGetUserColorMatrix
*@note MAT[0][0]+MAT[1][0]+MAT[2][0] is identical to 1.0
*@note MAT[0][1]+MAT[1][1]+MAT[2][1] is identical to 1.0
*@note MAT[0][2]+MAT[1][2]+MAT[2][2] is identical to 1.0
*@note Input color[B0. G0. R0]
*@note Output color[B1. G1. R1]
*@note B1=B0 * MAT[0][0]+G0 * MAT[1][0]+R0 * MAT[2][0]+MAT[3][0]
*@note G1=B0 * MAT[0][1]+G0 * MAT[1][1]+R0 * MAT[2][1]+MAT[3][1]
*@note R1=B0 * MAT[0][2]+G0 * MAT[1][2]+R0 * MAT[2][2]+MAT[3][2]
*/
dvp2api dvpStatus dvpSetUserColorMatrix(dvpHandle handle, dvpColorMatrix UserColorMatrix); 


/**
*@brief Get the value of the link timeout(in milliseconds)
*@param[in] handle Camera handle
*@param[out] pLinkTimeout The value of the link timeout(in milliseconds)
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ADVANCED
*@see dvpSetLinkTimeout
*@note If it is a network camera, it corresponds to the heartbeat packet timeout
*/
dvp2api dvpStatus dvpGetLinkTimeout(dvpHandle handle, dvpUint32* pLinkTimeout); 


/**
*@brief Set the link timeout value(in milliseconds)
*@param[in] handle Camera handle
*@param[in] LinkTimeout The value of the link timeout(in milliseconds)
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ADVANCED
*@see dvpGetLinkTimeout
*@note If it is a network camera, it corresponds to the heartbeat packet timeout
*/
dvp2api dvpStatus dvpSetLinkTimeout(dvpHandle handle, dvpUint32 LinkTimeout); 


/**
*@brief Get AE configuration information
*@param[in] handle Camera handle
*@param[out] pAeConfig AE Configuration Information
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_EXPOSURE
*@see dvpGetAeOperation
*@see dvpGetAeTarget
*@see dvpSetAeConfig
*@note Advanced strategy of AE algorithm, which is generally not recommended to be changed
*/
dvp2api dvpStatus dvpGetAeConfig(dvpHandle handle, dvpAeConfig* pAeConfig); 


/**
*@brief Set AE configuration information
*@param[in] handle Camera handle
*@param[in] AeConfig AE Configuration Information
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_EXPOSURE
*@see dvpGetAeOperation
*@see dvpGetAeTarget
*@see dvpGetAeConfig
*@note Advanced strategy of AE algorithm, which is generally not recommended to be changed
*/
dvp2api dvpStatus dvpSetAeConfig(dvpHandle handle, dvpAeConfig AeConfig); 


/**
*@brief Get buffer configuration
*@param[in] handle Camera handle
*@param[out] pBufferConfig buffer configuration
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ADVANCED
*@see dvpSetBufferConfig
*@note Change the frame buffer strategy to adapt to specific application scenarios
*@warning This operation is recommended to be set before@link #dvpStart starts the video stream @endlink
*/
dvp2api dvpStatus dvpGetBufferConfig(dvpHandle handle, dvpBufferConfig* pBufferConfig); 


/**
*@brief Set buffer configuration
*@param[in] handle Camera handle
*@param[in] BufferConfig buffer Configuration
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ADVANCED
*@see dvpGetBufferConfig
*@note Change the frame buffer strategy to adapt to specific application scenarios
*@warning This operation is recommended to be set before@link #dvpStart starts the video stream @endlink
*/
dvp2api dvpStatus dvpSetBufferConfig(dvpHandle handle, dvpBufferConfig BufferConfig); 


/**
*@brief Get trigger configuration of line scan camera
*@param[in] handle Camera handle
*@param[out] pLineTriggerConfig line scan camera trigger configuration
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_LINESCAN
*@see dvpSetLineTriggerConfig
*@note Configure the trigger function of the line scan camera
*@warning This operation is recommended to be set before@link #dvpStart starts the video stream @endlink
*/
dvp2api dvpStatus dvpGetLineTriggerConfig(dvpHandle handle, dvpLineTriggerConfig* pLineTriggerConfig); 


/**
*@brief Set trigger configuration of line scan camera
*@param[in] handle Camera handle
*@param[in] LineTriggerConfig line scan camera trigger configuration
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_LINESCAN
*@see dvpGetLineTriggerConfig
*@note Configure the trigger function of the line scan camera
*@warning This operation is recommended to be set before@link #dvpStart starts the video stream @endlink
*/
dvp2api dvpStatus dvpSetLineTriggerConfig(dvpHandle handle, dvpLineTriggerConfig LineTriggerConfig); 


/**
*@brief Get current user settings
*@param[in] handle Camera handle
*@param[out] pUserSet Current user settings
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP2_API_CONFIG
*@see dvpGetFunctionInfo
*@see dvpSetUserSet
*@note Initial settings after power on, which is only limited to the camera with parameter memory
*/
dvp2api dvpStatus dvpGetUserSet(dvpHandle handle, dvpUserSet* pUserSet); 


/**
*@brief Set current user settings
*@param[in] handle Camera handle
*@param[in] UserSet Current user settings
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP2_API_CONFIG
*@see dvpGetFunctionInfo
*@see dvpGetUserSet
*@note Initial settings after power on, which is only limited to the camera with parameter memory
*/
dvp2api dvpStatus dvpSetUserSet(dvpHandle handle, dvpUserSet UserSet); 


/**
*@brief Get the level status of input IO
*@param[in] handle Camera handle
*@param[in] inputIo Input IO
*@param[out] pInputIoLevel Level status of input IO
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@deprecated
*@ingroup DVP_LINE
*/
dvp2api dvpStatus dvpGetInputIoLevel(dvpHandle handle, dvpInputIo inputIo, bool* pInputIoLevel); 


/**
*@brief Get level of output pin @link dvpLineMode::LINE_MODE_OUTPUT @endlink
*@param[in] handle Camera handle
*@param[in] line pin
*@param[out] pLineLevel output pin@link dvpLineMode::LINE_MODE_OUTPUT @endlink
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_LINE
*@see dvpSetLineLevel
*/
dvp2api dvpStatus dvpGetLineLevel(dvpHandle handle, dvpLine line, bool* pLineLevel); 


/**
*@brief Set output pin level @link dvpLineMode::LINE_MODE_OUTPUT @endlink
*@param[in] handle Camera handle
*@param[in] line pin
*@param[in] LineLevel @link dvpLineMode::LINE_MODE_OUTPUT output pin @endlink
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_LINE
*@see dvpGetLineLevel
*/
dvp2api dvpStatus dvpSetLineLevel(dvpHandle handle, dvpLine line, bool LineLevel); 


/**
*@brief Get enabling status of pin level reverse
*@param[in] handle Camera handle
*@param[in] line pin
*@param[out] pLineInverter enable pin level reverse
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_LINE
*@see dvpSetLineInverter
*/
dvp2api dvpStatus dvpGetLineInverter(dvpHandle handle, dvpLine line, bool* pLineInverter); 


/**
*@brief Set enabling status of pin level reverse
*@param[in] handle Camera handle
*@param[in] line pin
*@param[in] LineInverter enable pin level reverse
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_LINE
*@see dvpGetLineInverter
*/
dvp2api dvpStatus dvpSetLineInverter(dvpHandle handle, dvpLine line, bool LineInverter); 


/**
*@brief Get the I/O mode of the pin
*@param[in] handle Camera handle
*@param[in] line pin
*@param[out] pLineMode I/O mode of  pin
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_LINE
*@see dvpSetLineMode
*/
dvp2api dvpStatus dvpGetLineMode(dvpHandle handle, dvpLine line, dvpLineMode* pLineMode); 


/**
*@brief Set the I/O mode of the pin
*@param[in] handle Camera handle
*@param[in] line pin
*@param[in] LineMode I/O mode of pin
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_LINE
*@see dvpGetLineMode
*/
dvp2api dvpStatus dvpSetLineMode(dvpHandle handle, dvpLine line, dvpLineMode LineMode);


/**
*@brief Get the output signal source of the pin
*@param[in] handle Camera handle
*@param[in] line pin
*@param[out] pLineSource Output signal source of pLineSource pin
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_LINE
*@see dvpSetLineSource
*/
dvp2api dvpStatus dvpGetLineSource(dvpHandle handle, dvpLine line, dvpLineSource* pLineSource); 


/**
*@brief Set the output signal source of the pin
*@param[in] handle Camera handle
*@param[in] line pin
*@param[in] LineSource Output signal source of pin
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_LINE
*@see dvpGetLineSource
*/
dvp2api dvpStatus dvpSetLineSource(dvpHandle handle, dvpLine line, dvpLineSource LineSource); 


/**
*@brief Get back reading of pin level status
*@param[in] handle Camera handle
*@param[in] line pin
*@param[out] pLineStatus Pin level status read back
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_LINE
*/
dvp2api dvpStatus dvpGetLineStatus(dvpHandle handle, dvpLine line, bool* pLineStatus); 


/**
*@brief Get the level status of output IO
*@param[in] handle Camera handle
*@param[in] outputIo Output IO
*@param[out] pOutputIoLevel Output IO level status
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@deprecated
*@ingroup DVP_LINE
*@see dvpSetOutputIoLevel
*/
dvp2api dvpStatus dvpGetOutputIoLevel(dvpHandle handle, dvpOutputIo outputIo, bool* pOutputIoLevel); 


/**
*@brief Set the level status of output IO
*@param[in] handle Camera handle
*@param[in] outputIo Output IO
*@param[in] OutputIoLevel Output IO level status
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@deprecated
*@ingroup DVP_LINE
*@see dvpGetOutputIoLevel
*/
dvp2api dvpStatus dvpSetOutputIoLevel(dvpHandle handle, dvpOutputIo outputIo, bool OutputIoLevel); 


/**
*@brief Get the function configuration of output IO
*@param[in] handle Camera handle
*@param[in] outputIo Output IO
*@param[out] pOutputIoFunction function configuration of output IO
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@deprecated
*@ingroup DVP_LINE
*@see dvpSetOutputIoFunction
*/
dvp2api dvpStatus dvpGetOutputIoFunction(dvpHandle handle, dvpOutputIo outputIo, dvpOutputIoFunction* pOutputIoFunction); 


/**
*@brief Set the function configuration of output IO
*@param[in] handle Camera handle
*@param[in] outputIo Output IO
*@param[in] OutputIoFunction function configuration of output IO
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@deprecated
*@ingroup DVP_LINE
*@see dvpGetOutputIoFunction
*/
dvp2api dvpStatus dvpSetOutputIoFunction(dvpHandle handle, dvpOutputIo outputIo, dvpOutputIoFunction OutputIoFunction); 


/**
*@brief Get the function configuration of input IO
*@param[in] handle Camera handle
*@param[in] inputIo Input IO
*@param[out] pInputIoFunction function configuration of input IO
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@deprecated
*@ingroup DVP_LINE
*@see dvpSetInputIoFunction
*/
dvp2api dvpStatus dvpGetInputIoFunction(dvpHandle handle, dvpInputIo inputIo, dvpInputIoFunction* pInputIoFunction); 


/**
*@brief Set the function configuration of input IO
*@param[in] handle Camera handle
*@param[in] inputIo Input IO
*@param[in] InputIoFunction function configuration of input IO
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@deprecated
*@ingroup DVP_LINE
*@see dvpGetInputIoFunction
*/
dvp2api dvpStatus dvpSetInputIoFunction(dvpHandle handle, dvpInputIo inputIo, dvpInputIoFunction InputIoFunction); 


/**
*@brief Get the index of the current video stream control scheme
*@param[in] handle Camera handle
*@param[out] pStreamFlowCtrlSel the index of the current video stream control scheme
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ADVANCED
*@note is generally used for network cameras
*@see dvpGetStreamFlowCtrlSelDescr
*@see dvpGetStreamFlowCtrlSelDetail
*@see dvpSetStreamFlowCtrlSel
*/
dvp2api dvpStatus dvpGetStreamFlowCtrlSel(dvpHandle handle, dvpUint32* pStreamFlowCtrlSel); 


/**
*@brief Set the index of the current video stream control scheme
*@param[in] handle Camera handle
*@param[in] StreamFlowCtrlSel Index of current flow control scheme options
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ADVANCED
*@note is generally used for network cameras
*@see dvpGetStreamFlowCtrlSelDescr
*@see dvpGetStreamFlowCtrlSelDetail
*@see dvpGetStreamFlowCtrlSel
*/
dvp2api dvpStatus dvpSetStreamFlowCtrlSel(dvpHandle handle, dvpUint32 StreamFlowCtrlSel); 


/**
*@brief Get descriptive information about the index of all video stream control scheme options
*@param[in] handle Camera handle
*@param[out] pStreamFlowCtrlSelDescr descriptive information about the video stream control scheme index
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ADVANCED
*@note is generally used for network cameras
*@see dvpGetStreamFlowCtrlSel
*@see dvpGetStreamFlowCtrlSelDetail
*/
dvp2api dvpStatus dvpGetStreamFlowCtrlSelDescr(dvpHandle handle, dvpSelectionDescr* pStreamFlowCtrlSelDescr); 


/**
*@brief Get the corresponding specific information by the index of the video stream control scheme
*@param[in] handle Camera handle
*@param[in] StreamFlowCtrlSel the index of the video stream control scheme
*@param[out] pStreamFlowCtrlSelDetail Return specific information about the video stream control scheme
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_ADVANCED
*@note is generally used for network cameras
*@see dvpGetStreamFlowCtrlSel
*@see dvpGetStreamFlowCtrlSelDescr
*/
dvp2api dvpStatus dvpGetStreamFlowCtrlSelDetail(dvpHandle handle, dvpUint32 StreamFlowCtrlSel, dvpSelection* pStreamFlowCtrlSelDetail); 


/**
*@brief Get the index of the current color scheme
*@param[in] handle Camera handle
*@param[out] pColorSolutionSel Index of current color scheme
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_COLOR
*@see dvpGetGamma
*@note adjusts the color restoration effect to make the color conform to the normal vision of human eyes
*@see dvpGetColorSolutionSelDescr
*@see dvpGetColorSolutionSelDetail
*@see dvpSetColorSolutionSel
*/
dvp2api dvpStatus dvpGetColorSolutionSel(dvpHandle handle, dvpUint32* pColorSolutionSel); 


/**
*@brief Set the index of the current color scheme
*@param[in] handle Camera handle
*@param[in] ColorSolutionSel Index of current color scheme
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_COLOR
*@see dvpGetGamma
*@note adjusts the color restoration effect to make the color conform to the normal vision of human eyes
*@see dvpGetColorSolutionSelDescr
*@see dvpGetColorSolutionSelDetail
*@see dvpGetColorSolutionSel
*/
dvp2api dvpStatus dvpSetColorSolutionSel(dvpHandle handle, dvpUint32 ColorSolutionSel); 


/**
*@brief Get descriptive information about the index of all color scheme options
*@param[in] handle Camera handle
*@param[out] pColorSolutionSelDescr return description information of color scheme index
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_COLOR
*@see dvpGetGamma
*@note adjusts the color restoration effect to make the color conform to the normal vision of human eyes
*@see dvpGetColorSolutionSel
*@see dvpGetColorSolutionSelDetail
*/
dvp2api dvpStatus dvpGetColorSolutionSelDescr(dvpHandle handle, dvpSelectionDescr* pColorSolutionSelDescr); 


/**
*@brief Get the corresponding specific information by the index of the color scheme
*@param[in] handle Camera handle
*@param[in] ColorSolutionSel the index of the color scheme
*@param[out] pColorSolutionSelDetail Returns specific information about the selected color scheme
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_COLOR
*@see dvpGetGamma
*@note adjusts the color restoration effect to make the color conform to the normal vision of human eyes
*@see dvpGetColorSolutionSel
*@see dvpGetColorSolutionSelDescr
*/
dvp2api dvpStatus dvpGetColorSolutionSelDetail(dvpHandle handle, dvpUint32 ColorSolutionSel, dvpSelection* pColorSolutionSelDetail); 


/**
*@brief Get the index of the current Bayer to RGB interpolation algorithm
*@param[in] handle Camera handle
*@param[out] pBayerDecodeSel the index of the current Bayer to RGB interpolation algorithm
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_COLOR
*@note affects the detail and edge effect of color images
*@see dvpGetBayerDecodeSelDescr
*@see dvpGetBayerDecodeSelDetail
*@see dvpSetBayerDecodeSel
*/
dvp2api dvpStatus dvpGetBayerDecodeSel(dvpHandle handle, dvpUint32* pBayerDecodeSel); 


/**
*@brief Set the index of the current Bayer to RGB interpolation algorithm
*@param[in] handle Camera handle
*@param[in] BayerDecodeSel Index of the current Bayer to RGB interpolation algorithm
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_COLOR
*@note affects the detail and edge effect of color images
*@see dvpGetBayerDecodeSelDescr
*@see dvpGetBayerDecodeSelDetail
*@see dvpGetBayerDecodeSel
*/
dvp2api dvpStatus dvpSetBayerDecodeSel(dvpHandle handle, dvpUint32 BayerDecodeSel); 


/**
*@brief Get descriptive information about the index of all interpolation algorithm options
*@param[in] handle camera handle
*@param[out] pBayerDecodeSelDescr the description information of all interpolation algorithm's index
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_COLOR
*@note affects the detail and edge effect of color images
*@see dvpGetBayerDecodeSel
*@see dvpGetBayerDecodeSelDetail
*/
dvp2api dvpStatus dvpGetBayerDecodeSelDescr(dvpHandle handle, dvpSelectionDescr* pBayerDecodeSelDescr); 


/**
*@brief Get the corresponding specific information by the index of the current Bayer to RGB interpolation algorithm
*@param[in] handle Camera handle
*@param[in] BayerDecodeSel the index of Bayer to RGB interpolation algorithm
*@param[out] pBayerDecodeSelDetail Specific information corresponding to the index of the current Bayer to RGB interpolation algorithm
*@return If the return is predictable, please make sure to return @link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_COLOR
*@note affects the detail and edge effect of color images
*@see dvpGetBayerDecodeSel
*@see dvpGetBayerDecodeSelDescr
*/
dvp2api dvpStatus dvpGetBayerDecodeSelDetail(dvpHandle handle, dvpUint32 BayerDecodeSel, dvpSelection* pBayerDecodeSelDetail); 


/**
*@brief Get the index of the current original image format options
*@param[in] handle camera handle
*@param[out] pSourceFormatSel Index of the current original image format options
*@return If the return is predictable, please make sure to return @link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_FORMAT
*@note is equivalent to using @link dvpStreamFormat @endlink
*@see dvpGetSourceFormatSelDescr
*@see dvpGetSourceFormatSelDetail
*@see dvpSetSourceFormatSel
*/
dvp2api dvpStatus dvpGetSourceFormatSel(dvpHandle handle, dvpUint32* pSourceFormatSel); 


/**
*@brief Set the index of the current original image format options
*@param[in] handle Camera handle
*@param[in] SourceFormatSel Index of the current original image format options
*@return If the return is predictable, please make sure to return @link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_FORMAT
*@note is equivalent to using @link dvpStreamFormat @endlink
*@see dvpGetSourceFormatSelDescr
*@see dvpGetSourceFormatSelDetail
*@see dvpGetSourceFormatSel
*/
dvp2api dvpStatus dvpSetSourceFormatSel(dvpHandle handle, dvpUint32 SourceFormatSel); 


/**
*@brief Get descriptive information about the index of all original image format options
*@param[in] handle camera handle
*@param[out] pSourceFormatSelDescr the description information of all original image format options's index
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_FORMAT
*@note is equivalent to using@link dvpStreamFormat @endlink
*@see dvpGetSourceFormatSel
*@see dvpGetSourceFormatSelDetail
*/
dvp2api dvpStatus dvpGetSourceFormatSelDescr(dvpHandle handle, dvpSelectionDescr* pSourceFormatSelDescr); 


/**
*@brief Get the corresponding specific information by the index of the current original image format option
*@param[in] handle camera handle
*@param[in] SourceFormatSel Index of the original image format
*@param[out] pSourceFormatSelDetail The specific information corresponding to the current original image format options
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_FORMAT
*@note is equivalent to using@link dvpStreamFormat @endlink
*@see dvpGetSourceFormatSel
*@see dvpGetSourceFormatSelDescr
*/
dvp2api dvpStatus dvpGetSourceFormatSelDetail(dvpHandle handle, dvpUint32 SourceFormatSel, dvpFormatSelection* pSourceFormatSelDetail); 


/**
*@brief Get the index of the current target image format options
*@param[in] handle Camera handle
*@param[out] pTargetFormatSel index of the current target image format option
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_FORMAT
*@note is equivalent to using@link dvpStreamFormat @endlink
*@see dvpGetTargetFormatSelDescr
*@see dvpGetTargetFormatSelDetail
*@see dvpSetTargetFormatSel
*/
dvp2api dvpStatus dvpGetTargetFormatSel(dvpHandle handle, dvpUint32* pTargetFormatSel); 


/**
*@brief Set the index of the current target image format options
*@param[in] handle camera handle
*@param[in] TargetFormatSel Index of the current target image format option
*@return If the return is predictable, please make sure to return @link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_FORMAT
*@note is equivalent to using @link dvpStreamFormat @endlink
*@see dvpGetTargetFormatSelDescr
*@see dvpGetTargetFormatSelDetail
*@see dvpGetTargetFormatSel
*/
dvp2api dvpStatus dvpSetTargetFormatSel(dvpHandle handle, dvpUint32 TargetFormatSel); 


/**
*@brief Get descriptive information about the index of all target image format options
*@param[in] handle camera handle
*@param[out] pTargetFormatSelDescr description of target image format option index
*@return If the return is predictable, please make sure to return @link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_FORMAT
*@note is equivalent to using @link dvpStreamFormat @endlink
*@see dvpGetTargetFormatSel
*@see dvpGetTargetFormatSelDetail
*/
dvp2api dvpStatus dvpGetTargetFormatSelDescr(dvpHandle handle, dvpSelectionDescr* pTargetFormatSelDescr); 


/**
*@brief Get the corresponding specific information by the index of the the current output image format option
*@param[in] handle Camera handle
*@param[in] TargetFormatSel Index of output image format
*@param[out] pTargetFormatSelDetail return specific information of the current output image format
*@return If the return is predictable, please make sure to return @link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_FORMAT
*@note is equivalent to using @link dvpStreamFormat @endlink
*@see dvpGetTargetFormatSel
*@see dvpGetTargetFormatSelDescr
*/
dvp2api dvpStatus dvpGetTargetFormatSelDetail(dvpHandle handle, dvpUint32 TargetFormatSel, dvpFormatSelection* pTargetFormatSelDetail); 


/**
*@brief Get the index of the current resolution mode option
*@param[in] handle Camera handle
*@param[out] pResolutionModeSel Index of the current resolution mode option
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_SIZE
*@note includes sum(adjacent sum), bin(adjacent sampling) and other modes, which determine the maximum resolution of the camera
*@see dvpGetQuickRoiSel
*@see dvpGetRoi
*@see dvpGetResolutionModeSelDescr
*@see dvpGetResolutionModeSelDetail
*@see dvpSetResolutionModeSel
*/
dvp2api dvpStatus dvpGetResolutionModeSel(dvpHandle handle, dvpUint32* pResolutionModeSel); 


/**
*@brief Set the index of the current resolution mode option
*@param[in] handle Camera handle
*@param[in] ResolutionModeSel Index of the current resolution mode option
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_SIZE
*@note includes sum(adjacent sum), bin(adjacent sampling) and other modes, which determine the maximum resolution of the camera
*@see dvpGetQuickRoiSel
*@see dvpGetRoi
*@see dvpGetResolutionModeSelDescr
*@see dvpGetResolutionModeSelDetail
*@see dvpGetResolutionModeSel
*/
dvp2api dvpStatus dvpSetResolutionModeSel(dvpHandle handle, dvpUint32 ResolutionModeSel); 


/**
*@brief Get descriptive information about the index of all resolution mode options
*@param[in] handle camera handle
*@param[out] pResolutionModeSelDescr return descriptive information about the index of all resolution mode options
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_SIZE
*@note includes sum(adjacent sum), bin(adjacent sampling) and other modes, which determine the maximum resolution of the camera
*@see dvpGetQuickRoiSel
*@see dvpGetRoi
*@see dvpGetResolutionModeSel
*@see dvpGetResolutionModeSelDetail
*/
dvp2api dvpStatus dvpGetResolutionModeSelDescr(dvpHandle handle, dvpSelectionDescr* pResolutionModeSelDescr); 


/**
*@brief Get the corresponding specific information by the index of the current resolution mode option
*@param[in] handle Camera handle
*@param[in] ResolutionModeSel Index of resolution mode
*@param[out] pResolutionModeSelDetail Specific information corresponding to the current resolution mode option index
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_SIZE
*@note includes sum(adjacent sum), bin(adjacent sampling) and other modes, which determine the maximum resolution of the camera
*@see dvpGetQuickRoiSel
*@see dvpGetRoi
*@see dvpGetResolutionModeSel
*@see dvpGetResolutionModeSelDescr
*/
dvp2api dvpStatus dvpGetResolutionModeSelDetail(dvpHandle handle, dvpUint32 ResolutionModeSel, dvpResolutionMode* pResolutionModeSelDetail); 


/**
*@brief Get the index of the current AE scheme options
*@param[in] handle Camera handle
*@param[out] pAeSchemeSel Index of current AE scheme options
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_EXPOSURE
*@note applies to specific light sources and scenes. Such as microscope
*@see dvpGetAeSchemeSelDescr
*@see dvpGetAeSchemeSelDetail
*@see dvpSetAeSchemeSel
*/
dvp2api dvpStatus dvpGetAeSchemeSel(dvpHandle handle, dvpUint32* pAeSchemeSel); 


/**
*@brief Set the index of the current AE scheme options
*@param[in] handle Camera handle
*@param[in] AeSchemeSel Index of current AE scheme options
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_EXPOSURE
*@note applies to specific light sources and scenes. Such as microscope
*@see dvpGetAeSchemeSelDescr
*@see dvpGetAeSchemeSelDetail
*@see dvpGetAeSchemeSel
*/
dvp2api dvpStatus dvpSetAeSchemeSel(dvpHandle handle, dvpUint32 AeSchemeSel); 


/**
*@brief Get descriptive information about the index of all AE scheme options
*@param[in] handle camera handle
*@param[out] pAeSchemeSelDescr return descriptive information about the index of all AE scheme options
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_EXPOSURE
*@note applies to specific light sources and scenes. Such as microscope
*@see dvpGetAeSchemeSel
*@see dvpGetAeSchemeSelDetail
*/
dvp2api dvpStatus dvpGetAeSchemeSelDescr(dvpHandle handle, dvpSelectionDescr* pAeSchemeSelDescr); 


/**
*@brief Get the corresponding specific information by the index of the the current AE scheme option
*@param[in] handle Camera handle
*@param[in] AeSchemeSel Index of automatic exposure scheme
*@param[out] pAeSchemeSelDetail Specific information corresponding to the index of the current AE scheme options
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_EXPOSURE
*@note applies to specific light sources and scenes. Such as microscope
*@see dvpGetAeSchemeSel
*@see dvpGetAeSchemeSelDescr
*/
dvp2api dvpStatus dvpGetAeSchemeSelDetail(dvpHandle handle, dvpUint32 AeSchemeSel, dvpSelection* pAeSchemeSelDetail); 


/**
*@brief Get the index of the current fast roi option
*@param[in] handle Camera handle
*@param[out] pQuickRoiSel Index of the current Quick Roi option
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_SIZE
*@see dvpGetResolutionModeSel
*@see dvpGetRoi
*@note is equivalent to a typical ROI
*@see dvpGetQuickRoiSelDescr
*@see dvpGetQuickRoiSelDetail
*@see dvpSetQuickRoiSel
*/
dvp2api dvpStatus dvpGetQuickRoiSel(dvpHandle handle, dvpUint32* pQuickRoiSel); 


/**
*@brief Set the index of the current quick roi option
*@param[in] handle Camera handle
*@param[in] QuickRoiSel Index of the current QuickRoi option
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_SIZE
*@see dvpGetResolutionModeSel
*@see dvpGetRoi
*@note is equivalent to a typical ROI
*@see dvpGetQuickRoiSelDescr
*@see dvpGetQuickRoiSelDetail
*@see dvpGetQuickRoiSel
*/
dvp2api dvpStatus dvpSetQuickRoiSel(dvpHandle handle, dvpUint32 QuickRoiSel); 


/**
*@brief Get descriptive information about the index of all quick roi options
*@param[in] handle Camera handle
*@param[out] pQuickRoiSelDescr return descriptive information about the index of all quick roi options
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_SIZE
*@see dvpGetResolutionModeSel
*@see dvpGetRoi
*@note is equivalent to a typical ROI
*@see dvpGetQuickRoiSel
*@see dvpGetQuickRoiSelDetail
*/
dvp2api dvpStatus dvpGetQuickRoiSelDescr(dvpHandle handle, dvpSelectionDescr* pQuickRoiSelDescr); 


/**
*@brief Get the corresponding specific information by the index of the current quick ROI option
*@param[in] handle Camera handle
*@param[in] QuickRoiSel Index of Quick Roi
*@param[out] pQuickRoiSelDetail The specific information corresponding to the index of the current fast Roi option
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_SIZE
*@see dvpGetResolutionModeSel
*@see dvpGetRoi
*@note is equivalent to a typical ROI
*@see dvpGetQuickRoiSel
*@see dvpGetQuickRoiSelDescr
*/
dvp2api dvpStatus dvpGetQuickRoiSelDetail(dvpHandle handle, dvpUint32 QuickRoiSel, dvpQuickRoi* pQuickRoiSelDetail); 


/**
*@brief Get the index of the current pixel acquisition rate option
*@param[in] handle Camera handle
*@param[out] pPixelRateSel Index of the current pixel acquisition rate option
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_PERFORMANCE
*@note Usually the interface of low-speed USB2 can only match low-speed pixel acquisition
*@see dvpGetPixelRateSelDescr
*@see dvpGetPixelRateSelDetail
*@see dvpSetPixelRateSel
*/
dvp2api dvpStatus dvpGetPixelRateSel(dvpHandle handle, dvpUint32* pPixelRateSel); 


/**
*@brief Set the index of the current pixel acquisition rate option
*@param[in] handle Camera handle
*@param[in] PixelRateSel Index of the current pixel acquisition rate option
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_PERFORMANCE
*@note Usually the interface of low-speed USB2 can only match low-speed pixel acquisition
*@see dvpGetPixelRateSelDescr
*@see dvpGetPixelRateSelDetail
*@see dvpGetPixelRateSel
*/
dvp2api dvpStatus dvpSetPixelRateSel(dvpHandle handle, dvpUint32 PixelRateSel); 


/**
*@brief Get descriptive information about the index of all pixel acquisition rate options
*@param[in] handle Camera handle
*@param[out] pPixelRateSelDescr return descriptive information about the index of all pixel acquisition rate options
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_PERFORMANCE
*@note Usually the interface of low-speed USB2 can only match low-speed pixel acquisition
*@see dvpGetPixelRateSel
*@see dvpGetPixelRateSelDetail
*/
dvp2api dvpStatus dvpGetPixelRateSelDescr(dvpHandle handle, dvpSelectionDescr* pPixelRateSelDescr); 


/**
*@brief Get the corresponding specific information by the index of the current pixel acquisition rate option
*@param[in] handle camera handle
*@param[in] PixelRateSel Index of pixel acquisition rate
*@param[out] pPixelRateSelDetail return specific information of the current pixel acquisition rate option
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_PERFORMANCE
*@note Usually the interface of low-speed USB2 can only match low-speed pixel acquisition
*@see dvpGetPixelRateSel
*@see dvpGetPixelRateSelDescr
*/
dvp2api dvpStatus dvpGetPixelRateSelDetail(dvpHandle handle, dvpUint32 PixelRateSel, dvpSelection* pPixelRateSelDetail); 


/**
*@brief Get the index of the current typical data transmission packet length option
*@param[in] handle Camera handle
*@param[out] pPacketSizeSel Index of the current typical packet length options for data transmission
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_PERFORMANCE
*@note If necessary, use@link #dvpGetStreamPackSize atypical transmission packet length @endlink
*@see dvpGetPacketSizeSelDescr
*@see dvpGetPacketSizeSelDetail
*@see dvpSetPacketSizeSel
*/
dvp2api dvpStatus dvpGetPacketSizeSel(dvpHandle handle, dvpUint32* pPacketSizeSel); 


/**
*@brief Set the index of the current typical data transmission packet length option
*@param[in] handle Camera handle
*@param[in] PacketSizeSel Index of the current typical packet length options for data transmission
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_PERFORMANCE
*@note If necessary, use@link #dvpGetStreamPackSize atypical transmission packet length @endlink
*@see dvpGetPacketSizeSelDescr
*@see dvpGetPacketSizeSelDetail
*@see dvpGetPacketSizeSel
*/
dvp2api dvpStatus dvpSetPacketSizeSel(dvpHandle handle, dvpUint32 PacketSizeSel); 


/**
*@brief Get descriptive information about the index of all data transmission packet length options
*@param[in] handle Camera handle
*@param[out] pPacketSizeSelDescr return descriptive information about the index of all data transmission packet length options
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_PERFORMANCE
*@note If necessary, use a typical transmission packet length @link #dvpGetStreamPackSize @endlink
*@see dvpGetPacketSizeSel
*@see dvpGetPacketSizeSelDetail
*/
dvp2api dvpStatus dvpGetPacketSizeSelDescr(dvpHandle handle, dvpSelectionDescr* pPacketSizeSelDescr); 


/**
*@brief Get the corresponding specific information by the index of the current data transmission packet length option
*@param[in] handle camera handle
*@param[in] PacketSizeSel index of typical data transmission packet length
*@param[out] pPacketSizeSelDetail return specific information of the typical data transmission packet length option
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_PERFORMANCE
*@note If necessary, dvpGetStreamPackSize can be used to obtain atypical transmission packet length @link #dvpGetStreamPackSize @endlink
*@see dvpGetPacketSizeSel
*@see dvpGetPacketSizeSelDescr
*/
dvp2api dvpStatus dvpGetPacketSizeSelDetail(dvpHandle handle, dvpUint32 PacketSizeSel, dvpSelection* pPacketSizeSelDetail); 


/**
*@brief Get the index of the current hardware accelerator
*@param[in] handle Camera handle
*@param[out] pAccelerateSel Index of current hardware accelerator
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_PERFORMANCE
*@note If the computer hardware supports it, the image processing capability of the GPU can be used
*@see dvpGetAccelerateSelDescr
*@see dvpGetAccelerateSelDetail
*@see dvpSetAccelerateSel
*/
dvp2api dvpStatus dvpGetAccelerateSel(dvpHandle handle, dvpUint32* pAccelerateSel); 


/**
*@brief Set the index of the current hardware accelerator
*@param[in] handle camera handle
*@param[in] AccelerateSel Index of current hardware accelerator
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_PERFORMANCE
*@note If the computer hardware supports it, the image processing capability of the GPU can be used
*@see dvpGetAccelerateSelDescr
*@see dvpGetAccelerateSelDetail
*@see dvpGetAccelerateSel
*/
dvp2api dvpStatus dvpSetAccelerateSel(dvpHandle handle, dvpUint32 AccelerateSel); 


/**
*@brief Get descriptive information about the index of all hardware accelerator options
*@param[in] handle camera handle
*@param[out] pAccelerateSelDescr return descriptive information about the index of all hardware accelerator options
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_PERFORMANCE
*@note If the computer hardware supports it, the image processing capability of GPU can be used
*@see dvpGetAccelerateSel
*@see dvpGetAccelerateSelDetail
*/
dvp2api dvpStatus dvpGetAccelerateSelDescr(dvpHandle handle, dvpSelectionDescr* pAccelerateSelDescr); 


/**
*@brief Get the corresponding specific information by the index of the current hardware accelerator option
*@param[in] handle Camera handle
*@param[in] AccelerateSel the index of Hardware Accelerator
*@param[out] pAccelerateSelDetail return specific information of hardware accelerator 
*@return If the return is predictable, please make sure to return@link #dvpStatus::DVP_STATUS_OK DVP_STATUS_OK @endlink
*@ingroup DVP_PERFORMANCE
*@note If the computer hardware supports it, the image processing capability of GPU can be used
*@see dvpGetAccelerateSel
*@see dvpGetAccelerateSelDescr
*/
dvp2api dvpStatus dvpGetAccelerateSelDetail(dvpHandle handle, dvpUint32 AccelerateSel, dvpSelection* pAccelerateSelDetail); 


/**
*@brief Set the digital gain of red, green and blue
*@param[in] handle Camera handle
*@param[in] rGain red digital gain
*@param[in] gGain green digital gain
*@param[in] bGain blue digital gain
*@ingroup DVP_COLOR
*@see dvpGetRgbGain
*@see dvpSetRgbGainState
*@see dvpGetRgbGainState
*@note This gain is attached to the white balance effect, but they are independent of each other @link #dvpSetAwbOperation @endlink 
*/
dvp2api dvpStatus dvpSetRgbGain(dvpHandle handle, float rGain, float gGain, float bGain); 


/**
*@brief Get the digital gain of red, green and blue
*@param[in] handle Camera handle
*@param[out] prGain red digital gain
*@param[out] pgGain green digital gain
*@param[out] pbGain blue digital gain
*@ingroup DVP_COLOR
*@see dvpSetRgbGain
*@see dvpSetRgbGainState
*@see dvpGetRgbGainState
*/
dvp2api dvpStatus dvpGetRgbGain(dvpHandle handle, float* prGain, float* pgGain, float* pbGain); 


/**
*@brief Set the enabling status of red, green and blue digital gain function
*@param[in] handle Camera handle
*@param[in] state enabling state of digital gain function in red, green and blue
*@ingroup DVP_COLOR
*@see dvpSetRgbGain
*@see dvpGetRgbGain
*@see dvpGetRgbGainState
*/
dvp2api dvpStatus dvpSetRgbGainState(dvpHandle handle, bool state); 


/**
*@brief Get the enabling status of red, green and blue digital gain function
*@param[in] handle Camera handle
*@param[out] pState enabling state of red, green and blue digital gain function
*@ingroup DVP_COLOR
*@see dvpSetRgbGain
*@see dvpGetRgbGain
*@see dvpSetRgbGainState
*/
dvp2api dvpStatus dvpGetRgbGainState(dvpHandle handle, bool* pState); 



/**@defgroup DVP2_API_BASE basic function
*@{
*/
/**
*@brief Refresh the list of connected cameras and get the number of cameras
*@param[out] pCount Total number of cameras
*@see dvpEnum
*@see dvpOpen
*@see dvpOpenByName
*@note should be executed before enumerates or opens camera(and is in the same thread) @link #dvpEnum @endlink @link #dvpOpen @endlink
*/
dvp2api dvpStatus dvpRefresh(dvpUint32* pCount); 


/**
*@brief Enumerate camera information by index
*@param[in] index Camera index
*@param[out] pCameraInfo Camera information
*@see dvpGetCameraInfo
*@code
dvpUint32 count = 0; 
dvpRefresh(&count); 
for(dvpUint32 i = 0; i < count; i++)
{
	dvpCameraInfo info; 
	if(dvpEnum(i,&info)==DVP_STATUS_OK)
	{
		printf("Camera %d FriendlyName : %s\n", i, info.FriendlyName); 
	}
}
*@endcode
*@note should be executed after refreshes the camera(and in the same thread) @link #dvpRefresh @endlink
*@warning index number and camera handle are two different concepts. Do not confuse them @link #dvpHandle @endlink 
*/
dvp2api dvpStatus dvpEnum(dvpUint32 index, dvpCameraInfo* pCameraInfo); 


/**
*@brief Open the camera according to friendly name @link #dvpCameraInfo::FriendlyName @endlink
*@param[in] friendlyName
*@note is solidified by the original factory, and in principle, it never has the same name
*@param[in] mode open mode
*@param[out] pHandle Camera handle
*@see dvpEnum
*@see dvpOpen
*@see dvpOpenByUserId
*@see dvpGetCameraInfo
*@note This operation will automatically refresh the total cameras @link #dvpRefresh @endlink
*@warning Do not use this function in callback function @link #dvpStreamCallback @endlink
*/
dvp2api dvpStatus dvpOpenByName(dvpStr friendlyName, dvpOpenMode mode, dvpHandle* pHandle); 


/**
*@brief Open the camera according to UserID(User Defined Name) @link #dvpCameraInfo::UserID @endlink
*@param[in] UserId User name
*@note set by the user. Please try to avoid repetition
*@param[in] mode open mode
*@param[out] pHandle Camera handle
*@see dvpSetUserId
*@see dvpEnum
*@see dvpOpen
*@see dvpOpenByName
*@see dvpGetCameraInfo
*@note This operation will automatically refresh the total cameras @link #dvpRefresh @endlink
*@warning Do not use this function in callback function @link #dvpStreamCallback @endlink
*/
dvp2api dvpStatus dvpOpenByUserId(dvpStr UserId, dvpOpenMode mode, dvpHandle* pHandle); 


/**
*@brief Open the camera according to the enumeration index
*@param[in] index Camera index
*@param[in] mode open mode
*@param[out] pHandle Camera handle
*@see dvpRefresh
*@see dvpEnum
*@see dvpOpenByName
*@see dvpGetCameraInfo
*@note should be executed after refreshes the camera(and in the same thread) @link #dvpRefresh @endlink
*@warning Do not use this function in callback function @link #dvpStreamCallback @endlink
*/
dvp2api dvpStatus dvpOpen(dvpUint32 index, dvpOpenMode mode, dvpHandle* pHandle); 


/**
*@brief Check whether the camera handle is valid
*@param[in] handle Camera handle
*@param[out] pIsValid valid status
*@note If opened successfully, the handle is valid; if closed successfully, the handle is invalid @link #dvpOpen @endlink @link #dvpClose @endlink
*/
dvp2api dvpStatus dvpIsValid(dvpHandle handle, bool* pIsValid);


/**
*@brief Check if the camera is online
*@param[in] handle Camera handle
*@param[out] pIsOnline online status
*@note It is recommended to use the method of registering event callback functions to listen online status @link #dvpRegisterEventCallback @endlink 
*/
dvp2api dvpStatus dvpIsOnline(dvpHandle handle, bool* pIsOnline); 


/**
*@brief Get link speed of the camera
*@param[in] handle Camera handle
*@param[out] pLinkSpeed link speed
                                
*/
dvp2api dvpStatus dvpGetLinkSpeed(dvpHandle handle, dvpUint32* pLinkSpeed); 


/**
*@brief Start the video stream and allocate necessary resources
*@param[in] handle Camera handle
*@see dvpOpen
*@see dvpHold
*@see dvpStop
*@note is a heavyweight operation that may take a long time. To achieve fast restart, you can execute dvpRestart, provided that dvpHold has been executed @link #dvpRestart @endlink @link #dvpHold @endlink
*/
dvp2api dvpStatus dvpStart(dvpHandle handle); 


/**
*@brief Stop the video stream and release the corresponding resources
*@param[in] handle Camera handle
*@see dvpStart
*@see dvpClose
*@see dvpHold
*@note is a heavyweight operation that may take a long time. To achieve a quick pause, you can execute dvpHold@link #dvpHold @endlink
*@warning Do not call this API in the callback function, or thread deadlock will result
*/
dvp2api dvpStatus dvpStop(dvpHandle handle); 


/**
*@brief Check if the video stream is in a held state
*@param[in] handle Camera handle
*@param[out] pIsHold hold Status
*@see dvpHold
*@see dvpRestart
*/
dvp2api dvpStatus dvpIsHold(dvpHandle handle, bool* pIsHold); 


/**
*@brief Get one frame
*@param[in] handle Camera handle
*@param[out] pFrame frame information
*@param[out] pBuffer The first memory address of image data
*@warning Do not release it manually
*@param[in] timeout timeout time(ms)
*@see dvpGetFrameBuffer
*@see dvpStreamCallback
*@note This function can be used only after start video stream@link #dvpStart @endlink
*@warning The image data buffer obtained by this function will become invalid the next time this function is called. Therefore, thread synchronization or critical section protection needs to be done well in applications where multiple threads obtain images simultaneously
*/
dvp2api dvpStatus dvpGetFrame(dvpHandle handle, dvpFrame* pFrame, void** pBuffer, dvpUint32 timeout); 


/**
*@brief Get the original frame and target frame with the same timestamp and frame ID at the same time
*@param[in] handle Camera handle
*@param[out] pRaw Original frame
*@param[out] pOut target frame
*@param[in] timeout timeout time(ms)
*@see dvpGetFrame
*@note This function can be used only after start video stream @link #dvpStart @endlink
*@warning The image data buffer obtained by this function will become invalid the next time this function is called. Therefore, thread synchronization or critical section protection needs to be done well in applications where multiple threads obtain images simultaneously
*/
dvp2api dvpStatus dvpGetFrameBuffer(dvpHandle handle, dvpFrameBuffer* pRaw, dvpFrameBuffer* pOut, dvpUint32 timeout); 


/**
*@brief Process the source image(ISP) in the user thread to get the target image
*@param[in] handle Camera handle
*@param[in] pSourceFrame source frame information
*@note usually get by @link #dvpGetFrame @endlink
*@param[in] pSourceBuffer The first memory address of the source frame image data
*@note usually get by @link #dvpGetFrame @endlink
*@param[out] pTargetFrame frame information of target image
*@param[out] pTargetBuffer first memory address of target buffer
*@note target buffer must be allocated and released by user
*@param[in] targetBufferSize  size of target buffer in byte
*@note must be greater than or equal to the data size of target image
*@param[in] targetFormat target image format
*@see dvpGetFrame
*@note This function can be used only after start video stream @link #dvpStart @endlink
*@warning If you use this function in video stream callback function @link #dvpStreamCallback @endlink, you can only register as Event Type@link #dvpStreamEvent::STREAM_EVENT_FRAME_THREAD @endlink
*/
dvp2api dvpStatus dvpProcessImage(dvpHandle handle, const dvpFrame* pSourceFrame, const void* pSourceBuffer, dvpFrame* pTargetFrame, void* pTargetBuffer, dvpUint32 targetBufferSize, dvpStreamFormat targetFormat); 


/**
*@brief Report the current camera status
*@param[in] handle Camera handle
*@param[in] part Function classification of debugging information report
*@param[in] level Level of debug information report
*@param[in] bForce Whether to force logging to a file, regardless of whether log file logging is enabled
*@param[in] text The text content related to the debug report will be inserted into the log file
*@param[in] param additional parameter. Generally, please set 0
*@warning Try to avoid frequent calls
*@note When the user finds an exception, he can call the API immediately to capture the current device status information and record it in the log file
*/
dvp2api dvpStatus dvpDebugReport(dvpHandle handle, dvpReportPart part, dvpReportLevel level, bool bForce, dvpStr text, dvpUint32 param); 


/**
*@brief Turn off the camera and enter standby mode
*@param[in] handle Camera handle
*@see dvpStop
*@see dvpIsValid
*@warning Do not use this function in callback function@link #dvpStreamCallback @endlink
*@warning When the camera is closed, the camera handle is no longer available. However, when you open the camera again, you may get the same handle value
*/
dvp2api dvpStatus dvpClose(dvpHandle handle); 



/**@defgroup DVP2_API_CALLBACK callback function
*@warning Do not call any function that stops the video stream in the callback function. Some functions may implicitly automatically stop the video stream, and it should also be avoided @link #dvpStop @endlink
*@{
*/
/** @brief the callback function of frame stream
*@param[in] handle Camera handle
*@param[in] event event type
*@param[in] pContext user pointer
*@param[in] pFrame frame information
*@param[in] pBuffer image data
*@see dvpRegisterStreamCallback
*@see dvpUnregisterStreamCallback
*@note Can process, display, and save images in callback functions @link #dvpProcessImage @endlink,@link #dvpDrawPicture @endlink @link #dvpSavePicture @endlink
*@warning Do not open, close, play, stop the camera, or grab images in callback functions @link #dvpOpen @endlink,@link #dvpClose @endlink,@link #dvpStart @endlink,@link #dvpStop @endlink @link #dvpGetFrame @endlink*/
typedef dvpInt32(* dvpStreamCallback)(dvpHandle handle, dvpStreamEvent event, void* pContext, dvpFrame* pFrame, void* pBuffer); 

/**
*@brief register the callback function of frame stream
*@param[in] handle Camera handle
*@param[in] proc Function pointer of callback function
*@param[in] event The event type of the callback function
*@param[in] pContext The user pointer of the callback function
*@see dvpUnregisterStreamCallback
*/
dvp2api dvpStatus dvpRegisterStreamCallback(dvpHandle handle, dvpStreamCallback proc, dvpStreamEvent event, void* pContext); 


/**
*@brief unregister the callback function of frame stream
*@param[in] handle Camera handle
*@param[in] proc Function pointer of callback function
*@param[in] event The event type of the callback function
*@param[in] pContext The user pointer of the callback function
*@note Must be consistent with the parameters used to register the callback function in order to unregister the corresponding callback function @link #dvpRegisterStreamCallback @endlink
*/
dvp2api dvpStatus dvpUnregisterStreamCallback(dvpHandle handle, dvpStreamCallback proc, dvpStreamEvent event, void* pContext); 


/**
*@brief the callback function of event notification type
*@param[in] handle Camera handle
*@param[in] event event type
*@param[in] pContext user pointer
*@param[in] param additional data of integer
*@param[in] pVariant additional data of struct
*@see dvpRegisterEventCallback*/
typedef dvpInt32(* dvpEventCallback)(dvpHandle handle, dvpEvent event, void* pContext, dvpInt32 param, struct dvpVariant* pVariant); 

/**
*@brief Register the callback function of event notification type
*@param[in] handle Camera handle
*@param[in] proc Function pointer of callback function
*@param[in] event The event type of the callback function
*@param[in] pContext The user pointer of the callback function
*@see dvpUnregisterEventCallback
*/
dvp2api dvpStatus dvpRegisterEventCallback(dvpHandle handle, dvpEventCallback proc, dvpEvent event, void* pContext); 


/**
*@brief unregister the callback function of event notification type
*@param[in] handle Camera handle
*@param[in] proc Function pointer of callback function
*@param[in] event The event type of the callback function
*@param[in] pContext The user pointer of the callback function
*@note Must be consistent with the parameters used to register the callback function in order to unregister the corresponding callback function @link #dvpRegisterEventCallback @endlink
*/
dvp2api dvpStatus dvpUnregisterEventCallback(dvpHandle handle, dvpEventCallback proc, dvpEvent event, void* pContext); 


/**@} end of DVP2_API_CALLBACK*/

/**@defgroup DVP2_API_CONFIG save and load configuration
*@{
*/
/**
*@brief Load default settings
*@param[in] handle Camera handle
*@see dvpLoadConfig
*@see dvpSaveConfig
*@warning will overwrite the current settings. If necessary, please backup the current settings to the configuration file @link #dvpSaveConfig @endlink
*/
dvp2api dvpStatus dvpLoadDefault(dvpHandle handle); 


/**
*@brief Load settings from configuration file
*@param[in] handle Camera handle
*@param[in] path Path/filename of the configuration file
*@see dvpLoadDefault
*@see dvpSaveConfig
*@warning will overwrite the current settings. If necessary, please backup the current settings to the configuration file @link #dvpSaveConfig @endlink
*/
dvp2api dvpStatus dvpLoadConfig(dvpHandle handle, dvpStr path); 


/**
*@brief Save the current settings to a configuration file
*@param[in] handle Camera handle
*@param[in] path Path/filename of the configuration file
*@see dvpLoadDefault
*@see dvpLoadConfig
*@warning Ensure that the current process has write permission to this file
*/
dvp2api dvpStatus dvpSaveConfig(dvpHandle handle, dvpStr path); 


/**
*@brief Save user settings to camera memory
*@param[in] handle Camera handle
*@param[in] UserSet the index of user settings
*@see dvpLoadUserSet
*@see dvpGetUserSet
*@warning cannot be saved as default user setting @link #dvpUserSet::USER_SET_DEFAULT @endlink
*/
dvp2api dvpStatus dvpSaveUserSet(dvpHandle handle, dvpUserSet UserSet); 


/**
*@brief Load user settings from camera memory
*@param[in] handle Camera handle
*@param[in] UserSet the index of user settings
*@see dvpSaveUserSet
*@see dvpGetUserSet
*@note can be loaded from the default user settings @link #dvpUserSet::USER_SET_DEFAULT @endlink
*/
dvp2api dvpStatus dvpLoadUserSet(dvpHandle handle, dvpUserSet UserSet); 


/**@} end of DVP2_API_CONFIG*/
/**@} end of DVP2_API_BASE*/

/**@defgroup DVP2_API_EXTEND practical function
*@{
*/
/**
*@brief Save image to file
*@param[in] pFrame frame information
*@param[in] pBuffer The first memory address of image data(currently supported data formats include RAW, RGB24)
*@param[in] file file name or full path(the image file format is determined by the file extension. Currently, the supported image formats include: bmp, jpeg, jpg, png, tiff, tif, gif, dat(pure image data))
*@param[in] quality Image quality, valid only for jpeg format, and the value range is[1,100]
*@see dvpGetFrame
*@see dvpStreamCallback
*/
dvp2api dvpStatus dvpSavePicture(const dvpFrame* pFrame, const void* pBuffer, dvpStr file, dvpInt32 quality); 


/**
*@brief Grab and save the image according to the specified resolution
*@param[in] handle Camera handle
*@param[in] ResoulutionModeSel Resolution mode Index@link #dvpGetResolutionModeSel @endlink
*@param[in] roi @link #dvpGetRoi @endlink, If it is a NULL pointer, it is the maximum width and height in this resolution mode
*@param[in] timeout @link #dvpGetFrame @endlink grab timeout
*@param[in] filePath file name or full path(the image file format is determined by the file extension. Currently, the supported image formats include: bmp, jpeg, jpg, png, tiff, tif, gif, dat(pure image data))
*@param[in] quality Image quality, valid only for jpeg format, and the value range is[1,100]
*@see dvpSavePicture
*/
dvp2api dvpStatus dvpCaptureFile(dvpHandle handle, dvpUint32 ResoulutionModeSel, dvpRegion* roi, dvpUint32 timeout, dvpStr filePath, dvpInt32 quality); 


/**
*@brief Convert image compression to user buffer
*@param[in] pFrame frame information
*@param[in] pSrcBuffer The first memory address of image data(currently supported data formats include RAW, RGB24)
*@param[in] format indicates the string of image format(in the form of "jpg", "png", or "*. jpg", "*. png". Currently, the supported image formats include: bmp, jpeg, jpg, png, tiff, tif, gif, dat(pure image data))
*@param[in] quality Image quality, valid only for jpeg format, and the value range is[1,100]
*@param[out] pDstBuffer dest buffer
*@param[in] uByteSize The number of bytes in the specified user image buffer
*@param[out] pValidByteSize Actual number of effective bytes output to user image buffer
*@see dvpSavePicture
*@see dvpPrintPictureToBuffer
*/
dvp2api dvpStatus dvpPrintPictureToBuffer(const dvpFrame* pFrame, const void* pSrcBuffer, dvpStr format, dvpInt32 quality, void* pDstBuffer, dvpUint32 uByteSize, dvpUint32* pValidByteSize); 


#ifdef_WIN32
/**
*@brief Display image on window
*@param[in] pFrame frame information
*@param[in] pBuffer The first memory address of image data(currently supported data formats include:@link #dvpImageFormat::FORMAT_MONO @endlink,@link #dvpImageFormat::FORMAT_BAYER_BG @endlink,@link #dvpImageFormat::FORMAT_BGR24 @endlink)
*@param[in] hWnd Window handle for displaying images
*@param[in] sRect is used to display the image region. The null pointer represents the entire image region
*@param[in] dRect is used to display the window region. The null pointer represents the entire window region
*@note It is recommended to use this function in video stream callback function @link #dvpStreamCallback @endlink
*/
dvp2api dvpStatus dvpDrawPicture(const dvpFrame* pFrame, const void* pBuffer, HWND hWnd, LPRECT sRect, LPRECT dRect); 


/**
*@brief Start recording, create a video file, and obtain the handle of the video encoder
*@param[in] filePath The video file name or full path(the format of the video file is determined by the file extension. Currently, the supported video formats include: wmv, avi)
*@param[in] width This parameter is obsolete and can be any value
*@param[in] height This parameter is obsolete and can be any value
*@param[in] quality Video picture quality, valid only for avi format, and the value range is[1,100]
*@param[out] pHandle handle of video encoder
*@see dvpSetTargetFormat
*@see dvpVideoRecordWrite
*@see dvpStopVideoRecord
*@warning The Windows operating system issued in Europe and South Korea does not have built-in WMV format support. You may need to go to<a href=“https://www.codecguide.com/windows_media_codecs.htm">Download and install the corresponding version of the update on Microsoft's website</a>
*/
dvp2api dvpStatus dvpStartVideoRecord(dvpStr filePath, dvpUint32 width, dvpUint32 height, dvpInt32 quality, dvpRecordHandle* pHandle); 


/**
*@brief Check whether the handle of the video encoder is valid
*@param[in] handle handle of video encoder 
*@param[out] pValid return status of RecorderValid
*@note If the recording is successfully started, the handle is valid; if the recording is successfully stopped, the handle is invalid @link #dvpStartVideoRecord @endlink @link #dvpStopVideoRecord @endlink
*/
dvp2api dvpStatus dvpIsVideoRecorderValid(dvpRecordHandle handle, bool* pValid); 


/**
*@brief Write the video stream data into the video file
*@param[in] handle handle of video encoder 
*@param[in] pFrame frame information
*@param[in] pBuffer The first memory address of image data(currently supported data formats include:@link #dvpImageFormat::FORMAT_BGR24 @endlink)
*@see dvpGetFrame
*@see dvpStartVideoRecord
*@see dvpStopVideoRecord
*@note Before starting recording , please set the target image format correctly @link #dvpStartVideoRecord @endlink @link #dvpSetTargetFormat @endlink
*/
dvp2api dvpStatus dvpVideoRecordWrite(dvpRecordHandle handle, const dvpFrame* pFrame, const void* pBuffer); 


/**
*@brief Stop recording, close the video file, and release the handle of the video encoder
*@param[in] handle Video encoder handle
*@see dvpVideoRecordWrite
*@see dvpStartVideoRecord
*/
dvp2api dvpStatus dvpStopVideoRecord(dvpRecordHandle handle); 


/**
*@brief Displays the modal dialog of camera properties 
*@param[in] handle Camera handle
*@param[in] hParent handle of the parent window
*/
dvp2api dvpStatus dvpShowPropertyModalDialog(dvpHandle handle, HWND hParent); 


#endif
/**
*@brief Read user-defined data from camera memory
*@param[in] handle Camera handle
*@param[in] addr Read address of camera
*@param[in, out] pBuffer first address of buffer data
*@param[in] size of user data in bytes
*@see dvpWriteUserData
*@note The amount of data read cannot be larger than the maximum space of the memory (usually no larger than 4k bytes)
*@note must open the camera before using this function @link #dvpOpen @endlink
*@warning may consume a long time
*/
dvp2api dvpStatus dvpReadUserData(dvpHandle handle, dvpUint32 addr, void* pBuffer, dvpUint32 size); 


/**
*@brief Write user-defined data into camera memory
*@param[in] handle Camera handle
*@param[in] addr Write address of camera
*@param[in] pBuffer first address of buffer data
*@param[in] size of user data in bytes
*@see dvpReadUserData
*@note The amount of data written cannot be larger than the maximum space of the memory (usually no larger than 4k bytes)
*@note must open the camera before using this function @link #dvpOpen @endlink
*@warning may consume a long time
*/
dvp2api dvpStatus dvpWriteUserData(dvpHandle handle, dvpUint32 addr, const void* pBuffer, dvpUint32 size);


/**
*@brief Setting a user-defined name for the camera @link #dvpCameraInfo @endlink
*@param[in] handle Camera handle
*@param[in] UserId User name
*@param[in, out] pLength Enter the string length provided and output the actual string length
*@see dvpGetCameraInfo
*/
dvp2api dvpStatus dvpSetUserId(dvpHandle handle, dvpStr UserId, dvpUint32* pLength); 


/**@} end of DVP2_API_EXTEND*/

/**@defgroup DVP2_API_INTERNAL internal functions
*@{
*/
/**
*@brief Get any parameters or information of the camera
*@param[in] handle Camera handle
*@param[in] command command code
*@param[in, out] pParam parameter pointer
*@param[in, out] pSize Enter the byte size of the parameter and output the actual size
*@warning This function is used internally. The parameter details are not disclosed yet. It is not recommended to use
*/
dvp2api dvpStatus dvpGet(dvpHandle handle, dvpUint32 command, void* pParam, dvpUint32* pSize); 


/**
*@brief Set any parameters or information of the camera
*@param[in] handle Camera handle
*@param[in] command command code
*@param[in, out] pParam parameter pointer
*@param[in, out] pSize Enter the byte size of the parameter and output the actual size
*@warning This function is used internally. The parameter details are not disclosed yet. It is not recommended to use
*/
dvp2api dvpStatus dvpSet(dvpHandle handle, dvpUint32 command, const void* pParam, dvpUint32* pSize); 


/**
*@brief Configure dvp
*@param[in] command command code
*@param[in] param parameter
*@param[in] pData data
*@warning This function is used internally. The parameter details are not disclosed yet. It is not recommended to use
*/
dvp2api dvpStatus dvpConfig(dvpUint32 command, dvpUint32 param, const void* pData); 


/**
*@brief Configure the parameters of the specified camera
*@param[in] index Camera index
*@param[in] command command code
*@param[in] param parameter
*@param[in] pData data
*@see dvpEnum
*@warning This function is used internally. The parameter details are not disclosed yet. It is not recommended to use
*/
dvp2api dvpStatus dvpConfigEx(dvpUint32 index, dvpUint32 command, dvpUint32 param, const void* pData); 


/**@} end of DVP2_API_INTERNAL*/

/**@defgroup DVP2_API_BRIDGE Universal Interface
*@{
*/
/**
*@brief Get property of Integer type
*@param[in] handle Camera handle
*@param[in] strKey Key(property name), the key is "Width" if the width information is obtained 
*@param[out] pCurIntValue Returns the property value of Integer type
*@param[out] pstIntDescr  Returns structural data of Integer type
*/
dvp2api dvpStatus dvpGetIntValue(IN dvpHandle handle, IN const char* strKey, OUT int64_t* pCurIntValue, OUT dvpIntDescr* pstIntDescr); 


/**
*@brief Set property of Integer type
*@param[in] handle Device handle
*@param[in] strKey Key(property name), the key is "Width" if the width information is obtained
*@param[in] nValue property value of Integer type
*/
dvp2api dvpStatus dvpSetIntValue(IN dvpHandle handle, IN const char* strKey, IN int64_t nValue); 


/**
*@brief Get the uInteger64 property value
*@param[in] handle Camera handle
*@param[in] strKey Key(property name), the key is "Width" if the width information is obtained 
*@param[out] pCurUint64Value Returns the property value of uInteger64 type
*@param[out] pstUint64Descr  Returns structural data of uInteger64 type
*/
dvp2api dvpStatus dvpGetUint64Value(IN dvpHandle handle, IN const char* strKey, OUT uint64_t* pCurUint64Value, OUT dvpUint64Descr* pstUint64Descr); 


/**
*@brief Set property of uInteger64 type
*@param[in] handle Device handle
*@param[in] strKey Key(property name), the key is "Width" if the width information is obtained 
*@param[in] nValue property value of uInteger64 type
*/
dvp2api dvpStatus dvpSetUint64Value(IN dvpHandle handle, IN const char* strKey, IN uint64_t nValue); 


/**
*@brief Set property of enumeration type
*@param[in] handle Device handle
*@param[in] strKey Key(property name), the key is "PixelFormat" if the pixel format information is obtained
*@param[in] pCurEnumValue  return current enumeration value
*@param[out] nszSupportValue Returns all enumeration values
*@param[out] pnSupportNum Number of valid enumeration values
*/
dvp2api dvpStatus dvpGetEnumValue(IN dvpHandle handle, IN const char* strKey, OUT unsigned int* pCurEnumValue, OUT unsigned int nszSupportValue[64], OUT unsigned int* pnSupportNum); 


/**
*@brief Get the enumeration property value and name through the enumeration value subscript
*@param[in] handle Device handle
*@param[in] strKey Key(property name), the key is "PixelFormat" if the pixel format information is obtained
*@param[in] uIndx Current enumeration value
*@param[out] pEnumDescr Returns structural data of enumeration type
*/
dvp2api dvpStatus dvpGetEnumNameValue(dvpHandle handle, dvpStr strKey, dvpUint32 uIndx, struct dvpEnumDescr* pEnumDescr); 


/**
*@brief Set property of enumeration type
*@param[in] handle Device handle
*@param[in] strKey Key(property name), the key is "PixelFormat" if the pixel format information is obtained
*@param[in] nValue the unsigned int enumeration value
*/
dvp2api dvpStatus dvpSetEnumValue(IN dvpHandle handle, IN const char* strKey, IN unsigned int nValue); 


/**
*@brief Set property of enumeration type by String
*@param[in] handle Device handle
*@param[in] strKey Key(property name), the key is "PixelFormat" if the pixel format information is obtained
*@param[in] strValue the string of enumeration value
*/
dvp2api dvpStatus dvpSetEnumValueByString(IN dvpHandle handle, IN const char* strKey, IN const char* strValue); 


/**
*@brief Get property of float type
*@param[in] handle Device handle
*@param[in] strKey Key(property name)
*@param[out] pCurFloatValue Returns the property value of float type
*@param[out] pstFloatDescr Returns structural data of float type
*/
dvp2api dvpStatus dvpGetFloatValue(IN dvpHandle handle, IN const char* strKey, OUT float* pCurFloatValue, OUT dvpFloatDescr* pstFloatDescr); 


/**
*@brief Set property of float type
*@param[in] handle Device handle
*@param[in] strKey Key(property name)
*@param[in] fValue property value of float type

*/
dvp2api dvpStatus dvpSetFloatValue(IN dvpHandle handle, IN const char* strKey, IN float fValue); 


/**
*@brief Get property of double type
*@param[in] handle Device handle
*@param[in] strKey Key(property name)
*@param[out] pCurDoubleValue Returns the property value of double type
*@param[out] pstDoubleDescr Returns structural data of double type
*/
dvp2api dvpStatus dvpGetDoubleValue(IN dvpHandle handle, IN const char* strKey, OUT double* pCurDoubleValue, OUT dvpDoubleDescr* pstDoubleDescr); 


/**
*@brief Set property of double type
*@param[in] handle Device handle
*@param[in] strKey Key(property name)
*@param[in] dValue property value of double type

*/
dvp2api dvpStatus dvpSetDoubleValue(IN dvpHandle handle, IN const char* strKey, IN double dValue); 


/**
*@brief Get property of bool type
*@param[in] handle Device handle
*@param[in] strKey Key(property name)
*@param[out] pbValue Returns the property value of bool type
*/
dvp2api dvpStatus dvpGetBoolValue(IN dvpHandle handle, IN const char* strKey, OUT bool* pbValue); 


/**
*@brief Set property of bool type
*@param[in] handle Device handle
*@param[in] strKey Key(property name)
*@param[in] bValue property value
*/
dvp2api dvpStatus dvpSetBoolValue(IN dvpHandle handle, IN const char* strKey, IN bool bValue); 


/**
*@brief Get property of String type
*@param[in] handle Device handle
*@param[in] strKey Key(property name)
*@param[out] pstStringValue Returns the property value of string type
*@param[in] i32StrValueSize size of strKey
*/
dvp2api dvpStatus dvpGetStringValue(IN dvpHandle handle, IN const char* strKey, OUT char* pstStringValue, IN const int i32StrValueSize); 


/**
*@brief Set property of String type
*@param[in] handle Device handle
*@param[in] strKey Key(property name)
*@param[in] strValue property value
*/
dvp2api dvpStatus dvpSetStringValue(IN dvpHandle handle, IN const char* strKey, IN const char* strValue); 


/**
*@brief Set property of Command type
*@param[in] handle Device handle
*@param[in] strKey property value
*/
dvp2api dvpStatus dvpSetCommandValue(IN dvpHandle handle, IN const char* strKey); 

/**
*@brief Rotate the image at a specific angle
*@param[in] handle Device handle
*@param[in] RotateAngle Clockwise angle 1: 90 degrees 2: 180 degrees 3: 270 degrees
*/
dvp2api dvpStatus dvpRotateImage(IN dvpHandle handle, IN int RotateAngle);

/**
*@brief Clear GenICam node cache(not implemented)
*@param[in] handle Device handle
*/
dvp2api dvpStatus dvpInvalidateNodes(IN dvpHandle handle); 


/**
*@brief Get XML file content
*@param[in] handle Device handle
*@param[out] pXmlBuf xml content buffer
*@param[in] nXmlSize buffer size
*/
dvp2api dvpStatus dvpGetXml(IN dvpHandle handle, OUT void* pXmlBuf, IN unsigned int nXmlSize); 


/**
*@brief Get the XML attribute type
*@param[in] handle Device handle
*@param[out] strKey attribute name
*@param[out] pEnInterfaceType Interface type
*/
dvp2api dvpStatus dvpGetXMLInterfaceType(dvpHandle handle, dvpStr strKey, dvpXMLInterfaceType* pEnInterfaceType); 

/**
*@brief set camera parameters through strings
*@param[in] handle Device handle
*@param[in] szName parameter name
*@param[in] szValue parameter value
*@note Please refer to the demo for specific usage methods
*/
dvp2api dvpStatus dvpSetConfigString(dvpHandle h, char const* szName, char const* szValue);

/**
*@brief get camera parameters through strings
*@param[in] handle Device handle
*@param[in] szName parameter name
*@param[out] szValue parameter value
*@note Please refer to the demo for specific usage methods
*/
dvp2api dvpStatus dvpGetConfigString(dvpHandle h, char const* szName, char const** szValue);


/**
*@brief get description information for the specified parameter option index
*@param[in] handle Device handle
*@param[in] name parameter name
*@param[out] pSelectionDescr the description information of the specify parameter option index
*@note Please refer to the demo for specific usage methods
*/
dvp2api dvpStatus dvpGetSelectionDescr(dvpHandle handle, dvpStr name, dvpSelectionDescr* pSelectionDescr);

/**
*@brief get detail information for the specified parameter option index
*@param[in] handle Device handle
*@param[in] name parameter name
*@param[in] index
*@param[out] pSelection the detail information of the specify parameter option index
*@note Please refer to the demo for specific usage methods
*/
dvp2api dvpStatus dvpGetSelectionDetail(dvpHandle handle, dvpStr name, dvpUint32 index, dvpSelection* pSelection);

/**@} end of DVP2_API_BRIDGE*/


/**@defgroup DVP_EXPOSURE exposure function
*@{
*/
/**@} end of DVP_EXPOSURE*/

/**@defgroup DVP_COLOR color adjustment
*@{
*/
/**@} end of DVP_COLOR*/

/**@defgroup DVP_ENHANCE enhancement effect
*@{
*/
/**@} end of DVP_ENHANCE*/

/**@defgroup DVP_SIZE Image Size
*@{
*/
/**@} end of DVP_SIZE*/

/**@defgroup DVP_FORMAT image format
*@{
*/
/**@} end of DVP_FORMAT*/

/**@defgroup DVP_TRIGGER trigger function
*@{
*/
/**@} end of DVP_TRIGGER*/

/**@defgroup DVP_LINE pin function
*@{
*/
/**@} end of DVP_LINE*/

/**@defgroup DVP_CORRECTION image correction
*@{
*/
/**@} end of DVP_CORRECTION*/

/**@defgroup DVP_PERFORMANCE Performance Tuning
*@{
*/
/**@} end of DVP_PERFORMANCE*/

/**@defgroup DVP_INFORMATION Basic Information
*@{
*/
/**@} end of DVP_INFORMATION*/

/**@defgroup DVP_ADVANCED Advanced Settings
*@{
*/
/**@} end of DVP_ADVANCED*/

/**@defgroup DVP_LINESCAN line scan camera
*@{
*/
/**@} end of DVP_LINESCAN*/

/**
 *  @example Dvp2Getframe.cpp
 *  An example of the test class.
 */
 
 /**
 *  @example Dvp2EventCallback.cpp
 *  An example of the test class.
 */
 
 /**
 *  @example Dvp2StreamCallback.cpp
 *  An example of the test class.
 */
 
 /**
 *  @example Dvp2VideoRecord.cpp
 *  An example of the test class.
 */
 
 /**
 * \htmlinclude VPN.htm
 */

#endif/*__DVPCAMERA_H__*/

