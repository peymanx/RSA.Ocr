// BasicFunctionDlg.h : header file

#pragma once
#include "afxwin.h"
#include "../../../library/Visual C++/include/DVPCamera.h"
#include <string>
#include "afxcmn.h"
#include<iostream>
// CBasicFunctionDlg dialog
class CBasicFunctionDlg : public CDialogEx
{
// Construction
public:
	CBasicFunctionDlg(CWnd* pParent = NULL);			// Standard constructor
	int iHoTemp,iVTemp,iWTemp,iHTemp;

// Dialog Data
	enum { IDD = IDD_BASICFUNCTION_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	void UpdateControls();
	void InitAWB();

	static int OnDrawPicture(dvpHandle handle,
		dvpStreamEvent event, 
		void *pContext, 
		dvpFrame *pFrame, 
		void *pBuffer);

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
	CComboBox m_listDevices;
	dvpHandle m_handle;
	CString m_strFriendlyName;
	
	float m_fMaxGain;
	float m_fMinGain;
	double m_fMaxExposureTime;
	double m_fMinExposureTime;

	// Display param
	unsigned int m_StartGrabClock;
	unsigned int m_CurGrabClock;
	double m_dfDisplayCount;

	bool m_bUserDefinedName;
	dvpCameraInfo m_info[16];
	int m_CamCount;

	// Generated message map functions
	void OnInitControlResolution();
	void UpdateControlExposure();
	void UpdateControlGain();
	void OnInitControlAeMode();
	void OnInitControlFlick();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	bool m_bSaveImageFlg;
	CString m_ImagePath;
public:
	BOOL m_bAeOp;
	afx_msg void OnBnClickedButtonScan();
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnNMCustomdrawSliderExposureTime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderExposureGain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComboResolution();
	afx_msg void OnBnClickedButtonProperty();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnCbnSelchangeComboAeMode();
	virtual void OnOK();
	afx_msg void OnBnClickedCheckAntiFlick();
	afx_msg void OnCbnSelchangeComboFlick();
	afx_msg void OnBnClickedCheckAeoperation();
	void UpdataPreWindowSize(int x, int y);

	afx_msg void OnBnClickedCheckAwb();
	afx_msg void OnBnClickedButtonOnceAwb();
	afx_msg void OnDeltaposSpinGain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinExposureTime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonGain();
	afx_msg void OnBnClickedButtonExposureTime();
	afx_msg void OnBnClickedCheckUserdefinedname();
};
