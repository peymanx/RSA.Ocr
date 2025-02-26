
// MultipleCameraDlg.cpp : implementation file

#include "stdafx.h"
#include "MultipleCamera.h"
#include "MultipleCameraDlg.h"
#include "afxdialogex.h" 
#include <set>

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

// CMultipleCameraDlg dialog

// Check whether the camera's handle is valid.
bool IsValidHandle(dvpHandle handle)
{
	bool bValidHandle;
	dvpStatus status = dvpIsValid(handle, &bValidHandle);
	if (status == DVP_STATUS_OK)
	{
		return bValidHandle;
	}

	return false;
}

// Callback function for acquiring the video stream.
INT CMultipleCameraDlg::OnDrawPicture(dvpHandle handle,
	dvpStreamEvent event, 
	void *pContext, 
	dvpFrame *pFrame, 
	void *pBuffer)
{
	// Preview the video in the callback function.
	CMultipleCameraDlg *pDlg = ((CMultipleCameraDlg*)pContext);

	
	int item = -1;
	for ( int i = 0; i < 4; i++ )
	{
		if ( handle == pDlg->m_handle[i] )
		{
			item = i;
			break;
		}
	}

	if ( item == -1 )
	{
		return 0;
	}

	// Get current clock
	pDlg->m_CurGrabClock[item] = GetTickCount();

	bool bDisplay = false;
	if ( pDlg->m_dfDisplayCount[item] != 0 )
	{
		// count the time of image acquisition
		unsigned int GrabMs = (pDlg->m_CurGrabClock[item] - pDlg->m_StartGrabClock[item]);

		// Calculate whether the current frame is displayed
		if ( GrabMs - pDlg->m_dfDisplayCount[item] * 33.3f  >= 33 )
		{
			bDisplay = true;
		}
	}
	else
	{
		bDisplay = true;
		pDlg->m_StartGrabClock[item] = pDlg->m_CurGrabClock[item];
	}

	if ( bDisplay )
	{
		// It demonstrates the usual video drawing,and it is not recommended to take a longer time operation in the callback function,
		// in order to avoid affecting the frame rate and the real-time of acquiring images.
		// The acquired image data buffer is valid only before the function returns,so the buffer pointer should not be passed out, 
		// however, the user can malloc memory and copy image data.

		HWND hWnd = NULL;
		if ( item == 0 )
		{
			hWnd = pDlg->GetDlgItem(IDC_STATIC_PREVIEW1)->GetSafeHwnd();
		}
		else if ( item == 1 )
		{
			hWnd = pDlg->GetDlgItem(IDC_STATIC_PREVIEW2)->GetSafeHwnd();
		}
		else if ( item == 2 )
		{
			hWnd = pDlg->GetDlgItem(IDC_STATIC_PREVIEW3)->GetSafeHwnd();
		}
		else if ( item == 3 )
		{
			hWnd = pDlg->GetDlgItem(IDC_STATIC_PREVIEW4)->GetSafeHwnd();
		}

		dvpStatus status = dvpDrawPicture(pFrame, pBuffer, hWnd, nullptr, nullptr);
		ASSERT(status == DVP_STATUS_OK);
		pDlg->m_dfDisplayCount[item]++;
	}

	
	// "return 0" represents the image data has been discarded and no longer been used.
	return 0;
}



CMultipleCameraDlg::CMultipleCameraDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMultipleCameraDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

void CMultipleCameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DEVICES1, m_listDevices1);
	DDX_Control(pDX, IDC_COMBO_DEVICES2, m_listDevices2);
	DDX_Control(pDX, IDC_COMBO_DEVICES3, m_listDevices3);
	DDX_Control(pDX, IDC_COMBO_DEVICES4, m_listDevices4);
}

BEGIN_MESSAGE_MAP(CMultipleCameraDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SCAN1, &CMultipleCameraDlg::OnBnClickedButtonScan1)
	ON_BN_CLICKED(IDC_BUTTON_SCAN2, &CMultipleCameraDlg::OnBnClickedButtonScan2)
	ON_BN_CLICKED(IDC_BUTTON_SCAN3, &CMultipleCameraDlg::OnBnClickedButtonScan3)
	ON_BN_CLICKED(IDC_BUTTON_SCAN4, &CMultipleCameraDlg::OnBnClickedButtonScan4)
	ON_BN_CLICKED(IDC_BUTTON_OPEN1, &CMultipleCameraDlg::OnBnClickedButtonOpen1)
	ON_BN_CLICKED(IDC_BUTTON_OPEN2, &CMultipleCameraDlg::OnBnClickedButtonOpen2)
	ON_BN_CLICKED(IDC_BUTTON_OPEN3, &CMultipleCameraDlg::OnBnClickedButtonOpen3)
	ON_BN_CLICKED(IDC_BUTTON_OPEN4, &CMultipleCameraDlg::OnBnClickedButtonOpen4)
	ON_BN_CLICKED(IDC_BUTTON_PROPERTY1, &CMultipleCameraDlg::OnBnClickedButtonProperty1)
	ON_BN_CLICKED(IDC_BUTTON_PROPERTY2, &CMultipleCameraDlg::OnBnClickedButtonProperty2)
	ON_BN_CLICKED(IDC_BUTTON_PROPERTY3, &CMultipleCameraDlg::OnBnClickedButtonProperty3)
	ON_BN_CLICKED(IDC_BUTTON_PROPERTY4, &CMultipleCameraDlg::OnBnClickedButtonProperty4)
	ON_BN_CLICKED(IDC_BUTTON_SET1, &CMultipleCameraDlg::OnBnClickedButtonSet1)
	ON_BN_CLICKED(IDC_BUTTON_SET2, &CMultipleCameraDlg::OnBnClickedButtonSet2)
	ON_BN_CLICKED(IDC_BUTTON_SET3, &CMultipleCameraDlg::OnBnClickedButtonSet3)
	ON_BN_CLICKED(IDC_BUTTON_SET4, &CMultipleCameraDlg::OnBnClickedButtonSet4)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_BIND, &CMultipleCameraDlg::OnBnClickedButtonSaveBind)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_BIND, &CMultipleCameraDlg::OnBnClickedButtonLoadBind)
	ON_BN_CLICKED(IDC_BUTTON_PLAY1, &CMultipleCameraDlg::OnBnClickedButtonPlay1)
	ON_BN_CLICKED(IDC_BUTTON_PLAY2, &CMultipleCameraDlg::OnBnClickedButtonPlay2)
	ON_BN_CLICKED(IDC_BUTTON_PLAY3, &CMultipleCameraDlg::OnBnClickedButtonPlay3)
	ON_BN_CLICKED(IDC_BUTTON_PLAY4, &CMultipleCameraDlg::OnBnClickedButtonPlay4)
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_DEVICES1, &CMultipleCameraDlg::OnCbnSelchangeComboDevices1)
	ON_CBN_SELCHANGE(IDC_COMBO_DEVICES2, &CMultipleCameraDlg::OnCbnSelchangeComboDevices2)
	ON_CBN_SELCHANGE(IDC_COMBO_DEVICES3, &CMultipleCameraDlg::OnCbnSelchangeComboDevices3)
	ON_CBN_SELCHANGE(IDC_COMBO_DEVICES4, &CMultipleCameraDlg::OnCbnSelchangeComboDevices4)
END_MESSAGE_MAP()


BOOL CMultipleCameraDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon.
	SetIcon(m_hIcon, FALSE);		// Set small icon.

	// Initialize the id of the camera's handle.
	m_handle[0] = 0;
	m_handle[1] = 0;
	m_handle[2] = 0;
	m_handle[3] = 0;

	m_dfDisplayCount[0] = 0;
	m_dfDisplayCount[1] = 0;
	m_dfDisplayCount[2] = 0;
	m_dfDisplayCount[3] = 0;

	// Scan devices that have been connected to a computer in advance.
	OnBnClickedButtonScan1();
	OnBnClickedButtonScan2();
	OnBnClickedButtonScan3();
	OnBnClickedButtonScan4();

	// Timer for updating the current information of the frame rate.
	SetTimer(0, 500, NULL);

	return TRUE;  
}

void CMultipleCameraDlg::OnDestroy()
{
	CDialogEx::OnDestroy();


	// Close devices at the moment of  destroying the window.
	for (UINT i = 0; i < 4; i++)
	{
		if(IsValidHandle(m_handle[i]))
		{
			dvpStop(m_handle[i]);
		}
	}

	for (UINT i = 0; i < 4; i++)
	{
		if(IsValidHandle(m_handle[i]))
		{
			dvpClose(m_handle[i]);
			m_handle[i] = 0;
		}
	}
}


void CMultipleCameraDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); 
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in the client rectangle.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


HCURSOR CMultipleCameraDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Open the camera by the user define name.
BOOL CMultipleCameraDlg::OpenByUserId(const std::string &UserId, dvpHandle *pHandle)
{
	if(UserId.empty())
	{
		return FALSE;
	}

	// open the camera with User Defined Name and get the camera's handle. 
	dvpStatus status = dvpOpenByUserId(UserId.c_str(), OPEN_NORMAL, pHandle);
	if (status == DVP_STATUS_OK)
	{
		return TRUE;
	}
	else
	{
		// If open the device unsuccessfully, get a corresponding referenced string according to the status code.
		// Note: This string may change according to the language and version.
		MessageBox(_T("Open the device failed"));
		return FALSE;
	}

	return FALSE;
}


// Initialize the first camera.
void CMultipleCameraDlg::InitCtrls1()
{
	dvpStatus status;
	if (IsValidHandle(m_handle[0]))
	{
		// The device has been opened at this time.
		dvpCameraInfo info;
		dvpStreamState state;
		status = dvpGetStreamState(m_handle[0], &state);
		ASSERT(status == DVP_STATUS_OK);

		// Set the basic controls.
		GetDlgItem(IDC_BUTTON_OPEN1)->SetWindowText(_T("Close"));
		GetDlgItem(IDC_BUTTON_PLAY1)->SetWindowText(state == STATE_STARTED ? _T("Stop") : _T("Start"));
		GetDlgItem(IDC_BUTTON_PLAY1)->EnableWindow(TRUE);

		// Set the enable status of the related controls.
		GetDlgItem(IDC_BUTTON_PROPERTY1)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_USER_ID1)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SET1)->EnableWindow(TRUE);

		// Get the camera's information.
		dvpGetCameraInfo(m_handle[0], &info);
		ASSERT(status == DVP_STATUS_OK);

		GetDlgItem(IDC_STATIC_OPEN1)->SetWindowText(a2w(info.FriendlyName).c_str());
		GetDlgItem(IDC_EDIT_USER_ID1)->SetWindowText(a2w(info.UserID).c_str());
	}
	else
	{
		// No device is opened at this time.
		// Set the basic controls.
		GetDlgItem(IDC_BUTTON_OPEN1)->SetWindowText(_T("Open"));
		GetDlgItem(IDC_STATIC_OPEN1)->SetWindowText(_T("Friendly Name"));
		GetDlgItem(IDC_BUTTON_PLAY1)->EnableWindow(FALSE);

		// Set the enable status of the related controls.
		GetDlgItem(IDC_BUTTON_PROPERTY1)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_PREVIEW1)->InvalidateRect(NULL);
		GetDlgItem(IDC_BUTTON_SET1)->EnableWindow(FALSE);
	}
}

// Initialize the second camera.
void CMultipleCameraDlg::InitCtrls2()
{
	dvpStatus status;
	if (IsValidHandle(m_handle[1]))
	{
		// The device has been opened at this time.
		dvpCameraInfo info;
		dvpStreamState state;
		status = dvpGetStreamState(m_handle[1], &state);
		ASSERT(status == DVP_STATUS_OK);

		// Set the basic controls.
		GetDlgItem(IDC_BUTTON_OPEN2)->SetWindowText(_T("Close"));
		GetDlgItem(IDC_BUTTON_PLAY2)->SetWindowText(state == STATE_STARTED ? _T("Stop") : _T("Start"));
		GetDlgItem(IDC_BUTTON_PLAY2)->EnableWindow(TRUE);

		// Set the enable status of the related controls.
		GetDlgItem(IDC_BUTTON_PROPERTY2)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_USER_ID2)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SET2)->EnableWindow(TRUE);	

		// Get the camera's information.
		dvpGetCameraInfo(m_handle[1], &info);
		ASSERT(status == DVP_STATUS_OK);
		GetDlgItem(IDC_STATIC_OPEN2)->SetWindowText(a2w(info.FriendlyName).c_str());
		GetDlgItem(IDC_EDIT_USER_ID2)->SetWindowText(a2w(info.UserID).c_str());
	}
	else
	{
		// No device is opened at this time.
		// Set the basic controls.
		GetDlgItem(IDC_BUTTON_OPEN2)->SetWindowText(_T("Open"));
		GetDlgItem(IDC_STATIC_OPEN2)->SetWindowText(_T("Friendly Name"));
		GetDlgItem(IDC_BUTTON_PLAY2)->EnableWindow(FALSE);

		// Set the enable status of the related controls.
		GetDlgItem(IDC_STATIC_PREVIEW2)->InvalidateRect(NULL);	
		GetDlgItem(IDC_BUTTON_PROPERTY2)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SET2)->EnableWindow(FALSE);
	}
}

// Initialize the third camera.
void CMultipleCameraDlg::InitCtrls3()
{
	dvpStatus status;
	if (IsValidHandle(m_handle[2]))
	{
		// The device has been opened at this time.
		dvpCameraInfo info;	
		dvpStreamState state;
		status = dvpGetStreamState(m_handle[2], &state);
		ASSERT(status == DVP_STATUS_OK);

		// Set the basic controls.
		GetDlgItem(IDC_BUTTON_OPEN3)->SetWindowText(_T("Close"));
		GetDlgItem(IDC_BUTTON_PLAY3)->SetWindowText(state == STATE_STARTED ? _T("Stop") : _T("Start"));
		GetDlgItem(IDC_BUTTON_PLAY3)->EnableWindow(TRUE);

		// Set the enable status of the related controls.
		GetDlgItem(IDC_BUTTON_PROPERTY3)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_USER_ID3)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SET3)->EnableWindow(TRUE);	

		// Get the camera's information.
		dvpGetCameraInfo(m_handle[2], &info);
		ASSERT(status == DVP_STATUS_OK);

		GetDlgItem(IDC_STATIC_OPEN3)->SetWindowText(a2w(info.FriendlyName).c_str());
		GetDlgItem(IDC_EDIT_USER_ID3)->SetWindowText(a2w(info.UserID).c_str());
	}
	else
	{
		// No device is opened at this time.
		// Set the basic controls.
		GetDlgItem(IDC_BUTTON_OPEN3)->SetWindowText(_T("Open"));
		GetDlgItem(IDC_STATIC_OPEN3)->SetWindowText(_T("Friendly Name"));
		GetDlgItem(IDC_BUTTON_PLAY3)->EnableWindow(FALSE);

		// Set the enable status of the related controls.
		GetDlgItem(IDC_STATIC_PREVIEW3)->InvalidateRect(NULL);
		GetDlgItem(IDC_BUTTON_PROPERTY3)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SET3)->EnableWindow(FALSE);
	}
}

// Initialize the fourth cameras.
void CMultipleCameraDlg::InitCtrls4()
{
	dvpStatus status;
	if (IsValidHandle(m_handle[3]))
	{
		// The device has been opened at this time.
		dvpCameraInfo info;	
		dvpStreamState state;
		status = dvpGetStreamState(m_handle[3], &state);
		ASSERT(status == DVP_STATUS_OK);

		// Set the basic controls.
		GetDlgItem(IDC_BUTTON_OPEN4)->SetWindowText(_T("Close"));
		GetDlgItem(IDC_BUTTON_PLAY4)->SetWindowText(state == STATE_STARTED ? _T("Stop") : _T("Start"));
		GetDlgItem(IDC_BUTTON_PLAY4)->EnableWindow(TRUE);

		// Set the enable status of the related controls.
		GetDlgItem(IDC_BUTTON_PROPERTY4)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_USER_ID4)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SET4)->EnableWindow(TRUE);	

		// Get the camera's information.
		dvpGetCameraInfo(m_handle[3], &info);
		ASSERT(status == DVP_STATUS_OK);
		GetDlgItem(IDC_STATIC_OPEN4)->SetWindowText(a2w(info.FriendlyName).c_str());
		GetDlgItem(IDC_EDIT_USER_ID4)->SetWindowText(a2w(info.UserID).c_str());
	}
	else
	{
		// No device is opened at this time.
		// Set the basic controls.
		GetDlgItem(IDC_BUTTON_OPEN4)->SetWindowText(_T("Open"));
		GetDlgItem(IDC_STATIC_OPEN4)->SetWindowText(_T("Friendly Name"));
		GetDlgItem(IDC_BUTTON_PLAY4)->EnableWindow(FALSE);

		// Set the enable status of the related controls.
		GetDlgItem(IDC_STATIC_PREVIEW4)->InvalidateRect(NULL);
		GetDlgItem(IDC_BUTTON_PROPERTY4)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SET4)->EnableWindow(FALSE);
	}
}

void CMultipleCameraDlg::OnBnClickedButtonScan1()
{
	dvpStatus status;
	dvpUint32 i,n = 0;
	

	// "n" represents the number of cameras that is enumerated successfully, the drop-down list contains each camera's FriendlyName.
	m_listDevices1.ResetContent();
	memset(m_info1, 0, sizeof(m_info1));

	// Get the number of cameras that has been connected to a computer.
	status = dvpRefresh(&n);
	ASSERT(status == DVP_STATUS_OK);

	if (status == DVP_STATUS_OK)
	{
		// Enumeration of up to 16 cameras.
		if (n > 16)
			n = 16;

		for  (i = 0; i < n; i++)
		{
			// Acquire each camera's information one by one.
			status = dvpEnum(i, &m_info1[i]);
			ASSERT(status == DVP_STATUS_OK);

			if (status == DVP_STATUS_OK)
			{
				// GUI need UNICODE,but the information acquired from camera is ANSI,so convert the character set from ANSI to UNICODE.
				m_listDevices1.AddString(a2w(m_info1[i].FriendlyName).c_str());
			}
		}

		if ( n > 0)
		{
			m_listDevices1.SetCurSel(0);
			SetDlgItemText(IDC_EDIT_USER_ID1, a2w(m_info1[0].UserID).c_str());
		}

		InitCtrls1();
	}
}


void CMultipleCameraDlg::OnBnClickedButtonScan2()
{
	dvpStatus status;
	dvpUint32 i,n = 0;

	// "n" represents the number of cameras that is enumerated successfully,the drop-down list contains each camera's FriendlyName.
	
	memset(m_info2, 0, sizeof(m_info2));
	m_listDevices2.ResetContent();

	// Get the number of cameras that has been connected to a computer.
	status = dvpRefresh(&n);
	ASSERT(status == DVP_STATUS_OK);

	if (status == DVP_STATUS_OK)
	{
		// Enumeration of up to 16 cameras.
		if (n > 16)
			n = 16;

		for (i = 0; i < n; i++)
		{
			// Acquire each camera's information one by one.
			status = dvpEnum(i, &m_info2[i]);
			ASSERT(status == DVP_STATUS_OK);

			if (status == DVP_STATUS_OK)
			{
				// GUI need UNICODE,but the information acquired from camera is ANSI,so convert the character set from ANSI to UNICODE.
				m_listDevices2.AddString(a2w(m_info2[i].FriendlyName).c_str());
			}
		}

		if ( n > 1)
		{
			m_listDevices2.SetCurSel(1);
			SetDlgItemText(IDC_EDIT_USER_ID2, a2w(m_info2[1].UserID).c_str());
		}

		InitCtrls2();
	}
}


void CMultipleCameraDlg::OnBnClickedButtonScan3()
{
	dvpStatus status;
	dvpUint32 i,n = 0;
	memset(m_info3, 0, sizeof(m_info3));

	// "n" represents the number of cameras that is enumerated successfully, the drop-down list contains each camera's FriendlyName.
	m_listDevices3.ResetContent();

	// Get the number of cameras that has been connected to a computer.
	status = dvpRefresh(&n);
	ASSERT(status == DVP_STATUS_OK);

	if (status == DVP_STATUS_OK)
	{
		// Enumeration of up to 16 cameras.
		if (n > 16)
			n = 16;

		for (i = 0; i < n; i++)
		{
			// Acquire each camera's information one by one.
			status = dvpEnum(i, &m_info3[i]);
			ASSERT(status == DVP_STATUS_OK);

			if (status == DVP_STATUS_OK)
			{
				// GUI need UNICODE,but the information acquired from camera is ANSI,so convert the character set from ANSI to UNICODE.
				m_listDevices3.AddString(a2w(m_info3[i].FriendlyName).c_str());
			}
		}

		if ( n > 2)
		{
			m_listDevices3.SetCurSel(2);
			SetDlgItemText(IDC_EDIT_USER_ID3, a2w(m_info3[2].UserID).c_str());
		}

		InitCtrls3();
	}
}


void CMultipleCameraDlg::OnBnClickedButtonScan4()
{
	dvpStatus status;
	dvpUint32 i,n = 0;
	memset(m_info4, 0, sizeof(m_info4));

	// "n" represents the number of cameras that is enumerated successfully, the drop-down list contains each camera's FriendlyName.
	m_listDevices4.ResetContent();

	// Get the number of cameras that has been connected to a computer.
	status = dvpRefresh(&n);
	ASSERT(status == DVP_STATUS_OK);

	if (status == DVP_STATUS_OK)
	{
		// Enumeration of up to 16 cameras.
		if (n > 16)
			n = 16;

		for (i = 0; i < n; i++)
		{
			// Acquire each camera's information one by one.
			status = dvpEnum(i, &m_info4[i]);
			ASSERT(status == DVP_STATUS_OK);

			if (status == DVP_STATUS_OK)
			{
				// GUI need UNICODE,but the information acquired from camera is ANSI,so convert the character set from ANSI to UNICODE.
				m_listDevices4.AddString(a2w(m_info4[i].FriendlyName).c_str());
			}
		}

		if (n > 3 )
		{
			m_listDevices4.SetCurSel(3);
			SetDlgItemText(IDC_EDIT_USER_ID4, a2w(m_info4[3].UserID).c_str());
		}

		InitCtrls4();
	}

}


void CMultipleCameraDlg::OnBnClickedButtonOpen1()
{
	dvpStatus status;
	CString strName;

	if (!IsValidHandle(m_handle[0]))
	{
		m_listDevices1.GetWindowText(strName);
		if (strName != "")
		{
			// Open the pointed device by the selected FriendlyName.
			status = dvpOpenByName(w2a(strName).c_str(), OPEN_NORMAL, &m_handle[0]);
			if (status != DVP_STATUS_OK)
			{
				m_listDevices1.EnableWindow(TRUE);
				MessageBox(_T("Open the device failed"));
			}	
			else
			{
				m_listDevices1.EnableWindow(FALSE);

				// If it needs to display images,the user should register a callback function and finish the operation of drawing pictures in the registered callback function.
				// Note: Drawing pictures in the callback function maybe generate some delays for acquiring image data by the use of "dvpGetFrame".
				status = dvpRegisterStreamCallback(m_handle[0], OnDrawPicture, STREAM_EVENT_PROCESSED, this);	
				ASSERT(status == DVP_STATUS_OK);

				SetDlgItemTextA(GetSafeHwnd(), IDC_STATIC_OPEN1, w2a(strName).c_str());

			}
		}
	}
	else
	{
		dvpStreamState state;
		dvpGetStreamState(m_handle[0], &state);
		if ( state == STATE_STARTED )
		{
			status = dvpStop(m_handle[0]);
			ASSERT(status == DVP_STATUS_OK);

			m_dfDisplayCount[0] = 0;
		}


		status = dvpClose(m_handle[0]);
		ASSERT(status == DVP_STATUS_OK);
		m_handle[0] = 0;
		m_listDevices1.EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_PREVIEW1)->InvalidateRect(NULL, 1);
	}
	InitCtrls1();
}


void CMultipleCameraDlg::OnBnClickedButtonOpen2()
{
	dvpStatus status;
	CString strName;

	if (!IsValidHandle(m_handle[1]))
	{
		m_listDevices2.GetWindowText(strName);
		if (strName != "")
		{
			// Open the pointed device by the selected FriendlyName.
			status = dvpOpenByName(w2a(strName).c_str(), OPEN_NORMAL, &m_handle[1]);
			if (status != DVP_STATUS_OK)
			{
				m_listDevices2.EnableWindow(TRUE);
				MessageBox(_T("Open the device failed"));
			}
			else
			{
				m_listDevices2.EnableWindow(FALSE);

				// If it needs to display images ,the user should register a callback function and finish the operation of drawing pictures in the registered callback function.
				// Note: Drawing pictures in the callback function maybe generate some delays for acquiring image data by the use of "dvpGetFrame".
				status = dvpRegisterStreamCallback(m_handle[1], OnDrawPicture, STREAM_EVENT_PROCESSED, this);	
				ASSERT(status == DVP_STATUS_OK);
				SetDlgItemTextA(GetSafeHwnd(), IDC_STATIC_OPEN2, w2a(strName).c_str());
			}
		}
	}
	else
	{

		dvpStreamState state;
		dvpGetStreamState(m_handle[1], &state);
		if ( state == STATE_STARTED )
		{
			
			status = dvpStop(m_handle[1]);
			ASSERT(status == DVP_STATUS_OK);

			m_dfDisplayCount[1] = 0;
		}

		status = dvpClose(m_handle[1]);
		ASSERT(status == DVP_STATUS_OK);
		m_handle[1] = 0;
		m_listDevices2.EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_PREVIEW2)->InvalidateRect(NULL, 1);
	}
	InitCtrls2();
}


void CMultipleCameraDlg::OnBnClickedButtonOpen3()
{
	dvpStatus status;
	CString strName;

	if (!IsValidHandle(m_handle[2]))
	{
		m_listDevices3.GetWindowText(strName);
		if (strName != "")
		{
			// Open the pointed device by the selected FriendlyName.
			status = dvpOpenByName(w2a(strName).c_str(), OPEN_NORMAL, &m_handle[2]);
			if (status != DVP_STATUS_OK)
			{
				m_listDevices3.EnableWindow(TRUE);
				MessageBox(_T("Open the device failed"));
			}
			else
			{
				m_listDevices3.EnableWindow(FALSE);

				// If it needs to display images ,the user should register a callback function and finish the operation of drawing pictures in the registered callback function.
				// Note: Drawing pictures in the callback function maybe generate some delays for acquiring image data by the use of "dvpGetFrame".
				status = dvpRegisterStreamCallback(m_handle[2], OnDrawPicture, STREAM_EVENT_PROCESSED, this);
				ASSERT(status == DVP_STATUS_OK);

				SetDlgItemTextA(GetSafeHwnd(), IDC_STATIC_OPEN3, w2a(strName).c_str());
			}
		}
	}
	else
	{
		dvpStreamState state;
		dvpGetStreamState(m_handle[2], &state);
		if ( state == STATE_STARTED )
		{
			
			status = dvpStop(m_handle[2]);
			ASSERT(status == DVP_STATUS_OK);

			m_dfDisplayCount[2] = 0;
		}

		status = dvpClose(m_handle[2]);
		ASSERT(status == DVP_STATUS_OK);
		m_handle[2] = 0;
		m_listDevices3.EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_PREVIEW3)->InvalidateRect(NULL, 1);
	}
	InitCtrls3();
}

 
void CMultipleCameraDlg::OnBnClickedButtonOpen4()
{
	dvpStatus status;
	CString strName;

	if (!IsValidHandle(m_handle[3]))
	{
		m_listDevices4.GetWindowText(strName);
		if (strName != "")
		{
			// Open the pointed device by the selected FriendlyName.
			status = dvpOpenByName(w2a(strName).c_str(), OPEN_NORMAL, &m_handle[3]);
			if (status != DVP_STATUS_OK)
			{
				m_listDevices4.EnableWindow(TRUE);
				MessageBox(_T("Open the device failed"));
			}
			else
			{
				m_listDevices4.EnableWindow(FALSE);

				// If it needs to display images ,the user should register a callback function and finish the operation of drawing pictures in the registered callback function.
				// Note: Drawing pictures in the callback function maybe generate some delays for acquiring image data by the use of "dvpGetFrame".
				status = dvpRegisterStreamCallback(m_handle[3], OnDrawPicture, STREAM_EVENT_PROCESSED, this);
				ASSERT(status == DVP_STATUS_OK);

				SetDlgItemTextA(GetSafeHwnd(), IDC_STATIC_OPEN4, w2a(strName).c_str());
			}
		}
	}
	else
	{

		dvpStreamState state;
		dvpGetStreamState(m_handle[3], &state);
		if ( state == STATE_STARTED )
		{
			
			status = dvpStop(m_handle[3]);
			ASSERT(status == DVP_STATUS_OK);

			m_dfDisplayCount[3] = 0;
		}


		status = dvpClose(m_handle[3]);
		ASSERT(status == DVP_STATUS_OK);
		m_handle[3] = 0;
		m_listDevices4.EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_PREVIEW4)->InvalidateRect(NULL, 1);
	}
	InitCtrls4();
}

// Display the attributive settings dialog.
void CMultipleCameraDlg::OnBnClickedButtonProperty1()
{
	if (IsValidHandle(m_handle[0]))
	{
		dvpStatus status = dvpShowPropertyModalDialog(m_handle[0], GetSafeHwnd());
	}
}

void CMultipleCameraDlg::OnBnClickedButtonProperty2()
{
	if (IsValidHandle(m_handle[1]))
	{
		dvpStatus status = dvpShowPropertyModalDialog(m_handle[1], GetSafeHwnd());
	}
}

void CMultipleCameraDlg::OnBnClickedButtonProperty3()
{
	if (IsValidHandle(m_handle[2]))
	{
		dvpStatus status = dvpShowPropertyModalDialog(m_handle[2], GetSafeHwnd());
	}
}

void CMultipleCameraDlg::OnBnClickedButtonProperty4()
{
	if (IsValidHandle(m_handle[3]))
	{
		dvpStatus status = dvpShowPropertyModalDialog(m_handle[3], GetSafeHwnd());
	}
}


// Update the user define name of the camera other than the camera number n_pos
void CMultipleCameraDlg::UpdataCameraId(int n_pos, char *pFriendlyName, char *pUserID)
{
	if ( pFriendlyName == NULL )
		return ;

	dvpCameraInfo *pCameraInfo1 = NULL;
	dvpCameraInfo *pCameraInfo2 = NULL;
	dvpCameraInfo *pCameraInfo3 = NULL;

	// 
	if ( n_pos == 1 )
	{
		pCameraInfo1 = m_info2;
		pCameraInfo2 = m_info3;
		pCameraInfo3 = m_info4;
	}
	else if ( n_pos == 2 )
	{
		pCameraInfo1 = m_info1;
		pCameraInfo2 = m_info3;
		pCameraInfo3 = m_info4;
	}
	else if ( n_pos == 3 )
	{
		pCameraInfo1 = m_info1;
		pCameraInfo2 = m_info2;
		pCameraInfo3 = m_info4;
	}
	else if ( n_pos == 4 )
	{
		pCameraInfo1 = m_info1;
		pCameraInfo2 = m_info2;
		pCameraInfo3 = m_info3;
	}
	else
	{
		return ;
	}


	for ( int i = 0; i < 16; i++ )
	{
		// Find the camera to modify the user define name based on the friendly name
		if (strcmp((pCameraInfo1+i)->FriendlyName, pFriendlyName) == 0 )
		{
			if ( pUserID != NULL && strlen(pUserID) > 0)
			{
				// Update the user define name
				strcpy_s((pCameraInfo1+i)->UserID, sizeof((pCameraInfo1+i)->UserID), pUserID);
			}
			else
			{
				// When the pUserID = NULL or the pUserID character is empty, the user define name is cleared
				(pCameraInfo1 + i)->UserID[0] = 0;
			}
			break;
		}
	}

	for ( int i = 0; i < 16; i++ )
	{
		if (strcmp((pCameraInfo2+i)->FriendlyName, pFriendlyName) == 0 )
		{
			if ( pUserID != NULL && strlen(pUserID) > 0)
			{
				// Update the user define name
				strcpy_s((pCameraInfo2+i)->UserID, sizeof((pCameraInfo2+i)->UserID), pUserID);
			}
			else
			{
				// When the pUserID = NULL or the pUserID character is empty, the user define name is cleared
				(pCameraInfo2 + i)->UserID[0] = 0;
			}
			break;
		}
	}

	for ( int i = 0; i < 16; i++ )
	{
		if (strcmp((pCameraInfo3+i)->FriendlyName, pFriendlyName) == 0 )
		{
			if ( pUserID != NULL && strlen(pUserID) > 0)
			{
				// Update the user define name
				strcpy_s((pCameraInfo3+i)->UserID, sizeof((pCameraInfo3+i)->UserID), pUserID);
			}
			else
			{
				// When the pUserID = NULL or the pUserID character is empty, the user define name is cleared
				(pCameraInfo3 + i)->UserID[0] = 0;
			}
			break;
		}
	}

	CString name;
	CString StrFriendlyName = a2w(pFriendlyName).c_str();
	CString strUserID = a2w(pUserID).c_str();
	m_listDevices1.GetWindowText(name);
	if ( name == StrFriendlyName )
	{
		SetDlgItemText(IDC_EDIT_USER_ID1, strUserID);
	}

	m_listDevices2.GetWindowText(name);
	if ( name == StrFriendlyName )
	{
		SetDlgItemText(IDC_EDIT_USER_ID2, strUserID);
	}

	m_listDevices3.GetWindowText(name);
	if ( name == StrFriendlyName )
	{
		SetDlgItemText(IDC_EDIT_USER_ID3, strUserID);
	}

	m_listDevices4.GetWindowText(name);
	if ( name == StrFriendlyName )
	{
		SetDlgItemText(IDC_EDIT_USER_ID4, strUserID);
	}
}

// Set the user define name of the first camera.
void CMultipleCameraDlg::OnBnClickedButtonSet1()
{
	if (IsValidHandle(m_handle[0]))
	{
		CString str;
		GetDlgItem(IDC_EDIT_USER_ID1)->GetWindowText(str);
		UINT Length = str.GetLength();
		
		// Set the user define name.
		dvpStatus status = dvpSetUserId(m_handle[0], w2a(str.GetBuffer()).c_str(), &Length);
		ASSERT(status == DVP_STATUS_OK);

		GetDlgItem(IDC_BUTTON_SAVE_BIND)->EnableWindow(TRUE);
		strcpy_s(m_info1[m_listDevices1.GetCurSel()].UserID, sizeof(m_info1[m_listDevices1.GetCurSel()].UserID), w2a(str.GetBuffer()).c_str());
		UpdataCameraId(1, m_info1[m_listDevices1.GetCurSel()].FriendlyName, m_info1[m_listDevices1.GetCurSel()].UserID);
	}
}

// Set the user define name of the second camera.
void CMultipleCameraDlg::OnBnClickedButtonSet2()
{
	if (IsValidHandle(m_handle[1]))
	{
		CString str;
		GetDlgItem(IDC_EDIT_USER_ID2)->GetWindowText(str);
		UINT Length = str.GetLength();
		
		// Set the user define name.
		dvpStatus status = dvpSetUserId(m_handle[1], w2a(str.GetBuffer()).c_str(), &Length);
		ASSERT(status == DVP_STATUS_OK);

		GetDlgItem(IDC_BUTTON_SAVE_BIND)->EnableWindow(TRUE);
		strcpy_s(m_info2[m_listDevices2.GetCurSel()].UserID, sizeof(m_info2[m_listDevices2.GetCurSel()].UserID), w2a(str.GetBuffer()).c_str());
		UpdataCameraId(2, m_info2[m_listDevices2.GetCurSel()].FriendlyName, m_info2[m_listDevices2.GetCurSel()].UserID);
	}
}

// Set the user define name of the third camera.
void CMultipleCameraDlg::OnBnClickedButtonSet3()
{
	if (IsValidHandle(m_handle[2]))
	{
		CString str;
		GetDlgItem(IDC_EDIT_USER_ID3)->GetWindowText(str);
		UINT Length = str.GetLength();
		
		// Set the user define name.
		dvpStatus status = dvpSetUserId(m_handle[2], w2a(str.GetBuffer()).c_str(), &Length);
		ASSERT(status == DVP_STATUS_OK);

		GetDlgItem(IDC_BUTTON_SAVE_BIND)->EnableWindow(TRUE);
		strcpy_s(m_info3[m_listDevices3.GetCurSel()].UserID, sizeof(m_info3[m_listDevices3.GetCurSel()].UserID), w2a(str.GetBuffer()).c_str());
		UpdataCameraId(3, m_info3[m_listDevices3.GetCurSel()].FriendlyName, m_info3[m_listDevices3.GetCurSel()].UserID);
	}
}

// Set the user define name of the fourth camera.
void CMultipleCameraDlg::OnBnClickedButtonSet4()
{
	if (IsValidHandle(m_handle[3]))
	{
		CString str;
		GetDlgItem(IDC_EDIT_USER_ID4)->GetWindowText(str);
		UINT Length = str.GetLength();
	
		// Set the user define name.
		dvpStatus status = dvpSetUserId(m_handle[3], w2a(str.GetBuffer()).c_str(), &Length);
		ASSERT(status == DVP_STATUS_OK);

		GetDlgItem(IDC_BUTTON_SAVE_BIND)->EnableWindow(TRUE);
		strcpy_s(m_info4[m_listDevices4.GetCurSel()].UserID, sizeof(m_info4[m_listDevices4.GetCurSel()].UserID), w2a(str.GetBuffer()).c_str());
		UpdataCameraId(4, m_info4[m_listDevices4.GetCurSel()].FriendlyName, m_info4[m_listDevices4.GetCurSel()].UserID);
	}
}


void CMultipleCameraDlg::OnBnClickedButtonSaveBind()
{
	CString strUserId0, strUserId1, strUserId2, strUserId3;
	GetDlgItem(IDC_EDIT_USER_ID1)->GetWindowText(strUserId0);
	GetDlgItem(IDC_EDIT_USER_ID2)->GetWindowText(strUserId1);
	GetDlgItem(IDC_EDIT_USER_ID3)->GetWindowText(strUserId2);
	GetDlgItem(IDC_EDIT_USER_ID4)->GetWindowText(strUserId3);

	// The count of non-empty "user define name". 
	UINT nNotEmptyCount = 0;
	std::set<CString> check;

	// Avoid repetitive "user define name", if not, binding relationship won't be able to set up.
	if (!strUserId0.IsEmpty()) {check.insert(strUserId0); nNotEmptyCount++;}
	if (!strUserId1.IsEmpty()) {check.insert(strUserId1); nNotEmptyCount++;}
	if (!strUserId2.IsEmpty()) {check.insert(strUserId2); nNotEmptyCount++;}
	if (!strUserId3.IsEmpty()) {check.insert(strUserId3); nNotEmptyCount++;}

	// Judge whether there is repetitive "user define name" except empty "user define name". 
	if (check.size() != nNotEmptyCount)
	{
		// If there is repetitive "user define name", refuse to save the binding relationship.
		MessageBox(_T("User Defined Name repetition"), _T("Save Failed"), MB_ICONSTOP);
		return;
	}

	// If there is not repetitive "user define name",save the binding relationship into registry.
	AfxGetApp()->WriteProfileString(_T("UserDefineName"), _T("0"), strUserId0);
	AfxGetApp()->WriteProfileString(_T("UserDefineName"), _T("1"), strUserId1);
	AfxGetApp()->WriteProfileString(_T("UserDefineName"), _T("2"), strUserId2);
	AfxGetApp()->WriteProfileString(_T("UserDefineName"), _T("3"), strUserId3);

	GetDlgItem(IDC_BUTTON_SAVE_BIND)->EnableWindow(FALSE);
}


void CMultipleCameraDlg::OnBnClickedButtonLoadBind()
{
	CString strUserId0 = AfxGetApp()->GetProfileString(_T("UserDefineName"), _T("0"));
	CString strUserId1 = AfxGetApp()->GetProfileString(_T("UserDefineName"), _T("1"));
	CString strUserId2 = AfxGetApp()->GetProfileString(_T("UserDefineName"), _T("2"));
	CString strUserId3 = AfxGetApp()->GetProfileString(_T("UserDefineName"), _T("3"));

	// The count of non-empty "user define name". 
	UINT nNotEmptyCount = 0;
	std::set<CString> check;

	// Avoid repetitive "user define name", if not,binding relationship won't be able to set up.
	if (!strUserId0.IsEmpty()) {check.insert(strUserId0); nNotEmptyCount++;}
	if (!strUserId1.IsEmpty()) {check.insert(strUserId1); nNotEmptyCount++;}
	if (!strUserId2.IsEmpty()) {check.insert(strUserId2); nNotEmptyCount++;}
	if (!strUserId3.IsEmpty()) {check.insert(strUserId3); nNotEmptyCount++;}

	if (check.size() != nNotEmptyCount)
	{
		MessageBox(_T("User Defined Name repetition"), _T("Save Failed"), MB_ICONSTOP);
		return;
	}

	// If the camera has been opened, close the camera one by one.
	for (UINT i = 0; i < 4; i++)
	{
		if (IsValidHandle(m_handle[i]))
		{
			dvpStatus status = dvpClose(m_handle[i]);
			ASSERT(status == DVP_STATUS_OK);

			m_handle[i] = 0;
		}
	}

	char szFriendlyName[128] = {0};

	// Open cameras by "user define name".
	if ( OpenByUserId(w2a(strUserId0.GetBuffer()), &m_handle[0]) )
	{
		dvpStatus status = dvpRegisterStreamCallback(m_handle[0], OnDrawPicture, STREAM_EVENT_PROCESSED, this);
		ASSERT(status == DVP_STATUS_OK);
		
		// Initialize cameras.
		InitCtrls1();
		m_listDevices1.EnableWindow(FALSE);
		SetDlgItemText(IDC_STATIC_OPEN1, strUserId0);
	}
	else
	{
		m_listDevices1.EnableWindow(TRUE);
		SetDlgItemTextA(GetSafeHwnd(), IDC_STATIC_OPEN1, "Friendly Name");
	}

	if ( OpenByUserId(w2a(strUserId1.GetBuffer()), &m_handle[1]) )
	{
		dvpStatus status = dvpRegisterStreamCallback(m_handle[1], OnDrawPicture, STREAM_EVENT_PROCESSED, this);
		ASSERT(status == DVP_STATUS_OK);

		// Initialize cameras.
		InitCtrls2();
		m_listDevices2.EnableWindow(FALSE);
		SetDlgItemText(IDC_STATIC_OPEN2, strUserId1);
	}
	else
	{
		m_listDevices1.EnableWindow(TRUE);
		SetDlgItemTextA(GetSafeHwnd(), IDC_STATIC_OPEN1, "Friendly Name");
	}

	if ( OpenByUserId(w2a(strUserId2.GetBuffer()), &m_handle[2]) )
	{
		dvpStatus status = dvpRegisterStreamCallback(m_handle[2], OnDrawPicture, STREAM_EVENT_PROCESSED, this);
		ASSERT(status == DVP_STATUS_OK);

		// Initialize cameras.
		InitCtrls3();
		m_listDevices3.EnableWindow(FALSE);
		SetDlgItemText(IDC_STATIC_OPEN3, strUserId2);
	}
	else
	{
		m_listDevices1.EnableWindow(TRUE);
		SetDlgItemTextA(GetSafeHwnd(), IDC_STATIC_OPEN1, "Friendly Name");
	}

	if ( OpenByUserId(w2a(strUserId3.GetBuffer()), &m_handle[3]) )
	{
		dvpStatus status = dvpRegisterStreamCallback(m_handle[3], OnDrawPicture, STREAM_EVENT_PROCESSED, this);
		ASSERT(status == DVP_STATUS_OK);

		// Initialize cameras.
		InitCtrls4();
		m_listDevices4.EnableWindow(FALSE);
		SetDlgItemText(IDC_STATIC_OPEN4, strUserId3);
	}
	else
	{
		m_listDevices1.EnableWindow(TRUE);
		SetDlgItemTextA(GetSafeHwnd(), IDC_STATIC_OPEN1, "Friendly Name");
	}
}


void CMultipleCameraDlg::OnBnClickedButtonPlay1()
{
	if (IsValidHandle(m_handle[0]))
	{
		dvpStreamState state;
		dvpStatus status;

		// Implement a button to start and stop according to the current video's status.
		status = dvpGetStreamState(m_handle[0], &state);
		ASSERT(status == DVP_STATUS_OK);

		if (state == STATE_STARTED)
		{
			
			status = dvpStop(m_handle[0]);

			m_dfDisplayCount[0] = 0;
			
		}
		else
		{
			m_dfDisplayCount[0] = 0;
			status = dvpStart(m_handle[0]);
		}

		ASSERT(status == DVP_STATUS_OK);
		InitCtrls1();
	}
}


void CMultipleCameraDlg::OnBnClickedButtonPlay2()
{
	if (IsValidHandle(m_handle[1]))
	{
		dvpStreamState state;
		dvpStatus status;

		// Implement a button to start and stop according to the current video's status.
		status = dvpGetStreamState(m_handle[1], &state);
		ASSERT(status == DVP_STATUS_OK);

		if (state == STATE_STARTED)
		{
			
			status = dvpStop(m_handle[1]);

			m_dfDisplayCount[1] = 0;
		}
		else
		{
			m_dfDisplayCount[1] = 0;
			status = dvpStart(m_handle[1]);
		}

		ASSERT(status == DVP_STATUS_OK);
		InitCtrls2();
	}
}


void CMultipleCameraDlg::OnBnClickedButtonPlay3()
{
	if (IsValidHandle(m_handle[2]))
	{
		dvpStreamState state;
		dvpStatus status;

		// Implement a button to start and stop according to the current video's status.
		status = dvpGetStreamState(m_handle[2], &state);
		ASSERT(status == DVP_STATUS_OK);

		if (state == STATE_STARTED)
		{
			
			status = dvpStop(m_handle[2]);
			
			m_dfDisplayCount[2] = 0;
		}
		else
		{
			m_dfDisplayCount[2] = 0;
			status = dvpStart(m_handle[2]);
		}

		ASSERT(status == DVP_STATUS_OK);
		InitCtrls3();
	}
}


void CMultipleCameraDlg::OnBnClickedButtonPlay4()
{
	if (IsValidHandle(m_handle[3]))
	{
		dvpStreamState state;
		dvpStatus status;

		// Implement a button to start and stop according to the current video's status.
		status = dvpGetStreamState(m_handle[3], &state);
		ASSERT(status == DVP_STATUS_OK);

		if (state == STATE_STARTED)
		{
			
			status = dvpStop(m_handle[3]);

			m_dfDisplayCount[3] = 0;
		}
		else
		{
			m_dfDisplayCount[3] = 0;
			status = dvpStart(m_handle[3]);
		}

		ASSERT(status == DVP_STATUS_OK);
		InitCtrls4();
	}
}


void CMultipleCameraDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == 0)
	{
		dvpFrameCount count;
		dvpStatus status;
		if (IsValidHandle(m_handle[0]))
		{
			// Update the information of the frame rate.
			
			status = dvpGetFrameCount(m_handle[0], &count);
			ASSERT(status == DVP_STATUS_OK);

			CString str;
			if ( m_dfDisplayCount[0] > 0 && m_CurGrabClock[0] - m_StartGrabClock[0] > 0)
			{
				str.Format(_T("[%d frames, %.3f fps, Display %.3f fps]"),
					count.uFrameCount,
					count.fFrameRate,
					m_dfDisplayCount[0]*1000.0/(m_CurGrabClock[0] - m_StartGrabClock[0]));
			}
			else
			{
				str.Format(_T("[%d frames, %.3f fps]"),
					count.uFrameCount,
					count.fFrameRate);
			}
			SetDlgItemText(IDC_STATIC_OPEN1, str);
		}
		if (IsValidHandle(m_handle[1]))
		{
			// Update the information of the frame rate.

			status = dvpGetFrameCount(m_handle[1], &count);
			ASSERT(status == DVP_STATUS_OK);

			CString str;
			if ( m_dfDisplayCount[1] > 0 && m_CurGrabClock[1] - m_StartGrabClock[1] > 0)
			{
				str.Format(_T("[%d frames, %.3f fps, Display %.3f fps]"),
					count.uFrameCount,
					count.fFrameRate,
					m_dfDisplayCount[1]*1000.0/(m_CurGrabClock[1] - m_StartGrabClock[1]));
			}
			else
			{
				str.Format(_T("[%d frames, %.3f fps]"),
					count.uFrameCount,
					count.fFrameRate);
			}
			SetDlgItemText(IDC_STATIC_OPEN2, str);
		}
		if (IsValidHandle(m_handle[2]))
		{
			// Update the information of the frame rate.

			status = dvpGetFrameCount(m_handle[2], &count);
			ASSERT(status == DVP_STATUS_OK);

			CString str;
			if ( m_dfDisplayCount[2] > 0 && m_CurGrabClock[2] - m_StartGrabClock[2] > 0)
			{
				str.Format(_T("[%d frames, %.3f fps, Display %.3f fps]"),
					count.uFrameCount,
					count.fFrameRate,
					m_dfDisplayCount[2]*1000.0/(m_CurGrabClock[2] - m_StartGrabClock[2]));
			}
			else
			{
				str.Format(_T("[%d frames, %.3f fps]"),
					count.uFrameCount,
					count.fFrameRate);
			}
			SetDlgItemText(IDC_STATIC_OPEN3, str);
		}
		if (IsValidHandle(m_handle[3]))
		{
			// Update the information of the frame rate.

			status = dvpGetFrameCount(m_handle[3], &count);
			ASSERT(status == DVP_STATUS_OK);

			CString str;
			if ( m_dfDisplayCount[3] > 0 && m_CurGrabClock[3] - m_StartGrabClock[3] > 0)
			{
				str.Format(_T("[%d frames, %.3f fps, Display %.3f fps]"),
					count.uFrameCount,
					count.fFrameRate,
					m_dfDisplayCount[3]*1000.0/(m_CurGrabClock[3] - m_StartGrabClock[3]));
			}
			else
			{
				str.Format(_T("[%d frames, %.3f fps]"),
					count.uFrameCount,
					count.fFrameRate);
			}
			SetDlgItemText(IDC_STATIC_OPEN4, str);
		}
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CMultipleCameraDlg::OnCbnSelchangeComboDevices1()
{
	if ( m_listDevices1.GetCurSel() != -1 )
	{
		SetDlgItemText(IDC_EDIT_USER_ID1, a2w(m_info1[m_listDevices1.GetCurSel()].UserID).c_str());
	}
	else
	{
		SetDlgItemText(IDC_EDIT_USER_ID1, NULL);
	}
}


void CMultipleCameraDlg::OnCbnSelchangeComboDevices2()
{
	if ( m_listDevices2.GetCurSel() != -1 )
	{
		SetDlgItemText(IDC_EDIT_USER_ID2, a2w(m_info2[m_listDevices2.GetCurSel()].UserID).c_str());
	}
	else
	{
		SetDlgItemText(IDC_EDIT_USER_ID2, NULL);
	}
}


void CMultipleCameraDlg::OnCbnSelchangeComboDevices3()
{
	if ( m_listDevices3.GetCurSel() != -1 )
	{
		SetDlgItemText(IDC_EDIT_USER_ID3, a2w(m_info3[m_listDevices3.GetCurSel()].UserID).c_str());
	}
	else
	{
		SetDlgItemText(IDC_EDIT_USER_ID3, NULL);
	}
}


void CMultipleCameraDlg::OnCbnSelchangeComboDevices4()
{
	if ( m_listDevices4.GetCurSel() != -1 )
	{
		SetDlgItemText(IDC_EDIT_USER_ID4, a2w(m_info4[m_listDevices4.GetCurSel()].UserID).c_str());
	}
	else
	{
		SetDlgItemText(IDC_EDIT_USER_ID4, NULL);
	}
}
