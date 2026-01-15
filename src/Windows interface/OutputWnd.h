#pragma once
#include "CharSetTransfer.h"

/////////////////////////////////////////////////////////////////////////////
// COutputList 窗口

class COutputList : public CListBox {
	// 构造
public:
	COutputList() = default;
	COutputList(const COutputList&) = delete;
	COutputList& operator=(const COutputList&) = delete;
	COutputList(const COutputList&&) = delete;
	COutputList& operator=(const COutputList&&) = delete;
	~COutputList() override = default ;

	// 实现
	void TruncateList(long lNumberOfTruncation = 1000);
	void SetCurAtLastLine();

	int GetLineNumber() const noexcept { return m_iLineNumber; }
	int AppendString(const string& str) { return AddString(Utf8ToWstring(str).c_str()); } // UNICODE下需要转换，utf-8 --> utf-16. 

protected:
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEditCopy();
	afx_msg void OnEditClear();
	afx_msg void OnViewOutput();

	DECLARE_MESSAGE_MAP()

protected:
	int m_iLineNumber{ 10 }; // 可显示行数,初始为10行

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};

class COutputWnd : public CDockablePane {
	// 构造
public:
	COutputWnd();
	COutputWnd(const COutputWnd&) = delete;
	COutputWnd& operator=(const COutputWnd&) = delete;
	COutputWnd(const COutputWnd&&) = delete;
	COutputWnd& operator=(const COutputWnd&&) = delete;
	~COutputWnd() override = default;

	void UpdateFonts();

	// 特性
protected:
	CMFCTabCtrl m_wndTabs;

	COutputList m_wndOutputInformation;
	COutputList m_wndOutputDayLineInfo;
	COutputList m_wndChinaMarketTaskQueue;
	COutputList m_wndWorldMarketTaskQueue;
	COutputList m_wndOutputWebSocketInfo;
	COutputList m_wndOutputInnerSystemInformation;
	COutputList m_wndErrorMessage;

	UINT m_uIdTimer;

protected:
	void AdjustHorzScroll(CListBox& wndListBox);

	// 实现

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

	time_t timeLast{ 0 };

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
