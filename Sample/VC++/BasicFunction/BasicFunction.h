// BasicFunction.h : main header file for the PROJECT_NAME application.

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

// main symbols
#include "resource.h"		

// CBasicFunctionApp:
// See BasicFunction.cpp for the implementation of this class. 

class CBasicFunctionApp : public CWinApp
{
public:
	CBasicFunctionApp();

// Overrides 
public:
	virtual BOOL InitInstance();

// Implementation 
	DECLARE_MESSAGE_MAP()
};

extern CBasicFunctionApp theApp;