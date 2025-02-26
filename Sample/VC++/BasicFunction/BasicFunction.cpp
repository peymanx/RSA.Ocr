// BasicFunction.cpp : Defines the class behaviors for the application.

#include "stdafx.h"
#include "BasicFunction.h"
#include "BasicFunctionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBasicFunctionApp

BEGIN_MESSAGE_MAP(CBasicFunctionApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CBasicFunctionApp construction

CBasicFunctionApp::CBasicFunctionApp()
{
	// Support restart manager.
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// Place all significant initialization in InitInstance. 
}


// The one and only CBasicFunctionApp object.

CBasicFunctionApp theApp;


// CBasicFunctionApp initialization

BOOL CBasicFunctionApp::InitInstance()
{
	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCt132.dll version 6 or later to enable
	// visual styles. Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);

	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// Create the shell manager, in case the dialog contains 
	// any shell tree view or shell list view controls.
	CShellManager *pShellManager = new CShellManager;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need.
	// Change the registry key under which our settings are stored.
	// TODO:You shuold modify this string to be something appropriatte
	// such as the name of your company or orgaization.
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	CBasicFunctionDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel.
	}

	// Delete the shell manager created above.
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

