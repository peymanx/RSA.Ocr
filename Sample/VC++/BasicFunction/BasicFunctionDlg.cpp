
// BasicFunctionDlg.cpp : Implementation file

#include "stdafx.h"
#include "BasicFunction.h"
#include "BasicFunctionDlg.h"
#include "afxdialogex.h"

using namespace std;

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

// CBasicFunctionDlg dialog
int grab_count = 0;
// Callback function for acquiring the video stream.
int CBasicFunctionDlg::OnDrawPicture(dvpHandle handle,
	dvpStreamEvent event, 
	void *pContext, 
	dvpFrame *pFrame, 
	void *pBuffer)
{
	CBasicFunctionDlg *pDlg = ((CBasicFunctionDlg*)pContext);

	// Get current Tick
	pDlg->m_CurGrabClock = GetTickCount();

	bool bDisplay = false;
	if ( pDlg->m_dfDisplayCount != 0 )
	{
		// count the time of image acquisition
		unsigned int GrabMs = (pDlg->m_CurGrabClock - pDlg->m_StartGrabClock);

		// Calculate whether the current frame is displayed
		if ( GrabMs - pDlg->m_dfDisplayCount * 33.3f  >= 33 )
		{
			bDisplay = true;
		}
	}
	else
	{
		bDisplay = true;
		pDlg->m_StartGrabClock = pDlg->m_CurGrabClock;
	}

	if ( bDisplay )
	{
		dvpStatus status = dvpDrawPicture(pFrame, pBuffer, pDlg->GetDlgItem(IDC_STATIC_PREVIEW)->GetSafeHwnd(), nullptr, nullptr);
		ASSERT(status == DVP_STATUS_OK);
		pDlg->m_dfDisplayCount++;
	}

	if (pDlg->m_bSaveImageFlg)
	{
		pDlg->m_bSaveImageFlg = false;
		dvpStatus status = dvpSavePicture(pFrame, pBuffer, w2a(pDlg->m_ImagePath.GetBuffer()).c_str(), 100);
		ASSERT(status == DVP_STATUS_OK);
		ShellExecute(NULL, TEXT("open"), pDlg->m_ImagePath, NULL, NULL, SW_SHOW);
	}
	// "return 0" represents the image data has been discarded and no longer been used.
	return 0;
}


CBasicFunctionDlg::CBasicFunctionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBasicFunctionDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_bAeOp = FALSE;

	m_fMaxGain = 0.0F;
	m_fMinGain = 0.0F;
	m_fMaxExposureTime = 0.0F;
	m_fMinExposureTime = 0.0F;

	m_bSaveImageFlg = false;
	m_ImagePath = TEXT("");
}

void CBasicFunctionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DEVICES, m_listDevices);
	DDX_Check(pDX, IDC_CHECK_AEENABLE, m_bAeOp);
}

BEGIN_MESSAGE_MAP(CBasicFunctionDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_SCAN, &CBasicFunctionDlg::OnBnClickedButtonScan)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CBasicFunctionDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_START, &CBasicFunctionDlg::OnBnClickedButtonStart)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_EXPOSURE_TIME, &CBasicFunctionDlg::OnNMCustomdrawSliderExposureTime)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_GAIN, &CBasicFunctionDlg::OnNMCustomdrawSliderExposureGain)
	ON_CBN_SELCHANGE(IDC_COMBO_RESOLUTION, &CBasicFunctionDlg::OnCbnSelchangeComboResolution)
	ON_BN_CLICKED(IDC_BUTTON_PROPERTY, &CBasicFunctionDlg::OnBnClickedButtonProperty)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CBasicFunctionDlg::OnBnClickedButtonSave)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_AE_MODE, &CBasicFunctionDlg::OnCbnSelchangeComboAeMode)
	ON_CBN_SELCHANGE(IDC_COMBO_FLICK, &CBasicFunctionDlg::OnCbnSelchangeComboFlick)
	ON_BN_CLICKED(IDC_CHECK_AEOPERATION, &CBasicFunctionDlg::OnBnClickedCheckAeoperation)
	ON_BN_CLICKED(IDC_CHECK_AWB, &CBasicFunctionDlg::OnBnClickedCheckAwb)
	ON_BN_CLICKED(IDC_BUTTON_ONCE_AWB, &CBasicFunctionDlg::OnBnClickedButtonOnceAwb)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_GAIN, &CBasicFunctionDlg::OnDeltaposSpinGain)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_EXPOSURE_TIME, &CBasicFunctionDlg::OnDeltaposSpinExposureTime)
	ON_BN_CLICKED(IDC_BUTTON_GAIN, &CBasicFunctionDlg::OnBnClickedButtonGain)
	ON_BN_CLICKED(IDC_BUTTON_EXPOSURE_TIME, &CBasicFunctionDlg::OnBnClickedButtonExposureTime)
	ON_BN_CLICKED(IDC_CHECK_USERDEFINEDNAME, &CBasicFunctionDlg::OnBnClickedCheckUserdefinedname)
END_MESSAGE_MAP()


// CBasicFunctionDlg message handlers

BOOL CBasicFunctionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon.
	SetIcon(m_hIcon, FALSE);		// Set small icon.

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

void CBasicFunctionDlg::OnPaint()
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

void CBasicFunctionDlg::UpdataPreWindowSize(int x, int y)
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

void CBasicFunctionDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// Adjust the video graphic window automatically according to the size of the window.
	UpdataPreWindowSize(cx, cy);
}

void CBasicFunctionDlg::OnTimer(UINT_PTR nIDEvent)
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
					m_dfDisplayCount * 1000.0/(m_CurGrabClock - m_StartGrabClock));
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

void CBasicFunctionDlg::OnOK()
{
	if (IsValidHandle(m_handle))
	{
		// After clicking "OK" ,configured parameters will be save,default to the path "C:\ProgramData\DVP2" if not specified.
		dvpStatus status = dvpSaveConfig(m_handle, NULL);
        ASSERT(status == DVP_STATUS_OK);
	}

	CDialogEx::OnOK();
}

HCURSOR CBasicFunctionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CBasicFunctionDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// Close the device at the moment of destroying the window.
	if (IsValidHandle(m_handle))
	{
		dvpStop(m_handle);
		dvpClose(m_handle);
		m_handle= 0;
	}

}

void CBasicFunctionDlg::UpdateControls()
{
	dvpStatus status;
	if (IsValidHandle(m_handle))
	{
		// The device has been opened at this time.
		// Update the basic controls.
		dvpStreamState state;
		status = dvpGetStreamState(m_handle, &state);
        ASSERT(status == DVP_STATUS_OK);

		GetDlgItem(IDC_BUTTON_START)->SetWindowText(state == STATE_STARTED ? _T("Stop") : _T("Start"));
		GetDlgItem(IDC_BUTTON_OPEN)->SetWindowTextW(_T("Close"));
	    GetDlgItem(IDC_BUTTON_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_PROPERTY)->EnableWindow(TRUE);

		if (state == STATE_STARTED)
		{
			GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
		}
		
		// Update the related controls.
		GetDlgItem(IDC_COMBO_RESOLUTION)->EnableWindow(TRUE);
	    GetDlgItem(IDC_COMBO_AE_MODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_FLICK)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_AEENABLE)->EnableWindow(TRUE);

		// Update the AE operation control.
		dvpAeOperation AeOp;
        dvpAeMode AeMode;

		status = dvpGetAeOperation(m_handle,&AeOp);
        ASSERT(status == DVP_STATUS_OK);

        status = dvpGetAeMode(m_handle, &AeMode);
        ASSERT(status == DVP_STATUS_OK);

 
        if (AeOp == AE_OP_OFF)
        {
            m_bAeOp = FALSE;
            UpdateData(FALSE);
			GetDlgItem(IDC_EDIT_EXPOSURE_TIME)->EnableWindow(TRUE);
			GetDlgItem(IDC_SPIN_EXPOSURE_TIME)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_EXPOSURE_TIME)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(TRUE);
			GetDlgItem(IDC_SPIN_GAIN)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_GAIN)->EnableWindow(TRUE);
        }
        else if (AeOp == AE_OP_CONTINUOUS || AeOp == AE_OP_ONCE)
        {		
            m_bAeOp = TRUE;
            UpdateData(FALSE);

			GetDlgItem(IDC_EDIT_EXPOSURE_TIME)->EnableWindow(AeMode == AE_MODE_AG_ONLY);
			GetDlgItem(IDC_SPIN_EXPOSURE_TIME)->EnableWindow(AeMode == AE_MODE_AG_ONLY);
			GetDlgItem(IDC_BUTTON_EXPOSURE_TIME)->EnableWindow(AeMode == AE_MODE_AG_ONLY);
			GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(AeMode == AE_MODE_AE_ONLY);
			GetDlgItem(IDC_SPIN_GAIN)->EnableWindow(AeMode == AE_MODE_AE_ONLY);
			GetDlgItem(IDC_BUTTON_GAIN)->EnableWindow(AeMode == AE_MODE_AE_ONLY);
        }
		
		// Set the current index of AeMode.
		((CComboBox*)GetDlgItem(IDC_COMBO_AE_MODE))->SetCurSel(AeMode);

		// Update the Anti-Flick control.
		dvpAntiFlick AntiFlick;
		status = dvpGetAntiFlick(m_handle, &AntiFlick);
        ASSERT(status == DVP_STATUS_OK);
        ((CComboBox*)GetDlgItem(IDC_COMBO_FLICK))->SetCurSel(AntiFlick);

		// Update the exposure time.
		double fExpoTime;
		CString strExpoTime;
		status = dvpGetExposure(m_handle,&fExpoTime);
        ASSERT(status == DVP_STATUS_OK);
		strExpoTime.Format(_T("%5f"),fExpoTime);					 
		SetDlgItemText(IDC_STATIC_EXPOTIME,strExpoTime);
		 
		// Update the analog gain.
		CString strGain;
		float fGain;
		status = dvpGetAnalogGain(m_handle,&fGain);
        ASSERT(status == DVP_STATUS_OK);
		strGain.Format(_T("%1.2f"),fGain);				
		SetDlgItemText(IDC_STATIC_EXPOGAIN,strGain);

		// Update the Reset ROI.
		dvpUint32 unRoiSel;
		status = dvpGetQuickRoiSel(m_handle, &unRoiSel);
		ASSERT(status == DVP_STATUS_OK);
		((CComboBox*)GetDlgItem(IDC_COMBO_RESOLUTION))->SetCurSel(unRoiSel);
	
        UpdateControlExposure();
        UpdateControlGain();
		
	}
	else
	{
		// No device is opened at this time.
		// Update the basic controls.
		GetDlgItem(IDC_BUTTON_OPEN)->SetWindowTextW(_T("Open"));	
		GetDlgItem(IDC_BUTTON_START)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_PROPERTY)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);

		if (m_listDevices.GetCount() == 0)
		{
			// No device exists.
			GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(FALSE);
		}
		else
		{
			GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(TRUE);
		}

		GetDlgItem(IDC_EDIT_EXPOSURE_TIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_EXPOSURE_TIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_EXPOSURE_TIME)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_GAIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_GAIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_GAIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_RESOLUTION)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_AE_MODE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_FLICK)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_PREVIEW)->InvalidateRect(NULL);
		GetDlgItem(IDC_CHECK_AEOPERATION)->EnableWindow(FALSE);	
	}
	InitAWB();
	SendMessage(WM_SIZE);
}

void CBasicFunctionDlg::UpdateControlExposure()
{
	double fExpoTime;
	CString strValue;
	dvpDoubleDescr ExpoTimeDescr;
    dvpStatus status;

	// Get the descriptive information about the exposure time.
    status = dvpGetExposureDescr(m_handle, &ExpoTimeDescr);
    ASSERT(status == DVP_STATUS_OK);
	if (status == DVP_STATUS_OK)
	{
		// Set the range of the exposure time. Unit us
		m_fMaxExposureTime = ExpoTimeDescr.fMax;
		m_fMinExposureTime = ExpoTimeDescr.fMin;
	}

	// Get the initial value of the exposure time.
    status = dvpGetExposure(m_handle, &fExpoTime);
    ASSERT(status == DVP_STATUS_OK);
	if (status == DVP_STATUS_OK)
	{
		// Set the initial value of the exposure time. 
		strValue.Format(_T("%d"), (int)fExpoTime);
		SetDlgItemText(IDC_EDIT_EXPOSURE_TIME,strValue);
	}
}

void CBasicFunctionDlg::UpdateControlGain()
{
	dvpStatus  status;
	float fAnalogGain;
	CString strValue;
	dvpFloatDescr AnalogGainDescr;

	// Get the descriptive information about the analog gain.
    status = dvpGetAnalogGainDescr(m_handle,&AnalogGainDescr);
    ASSERT(status == DVP_STATUS_OK);
    if (status == DVP_STATUS_OK)
    {
		// Set the range of the analog gain.
		m_fMinGain = AnalogGainDescr.fMin;
		m_fMaxGain = AnalogGainDescr.fMax;
    }

	// Get the value of the analog gain.
    status = dvpGetAnalogGain(m_handle, &fAnalogGain);
    ASSERT(status == DVP_STATUS_OK);
    if (status == DVP_STATUS_OK)
    {
		// Set the initial value of the analog gain.
        strValue.Format(_T("%.03f"),fAnalogGain);
        SetDlgItemText(IDC_EDIT_GAIN,strValue);
    }
}

void CBasicFunctionDlg::OnInitControlResolution()
{
	dvpUint32 RoiSel = 0;
	dvpQuickRoi RoiDetail;
	dvpStatus status; 
	dvpSelectionDescr RoiDescr;

	((CComboBox*)(GetDlgItem(IDC_COMBO_RESOLUTION)))->ResetContent();

	// Get the index number of the ROI.
	status = dvpGetQuickRoiSelDescr(m_handle, &RoiDescr);
    ASSERT(status == DVP_STATUS_OK);
	if (status == DVP_STATUS_OK)
	{
		for (UINT iNum = 0; iNum<RoiDescr.uCount; iNum++)
		{
			status = dvpGetQuickRoiSelDetail(m_handle,iNum, &RoiDetail);
            ASSERT(status == DVP_STATUS_OK);
			if (status == DVP_STATUS_OK)
			{			
				((CComboBox*)GetDlgItem(IDC_COMBO_RESOLUTION))->InsertString(iNum,a2w(RoiDetail.selection.string).c_str());
			}
		}
	}

	// Get the index number of the roi.
	status = dvpGetQuickRoiSel(m_handle,&RoiSel);
    ASSERT(status == DVP_STATUS_OK);
	if (status == DVP_STATUS_OK)
	{
		((CComboBox*)GetDlgItem(IDC_COMBO_RESOLUTION))->SetCurSel(RoiSel);
	}
}

void CBasicFunctionDlg::OnInitControlAeMode()
{
	((CComboBox*)(GetDlgItem(IDC_COMBO_AE_MODE)))->ResetContent();
	
	if (IsValidHandle(m_handle))
	{
		// Add the index of the AE mode to the AE mode list.
		((CComboBox*)GetDlgItem(IDC_COMBO_AE_MODE))->InsertString(0,a2w("AE_MODE_AE_AG").c_str());
		((CComboBox*)GetDlgItem(IDC_COMBO_AE_MODE))->InsertString(1,a2w("AE_MODE_AG_AE").c_str());
		((CComboBox*)GetDlgItem(IDC_COMBO_AE_MODE))->InsertString(2,a2w("AE_MODE_AE_ONLY").c_str());	
		((CComboBox*)GetDlgItem(IDC_COMBO_AE_MODE))->InsertString(3,a2w("AE_MODE_AG_ONLY").c_str());

		// Set the current AE mode.
		((CComboBox*)GetDlgItem(IDC_COMBO_AE_MODE))->SetCurSel(3);	
	}
}

void CBasicFunctionDlg::OnInitControlFlick()
{
	((CComboBox*)(GetDlgItem(IDC_COMBO_FLICK)))->ResetContent();
	if (IsValidHandle(m_handle))
	{
		// Add the index of the anti-flick setting to the anti-flick mode list.
		((CComboBox*)GetDlgItem(IDC_COMBO_FLICK))->InsertString(0,a2w("ANTIFLICK_DISABLE").c_str());
		((CComboBox*)GetDlgItem(IDC_COMBO_FLICK))->InsertString(1,a2w("ANTIFLICK_50HZ").c_str());
		((CComboBox*)GetDlgItem(IDC_COMBO_FLICK))->InsertString(2,a2w("ANTIFLICK_60HZ").c_str());	

		// Set the current anti-flick status.
		((CComboBox*)GetDlgItem(IDC_COMBO_FLICK))->SetCurSel(2);
	}
}

void CBasicFunctionDlg::OnBnClickedButtonScan()
{
	dvpStatus status;
	dvpUint32 i,n = 0;

	// "n" represent the number of cameras that is enumerated successfully, the drop-down list contains each camera's FriendlyName.

	m_listDevices.ResetContent();

	// Get the number of cameras that has been connected to a computer.
	status = dvpRefresh(&n);
    ASSERT(status == DVP_STATUS_OK);

    if (status == DVP_STATUS_OK)
    {
		// Enumeration of up to 16 cameras.
	    if ( n > 16)
		    n = 16;

		memset(m_info, 0, sizeof(dvpCameraInfo)*16);
		m_CamCount = 0;
	    for (i=0; i < n; i++)
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


void CBasicFunctionDlg::OnBnClickedButtonOpen()
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
				MessageBox(_T("Open the device failed"));
			}
			else
			{
				m_strFriendlyName = strName;	

				// If it needs to display images,the user should register a callback function and finish the operation of drawing pictures in the registered callback function.
				// Note: Drawing pictures in the callback function maybe generate some delays for acquiring image data by the use of "dvpGetFrame".
				status = dvpRegisterStreamCallback(m_handle, OnDrawPicture, STREAM_EVENT_PROCESSED, this);
				ASSERT(status == DVP_STATUS_OK);
				OnInitControlResolution();
				OnInitControlAeMode();
				OnInitControlFlick();
				UpdateControlExposure();
				UpdateControlGain();
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
		}

		status = dvpClose(m_handle);
        ASSERT(status == DVP_STATUS_OK);

		m_handle = 0;
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_STATIC_PREVIEW)->InvalidateRect(NULL, 1);
	}

	UpdateControls();
}


void CBasicFunctionDlg::OnBnClickedButtonStart()
{
	dvpStreamState state;
	dvpStatus status;

	if (IsValidHandle(m_handle))
	{
		// Implement a button to start and stop according to the current video's status.
		status = dvpGetStreamState(m_handle, &state);
		ASSERT(status == DVP_STATUS_OK);

		if (state == STATE_STARTED)
		{
			status = dvpStop(m_handle);
			ASSERT(status == DVP_STATUS_OK);
			GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
		}
		else
		{
			// init display count
			m_dfDisplayCount = 0;
			m_bSaveImageFlg = false;

			status = dvpStart(m_handle);
			ASSERT(status == DVP_STATUS_OK);

			GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
			UpdateControlExposure();
		}       
	}

	UpdateControls();
}


void CBasicFunctionDlg::OnNMCustomdrawSliderExposureTime(NMHDR *pNMHDR, LRESULT *pResult)
{
	CString strExpoTime;
	double  fExpoTime;
    dvpDoubleDescr ExpDescr;
    
    dvpStatus status;
    

    if (IsValidHandle(m_handle))
    {
		// Get the value of the exposure time.
	    fExpoTime = ((CSliderCtrl*)GetDlgItem(IDC_SLIDER_EXPOSURE_TIME))->GetPos();

		// Set the exposure time. 
		// Get the range of the exposure time,in order to avoid that the set value is out of range.
        status = dvpGetExposureDescr(m_handle, &ExpDescr);
        ASSERT(status == DVP_STATUS_OK);

        if (fExpoTime > ExpDescr.fMax)
            fExpoTime = ExpDescr.fMax;

        if (fExpoTime < ExpDescr.fMin)
            fExpoTime = ExpDescr.fMin;

		// Set the exposure time.
        status = dvpSetExposure(m_handle,fExpoTime);
        ASSERT(status == DVP_STATUS_OK);

		// Get the value of the exposure time again,there are differences between the set value and the obtained value for the reason of accuracy(step value),
		// it is subject to the obtained value.
        status = dvpSetExposure(m_handle,fExpoTime);
        ASSERT(status == DVP_STATUS_OK);

		// Set the text of the exposure time.
	    strExpoTime.Format(_T("%.3f"),fExpoTime);					
	    SetDlgItemText(IDC_STATIC_EXPOTIME,strExpoTime);
    }

	*pResult = 0;
}


void CBasicFunctionDlg::OnNMCustomdrawSliderExposureGain(NMHDR *pNMHDR, LRESULT *pResult)
{
	CString strGain;
	float fGain;
    dvpStatus status;

    if (IsValidHandle(m_handle))
    {
		// Get the value of the analog gain.
	    fGain = static_cast<float>(((CSliderCtrl*)GetDlgItem(IDC_SLIDER_GAIN))->GetPos()) / 100;

		// Firstly,set the value of the analog gain.
        status = dvpSetAnalogGain(m_handle,fGain);
        ASSERT(status == DVP_STATUS_OK);

		// Secondly,Get the value of the analog gain,there are differences between the set value and the obtained value for the reason of accuracy(step value),
		// it is subject to the obtained value.
        status = dvpGetAnalogGain(m_handle, &fGain);
        ASSERT(status == DVP_STATUS_OK);

		// Set the text of the analog gain.
	    strGain.Format(_T("%1.3f%s"),fGain,"x");				
	    SetDlgItemText(IDC_STATIC_EXPOGAIN, strGain);
    }

	*pResult = 0;
}


void CBasicFunctionDlg::OnCbnSelchangeComboResolution()
{
	dvpStatus status;

	if ( !IsValidHandle(m_handle) )
		return ;

	dvpStreamState state ;
	status = dvpGetStreamState(m_handle, &state);
	ASSERT(status == DVP_STATUS_OK);

	// Get the index of the ROI.
	int iSel = ((CComboBox*)GetDlgItem(IDC_COMBO_RESOLUTION))->GetCurSel();	
	if (iSel < 0)
    {
        ASSERT(FALSE);
        return;
    }

	if ( state == STATE_STARTED )
	{
		// Close the video stream.
		status = dvpStop(m_handle);
		if (status != DVP_STATUS_OK)
		{
			MessageBox(_T("Close the video stream failed!"));
			return ;
		}
	}

	// Set the ROI.
	status = dvpSetQuickRoiSel(m_handle,iSel);
	if (status != DVP_STATUS_OK)
	{
		MessageBox(_T("Set the ROI failed!"));
		return ;
	}
	if ( state == STATE_STARTED )
	{
		// Open the video stream.
		status = dvpStart(m_handle);
		if (status != DVP_STATUS_OK)
		{
			MessageBox(_T("Start the video stream failed!"));
			return ;
		}
	}
	UpdateControlExposure();
	UpdateControls();
	RECT rt = {0};
	GetClientRect(&rt);
	UpdataPreWindowSize(rt.right, rt.bottom);
}


void CBasicFunctionDlg::OnBnClickedButtonProperty()
{
	if (IsValidHandle(m_handle))
	{
		dvpStatus status = dvpShowPropertyModalDialog(m_handle, GetSafeHwnd());
		
		// At this time some configurations may change, synchronize it to the window GUI.
		UpdateControls();
		RECT rt = {0};
		GetClientRect(&rt);
		UpdataPreWindowSize(rt.right, rt.bottom);
	}
}


void CBasicFunctionDlg::OnBnClickedButtonSave()
{
	// Display dialog for saving pictures.
	CFileDialog file(
		FALSE, 
		_T("Picture"),
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("BITMAP Files(*.bmp)|*.bmp|JPEG Files(*.jpeg)|*.jpeg|TIFF Files(*.tiff)|\
		   *.tiff|PNG Files(*.png)|*.png|GIF Files(*.gif)|*.gif|RAW Files(*.dat)|*.dat|"),
		   this);

	if (file.DoModal() == IDOK)
	{
		m_ImagePath = file.GetPathName();
		m_bSaveImageFlg = true;
	}
}


void CBasicFunctionDlg::OnCbnSelchangeComboAeMode()
{
	dvpStatus  status = DVP_STATUS_OK;
	
	// Get the index of the AE mode.
	int iSel = ((CComboBox*)GetDlgItem(IDC_COMBO_AE_MODE))->GetCurSel();	
    if (iSel > 3)
    {
        ASSERT(FALSE);
        return;
    }

	// Prioritize the auto exposure.
    if (iSel == 0 )
    {
        status = dvpSetAeMode(m_handle, AE_MODE_AE_AG);
    }

	// Prioritize the auto gain.
    else if (iSel == 1)
    {
        status = dvpSetAeMode(m_handle, AE_MODE_AG_AE);
    }

	// Open the auto exposure only.
	else if (iSel == 2)
    {
        status = dvpSetAeMode(m_handle, AE_MODE_AE_ONLY);
    }

	// Open the auto gain only.
    else if (iSel == 3)
    {
        status = dvpSetAeMode(m_handle, AE_MODE_AG_ONLY);
    }

    ASSERT(status == DVP_STATUS_OK);
    UpdateControls();
}


void CBasicFunctionDlg::OnCbnSelchangeComboFlick()
{
	dvpStatus status = DVP_STATUS_OK;

	// Get the index of the AE.
	int iSel = ((CComboBox*)GetDlgItem(IDC_COMBO_FLICK))->GetCurSel();
    if (iSel > 2)
    {
		ASSERT(FALSE);;
    }

	if (iSel == 0)
	{
		status = dvpSetAntiFlick(m_handle, ANTIFLICK_DISABLE);
	}
	else if (iSel == 1)
	{
		status = dvpSetAntiFlick(m_handle, ANTIFLICK_50HZ);
	}
    else if (iSel == 2)
	{
		status = dvpSetAntiFlick(m_handle, ANTIFLICK_60HZ);
	}

    ASSERT(status == DVP_STATUS_OK);
}


void CBasicFunctionDlg::OnBnClickedCheckAeoperation()
{
     
	UpdateData(TRUE);
    dvpStatus status;

	if (m_bAeOp == TRUE)
	{
		status = dvpSetAeOperation(m_handle, AE_OP_CONTINUOUS);
	}
	else
	{
		status = dvpSetAeOperation(m_handle, AE_OP_OFF);
	}

    ASSERT(status == DVP_STATUS_OK);
    UpdateControls();
}

// Init the anto white balance.
void CBasicFunctionDlg::InitAWB()
{
	dvpStatus status;
	dvpAwbOperation  AwbOperation;

	if ( IsValidHandle(m_handle) )
	{
		// Get the current white balance property.
		status = dvpGetAwbOperation(m_handle, &AwbOperation);
		// ASSERT(status == DVP_STATUS_OK);
		if ( status != DVP_STATUS_OK )
		{
			// Auto white balance is not supported or get fails.
			GetDlgItem(IDC_CHECK_AWB)->EnableWindow(false);
			GetDlgItem(IDC_BUTTON_ONCE_AWB)->EnableWindow(false);
			return ;
		}
		// Set the control
		GetDlgItem(IDC_CHECK_AWB)->EnableWindow(TRUE);
		CheckDlgButton(IDC_CHECK_AWB, AwbOperation ==  AWB_OP_CONTINUOUS ? BST_CHECKED : BST_UNCHECKED);
		GetDlgItem(IDC_BUTTON_ONCE_AWB)->EnableWindow(AwbOperation ==  AWB_OP_CONTINUOUS ? FALSE : TRUE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_AWB)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_ONCE_AWB)->EnableWindow(false);
	}
}

void CBasicFunctionDlg::OnBnClickedCheckAwb()
{
	dvpStatus status;
	dvpAwbOperation  AwbOperation;

	if ( IsValidHandle(m_handle) )
	{
		// Auto white balance,set to AWB_OP_CONTINUOUS stands for continuous operation. 
		if ( IsDlgButtonChecked(IDC_CHECK_AWB) == BST_CHECKED )
		{
			status = dvpGetAwbOperation(m_handle, &AwbOperation);
			if ( AwbOperation != AWB_OP_CONTINUOUS )
			{
				status = dvpSetAwbOperation(m_handle, AWB_OP_CONTINUOUS);
				ASSERT(status == DVP_STATUS_OK);
			}
		}
		else
		{
			// Close auto white balance.
			status = dvpSetAwbOperation(m_handle, AWB_OP_OFF);
			ASSERT(status == DVP_STATUS_OK);
		}

		InitAWB();
	}
}


void CBasicFunctionDlg::OnBnClickedButtonOnceAwb()
{
	dvpStatus status;

	if ( IsValidHandle(m_handle) )
	{
		// Auto white balance,set to AWB_OP_ONCE stands for operating once. 
		status = dvpSetAwbOperation(m_handle, AWB_OP_ONCE);
		ASSERT(status == DVP_STATUS_OK);

		// The RGB gain is set to 1.00, avoiding a superimposed gain effect
		bool bGainState = false;
		status = dvpGetRgbGainState(m_handle, &bGainState);
		ASSERT(status == DVP_STATUS_OK);
		if ( status == DVP_STATUS_OK )
		{
			if ( bGainState )
			{
				status = dvpSetRgbGain(m_handle, 1.0f, 1.0f, 1.0f);
				ASSERT(status == DVP_STATUS_OK);
			}
			else
			{
				status = dvpSetRgbGainState(m_handle, true);
				ASSERT(status == DVP_STATUS_OK);
				status = dvpSetRgbGain(m_handle, 1.0f, 1.0f, 1.0f);
				ASSERT(status == DVP_STATUS_OK);
				status = dvpSetRgbGainState(m_handle, bGainState);
				ASSERT(status == DVP_STATUS_OK);
			}
		}
		InitAWB();
	}
}


void CBasicFunctionDlg::OnDeltaposSpinGain(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	*pResult = 0;

	TCHAR szTmp[128] = {0};
	GetDlgItemText(IDC_EDIT_GAIN, szTmp, sizeof(szTmp)-1);
	float fGain = (float)_ttof(szTmp);

	fGain -= pNMUpDown->iDelta * 0.125f;
	if ( fGain > m_fMaxGain )
		fGain = m_fMaxGain;
	if ( fGain < m_fMinGain )
		fGain = m_fMinGain;

	// Set the analog gain.
	_stprintf_s(szTmp, TEXT("%.03f"), fGain);	
	SetDlgItemText(IDC_EDIT_GAIN, szTmp);

}

void CBasicFunctionDlg::OnDeltaposSpinExposureTime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	*pResult = 0;
	TCHAR szTmp[128] = {0};
	GetDlgItemText(IDC_EDIT_EXPOSURE_TIME, szTmp, sizeof(szTmp)-1);
	double f_time = (float)_ttof(szTmp);

	f_time -= pNMUpDown->iDelta * 100;
	if ( f_time > m_fMaxExposureTime )
		f_time = m_fMaxExposureTime;
	if ( f_time < m_fMinExposureTime )
		f_time = m_fMinExposureTime;

	_stprintf_s(szTmp, TEXT("%d"), (long)f_time);	
	SetDlgItemText(IDC_EDIT_EXPOSURE_TIME, szTmp);

}

void CBasicFunctionDlg::OnBnClickedButtonGain()
{
	TCHAR szTmp[128] = {0};
	GetDlgItemText(IDC_EDIT_GAIN, szTmp, sizeof(szTmp)-1);
	float fGain = (float)_ttof(szTmp);
	dvpStatus status ;

	if ( fGain > m_fMaxGain )
		fGain = m_fMaxGain;
	if ( fGain < m_fMinGain )
		fGain = m_fMinGain;

	if ( IsValidHandle(m_handle) )
	{
		// Set the analog gain.
		status = dvpSetAnalogGain(m_handle, fGain);
		ASSERT(status == DVP_STATUS_OK);

		// Set the value of the analog gain,there are differences between the set value and the obtained value for the reason of accuracy(step value),
		// it is subject to the obtained value.
		status = dvpGetAnalogGain(m_handle, &fGain);
		ASSERT(status == DVP_STATUS_OK);

		// Set the analog gain.
		_stprintf_s(szTmp, TEXT("%.03f"), fGain);	
		SetDlgItemText(IDC_EDIT_GAIN, szTmp);
	}
}

void CBasicFunctionDlg::OnBnClickedButtonExposureTime()
{
	TCHAR szTmp[128] = {0};
	GetDlgItemText(IDC_EDIT_EXPOSURE_TIME, szTmp, sizeof(szTmp)-1);
	double f_time = _ttof(szTmp);
	dvpStatus status ;

	if ( f_time > m_fMaxExposureTime -1 )
		f_time = m_fMaxExposureTime -1;
	if ( f_time < m_fMinExposureTime+1 )
		f_time = m_fMinExposureTime+1;

	if ( IsValidHandle(m_handle) )
	{
		status = dvpSetExposure(m_handle, f_time);
		ASSERT(status==DVP_STATUS_OK);

		// Get the value of the exposure time again,there are differences between the set value and the obtained value for the reason of accuracy(step value),
		// it is subject to the obtained value.
		status = dvpGetExposure(m_handle, &f_time);
		ASSERT(status == DVP_STATUS_OK);

		// Set the exposure time
		_stprintf_s(szTmp, TEXT("%d"), (long)f_time);	
		SetDlgItemText(IDC_EDIT_EXPOSURE_TIME, szTmp);
	}
}

void CBasicFunctionDlg::OnBnClickedCheckUserdefinedname()
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
