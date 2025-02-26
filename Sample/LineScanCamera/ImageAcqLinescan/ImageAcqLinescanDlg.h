
// ImageAcqLinescanDlg.h : header file
//

#pragma once

#include <stdint.h>

// DVP API
#include "..\\..\\..\\library\\Visual C++\\include\\DVPCamera.h"
//#include "C:\Program Files (x86)\DVP2 SDK\library\Visual C++\include"
#ifdef _M_X64
//#pragma comment(lib, "../../../library/Visual C++/lib/x64/DVPCamera64.lib")
#pragma comment(lib, "..\\..\\..\\library\\Visual C++\\lib\\x64\\DVPCamera64.lib")
#else				  
#pragma comment(lib, "..\\..\\..\\library\\Visual C++\\lib\\x86\\DVPCamera.lib")
#endif
#include "afxwin.h"

// CImageAcqLinescanDlg dialog
class CImageAcqLinescanDlg : public CDialogEx
{
	// Construction
public:
	CImageAcqLinescanDlg(CWnd* pParent = NULL);	// standard constructor
	~CImageAcqLinescanDlg();

	// Dialog Data
	enum { IDD = IDD_IMAGEACQLINESCAN_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	void UpdateControls();
	UINT ImageThread();
	static UINT ImageThread(void* pParam);
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

	// Implementation
protected:
	HICON m_hIcon;
	CWinThread* m_pThread;
	CEvent m_evtExit;
	dvpHandle m_handle;
	CComboBox m_listDevices;
	CComboBox m_cbxTrigType;
	CComboBox m_cbxTrigSource;
	CButton m_chkForward;


	CListBox  m_listbox;
	CString m_strFriendlyName;
	BOOL m_bSoftTrigger;
	UINT m_uImageCount;


	// Display param
	unsigned int m_StartGrabClock;
	unsigned int m_CurGrabClock;
	double m_dfDisplayCount;

	bool m_bUserDefinedName;
	dvpCameraInfo m_info[16];
	int m_CamCount;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	void InitUI();

	CString m_strPicturePath;
	CString m_strFileName;
	UINT m_RecentCount;
	BOOL m_bDisplay;
	BOOL m_bSave;
	CWnd m_Picture;
	UINT m_uWaitTimer;
	UINT m_uGrabTimeout;
	UINT m_uGrabQuickRoiVar;				//快速切换roi的列表变量
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonProperty();
	afx_msg void OnBnClickedButtonScan();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnClose();
	afx_msg void OnBnClickedCheckUserdefinedname();
	
	afx_msg void OnCbnSelchangeCbxtrigtype();
	
	
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonSofttrig();
	afx_msg void OnBnClickedCheckForward();
};
