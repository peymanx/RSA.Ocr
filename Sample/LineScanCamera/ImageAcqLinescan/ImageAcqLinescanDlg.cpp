
// ImageAcqLinescanDlg.cpp : Implementation file
//

#include "stdafx.h"
#include "ImageAcqLinescan.h"
#include "ImageAcqLinescanDlg.h"
#include "afxdialogex.h"
#include <string> 
#include <sstream>
#include <queue> 
#include <afxdlgs.h>
#include <time.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageAcqLinescanDlg dialog
CImageAcqLinescanDlg::CImageAcqLinescanDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImageAcqLinescanDlg::IDD, pParent)
	, m_pThread(NULL)
	, m_RecentCount(100)
	, m_bDisplay(TRUE)
	, m_bSave(FALSE)
	, m_uWaitTimer(0)
	, m_bSoftTrigger(FALSE)
	, m_uImageCount(0)
	, m_uGrabTimeout(4000)
	, m_uGrabQuickRoiVar(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_dfDisplayCount = 0;
}

CImageAcqLinescanDlg::~CImageAcqLinescanDlg()
{
}



void CImageAcqLinescanDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DEVICES, m_listDevices);
	DDX_Control(pDX, IDC_STATIC_PREVIEW, m_Picture);
	DDX_Control(pDX, IDC_CbxTrigType, m_cbxTrigType);
	DDX_Control(pDX, IDC_CbxTrigSource, m_cbxTrigSource);
	DDX_Control(pDX, IDC_CHECK_FORWARD, m_chkForward);
}

BEGIN_MESSAGE_MAP(CImageAcqLinescanDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CImageAcqLinescanDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_PROPERTY, &CImageAcqLinescanDlg::OnBnClickedButtonProperty)
	ON_BN_CLICKED(IDC_BUTTON_SCAN, &CImageAcqLinescanDlg::OnBnClickedButtonScan)
	ON_BN_CLICKED(IDC_BUTTON_START, &CImageAcqLinescanDlg::OnBnClickedButtonStart)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK_USERDEFINEDNAME, &CImageAcqLinescanDlg::OnBnClickedCheckUserdefinedname)
	ON_CBN_SELCHANGE(IDC_CbxTrigType, &CImageAcqLinescanDlg::OnCbnSelchangeCbxtrigtype)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &CImageAcqLinescanDlg::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_SOFTTRIG, &CImageAcqLinescanDlg::OnBnClickedButtonSofttrig)
	ON_BN_CLICKED(IDC_CHECK_FORWARD, &CImageAcqLinescanDlg::OnBnClickedCheckForward)
END_MESSAGE_MAP()

// CImageAcqLinescanDlg message handlers
BOOL CImageAcqLinescanDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon.
	SetIcon(m_hIcon, FALSE);		// Set big icon.

	// Initialize open mode
	// false: user dvpOpenByName open the camear
	// true : user dvpOpenByUserId open the camear
	m_bUserDefinedName = false;
	CheckDlgButton(IDC_CHECK_USERDEFINEDNAME, BST_UNCHECKED);

	// Scan devices that have been connected to a computer in advance.
	OnBnClickedButtonScan();

	// Timer for updating the current information of the frame rate.
	SetTimer(0, 500, NULL);

	m_cbxTrigType.ResetContent();
	m_cbxTrigSource.ResetContent();
	

	GetDlgItem(IDC_EDIT_EXPOSE)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(false);
	m_cbxTrigType.EnableWindow(false);
	m_cbxTrigSource.EnableWindow(false);

	GetDlgItem(IDC_CHECK_FORWARD)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_SOFTTRIG)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_TRIGFILTER)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_TRIGDELAY)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_TRIGFREQPREDIV)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_TRIGFREQMULT)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_TRIGFREQDIV)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_LINERATE)->EnableWindow(false);
	GetDlgItem(IDC_BUTTON_APPLY)->EnableWindow(false);

	m_cbxTrigType.AddString(CString("line tirgger"));
	m_cbxTrigType.AddString(CString("frame tirgger"));
	m_cbxTrigType.AddString(CString("continue mode"));
	m_cbxTrigType.AddString(CString("line and frame"));
	m_cbxTrigType.SetCurSel(2);


	GetDlgItem(IDC_BUTTON_SOFTTRIG)->EnableWindow(false);
	
	return TRUE;
}

void CImageAcqLinescanDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this);// The context of device that is used for drawing.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in the client rectangle.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


void CImageAcqLinescanDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
}


void CImageAcqLinescanDlg::OnTimer(UINT_PTR nIDEvent)
{

	if (nIDEvent == 0)
	{
		if (IsValidHandle(m_handle))
		{
			// Update the information of the frame rate.
			dvpFrameCount count;
			dvpStatus status = dvpGetFrameCount(m_handle, &count);
			ASSERT(status == DVP_STATUS_OK);

			CString str;
			if (m_dfDisplayCount > 0)
			{
				str.Format(_T("%s [%d frames, %.3f fps, Display %.3f fps]"),
					m_strFriendlyName,
					count.uFrameCount,
					count.fFrameRate,
					m_dfDisplayCount * 1000.0 / (m_CurGrabClock - m_StartGrabClock));
			}
			else
			{
				str.Format(_T("%s [%d frames, %.3f fps]"),
					m_strFriendlyName,
					count.uFrameCount,
					count.fFrameRate);
			}

			SetWindowText(str);

		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

HCURSOR CImageAcqLinescanDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


// ch:打开关闭相机操作 | en:open or close camera
void CImageAcqLinescanDlg::OnBnClickedButtonOpen()
{
	dvpStatus status;
	CString strName;

	if (!IsValidHandle(m_handle))
	{
		m_listDevices.GetWindowText(strName);
		if (strName != "")
		{
			// Open the specific device by the selected FriendlyName.
			if (m_bUserDefinedName)
			{
				status = dvpOpenByUserId(CStringA(strName), OPEN_NORMAL, &m_handle);
			}
			else
			{
				status = dvpOpenByName(CStringA(strName), OPEN_NORMAL, &m_handle);
			}

			if (status != DVP_STATUS_OK)
			{
				GetDlgItem(IDC_EDIT_EXPOSE)->EnableWindow(false);
				GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(false);
				m_cbxTrigType.EnableWindow(false);
				m_cbxTrigSource.EnableWindow(false);

				GetDlgItem(IDC_CHECK_FORWARD)->EnableWindow(false);
				GetDlgItem(IDC_BUTTON_SOFTTRIG)->EnableWindow(false);
				GetDlgItem(IDC_EDIT_TRIGFILTER)->EnableWindow(false);
				GetDlgItem(IDC_EDIT_TRIGDELAY)->EnableWindow(false);
				GetDlgItem(IDC_EDIT_TRIGFREQPREDIV)->EnableWindow(false);
				GetDlgItem(IDC_EDIT_TRIGFREQMULT)->EnableWindow(false);
				GetDlgItem(IDC_EDIT_TRIGFREQDIV)->EnableWindow(false);
				GetDlgItem(IDC_EDIT_LINERATE)->EnableWindow(false);
				GetDlgItem(IDC_BUTTON_APPLY)->EnableWindow(false);

				MessageBox(_T("Open the device failed!"));
			}
			else
			{
				m_cbxTrigSource.ResetContent();

				GetDlgItem(IDC_EDIT_EXPOSE)->EnableWindow(true);
				GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(true);
				m_cbxTrigType.EnableWindow(true);
				m_cbxTrigSource.EnableWindow(true);

				GetDlgItem(IDC_CHECK_FORWARD)->EnableWindow(true);
				GetDlgItem(IDC_BUTTON_SOFTTRIG)->EnableWindow(true);
				GetDlgItem(IDC_EDIT_TRIGFILTER)->EnableWindow(true);
				GetDlgItem(IDC_EDIT_TRIGDELAY)->EnableWindow(true);
				GetDlgItem(IDC_EDIT_TRIGFREQPREDIV)->EnableWindow(true);
				GetDlgItem(IDC_EDIT_TRIGFREQMULT)->EnableWindow(true);
				GetDlgItem(IDC_EDIT_TRIGFREQDIV)->EnableWindow(true);
				GetDlgItem(IDC_EDIT_LINERATE)->EnableWindow(true);
				GetDlgItem(IDC_BUTTON_APPLY)->EnableWindow(true);

				m_strFriendlyName = strName;

				InitUI();
				OnCbnSelchangeCbxtrigtype();		
			}
		}
	}
	else
	{
		dvpStreamState state;
		dvpGetStreamState(m_handle, &state);
		if (state == STATE_STARTED)
		{
			status = dvpStop(m_handle);
			ASSERT(status == DVP_STATUS_OK);

			// stop grab thread
			m_dfDisplayCount = 0;
			if (m_pThread != NULL)
			{
				m_evtExit.SetEvent();

				// Wait for ending the thread of acquiring images,Up to 10 seconds,avoid the program dump.
				if (m_pThread->m_hThread != INVALID_HANDLE_VALUE)
				{
					DWORD dw = WaitForSingleObject(m_pThread->m_hThread, 1000);

					// Under normal condition,it should not time out.
					ASSERT(dw == WAIT_OBJECT_0);
					CloseHandle(m_pThread->m_hThread);
					m_pThread->m_hThread = INVALID_HANDLE_VALUE;
				}

				m_pThread->Delete();
				m_pThread = NULL;
			}

		}

		status = dvpClose(m_handle);
		ASSERT(status == DVP_STATUS_OK);

		GetDlgItem(IDC_EDIT_EXPOSE)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(false);
		m_cbxTrigType.EnableWindow(false);
		m_cbxTrigSource.EnableWindow(false);

		GetDlgItem(IDC_CHECK_FORWARD)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_SOFTTRIG)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_TRIGFILTER)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_TRIGDELAY)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_TRIGFREQPREDIV)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_TRIGFREQMULT)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_TRIGFREQDIV)->EnableWindow(false);
		GetDlgItem(IDC_EDIT_LINERATE)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_APPLY)->EnableWindow(false);


		m_handle = 0;
		GetDlgItem(IDC_STATIC_PREVIEW)->InvalidateRect(NULL, 1);
	}

	UpdateControls();
}

void CImageAcqLinescanDlg::UpdateControls()
{
	dvpStatus status;

	if (IsValidHandle(m_handle))
	{
		// The device has been opened at this time.
		// Update the basic controls.
		dvpStreamState state;
		status = dvpGetStreamState(m_handle, &state);
		ASSERT(status == DVP_STATUS_OK);

		GetDlgItem(IDC_BUTTON_OPEN)->SetWindowText(_T("Close"));
		GetDlgItem(IDC_BUTTON_START)->SetWindowText(state == STATE_STARTED ? _T("Stop") : _T("Start"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_PROPERTY)->EnableWindow(TRUE);
	}
	else
	{
		// No device is opened at this time.
		// Update the basic controls.
		GetDlgItem(IDC_BUTTON_OPEN)->SetWindowText(_T("Open"));
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_PROPERTY)->EnableWindow(FALSE);

		if (m_listDevices.GetCount() == 0)
		{
			// No device exists.
			GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(TRUE);
		}

	}

}

// Grab image in a dependent thread, save the latest some frames by using the frame ID as filename and display the image on the window of previewing, if needed.  
 
// ch:取帧和显示线程 | en:grab and display thread
UINT CImageAcqLinescanDlg::ImageThread()
{
	dvpStatus status;
	SYSTEMTIME m_systm;

	// Wait the event of exitting thread.
	while (WaitForSingleObject(m_evtExit, m_uWaitTimer) != WAIT_OBJECT_0)
	{
		void* pBuffer;
		dvpFrame frame;

		// Grab a frame image from the video stream and timeout should not less than the current exposure time.
		status = dvpGetFrame(m_handle, &frame, &pBuffer, m_uGrabTimeout);
		if (status != DVP_STATUS_OK)
		{
			TRACE("dvpGetFrame operation failed, status:%d\r\n", status);
			continue;
		}

		// If need to save images.
		if (m_bSave)
		{
			// Save the bmp image by using systematic time as filename.
			::GetLocalTime(&m_systm);
			m_strFileName.Format(_T("%s\\%d_%d_%d_%d_%d_%d_%d.bmp"), m_strPicturePath, m_systm.wYear, m_systm.wMonth, m_systm.wDay, m_systm.wHour, m_systm.wMinute, m_systm.wSecond, m_systm.wMilliseconds);

			status = dvpSavePicture(&frame, pBuffer, CStringA(m_strFileName), 100);
			ASSERT(status == DVP_STATUS_OK);

			m_uImageCount++;
		}

		// Display images in the preview window, if needed.
		if (m_bDisplay)
		{
			// Get current clock
			m_CurGrabClock = GetTickCount();

			bool bDisplay = false;
			if (m_dfDisplayCount != 0)
			{
				// count the time of image acquisition
				unsigned int GrabMs = (m_CurGrabClock - m_StartGrabClock);

				// Calculate whether the current frame is displayed
				if (GrabMs - m_dfDisplayCount * 33.3f >= 33)
				{
					bDisplay = true;
				}
			}
			else
			{
				bDisplay = true;
				m_StartGrabClock = m_CurGrabClock;
			}

			if (bDisplay)
			{
				dvpStatus status = dvpDrawPicture(&frame, pBuffer, GetDlgItem(IDC_STATIC_PREVIEW)->GetSafeHwnd(), nullptr, nullptr);
				ASSERT(status == DVP_STATUS_OK);
				m_dfDisplayCount++;
			}
		}
	}

	m_pThread->ExitInstance();
	return 0;
}


UINT CImageAcqLinescanDlg::ImageThread(void* pParam)
{
	return ((CImageAcqLinescanDlg*)pParam)->ImageThread();
}

void CImageAcqLinescanDlg::OnDestroy()
{
	if (IsValidHandle(m_handle))
	{
		// Close the thread of grabbing images.
		if (m_pThread != NULL)
		{
			m_evtExit.SetEvent();

			// Wait for ending the thread of acquiring images,Up to 10 seconds,avoid the program dump.
			if (m_pThread->m_hThread != INVALID_HANDLE_VALUE)
			{
				DWORD dw = WaitForSingleObject(m_pThread->m_hThread, 1000);

				// Under normal condition,it should not time out.
				ASSERT(dw == WAIT_OBJECT_0);
				CloseHandle(m_pThread->m_hThread);
				m_pThread->m_hThread = INVALID_HANDLE_VALUE;
			}

			m_pThread->Delete();
			m_pThread = NULL;
		}

		// Stop the video stream.
		dvpStatus status = dvpStop(m_handle);
		ASSERT(status == DVP_STATUS_OK);

		// Close the camera.
		status = dvpClose(m_handle);
		ASSERT(status == DVP_STATUS_OK);
		m_handle = 0;
	}

	CDialogEx::OnDestroy();
}

// Show the dialog box of the camera properties.
// ch:显示相机属性对话框 | en:show the dialog of camera properties
void CImageAcqLinescanDlg::OnBnClickedButtonProperty()
{
	if (IsValidHandle(m_handle))
	{
		dvpShowPropertyModalDialog(m_handle, GetSafeHwnd());

		// At this time some configurations may change, synchronize it to the window GUI.
		UpdateControls();

		// ch:初始化界面 | en:initialize UI
		InitUI();
		
		//char const* szValue = NULL;
		bool TrigEnable;

		dvpStatus st =dvpGetBoolValue(m_handle, "LineTrigEnable", &TrigEnable);
		if (st != DVP_STATUS_OK)
		{
			return;	
		}

		bool pTriggerState;
		st = dvpGetTriggerState(m_handle, &pTriggerState);
		if (st != DVP_STATUS_OK)
		{
			return;
		}

		//if ((strcmp(szValue, "true")==0) && !pTriggerState)
		if((TrigEnable == true) && !pTriggerState)
		{
			// ch:行触发 | en:line tirgger
			m_cbxTrigType.SetCurSel(0);
		}
		//else if ((strcmp(szValue, "false")==0) && pTriggerState)
		else if((TrigEnable == false) && pTriggerState)
		{
			// ch:帧触发 | en:frame tirgger
			m_cbxTrigType.SetCurSel(1);
		}
		//else if ((strcmp(szValue, "false")==0) && !pTriggerState)
		else if((TrigEnable == false) && !pTriggerState)
		{
			// ch:连续模式 | en:continue mode
			m_cbxTrigType.SetCurSel(2);
		}
		else
		{
			// ch:行+帧模式 | en:line and frame
			m_cbxTrigType.SetCurSel(3);
		}
	}
}

// ch:枚举相机 | en:Enumerate Cameras
void CImageAcqLinescanDlg::OnBnClickedButtonScan()
{
	dvpStatus status;
	dvpUint32 i, n = 0;

	// "n" represents the number of cameras that is enumerated successfully, the drop-down list contains each camera's FriendlyName.
	m_listDevices.ResetContent();

	// Get the number of cameras that has been connected to a computer.
	status = dvpRefresh(&n);
	ASSERT(status == DVP_STATUS_OK);

	if (status == DVP_STATUS_OK)
	{
		// Enumeration of up to 16 cameras.
		if (n > 16)
			n = 16;

		memset(m_info, 0, sizeof(dvpCameraInfo) * 16);
		m_CamCount = 0;

		for (i = 0; i < n; i++)
		{
			// Acquire each camera's information one by one.
			status = dvpEnum(i, &m_info[m_CamCount]);
			ASSERT(status == DVP_STATUS_OK);

			if (status == DVP_STATUS_OK)
			{
				// GUI need UNICODE,but the information acquired from cameras is ANSI,so convert the character set from ANSI to UNICODE.
				int item = CB_ERR;
				if (!m_bUserDefinedName)
				{
					item = m_listDevices.AddString(CString(m_info[m_CamCount].FriendlyName));
				}
				else
				{
					// check User Defined Name is null
					if (strlen(m_info[m_CamCount].UserID) == 0)
						continue;
					item = m_listDevices.AddString(CString(m_info[m_CamCount].UserID));
				}
				if (item != CB_ERR)
				{
					m_listDevices.SetItemData(item, m_CamCount);
				}
				m_CamCount++;
			}
		}

		if (i != 0)
		{
			m_listDevices.SetCurSel(0);
		}

		UpdateControls();
	}
}

// ch:视频流开始或停止 | en:start or stop camera stream
void CImageAcqLinescanDlg::OnBnClickedButtonStart()
{
	if (IsValidHandle(m_handle))
	{
		dvpStreamState state;
		dvpStatus status;

		// Implement a button to start and stop according to the current video's status.
		status = dvpGetStreamState(m_handle, &state);
		ASSERT(status == DVP_STATUS_OK);

		if (state != STATE_STOPED)
		{
			status = dvpStop(m_handle);
			ASSERT(status == DVP_STATUS_OK);

			if (m_pThread != NULL)
			{
				m_evtExit.SetEvent();

				// Wait for ending the thread of acquiring images,Up to 10 seconds,avoid the program dump.
				if (m_pThread->m_hThread != INVALID_HANDLE_VALUE)
				{
					DWORD dw = WaitForSingleObject(m_pThread->m_hThread, 1000);

					// Under normal condition,it should not time out.
					ASSERT(dw == WAIT_OBJECT_0);
					CloseHandle(m_pThread->m_hThread);
					m_pThread->m_hThread = INVALID_HANDLE_VALUE;
				}

				m_pThread->Delete();
				m_pThread = NULL;
			}

		}
		else
		{
			if (UpdateData(TRUE))
			{
				bool bTrigerFlg = false;
				status = dvpGetTriggerState(m_handle, &bTrigerFlg);

				// Set as Trigger mode before starting video stream.
				if (status == DVP_STATUS_OK)
				{
					//status = dvpSetTriggerState(m_handle, m_bSoftTrigger ? TRUE : FALSE);
					//if (status != DVP_STATUS_OK)
					//{
					//	// Confirm whether the camera support the trigger function.
					//	MessageBox(_T("Set trigger mode fails"));
					//}
				}

				// init display count
				m_dfDisplayCount = 0;
	
				status = dvpStart(m_handle);
				if (status != DVP_STATUS_OK)
				{
					MessageBox(_T("Start the video stream failed!"));
				}


				// Reset the event of exitting thread.
				m_evtExit.ResetEvent();

				// Create the thread of acquiring image.
				m_pThread = AfxBeginThread(ImageThread, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
				if (m_pThread != NULL)
				{
					m_pThread->m_bAutoDelete = FALSE;

					// Start the thread.
					m_pThread->ResumeThread();
				}
				else
				{
					ASSERT(FALSE);
				}

			}
		}
	}

	UpdateControls();
}


void CImageAcqLinescanDlg::OnClose()
{
	UpdateData(TRUE);
	CDialogEx::OnClose();
}


void CImageAcqLinescanDlg::OnBnClickedCheckUserdefinedname()
{
	if (m_bUserDefinedName == (IsDlgButtonChecked(IDC_CHECK_USERDEFINEDNAME) == BST_CHECKED ? true : false))
	{
		return;
	}

	m_bUserDefinedName = !m_bUserDefinedName;

	// save cur sel item
	CString strName;
	m_listDevices.GetWindowText(strName);

	// reset m_listDevices values
	m_listDevices.ResetContent();
	for (int i = 0; i < m_CamCount; i++)
	{
		int item = CB_ERR;
		if (!m_bUserDefinedName)
		{
			item = m_listDevices.AddString(CString(m_info[i].FriendlyName));
			if (strName == CString(m_info[i].UserID))
			{
				m_listDevices.SetCurSel(item);
			}
		}
		else
		{
			// check User Defined Name is null
			if (strlen(m_info[i].UserID) == 0)
				continue;

			item = m_listDevices.AddString(CString(m_info[i].UserID));
			if (strName == CString(m_info[i].FriendlyName))
			{
				m_listDevices.SetCurSel(item);
			}
		}
		if (item != CB_ERR)
		{
			m_listDevices.SetItemData(item, i);
		}
	}
}


// ch:选择触发类型 | en:select trigger type
void CImageAcqLinescanDlg::OnCbnSelchangeCbxtrigtype()
{
	if (!IsValidHandle(m_handle))
	{
		return;
	}

	int nSel = m_cbxTrigType.GetCurSel();

	CString  strTrigSource;
	m_cbxTrigSource.GetWindowText(strTrigSource);
	
	if (nSel == 0)   // ch:行触发 | en:line tirgger
	{
		// ch:使能行触发 | en:enable line tirgger
		dvpStatus st = dvpSetBoolValue(m_handle, "LineTrigEnable", true);
		// ch:设置帧触发为不使能 | en:disable frame tirgger
		st = dvpSetTriggerState(m_handle, false);

		GetDlgItem(IDC_BUTTON_SOFTTRIG)->EnableWindow(false);

	}
	else if (nSel == 1) // ch:帧触发 | en:frame tirgger
	{
		// ch:设置行触发不使能 | en:disable line tirgger
		dvpStatus st = dvpSetBoolValue(m_handle, "LineTrigEnable", false);
		// ch:设置帧触发为使能 | en:enable frame tirgger
		st = dvpSetTriggerState(m_handle, true);

		GetDlgItem(IDC_BUTTON_SOFTTRIG)->EnableWindow(true);
	}
	else if (nSel == 2)  // ch:连续模式 | en:continue mode
	{
		// ch:设置行触发不使能 | en:disable line tirgger
		dvpStatus st = dvpSetBoolValue(m_handle, "LineTrigEnable", false);
		// ch:设置帧触发为不使能 | en:disable frame tirgger
		st = dvpSetTriggerState(m_handle, false);

		GetDlgItem(IDC_BUTTON_SOFTTRIG)->EnableWindow(false);
	}
	else  // ch:行+帧模式 | en:line and frame tirgger
	{
		// ch:使能行触发 | en:enable line tirgger
		dvpStatus st = dvpSetBoolValue(m_handle, "LineTrigEnable", true);
		// ch:设置帧触发为使能 | en:enable frame tirgger
		st = dvpSetTriggerState(m_handle, true);

		GetDlgItem(IDC_BUTTON_SOFTTRIG)->EnableWindow(true);
	}
	InitUI();

}

// ch:应用配置 | en:apply configuration
void CImageAcqLinescanDlg::OnBnClickedButtonApply()
{
	if (!IsValidHandle(m_handle))
	{
		return;
	}

	GetDlgItem(IDC_BUTTON_APPLY)->EnableWindow(false);


	dvpStreamState state;
	dvpGetStreamState(m_handle, &state);
	if (state == STATE_STARTED)
	{
	}

	//set exposure
	CString strExpose;
	GetDlgItem(IDC_EDIT_EXPOSE)->GetWindowText(strExpose);
	double dExpose = _ttof(strExpose);
	dvpStatus st = dvpSetExposure(m_handle, dExpose);
	if (st != DVP_STATUS_OK)
	{
		MessageBox(_T("failed to set exposure"), _T("Error"));
	}

	//set gain
	CString strGain;
	GetDlgItem(IDC_EDIT_GAIN)->GetWindowText(strGain);
	double fGain = _ttof(strGain);
	st = dvpSetAnalogGain(m_handle, fGain);
	if (st != DVP_STATUS_OK)
	{
		MessageBox(_T("failed to set gain"), _T("Error"));
	}

	//set trigger source
	CString strTrigSource;
	m_cbxTrigSource.GetWindowText(strTrigSource);
	//st = dvpSetConfigString(m_handle, "LineTrigSource", strTrigSource.GetBuffer(0));
	st = dvpSetStringValue(m_handle, "LineTrigSource", strTrigSource.GetBuffer(0));
	strTrigSource.ReleaseBuffer();
	if (st != DVP_STATUS_OK)
	{
		MessageBox(_T("failed to set trigger source"), _T("Error"));
	}

	//set trigger filter
	CString strTrigFilter;
	GetDlgItem(IDC_EDIT_TRIGFILTER)->GetWindowText(strTrigFilter);
	//st = dvpSetConfigString(m_handle, "LineTrigFilter", strTrigFilter.GetBuffer(0));
	st = dvpSetStringValue(m_handle, "LineTrigFilter", strTrigFilter.GetBuffer(0));
	strTrigFilter.ReleaseBuffer();
	if (st != DVP_STATUS_OK)
	{
		MessageBox(_T("failed to set trigger filter"), _T("Error"));
	}

	//set trigger delay
	CString strTrigDelay;
	GetDlgItem(IDC_EDIT_TRIGDELAY)->GetWindowText(strTrigDelay);
	st = dvpSetStringValue(m_handle, "LineTrigDelay", strTrigDelay.GetBuffer(0));
	strTrigDelay.ReleaseBuffer();
	if (st != DVP_STATUS_OK)
	{
		MessageBox(_T("failed to set trigger delay"), _T("Error"));
	}

	CString strTrigFreqPreDiv;
	GetDlgItem(IDC_EDIT_TRIGFREQPREDIV)->GetWindowText(strTrigFreqPreDiv);

	CString strTrigFreqMult;
	GetDlgItem(IDC_EDIT_TRIGFREQMULT)->GetWindowText(strTrigFreqMult);
 
	CString strTrigFreqDiv;
	GetDlgItem(IDC_EDIT_TRIGFREQDIV)->GetWindowText(strTrigFreqDiv);

	int nFreqPreDiv= atoi(strTrigFreqPreDiv);
	int nFreqMult = atoi(strTrigFreqMult);
	int nFreqDiv = atoi(strTrigFreqDiv);

	dvpLineTriggerConfig lineTrigger;
	//pre frequency division of tirgger
	lineTrigger.uPreDiv = nFreqPreDiv;
	//frequency multiplication of tirgger  
	lineTrigger.uMult = nFreqMult;
	//frequency division of tirgger
	lineTrigger.uDiv = nFreqDiv;

	//Set pre frequency division of tirgger
	st = dvpSetIntValue(m_handle,"LinePreDiv",lineTrigger.uPreDiv);
	if (st != DVP_STATUS_OK)
	{
		MessageBox(_T("failed to set pre frequency division"), _T("Error"));
	}
	//Set frequency multiplication of tirgger
	st = dvpSetIntValue(m_handle,"LineMult",lineTrigger.uMult);
	if (st != DVP_STATUS_OK)
	{
		MessageBox(_T("failed to set frequency multiplication"), _T("Error"));
	}

	//Set frequency division of tirgger   
	st = dvpSetIntValue(m_handle,"LineDiv",lineTrigger.uDiv);
	if (st != DVP_STATUS_OK)
	{
		MessageBox(_T("failed to set frequency division"), _T("Error"));
	}

	//Set line rate   
	CString strTrigLineRate;
	GetDlgItem(IDC_EDIT_LINERATE)->GetWindowText(strTrigLineRate);
	double dLine = _ttof(strTrigLineRate);
	st = dvpSetFloatValue(m_handle,"LineRate", dLine);
	if (st != DVP_STATUS_OK)
	{
		MessageBox(_T("failed to set line rate "), _T("Error"));
	}

	InitUI();
	GetDlgItem(IDC_BUTTON_APPLY)->EnableWindow(true);
}


//Initialize ui of tirgger configuration
void CImageAcqLinescanDlg::InitUI()
{
	if (IsValidHandle(m_handle))
	{
		if (m_cbxTrigSource.GetCount() <= 0)
		{
			dvpSelectionDescr SelectionDescr;
			dvpStatus st = dvpGetSelectionDescr(m_handle, "LineTrigSource", &SelectionDescr);
			for (int i = 0; i < SelectionDescr.uCount; i++)
			{
				dvpSelection pSelection;
				 st = dvpGetSelectionDetail(m_handle, "LineTrigSource", i, &pSelection);

				m_cbxTrigSource.AddString(pSelection.string);
			}


		}
		//get exposure
		double exposure;
		if (dvpGetExposure(m_handle, &exposure) == DVP_STATUS_OK)
		{
			CString strExpose;
			strExpose.Format(_T("%.3f"), exposure);
			GetDlgItem(IDC_EDIT_EXPOSE)->SetWindowText(strExpose.GetBuffer(0));
			strExpose.ReleaseBuffer();
		}
		else
		{
			GetDlgItem(IDC_EDIT_EXPOSE)->SetWindowText(_T("0.0"));

			MessageBox(_T("failed to get exposure"), _T("Error"));
		}


		//get gain
		float gain;
		if (dvpGetAnalogGain(m_handle, &gain) == DVP_STATUS_OK)
		{
			CString strGain;
			strGain.Format(_T("%.3f"), gain);
			GetDlgItem(IDC_EDIT_GAIN)->SetWindowText(strGain.GetBuffer(0));
			strGain.ReleaseBuffer();
		}
		else
		{
			GetDlgItem(IDC_EDIT_GAIN)->SetWindowText(_T("0.0"));

			MessageBox(_T("failed to get gain"), _T("Error"));

		}

		//get tirgger source
		char  LineTrigSource[128] = {0};
		dvpStatus st = dvpGetStringValue(m_handle, "LineTrigSource", LineTrigSource,sizeof(LineTrigSource));
		if (st == DVP_STATUS_OK)
		{
			m_cbxTrigSource.SelectString(0, (LPCTSTR)LineTrigSource);
		}
		else
		{
			MessageBox(_T("failed to get tirgger source"), _T("Error"));
		}

		//get scan direction
		bool ReverseScanDirection;
		st = dvpGetBoolValue(m_handle, "ReverseScanDirection", &ReverseScanDirection);
		if (st == DVP_STATUS_OK)
		{
			if (ReverseScanDirection)
			{
				m_chkForward.SetCheck(true);
			}
			else
			{
				m_chkForward.SetCheck(false);
			}
		}
		else
		{
			MessageBox(_T("failed to get scan direction"), _T("Error"));
		}

		//get tirgger filter
		char LineTrigFilter[128] = {0};
		st = dvpGetStringValue(m_handle, "LineTrigFilter", LineTrigFilter,sizeof(LineTrigFilter));
		if (st == DVP_STATUS_OK)
		{
			GetDlgItem(IDC_EDIT_TRIGFILTER)->SetWindowText((LPCTSTR)LineTrigFilter);
		}
		else
		{
			GetDlgItem(IDC_EDIT_TRIGFILTER)->SetWindowText((LPCTSTR)"");
			MessageBox(_T("failed to get tirgger filter"), _T("Error"));
		}

		//get tirgger delay
		char  LineTrigDelay[128] = {0};
		st = dvpGetStringValue(m_handle, "LineTrigDelay", LineTrigDelay,sizeof(LineTrigDelay));
		if (st == DVP_STATUS_OK)
		{
			GetDlgItem(IDC_EDIT_TRIGDELAY)->SetWindowText((LPCTSTR)LineTrigDelay);
		}
		else
		{
			GetDlgItem(IDC_EDIT_TRIGDELAY)->SetWindowText((LPCTSTR)"");
			MessageBox(_T("failed to get tirgger delay"), _T("Error"));
		}

		//get pre frequency division of tirgger
		int64_t dLinePreDiv;
		dvpIntDescr LinePreDiv;
		st = dvpGetIntValue(m_handle,"LinePreDiv",&dLinePreDiv,&LinePreDiv);
		if (st == DVP_STATUS_OK)
		{
			CString strPreDiv;
			strPreDiv.Format(_T("%d"), dLinePreDiv); 
			GetDlgItem(IDC_EDIT_TRIGFREQPREDIV)->SetWindowText(strPreDiv);
		}
		else
		{
			GetDlgItem(IDC_EDIT_TRIGFREQPREDIV)->SetWindowText((LPCTSTR)"");
			MessageBox(_T("failed to get pre frequency division of tirgger"), _T("Error"));
		}

		//get frequency multiplication of tirgger
		int64_t dLineMultDiv;
		dvpIntDescr LineMultDiv;
		st = dvpGetIntValue(m_handle,"LineMult",&dLineMultDiv,&LineMultDiv);
		if(st == DVP_STATUS_OK)
		{
			CString strMult;
			strMult.Format(_T("%d"), dLineMultDiv);
			GetDlgItem(IDC_EDIT_TRIGFREQMULT)->SetWindowText(strMult);
		}
		else
		{
			GetDlgItem(IDC_EDIT_TRIGFREQMULT)->SetWindowText((LPCTSTR)"");
			MessageBox(_T("failed to get frequency multiplication of tirgger"), _T("Error"));
		}

		//get frequency division of tirgger
		int64_t dLineDiv;
		dvpIntDescr LineDiv;
		st = dvpGetIntValue(m_handle,"LineDiv",&dLineDiv,&LineDiv);
		if(st == DVP_STATUS_OK)
		{
			CString strDiv;
			strDiv.Format(_T("%d"), dLineDiv);
			GetDlgItem(IDC_EDIT_TRIGFREQDIV)->SetWindowText(strDiv);
		}
		else
		{
			GetDlgItem(IDC_EDIT_TRIGFREQDIV)->SetWindowText((LPCTSTR)"");
			MessageBox(_T("failed to get frequency division of tirgger"), _T("Error"));
		}

		//get line rate
		float dLineRate;
		dvpFloatDescr LineRate;
		st = dvpGetFloatValue(m_handle,"LineRate", &dLineRate,&LineRate);

		if (st == DVP_STATUS_OK)
		{
			CString strLineRate;
			strLineRate.Format("%f", dLineRate);
			GetDlgItem(IDC_EDIT_LINERATE)->SetWindowText(strLineRate);
		}
		else
		{
			GetDlgItem(IDC_EDIT_LINERATE)->SetWindowText((LPCTSTR)"");
			MessageBox(_T("failed to get line rate"), _T("Error"));
		}

		//get encoder counter
		dvpStr  strEncoderForwardCounter,strEncoderBackwardCounter;

		//get counter of Forward Encoder
		st = dvpGetConfigString(m_handle,"EncoderForwardCounter",&strEncoderForwardCounter);
		if(st == DVP_STATUS_OK)
		{
			GetDlgItem(IDC_EDIT_EncodeForCount)->SetWindowText(strEncoderForwardCounter);
		}
		else
		{
			GetDlgItem(IDC_EDIT_EncodeForCount)->SetWindowText((LPCTSTR)"");
			MessageBox(_T("failed to get counter of Forward Encoder"), _T("Error"));
		}

		//get counter of Backward Encoder
		st = dvpGetConfigString(m_handle,"EncoderBackwardCounter",&strEncoderBackwardCounter);
		if(st == DVP_STATUS_OK)
		{
			GetDlgItem(IDC_EDIT_EncodeBackCount)->SetWindowText(strEncoderBackwardCounter);
		}
		else
		{
			GetDlgItem(IDC_EDIT_EncodeBackCount)->SetWindowText((LPCTSTR)"");
			MessageBox(_T("failed to get counter of Backward Encoder"), _T("Error"));
		}
	}

}

//soft tirgger
void CImageAcqLinescanDlg::OnBnClickedButtonSofttrig()
{
	if (IsValidHandle(m_handle))
	{
		//dvpStatus status = dvpSetTriggerState(m_handle, TRUE);
		dvpStatus status = dvpTriggerFire(m_handle);
	}
}

/* set Scan Direction */
void CImageAcqLinescanDlg::OnBnClickedCheckForward()
{
	int nForward = m_chkForward.GetCheck();
	if (nForward == 0)
	{
		//dvpSetConfigString(m_handle, "LineImageForward","false");
		dvpStatus st = dvpSetBoolValue(m_handle, "ReverseScanDirection",false);
	}
	else
	{
		dvpStatus st = dvpSetBoolValue(m_handle, "ReverseScanDirection",true);
	}
}
