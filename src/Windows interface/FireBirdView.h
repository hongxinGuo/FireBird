﻿#pragma once

#include"ChinaStock.h"
#include"FireBirdDoc.h"

enum {
	_SHOW_DAY_LINE_DATA_ = 1,
	_SHOW_REAL_TIME_DATA_ = 2,
	_SHOW_WEEK_LINE_DATA_ = 3,
};

class CFireBirdView : public CView {
protected: // 使用protected关键字，以保证仅从序列化创建
	DECLARE_DYNCREATE(CFireBirdView)
	CFireBirdView();

	// 特性
public:
	~CFireBirdView() override = default;
	CFireBirdDoc* GetDocument() const;

	CRect GetClientSize() const noexcept { return m_rectClient; }
	bool IsShowRS() const noexcept { return m_fShowRS; }
	bool IsShow3DaysRS() const noexcept { return m_fShow3DaysRS; }
	bool IsShow5DaysRS() const noexcept { return m_fShow5DaysRS; }
	bool IsShow10DaysRS() const noexcept { return m_fShow10DaysRS; }
	bool IsShow30DaysRS() const noexcept { return m_fShow30DaysRS; }
	bool IsShow60DaysRS() const noexcept { return m_fShow60DaysRS; }
	bool IsShow120DaysRS() const noexcept { return m_fShow120DaysRS; }
	bool IsShowRSInLogarithm() const noexcept { return (m_iShowRSOption == 2); }
	bool IsShowRSInLinear() const noexcept { return (m_iShowRSOption == 1); }
	bool IsShowRSInIndex() const noexcept { return (m_iShowRSOption == 0); }
	void SetShowRSOption(const int iValue) noexcept { m_iShowRSOption = iValue; }

	// 操作
public:
	void Show(CDC* pdc);
	virtual void ShowStockHistoryDataLine(CDC* pDC);
	virtual void ShowRealtimeData(CDC* pDC);

	bool ShowGuadan(CDC* pDC, const CChinaStockPtr& pStock, int iXStart, int iYStart, int iYEnd);
	bool ShowCurrentTransactionInfo(CDC* pDC, CChinaStockPtr pStock, int iXStart, int iYStart);
	void ShowRealtimeGuadan(CDC* pdc);
	void ShowBuySell(CDC* pDC, CChinaStockPtr pStock, CRect rectArea);
	void ShowOrdinaryBuySell(CDC* pDC, const CChinaStockPtr& pStock, CRect rectArea);
	void ShowAttackBuySell(CDC* pDC, const CChinaStockPtr& pStock, CRect rectArea);
	void ShowCanceledBuySell(CDC* pDC, const CChinaStockPtr& pStock, CRect rectArea);
	void ShowCurrentRS(CDC* pDC, vector<double>& vRS);
	bool RSLineTo(CDC* pDC, int i, double dValue, int iSize);

	void SetClientSize(CRect rect) noexcept { m_rectClient = rect; }

	int GetCurrentShowType() const noexcept { return m_iCurrentShowType; }
	void SetCurrentShowType(const int iValue) noexcept { m_iCurrentShowType = iValue; }

	void ZoomIn(vector<double>& vData, double dLevel, double dRate);

	//系统包裹函数
public:
	virtual CSize SysCallGetTextExtent(CDC* pDC, CString str) { return (pDC->GetTextExtent(str)); }
	virtual BOOL SysCallTextOut(CDC* pDC, int x, int y, CString str) { return (pDC->TextOut(x, y, str)); }
	virtual CPen* SysCallSelectObject(CDC* pDC, CPen* ppen) { return (pDC->SelectObject(ppen)); }
	virtual CPoint SysCallMoveTo(CDC* pDC, int x, int y) { return (pDC->MoveTo(x, y)); }
	virtual CPoint SysCallMoveTo(CDC* pDC, CPoint pt) { return (pDC->MoveTo(pt)); }
	virtual BOOL SysCallLineTo(CDC* pDC, int x, int y) { return (pDC->LineTo(x, y)); }
	virtual BOOL SysCallLineTo(CDC* pDC, CPoint pt) { return (pDC->LineTo(pt)); }
	virtual void SysCallFillSolidRect(CDC* pDC, LPRECT rect, COLORREF ref) { pDC->FillSolidRect(rect, ref); }
	virtual void SysCallOnSize(UINT nType, int cx, int cy) { CView::OnSize(nType, cx, cy); }
	virtual void SysCallCmdUISetCheck(CCmdUI* pCmdUI, int iCheck) { pCmdUI->SetCheck(iCheck); }
	virtual void SysCallCmdUIEnable(CCmdUI* pCmdUI, bool fEnable) { pCmdUI->Enable(fEnable); }
	virtual void SysCallGetClientRect(LPRECT lpRect) { GetClientRect(lpRect); }
	virtual BOOL SysCallBitBlt(CDC* pdc, int x, int y, int nWidth, int nHeight, CDC* pSrcDC, int xSrc, int ySrc, DWORD dwRop) { return (pdc->BitBlt(x, y, nWidth, nHeight, pSrcDC, xSrc, ySrc, dwRop)); }

	// 重写
public:
	void OnDraw(CDC* pDC) override; // 重写以绘制该视图
	BOOL PreCreateWindow(CREATESTRUCT& cs) override;

	// 实现

protected:
	BOOL OnPreparePrinting(CPrintInfo* pInfo) override;
	void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) override;
	void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) override;

	bool m_fCreateMemoryDC;
	CDC m_MemoryDC;
	CBitmap m_Bitmap;

	CRect m_rectClient;

	UINT m_uIdTimer;
	int m_iCurrentShowType; // 当前显示状态（日线、周线或实时。。。）

	bool m_fShowTransactionGraph; // 显示交易具体情况的图
	CRect m_rectTransactionGraph; // 交易具体情况图的位置和大小。

	long m_lCurrentPos;

	bool m_fShowRS;
	bool m_fShow3DaysRS;
	bool m_fShow5DaysRS;
	bool m_fShow10DaysRS;
	bool m_fShow30DaysRS;
	bool m_fShow60DaysRS;
	bool m_fShow120DaysRS;
	int m_iShowRSOption; // 显示相对相对强度的选项。1 = 线性； 2 = 对数；3 = 指数相对；
	vector<double> m_vRSShow;

	// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowRs3();
	afx_msg void OnShowRs5();
	afx_msg void OnShowRs10();
	afx_msg void OnShowRs30();
	afx_msg void OnShowRs60();
	afx_msg void OnShowRs120();
	afx_msg void OnUpdateShowRs10(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShowRs120(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShowRs3(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShowRs30(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShowRs5(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShowRs60(CCmdUI* pCmdUI);
	afx_msg void OnShowRsInLogarithm();
	afx_msg void OnUpdateShowRsInLogarithm(CCmdUI* pCmdUI);
	afx_msg void OnShowRsInLinear();
	afx_msg void OnUpdateShowRsInLinear(CCmdUI* pCmdUI);
	afx_msg void OnShowRsIndex();
	afx_msg void OnUpdateShowRsIndex(CCmdUI* pCmdUI);
	afx_msg void OnShowDayLine();
	afx_msg void OnUpdateShowDayLine(CCmdUI* pCmdUI);
	afx_msg void OnShowRealTime();
	afx_msg void OnUpdateShowRealTime(CCmdUI* pCmdUI);
	afx_msg void OnShowWeekLine();
	afx_msg void OnUpdateShowWeekLine(CCmdUI* pCmdUI);
};

#ifndef _DEBUG  // FireBirdView.cpp 中的调试版本
inline CFireBirdDoc* CFireBirdView::GetDocument() const {
	return reinterpret_cast<CFireBirdDoc*>(m_pDocument);
}
#endif
