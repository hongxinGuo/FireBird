// MainFrm.h: CMainFrame 类的接口
//
#pragma once
#include "OutputWnd.h"

constexpr int __STOCK_ANALYSIS_TIMER__ = 1;

class CMainFrame : public CFrameWndEx
{
	DECLARE_DYNCREATE(CMainFrame)
public:
	CMainFrame();
	virtual ~CMainFrame();

	void Reset(void);

	// 特性
public:

	// 操作
public:
	virtual bool SchedulingTask(void);
	void UpdateStatus(void);
	void UpdateInnerSystemStatus(void);
	bool CreateMarketContainer(void); // 生成各市场容器（只用于调度）

public:
	// 需包裹的调用系统函数的函数（以便于使用GMock），前缀为SysCall
	virtual void SysCallOnTimer(UINT_PTR nIDEvent) { CFrameWndEx::OnTimer(nIDEvent); }
	virtual void SysCallSetPaneText(int iIndex, LPCTSTR lpszNewText) { m_wndStatusBar.SetPaneText(iIndex, lpszNewText); }
	virtual void SysCallSetInnerSystemPaneText(int iIndex, LPCTSTR lpszNewText) { m_wndInnerSystemBar.SetPaneText(iIndex, lpszNewText); }
	virtual void SysCallOnSysCommand(UINT nID, LPARAM lParam) { CFrameWndEx::OnSysCommand(nID, lParam); }
	virtual void SysCallCmdUIEnable(CCmdUI* pCmdUI, bool fFlag) { pCmdUI->Enable(fFlag); }
	virtual void SysCallCmdUISetCheck(CCmdUI* pCmdUI, bool fFlag) { pCmdUI->SetCheck(fFlag); }
	virtual void SysCallSendMessage(UINT message, WPARAM wParam, LPARAM lParam) { SendMessage(message, wParam, lParam); }
	virtual BOOL SysCallPreTranslateMessage(MSG* pMsg) { return CFrameWndEx::PreTranslateMessage(pMsg); }
	virtual void SysCallInvalidate(void) { Invalidate(); }
	virtual void SysCallOnChar(UINT nChar, UINT nRepCnt, UINT nFlags) { CFrameWndEx::OnChar(nChar, nRepCnt, nFlags); }
	virtual void SysCallOnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) { CFrameWndEx::OnKeyUp(nChar, nRepCnt, nFlags); }

	virtual void CalculateTodayRS(void);
	virtual void ProcessChinaMarketStock(void);

public:
	// 重置系统（恢复系统的初始态，准备第二天继续工作。
	virtual bool ResetMarket(void);

	long GetCurrentPos(void) noexcept { return m_lCurrentPos; }

	// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr);

	// 实现
public:
	CString FormatToMK(long long iNumber); // 将数字转换成以M或者K为单位

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	UINT m_uIdTimer;
	long m_lCurrentPos;
	time_t m_timeLast;

	char m_aStockCodeTemp[30];

	static bool sm_fInitIxWebSocket;

protected:  // 控件条嵌入成员
	CMFCMenuBar m_wndMenuBar;
	CMFCToolBar m_wndToolBar;
	CMFCStatusBar m_wndInnerSystemBar;
	CMFCStatusBar m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	COutputWnd m_wndOutput;

	// 生成的消息映射函数
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

		BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnProcessTodayStock();
	afx_msg void OnUpdateProcessTodayStock(CCmdUI* pCmdUI);
	afx_msg void OnCalculateTodayRS();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnUpdateCalculateTodayRS(CCmdUI* pCmdUI);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRebuildDayLineRS();
	afx_msg void OnBuildResetMarket();
	afx_msg void OnUpdateRebuildDayLineRS(CCmdUI* pCmdUI);
	afx_msg void OnAbortBuindingRS();
	afx_msg void OnUpdateAbortBuindingRS(CCmdUI* pCmdUI);
	afx_msg void OnRecordRTData();
	afx_msg void OnUpdateRecordRTData(CCmdUI* pCmdUI);
	afx_msg void OnCalculate10dayRS1();
	afx_msg void OnCalculate10dayRS2();
	afx_msg void OnCalculate10dayRS();
	afx_msg void OnUpdateCalculate10dayRS1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCalculate10dayRS2(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCalculate10dayRS(CCmdUI* pCmdUI);
	afx_msg void OnStopUpdateDayLine();
	afx_msg void OnUsingNeteaseRealtimeDataServer();
	afx_msg void OnUsingSinaRealtimeDataServer();
	afx_msg void OnUpdateUsingNeteaseRealtimeDataServer(CCmdUI* pCmdUI);
	afx_msg void OnUpdateUsingSinaRealtimeDataServer(CCmdUI* pCmdUI);
	afx_msg void OnBuildCreateWeekLine();
	afx_msg void OnUpdateBuildCreateWeekLine(CCmdUI* pCmdUI);
	afx_msg void OnRebuildWeekLineRS();
	afx_msg void OnUpdateRebuildWeekLineRS(CCmdUI* pCmdUI);
	afx_msg void OnBuildCurrentWeekLine();
	afx_msg void OnUpdateBuildCurrentWeekLine(CCmdUI* pCmdUI);
	afx_msg void OnBuildRebuildCurrentWeekLine();
	afx_msg void OnUpdateBuildRebuildCurrentWeekLine(CCmdUI* pCmdUI);
	afx_msg void OnBuildRebuildCurrentWeekWeeklineTable();
	afx_msg void OnUpdateBuildRebuildCurrentWeekWeeklineTable(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStockSection();
	afx_msg void OnUpdateStockCode();
	afx_msg void OnRebuildEpsSurprise();
	afx_msg void OnRebuildPeer();
	afx_msg void OnRebuildDayline();
	afx_msg void OnUpdateWorldStockDaylineStartEnd();
	afx_msg void OnRecordFinnhubWebSocket();
	afx_msg void OnUpdateRecordFinnhubWebSocket(CCmdUI* pCmdUI);
	afx_msg void OnRecordTiingoCryptoWebSocket();
	afx_msg void OnUpdateRecordTiingoCryptoWebSocket(CCmdUI* pCmdUI);
	afx_msg void OnRecordTiingoForexWebSocket();
	afx_msg void OnUpdateRecordTiingoForexWebSocket(CCmdUI* pCmdUI);
	afx_msg void OnRecordTiingoIexWebSocket();
	afx_msg void OnUpdateRecordTiingoIexWebSocket(CCmdUI* pCmdUI);
	afx_msg void OnRebuildBasicFinancial();
	afx_msg void OnMaintainDayline();
	afx_msg void OnUpdateMaintainDayline(CCmdUI* pCmdUI);
};
