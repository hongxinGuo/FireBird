#pragma once

/////////////////////////////////////////////////////////////////////////////
// COutputList 窗口

class COutputList : public CListBox {
	// 构造
public:
	COutputList();

	// 实现
	void TruncateList(long lNumberOfTruncation = 1000);
	void SetCurAtLastLine(void);

public:
	~COutputList() override;

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()
};

class COutputWnd : public CDockablePane {
	// 构造
public:
	COutputWnd();

	void UpdateFonts();

	// 特性
protected:
	CMFCTabCtrl m_wndTabs;

	COutputList m_wndOutputInformation;
	COutputList m_wndOutputDayLineInfo;
	COutputList m_wndOutputTransaction;
	COutputList m_wndOutputCancelSell;
	COutputList m_wndOutputCancelBuy;
	COutputList m_wndOutputTrace2;
	COutputList m_wndOutputWebSocketInfo;
	COutputList m_wndOutputInnerSystemInformation;
	COutputList m_wndErrorMessage;
	COutputList m_wndChinaMarketTaskQueue;

	UINT m_uIdTimer;

protected:
	void AdjustHorzScroll(CListBox& wndListBox);

	// 实现
public:
	~COutputWnd() override;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
