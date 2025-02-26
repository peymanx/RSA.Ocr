### If you want the same set of code to compile and run normally on different platforms, you need to do the following:

#### 1. Download the driver package for the corresponding camera and install the driver package

```
Linux/ /* DVP Linux platform driver (Arm64, Arm32, Linux x86 64-bit, Linux x86 32-bit) */
```

The installation of Linux arm platform and Linux x86 driver is the same, the steps are as follows:

```
1. Installation: After connecting the camera, enter the driver package and enter the command sudo ./install.sh, you can choose to restart the system
2. Introduction to the sdk directory:
------------------------------
-- basedcam2 /* DVP2 camera demonstration program */
-- Demo_x64 /* Executable program demo, no interface */
-- libdvp.so /* sdk api library */
-- libhzd.so /* core algorithm library */
-- usb2_m_all.dscam.so 
-- usb3_m_all.dscam.so 
-- usb3_v_all.dscam.so 
-- usb3_m3s_all.dscam.so 
-- BasicFuncation.tar.gz /* qt sample source code
Note:
libdvp.so libhzd.so usbxxx.dscam.so libraries must be placed in the same directory
```

Open the exe directly on a Windows computer and follow the prompts to install.

#### 2. Configure the reference library file for the corresponding platform

Enter the directory and use Qt to open the .pro file. The header file is DVPCamera.h. The dvplib.so under Linux corresponds to the DVPCamera.dll file on Windows. You can refer to the following configuration method of QtBasicFunction on the Linux x86 virtual machine.

Compiling on Linux x86 requires modifying the library directory configuration in the pro file:

```
unix:!macx: LIBS += -L$$PWD/lib_linux_x86_64/ -ldvp

INCLUDEPATH += $$PWD/lib_linux_x86_64
DEPENDPATH += $$PWD/lib_linux_x86_64
```
Compiling on Linux arm requires modifying the library directory configuration in the pro file:

```
unix:!macx: LIBS += -L$$PWD/lib_linux_arm64/ -ldvp

INCLUDEPATH += $$PWD/lib_linux_arm64
DEPENDPATH += $$PWD/lib_linux_arm64
```

Windows configuration is similar.

#### 3. Compile and run

Windows: compile in Qt Creator

Linux x86: Install Qt Creator, compiler and other Qt environments to compile

Linux Arm has two compilation methods:

- Cross compilation: You need to install the cross compiler and Arm Qt library in the virtual machine before compiling

- Arm direct compilation: For example, RK3399 and Nvidia TX2 platforms, after we install the Qt environment on RK3399 and Nvidia TX2, we can compile, run and debug on ARM

After the compilation is completed, copy the relevant .so library under the driver installation package to the same level directory of the executable file BasicFunction

```
build-BasicFunction-Desktop-Debug$ tree
.
├── BasicFunction
├── BasicFunction.o
├── ImageAcquisition.o
├── libD3tKit.so // √
├── libdvp.so //DVPCamera.dll corresponding to Win10 platform, camera API library √
├── libhzd.so //Some image processing libraries √
├── main.o
├── Makefile
├── moc_BasicFunction.cpp
├── moc_BasicFunction.o
├── moc_ImageAcquisition.cpp
├── moc_ImageAcquisition.o
├── ui_BasicFunction.h
├── usb2_m_all.dscam.so // √
├── usb3_m3s_all.dscam.so // √
├── usb3_m_all.dscam.so // √
└── usb3_v_all.dscam.so // √
```
### Linux x86 virtual machine running screenshots are as follows:

![avatar](./Image/BasicFunction.png)

### Win x86 running screenshots

![Win x86](./Image/BasicFunction_x86.png)

### Linux Arm (RK3399) running screenshots are as follows

![linux arm64](./Image/Linux_arm64_rk3399.png)

### 4. Others
If the images collected by the thread are to be displayed on the Qt control, please pay attention to converting them to BGR24. Other target formats are not processed too much here for the time being

```
//ImageAcquisition.cpp
status = dvpGetFrame(m_handle, &m_pFrame, &pBuffer, m_uGrabTimeout);

if (status == DVP_STATUS_OK)
{
//Here, the image acquisition and image conversion are placed in the working thread to solve the problem of the main interface freezing when displayed at a high frame rate
if(m_pFrame.format==FORMAT_BGR24)
{
m_threadMutex.lock();
m_ShowImage = QPixmap::fromImage(QImage((uchar*)pBuffer,m_pFrame.iWidth, m_pFrame.iHeight,m_pFrame.iWidth*3, QImage::Format_BGR888,0,0));
m_threadMutex.unlock();
emit dispSignal();
}
/*
else if(m_pFrame.format==FORMAT_MONO)
{
m_threadMutex.lock();
m_ShowImage = QPixmap::fromImage(QImage((uchar*)pBuffer,m_pFrame.iWidth, m_pFrame.iHeight,m_pFrame.iWidth, QImage::Format_Mono,0,0));
m_threadMutex.unlock();
emit dispSignal();
}*/
else
{
//If it is not BGR24 format, display a custom image
m_threadMutex.lock();
QPixmap clearPix = QPixmap("../QtBasicFunction/warning.png");
m_ShowImage = clearPix;
m_threadMutex.unlock();
emit dispSignal();
}
//【Note:】The example does not support the display of other target formats (MONO, RAW)
}
```


