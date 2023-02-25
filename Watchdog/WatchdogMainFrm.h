// MainFrm.h : interface of the CWatchdogMainFrame class
//

#pragma once
#include "WatchdogFileView.h"
#include "WatchdogClassView.h"
#include "WatchdogOutputWnd.h"
#include "WatchdogPropertiesWnd.h"

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
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);

	// Implementation
public:
	virtual ~CWatchdogMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
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

	UINT m_uTimerID;

	// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
