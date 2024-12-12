module;
#include"pch.h"
export module Watchdog.MainFrm;

import Watchdog.FileView;
import Watchdog.ClassView;

import Watchdog.PropertiesWnd;
import Watchdog.OutputWnd;

export {
	class CWatchdogMainFrame : public CMDIFrameWndEx {
		DECLARE_DYNAMIC(CWatchdogMainFrame)

	public:
		CWatchdogMainFrame() noexcept;

		// Attributes
	public:
		// Operations
	public:
		// Overrides
	public:
		BOOL PreCreateWindow(CREATESTRUCT& cs) override;
		BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr) override;

		// Implementation
	public:
		~CWatchdogMainFrame() override;
#ifdef _DEBUG
		void AssertValid() const override;
		void Dump(CDumpContext& dc) const override;
#endif

	protected:  // control bar embedded members
		CMFCMenuBar m_wndMenuBar;
		CMFCToolBar m_wndToolBar;
		CMFCStatusBar m_wndStatusBar;
		CMFCToolBarImages m_UserImages;
		CWatchdogFileView m_wndFileView;
		CWatchdogClassView m_wndClassView;
		CWatchdogOutputWnd m_wndOutput;
		CWatchdogPropertiesWnd m_wndProperties;

		UINT_PTR m_uTimerID;

		// Generated message map functions
	protected:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnWindowManager();
		afx_msg void OnViewCustomize();
		afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
		afx_msg void OnApplicationLook(UINT id);
		afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
		afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
		afx_msg void OnTimer(UINT_PTR nIDEvent);
		DECLARE_MESSAGE_MAP()

		BOOL CreateDockingWindows();
		void SetDockingWindowIcons(BOOL bHiColorIcons);

	public:
	};
}
