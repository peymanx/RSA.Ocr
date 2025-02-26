
// ImageAcquisitionDlg.cpp : Implementation file
//

#include "stdafx.h"
#include "ImageAcquisition.h"
#include "ImageAcquisitionDlg.h"
#include "afxdialogex.h"
#include <string> 
#include <sstream>
#include <queue> 
#include <afxdlgs.h>
#include <time.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
   
// DVP API rely on.
#ifdef _M_X64
#pragma comment(lib, "../../../library/Visual C++/lib/x64/DVPCamera64.lib")
#else
#pragma comment(lib, "../../../library/Visual C++/lib/x86/DVPCamera.lib")
#endif

// Convert the type of string from ANSI to UNICODE.
std::wstring a2w(LPCCH str)
{
	std::wstring wstr = _T("");
	if (str == NULL)
		return wstr;
	if (strlen(str) == 0)
		return wstr;
	wstr.resize(MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0) - 1);
	MultiByteToWideChar(CP_ACP, 0, str, -1, &wstr[0], wstr.size());
	return wstr;
}

// Convert the type of string from UNICODE to ANSI.
std::string w2a(LPCWCH wstr)
{
	std::string str = "";
	if (wstr == NULL)
		return str;
	if (wcslen(wstr) == 0)
		return str;
	str.resize(WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL) - 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, &str[0], str.size(), NULL, NULL);
	return str;
}

// CImageAcquisitionDlg dialog
CImageAcquisitionDlg::CImageAcquisitionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImageAcquisitionDlg::IDD, pParent),
	 m_pThread(NULL)
	, m_strPicturePath(_T("."))
	, m_RecentCount(100)
	, m_bDisplay(TRUE)
	, m_bSave(FALSE)
    , m_uWaitTimer(0)
    , m_bSoftTrigger(FALSE)
    , m_uImageCount(0)
    , m_uGrabTimeout(4000)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    
    // Load the parameter of window.
    m_bDisplay = GetPrivateProfileInt(_T("param"), _T("display"), m_bDisplay, _T(".\\ImageAcquisition.ini"));
    m_bSave = GetPrivateProfileInt(_T("param"), _T("save"), m_bSave, _T(".\\ImageAcquisition.ini"));
    m_uWaitTimer = GetPrivateProfileInt(_T("param"), _T("wait"), m_uWaitTimer, _T(".\\ImageAcquisition.ini"));
    m_uGrabTimeout = GetPrivateProfileInt(_T("param"), _T("timeout"), m_uGrabTimeout, _T(".\\ImageAcquisition.ini"));
    m_bSoftTrigger = GetPrivateProfileInt(_T("param"), _T("SoftTrig"), m_bSoftTrigger, _T(".\\ImageAcquisition.ini"));

    TCHAR tmp[MAX_PATH];
    GetPrivateProfileString(_T("param"), _T("path"), m_strPicturePath, tmp, MAX_PATH, _T(".\\ImageAcquisition.ini"));
    m_strPicturePath = tmp;
	if ( m_strPicturePath.IsEmpty() )
	{
		// m_strPicturePath
		// 获取当前目录
		TCHAR szBuf[MAX_PATH] = {0};
		GetModuleFileName(NULL, szBuf, MAX_PATH);
		TCHAR *p = _tcsrchr(szBuf, '\\');
		*p = 0;
		m_strPicturePath = szBuf;
	}

	m_dfDisplayCount = 0;
}

CImageAcquisitionDlg::~CImageAcquisitionDlg()
{
    // Save the parameter of window.
    CString str;

    str.Format(_T("%d"), m_bDisplay);
    WritePrivateProfileString(_T("param"), _T("display"), str, _T(".\\ImageAcquisition.ini"));
    str.Format(_T("%d"), m_bSave);
    WritePrivateProfileString(_T("param"), _T("save"), str, _T(".\\ImageAcquisition.ini"));
    str.Format(_T("%d"), m_uWaitTimer);
    WritePrivateProfileString(_T("param"), _T("wait"), str, _T(".\\ImageAcquisition.ini"));
    str.Format(_T("%d"), m_uGrabTimeout);
    WritePrivateProfileString(_T("param"), _T("timeout"), str, _T(".\\ImageAcquisition.ini"));
    str.Format(_T("%d"), m_bSoftTrigger);
    WritePrivateProfileString(_T("param"), _T("SoftTrig"), str, _T(".\\ImageAcquisition.ini"));
    WritePrivateProfileString(_T("param"), _T("path"), m_strPicturePath, _T(".\\ImageAcquisition.ini"));
}



void CImageAcquisitionDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Check(pDX, IDC_CHECK_DISPLAY, m_bDisplay);
    DDX_Check(pDX, IDC_CHECK_SAVE, m_bSave);
    DDX_Control(pDX, IDC_COMBO_DEVICES, m_listDevices);
    DDX_Control(pDX,IDC_STATIC_PREVIEW,m_Picture);
    DDX_Text(pDX, IDC_EDIT_SAVEPATH, m_strPicturePath);
    DDX_Text(pDX, IDC_EDIT_WAIT, m_uWaitTimer);
    DDX_Check(pDX, IDC_CHECK_SOFT_TRIGGER, m_bSoftTrigger);
    DDX_Text(pDX, IDC_EDIT_TIMEOUT, m_uGrabTimeout);
}

BEGIN_MESSAGE_MAP(CImageAcquisitionDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CImageAcquisitionDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_PROPERTY, &CImageAcquisitionDlg::OnBnClickedButtonProperty)
	ON_BN_CLICKED(IDC_BUTTON_SCAN, &CImageAcquisitionDlg::OnBnClickedButtonScan)
	ON_BN_CLICKED(IDC_BUTTON_START, &CImageAcquisitionDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_FILE, &CImageAcquisitionDlg::OnBnClickedButtonOpenFile)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SELECTPATH, &CImageAcquisitionDlg::OnBnClickedButtonSelectpath)
    ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CHECK_SOFT_TRIGGER, &CImageAcquisitionDlg::OnBnClickedCheckSoftTrigger)
	ON_BN_CLICKED(IDC_CHECK_USERDEFINEDNAME, &CImageAcquisitionDlg::OnBnClickedCheckUserdefinedname)
END_MESSAGE_MAP()

// CImageAcquisitionDlg message handlers
BOOL CImageAcquisitionDlg::OnInitDialog()
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
	return TRUE;  
}

void CImageAcquisitionDlg::OnPaint()
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


void CImageAcquisitionDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// Adjust the video graphic window automatically according to the size of the window.
	UpdataPreWindowSize(cx, cy);
}

void CImageAcquisitionDlg::UpdataPreWindowSize(int x, int y)
{
	if (IsValidHandle(m_handle))
	{
		dvpRegion roi;
		dvpStatus status = dvpGetRoi(m_handle, &roi);

		CRect rc, rcPreview, rcOld;
		GetDlgItem(IDC_STATIC_PREVIEW)->GetWindowRect(rcOld);
		ScreenToClient(rcOld);

		GetClientRect(rc);
		rc.left += rcOld.left;
		rc.right -= 6;
		rc.top += rcOld.top;
		rc.bottom -= 6;

		rcPreview.SetRect(0, 0, roi.W, roi.H);

		if(rc.Width() * rcPreview.Height() > 
			rc.Height() * rcPreview.Width())
		{
			rcPreview.SetRect(
				0,
				0,
				rc.Height() * roi.W / roi.H, 
				rc.Height());
		}
		else
		{
			rcPreview.SetRect(
				0, 
				0,
				rc.Width(),
				rc.Width() * roi.H / roi.W);
		}

		rcPreview.MoveToXY(rc.TopLeft());

		GetDlgItem(IDC_STATIC_PREVIEW)->MoveWindow(rcPreview);
	}
}


void CImageAcquisitionDlg::OnTimer(UINT_PTR nIDEvent)
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
			if ( m_dfDisplayCount > 0 )
			{
				str.Format(_T("%s [%d frames, %.3f fps, Display %.3f fps]"),
					m_strFriendlyName,
					count.uFrameCount,
					count.fFrameRate,
					m_dfDisplayCount*1000.0/(m_CurGrabClock - m_StartGrabClock));
			}
			else
			{
				str.Format(_T("%s [%d frames, %.3f fps]"),
					m_strFriendlyName,
					count.uFrameCount,
					count.fFrameRate);
			}

			SetWindowText(str);

            str.Format(_T("%d"), m_uImageCount);
            GetDlgItem(IDC_STATIC_COUNT)->SetWindowText(str);

		}
	}

	CDialogEx::OnTimer(nIDEvent);
}

HCURSOR CImageAcquisitionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CImageAcquisitionDlg::OnBnClickedButtonOpen()
{
    dvpStatus status;
    CString strName;

    if (!IsValidHandle(m_handle))
    {
        m_listDevices.GetWindowText(strName);
        if (strName != "")
        {
			// Open the specific device by the selected FriendlyName.
			if ( m_bUserDefinedName )
			{
				status = dvpOpenByUserId(w2a(strName).c_str(), OPEN_NORMAL, &m_handle);
			}
			else
			{
				status = dvpOpenByName(w2a(strName).c_str(), OPEN_NORMAL, &m_handle);
			}

            if(status != DVP_STATUS_OK)
            {
                MessageBox(_T("Open the device failed!"));
            }
            else
            {
                m_strFriendlyName = strName;
            }
        }
    }
    else
    {
		dvpStreamState state;
		dvpGetStreamState(m_handle, &state);
		if ( state == STATE_STARTED )
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
        m_handle = 0;
		GetDlgItem(IDC_STATIC_PREVIEW)->InvalidateRect(NULL, 1);
    }

    UpdateControls();
}

void CImageAcquisitionDlg::UpdateControls()
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

		// Check whether the trigger function can be supported by camera.
		bool bTriggerFlg = false;
		status = dvpGetTriggerState(m_handle, &bTriggerFlg);
		if ( status == DVP_STATUS_OK )
		{
			m_bSoftTrigger = bTriggerFlg;
			GetDlgItem(IDC_CHECK_SOFT_TRIGGER)->EnableWindow(TRUE);
		}
		else
		{
			m_bSoftTrigger = FALSE;
			GetDlgItem(IDC_CHECK_SOFT_TRIGGER)->EnableWindow(FALSE);
		}
		CheckDlgButton(IDC_CHECK_SOFT_TRIGGER, m_bSoftTrigger ? BST_CHECKED: BST_UNCHECKED);
		// Update the related controls.
		if (state == STATE_STARTED)
		{
            // If the thread of acquiring images is still running,it can simplify multithreaded synchronization by disabling some operations.
			GetDlgItem(IDC_CHECK_SAVE)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_DISPLAY)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON_SELECTPATH)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_SAVEPATH)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDIT_WAIT)->EnableWindow(FALSE);
            GetDlgItem(IDC_EDIT_TIMEOUT)->EnableWindow(FALSE);
			if ( status == DVP_STATUS_OK )
			{
				GetDlgItem(IDC_CHECK_SOFT_TRIGGER)->EnableWindow(FALSE);
			}
		}
		else
		{
			GetDlgItem(IDC_CHECK_SAVE)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_DISPLAY)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_SELECTPATH)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_SAVEPATH)->EnableWindow(TRUE);
            GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(TRUE);
            GetDlgItem(IDC_EDIT_WAIT)->EnableWindow(TRUE);
            GetDlgItem(IDC_EDIT_TIMEOUT)->EnableWindow(TRUE);
			if ( status == DVP_STATUS_OK )
			{
				GetDlgItem(IDC_CHECK_SOFT_TRIGGER)->EnableWindow(TRUE);
			}
		}
	}
	else
	{
		// No device is opened at this time.
		// Update the basic controls.
		GetDlgItem(IDC_BUTTON_OPEN)->SetWindowTextW(_T("Open"));
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

		// Update the related controls.
		GetDlgItem(IDC_STATIC_PREVIEW)->InvalidateRect(NULL);
		GetDlgItem(IDC_CHECK_SAVE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_DISPLAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SELECTPATH)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SAVEPATH)->EnableWindow(TRUE);
		m_bSoftTrigger = FALSE;
		GetDlgItem(IDC_CHECK_SOFT_TRIGGER)->EnableWindow(FALSE);
		CheckDlgButton(IDC_CHECK_SOFT_TRIGGER, BST_UNCHECKED);
	}

	SendMessage(WM_SIZE);
}

// Grab image in a dependent thread, save the latest some frames by using the frame ID as filename and display the image on the window of previewing, if needed.   
UINT CImageAcquisitionDlg::ImageThread()
{
	dvpStatus status;
	SYSTEMTIME m_systm;

	// Wait the event of exitting thread.
	while (WaitForSingleObject(m_evtExit, m_uWaitTimer) != WAIT_OBJECT_0) 
	{
		void *pBuffer;
		dvpFrame frame;

        if (m_bSoftTrigger)
		{
			bool bSoftTrigger = false;
			status = dvpGetTriggerState(m_handle, &bSoftTrigger);
			// ASSERT(status == DVP_STATUS_OK);
			if ( bSoftTrigger )
			{
				// Check Soft Trigger Loop
				bool bSoftTriggerLoop = false;
				status = dvpGetSoftTriggerLoopState(m_handle, &bSoftTriggerLoop);
				// ASSERT(status == DVP_STATUS_OK);
				if ( !bSoftTriggerLoop )
				{
					status = dvpTriggerFire(m_handle);
					if (status != DVP_STATUS_OK)
					{
						// Trigger failure maybe result from that the trigger signal interval is too dense.
						// ASSERT(status == DVP_STATUS_OK);
						continue;
					}
				}
			}
        }

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
			m_strFileName.Format(_T("%s\\%d_%d_%d_%d_%d_%d_%d.bmp"),m_strPicturePath,m_systm.wYear,m_systm.wMonth,m_systm.wDay,m_systm.wHour,m_systm.wMinute,m_systm.wSecond,m_systm.wMilliseconds);

            status = dvpSavePicture(&frame, pBuffer, w2a(m_strFileName).c_str(), 100);
			ASSERT(status == DVP_STATUS_OK);

            m_uImageCount++;
		}

		// Display images in the preview window, if needed.
		if (m_bDisplay)
		{
			// Get current clock
			m_CurGrabClock = GetTickCount();

			bool bDisplay = false;
			if ( m_dfDisplayCount != 0 )
			{
				// count the time of image acquisition
				unsigned int GrabMs = (m_CurGrabClock - m_StartGrabClock);

				// Calculate whether the current frame is displayed
				if ( GrabMs - m_dfDisplayCount * 33.3f  >= 33 )
				{
					bDisplay = true;
				}
			}
			else
			{
				bDisplay = true;
				m_StartGrabClock = m_CurGrabClock;
			}

			if ( bDisplay )
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


UINT CImageAcquisitionDlg::ImageThread(void *pParam)
{
	return ((CImageAcquisitionDlg*)pParam)->ImageThread();
}

void CImageAcquisitionDlg::OnDestroy()
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
void CImageAcquisitionDlg::OnBnClickedButtonProperty()
{
	if (IsValidHandle(m_handle))
	{
		dvpShowPropertyModalDialog(m_handle, GetSafeHwnd());

		// At this time some configurations may change, synchronize it to the window GUI.
		UpdateControls();
		RECT rt = {0};
		GetClientRect(&rt);
		UpdataPreWindowSize(rt.right, rt.bottom);
	}
}

void CImageAcquisitionDlg::OnBnClickedButtonScan()
{
	dvpStatus status;
	dvpUint32 i, n=0;
	
	// "n" represents the number of cameras that is enumerated successfully, the drop-down list contains each camera's FriendlyName.
	m_listDevices.ResetContent();

	// Get the number of cameras that has been connected to a computer.
	status = dvpRefresh(&n);
	ASSERT(status == DVP_STATUS_OK);

	if ( status == DVP_STATUS_OK )
	{
		// Enumeration of up to 16 cameras.
		if ( n > 16 )
			n = 16;

		memset(m_info, 0, sizeof(dvpCameraInfo)*16);
		m_CamCount = 0;

		for ( i = 0; i < n; i++)
		{
			// Acquire each camera's information one by one.
			status = dvpEnum(i, &m_info[m_CamCount]);
			ASSERT(status == DVP_STATUS_OK);

			if (status == DVP_STATUS_OK)
			{
				// GUI need UNICODE,but the information acquired from cameras is ANSI,so convert the character set from ANSI to UNICODE.
				int item = CB_ERR;
				if ( !m_bUserDefinedName )
				{
					item = m_listDevices.AddString(a2w(m_info[m_CamCount].FriendlyName).c_str());
				}
				else
				{
					// check User Defined Name is null
					if ( strlen(m_info[m_CamCount].UserID) == 0 )
						continue;
					item = m_listDevices.AddString(a2w(m_info[m_CamCount].UserID).c_str());
				}
				if ( item != CB_ERR )
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


void CImageAcquisitionDlg::OnBnClickedButtonStart()
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
				if ( status == DVP_STATUS_OK )
				{
					status = dvpSetTriggerState(m_handle, m_bSoftTrigger?TRUE:FALSE);
					if (status != DVP_STATUS_OK)
					{
						// Confirm whether the camera support the trigger function.
						MessageBox(_T("Set trigger mode fails"));
					}
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

void CImageAcquisitionDlg::OnBnClickedButtonOpenFile()
{
	ShellExecute(NULL,_T("open"),m_strPicturePath,NULL,NULL,SW_SHOWNORMAL); 
}

 void CImageAcquisitionDlg::OnBnClickedButtonSelectpath()
 {
	 BROWSEINFO bi;
	 TCHAR Buffer[MAX_PATH];

	 // Initialize the parameter of bi's entrance.
	 bi.hwndOwner = NULL;

	 // Initialize the root directory.
	 bi.pidlRoot =NULL;
     
	 // If the following parameter is NULL,the dialog box can not be showed.
	 bi.pszDisplayName =(LPWSTR)Buffer;
	 bi.lpszTitle = _T("Select file path");
	 
	 // With the style of the edit box.
	 bi.ulFlags = 0x0040 | BIF_RETURNONLYFSDIRS | BIF_EDITBOX;
	 bi.lpfn = NULL;
	 bi.lParam = 0;
	 bi.iImage=IDR_MAINFRAME;

	 // End initializing the parameter of bi's entrance.
	
	 // Call and show the selection dialog.
	 LPITEMIDLIST pIDList = SHBrowseForFolder(&bi);

	 if(pIDList)
	 {
		// Get the folder path and put it into the Buffer.
		 SHGetPathFromIDList(pIDList, (LPWSTR)Buffer);

		 // Save the path in a CString object.
		 m_strPicturePath = (LPWSTR)Buffer;
	 }
	 m_bSave = TRUE;
	 UpdateData(FALSE);
 }


 void CImageAcquisitionDlg::OnClose()
 {
     UpdateData(TRUE);
     CDialogEx::OnClose();
 }


 void CImageAcquisitionDlg::OnBnClickedCheckSoftTrigger()
 {
	 if ( !IsValidHandle(m_handle) )
		 return ;

	 dvpStatus status;
	 dvpStreamState state ;
	 status = dvpGetStreamState(m_handle, &state);
	 ASSERT(status == DVP_STATUS_OK);

	 if ( state == STATE_STARTED )
	 {
		 // Close the video stream.
		 status = dvpStop(m_handle);
		 ASSERT(status == DVP_STATUS_OK);
	 }

	 // Set the Trigger State.
	 m_bSoftTrigger = IsDlgButtonChecked(IDC_CHECK_SOFT_TRIGGER) == BST_CHECKED;
	 status = dvpSetTriggerState(m_handle, m_bSoftTrigger? true : false);
	 // ASSERT(status == DVP_STATUS_OK);

	 if ( state == STATE_STARTED )
	 {
		 // Open the video stream.
		 status = dvpStart(m_handle);
		ASSERT(status == DVP_STATUS_OK);
	 }
	 
 }


 void CImageAcquisitionDlg::OnBnClickedCheckUserdefinedname()
 {
	 if ( m_bUserDefinedName == (IsDlgButtonChecked(IDC_CHECK_USERDEFINEDNAME) == BST_CHECKED ? true : false))
	 {
		 return ;
	 }

	 m_bUserDefinedName = !m_bUserDefinedName;

	 // save cur sel item
	 CString strName;
	 m_listDevices.GetWindowText(strName);

	 // reset m_listDevices values
	 m_listDevices.ResetContent();
	 for ( int i = 0; i < m_CamCount; i++ )
	 {
		 int item = CB_ERR;
		 if ( !m_bUserDefinedName )
		 {
			 item = m_listDevices.AddString(a2w(m_info[i].FriendlyName).c_str());
			 if ( strName == a2w(m_info[i].UserID).c_str() )
			 {
				 m_listDevices.SetCurSel(item);
			 }
		 }
		 else
		 {
			 // check User Defined Name is null
			 if ( strlen(m_info[i].UserID) == 0 )
				 continue;

			 item = m_listDevices.AddString(a2w(m_info[i].UserID).c_str());
			 if ( strName == a2w(m_info[i].FriendlyName).c_str() )
			 {
				 m_listDevices.SetCurSel(item);
			 }
		 }
		 if ( item != CB_ERR )
		 {
			 m_listDevices.SetItemData(item, i);
		 }
	 }
 }
