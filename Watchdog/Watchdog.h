
// Watchdog.h : main header file for the Watchdog application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CWatchdogApp:
// See Watchdog.cpp for the implementation of this class
//

class CWatchdogApp : public CWinApp
{
public:
	CWatchdogApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CWatchdogApp theApp;
