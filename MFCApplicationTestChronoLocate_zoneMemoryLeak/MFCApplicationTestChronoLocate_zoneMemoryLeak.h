
// MFCApplicationTestChronoLocate_zoneMemoryLeak.h : main header file for the MFCApplicationTestChronoLocate_zoneMemoryLeak application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMFCApplicationTestChronoLocatezoneMemoryLeakApp:
// See MFCApplicationTestChronoLocate_zoneMemoryLeak.cpp for the implementation of this class
//

class CMFCApplicationTestChronoLocatezoneMemoryLeakApp : public CWinApp
{
public:
	CMFCApplicationTestChronoLocatezoneMemoryLeakApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMFCApplicationTestChronoLocatezoneMemoryLeakApp theApp;
