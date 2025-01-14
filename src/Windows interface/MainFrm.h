// MainFrm.h: CMainFrame 类的接口
//
#pragma once

#include"stdafx.h"
#include "OutputWnd.h"
#include "PropertiesWnd.h"
#include "PropertyRealTimeDataWnd.h"

class CMainFrame : public CMDIFrameWndEx {
	DECLARE_DYNCREATE(CMainFrame)

public:
	CMainFrame();
	~CMainFrame() override;

	// 特性

	// 操作
	void UpdateStatus();
	void UpdateInnerSystemStatus();

	// 需包裹的调用系统函数的函数（以便于使用GMock），前缀为SysCall
	virtual void SysCallOnTimer(UINT_PTR nIDEvent) { CMDIFrameWndEx::OnTimer(nIDEvent); }
	virtual void SysCallSetPaneText(int iIndex, LPCTSTR lpszNewText) { m_wndStatusBar.SetPaneText(iIndex, lpszNewText); }
	virtual void SysCallSetInnerSystemPaneText(int iIndex, LPCTSTR lpszNewText) { m_wndInnerSystemBar.SetPaneText(iIndex, lpszNewText); }
	virtual void SysCallOnSysCommand(UINT nID, LPARAM lParam) { CMDIFrameWndEx::OnSysCommand(nID, lParam); }
	virtual void SysCallCmdUIEnable(CCmdUI* pCmdUI, bool fFlag) { pCmdUI->Enable(fFlag); }
	virtual void SysCallCmdUISetCheck(CCmdUI* pCmdUI, bool fFlag) { pCmdUI->SetCheck(fFlag); }
	virtual LRESULT SysCallSendMessage(UINT message, WPARAM wParam, LPARAM lParam) { return SendMessage(message, wParam, lParam); }
	virtual BOOL SysCallPreTranslateMessage(MSG* pMsg) { return CMDIFrameWndEx::PreTranslateMessage(pMsg); }
	virtual void SysCallInvalidate() { Invalidate(); }
	virtual void SysCallOnChar(UINT nChar, UINT nRepCnt, UINT nFlags) { CMDIFrameWndEx::OnChar(nChar, nRepCnt, nFlags); }
	virtual void SysCallOnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) { CMDIFrameWndEx::OnKeyUp(nChar, nRepCnt, nFlags); }

	virtual void CalculateTodayRS();
	virtual void ProcessChinaMarketStock();

	long GetCurrentPos() const noexcept { return m_lCurrentPos; }

	// 重写
	BOOL PreCreateWindow(CREATESTRUCT& cs) override;
	BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = nullptr, CCreateContext* pContext = nullptr) override;

protected:
	UINT m_uIdTimer{ 0 };
	long m_lCurrentPos{ 0 };

	char m_aStockCodeTemp[30]{};

	static bool sm_fGlobeInit;

	// 控件条嵌入成员
	CMFCMenuBar m_wndMenuBar;
	CMFCToolBar m_wndToolBar;
	CMFCStatusBar m_wndInnerSystemBar;
	CMFCStatusBar m_wndStatusBar;
	CMFCToolBarImages m_UserImages;
	COutputWnd m_wndOutput;
	CPropertiesWnd m_wndProperties;
	CPropertyRealtimeWnd m_wndPropertyRealTime;

	BOOL CreateDockingWindows();
	void SetDockingWindowIcons(BOOL bHiColorIcons);
	// 生成的消息映射函数
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

public:
	BOOL PreTranslateMessage(MSG* pMsg) override;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnProcessTodayStock();
	afx_msg void OnUpdateProcessTodayStock(CCmdUI* pCmdUI);
	afx_msg void OnCalculateTodayRS();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnUpdateCalculateTodayRS(CCmdUI* pCmdUI);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRebuildChinaMarketStockDayLineRS();
	afx_msg void OnUpdateRebuildChinaMarketStockDayLineRS(CCmdUI* pCmdUI);
	afx_msg void OnAbortBuildingRS();
	afx_msg void OnUpdateAbortBuildingRS(CCmdUI* pCmdUI);
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
	afx_msg void OnRebuildChinaMarketStockWeekLineRS();
	afx_msg void OnUpdateRebuildChinaMarketStockWeekLineRS(CCmdUI* pCmdUI);
	afx_msg void OnBuildCurrentWeekLine();
	afx_msg void OnUpdateBuildCurrentWeekLine(CCmdUI* pCmdUI);
	afx_msg void OnBuildRebuildCurrentWeekLine();
	afx_msg void OnUpdateBuildRebuildCurrentWeekLine(CCmdUI* pCmdUI);
	afx_msg void OnBuildRebuildCurrentWeekWeekLineTable();
	afx_msg void OnUpdateBuildRebuildCurrentWeekWeekLineTable(CCmdUI* pCmdUI);
	afx_msg void OnUpdateStockSection();
	afx_msg void OnUpdateStockCode();
	afx_msg void OnRebuildEpsSurprise();
	afx_msg void OnRebuildPeer();
	afx_msg void OnRebuildDayLine();
	afx_msg void OnUpdateFinnhubStockDayLineStartEnd();
	afx_msg void OnRecordFinnhubWebSocket();
	afx_msg void OnUpdateRecordFinnhubWebSocket(CCmdUI* pCmdUI);
	afx_msg void OnRecordTiingoCryptoWebSocket();
	afx_msg void OnUpdateRecordTiingoCryptoWebSocket(CCmdUI* pCmdUI);
	afx_msg void OnRecordTiingoForexWebSocket();
	afx_msg void OnUpdateRecordTiingoForexWebSocket(CCmdUI* pCmdUI);
	afx_msg void OnRecordTiingoIEXWebSocket();
	afx_msg void OnUpdateRecordTiingoIEXWebSocket(CCmdUI* pCmdUI);
	afx_msg void OnRebuildBasicFinancial();
	afx_msg void OnMaintainChinaMarketStockDayLine();
	afx_msg void OnUpdateMaintainChinaMarketStockDayLine(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnUsingNeteaseDayLineDataServer();
	afx_msg void OnUpdateUsingNeteaseDayLineDataServer(CCmdUI* pCmdUI);
	afx_msg void OnUsingTengxunDayLineDataServer();
	afx_msg void OnUpdateUsingTengxunDayLineDataServer(CCmdUI* pCmdUI);
	afx_msg void OnUsingTengxunRealtimeDataServer();
	afx_msg void OnUpdateUsingTengxunRealtimeDataServer(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTiingoFundamentalDefinition();
	afx_msg void OnUpdateUpdateTiingoFundamentalDefinition(CCmdUI* pCmdUI);
	afx_msg void OnResetTiingoDaylineDate();
	afx_msg void OnCreateTiingoTradeDayDayline();
	afx_msg void OnUpdateCreateTiingoTradeDayDayline(CCmdUI* pCmdUI);
	afx_msg void OnProcessTiingoDayline();
	afx_msg void OnUpdateProcessTiingoDayline(CCmdUI* pCmdUI);
};
