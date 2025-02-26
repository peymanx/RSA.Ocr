// Trigger.h : main header file for the PROJECT_NAME application.

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

// main symbols
#include "resource.h"		

// CTriggerApp:
// See Trigger.cpp for the implementation of this class.

class CTriggerApp : public CWinApp
{
public:
	CTriggerApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTriggerApp theApp;