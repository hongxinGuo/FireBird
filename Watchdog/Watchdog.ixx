// Watchdog.h : main header file for the Watchdog application
//
module;
#include"pch.h"
#include <afxwinappex.h>
#include "resource.h"       // main symbols
export module Watchdog.App;

export {
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
}
