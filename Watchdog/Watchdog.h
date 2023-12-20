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

class CWatchdogApp : public CWinAppEx {
public:
	CWatchdogApp() noexcept;

	// Overrides
public:
	BOOL InitInstance() override;
	int ExitInstance() override;

	// Implementation
	UINT m_nAppLook;
	BOOL m_bHiColorIcons;

	void PreLoadState() override;
	void LoadCustomState() override;
	void SaveCustomState() override;

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CWatchdogApp theApp;
