#pragma once

#include"ChinaStock.h"
#include"FireBirdDoc.h"

enum {
	SHOW_DAY_LINE_DATA_ = 1,
	SHOW_WEEK_LINE_DATA_ = 3,
	SHOW_MONTH_LINE_DATA_ = 4,
};

enum Indicators {
	SHOW_INDICATOR_NONE_ = 0,
	SHOW_INDICATOR_KDJ_,
	SHOW_INDICATOR_MACD_,
	SHOW_INDICATOR_RSI_,
	SHOW_INDICATOR_BOLL_,
	SHOW_INDICATOR_RS_,
};

class CFireBirdView : public CView {
protected: // 使用protected关键字，以保证仅从序列化创建
	DECLARE_DYNCREATE(CFireBirdView)
	CFireBirdView();

	// 特性
public:
	~CFireBirdView() override = default;
	// ReSharper disable once CppHidingFunction
	CFireBirdDoc* GetDocument() const;
	CVirtualStockPtr GetCurrentStock() const { return GetDocument()->GetCurrentStock(); }

	void ShowCandleData(CDC* pDC, CRect rectDrawArea);
	void ShowCross(CDC* pDC, CPoint ptCurrent);
	void ShowIndicator(CDC* pDC, CRect rectDrawArea);
	void ShowIndicatorRS(CDC* pDC, CRect rectDrawArea);
	void ShowIndicatorKDJ(CDC* pDC, CRect rectDrawArea);
	void Show8020Line(CDC* pDC, CRect rectDrawArea);

	CRect GetClientSize() const noexcept { return m_rectClient; }
	bool IsShowRS() const noexcept { return m_fShowRS; }
	bool IsShow5Days() const noexcept { return m_fShow5Days; }
	bool IsShow10Days() const noexcept { return m_fShow10Days; }
	bool IsShow30Days() const noexcept { return m_fShow30Days; }
	bool IsShow60Days() const noexcept { return m_fShow60Days; }
	bool IsShow120Days() const noexcept { return m_fShow250Days; }
	bool IsShowRSInLogarithm() const noexcept { return (m_iShowRSOption == 2); }
	bool IsShowRSInLinear() const noexcept { return (m_iShowRSOption == 1); }
	bool IsShowRSInIndex() const noexcept { return (m_iShowRSOption == 0); }
	void SetShowRSOption(const int iValue) noexcept { m_iShowRSOption = iValue; }

	// 操作
public:
	void Show(CDC* pdc);
	virtual void ShowStockHistoryDataLine(CDC* pDC);

	int GetCurrentShowType() const noexcept { return m_iCurrentShowType; }
	void SetCurrentShowType(const int iValue) noexcept { m_iCurrentShowType = iValue; }

	int GetCandleWidth() const noexcept { return m_iCandleWidth; }
	void SetCandleWidth(const int width) noexcept { m_iCandleWidth = width; }

	//系统包裹函数
public:
	virtual CSize SysCallGetTextExtent(CDC* pDC, const string& str) { return (pDC->GetTextExtent(str.c_str())); }
	virtual BOOL SysCallTextOut(CDC* pDC, int x, int y, const string& str) { return (pDC->TextOut(x, y, str.c_str())); }
	virtual CPen* SysCallSelectObject(CDC* pDC, CPen* pPen) { return (pDC->SelectObject(pPen)); }
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

	CRect m_rectClient; // 客户区大小
	CRect m_rectInformation; // 显示股票信息的区域，文字宽度
	CRect m_rectCandle;
	CRect m_rectIndicator;
	CPoint m_ptMouse;
	CPoint m_ptMouseOld{ -1, -1 };// 记录上一次鼠标位置
	bool m_bNeedErase{ false };
	bool m_bDrawAll{ false };

	int m_iShowIndicator{ SHOW_INDICATOR_KDJ_ }; // 显示的技术指标

	long m_lDayLineHigh{ 0 };
	long m_lDayLineLow{ 0 };
	long m_lWeekLineHigh{ 0 };
	long m_lWeekLineLow{ 0 };
	long m_lMonthLineHigh{ 0 };
	long m_lMonthLineLow{ 0 };

	UINT m_uIdTimer;
	int m_iCurrentShowType; // 当前显示状态（日线、周线或实时。。。）

	int m_iCandleWidth{ 3 }; // 蜡烛宽度。

	bool m_fShowTransactionGraph; // 显示交易具体情况的图
	CRect m_rectTransactionGraph; // 交易具体情况图的位置和大小。

	CRect m_rectOrdinaryBuyVolume, m_rectOrdinarySellVolume;
	CRect m_rectAttackBuyVolume, m_rectAttackSellVolume;
	CRect m_rectStrongBuyVolume, m_rectStrongSellVolume;
	CRect m_rectCancelBuyVolume, m_rectCancelSellVolume;

	long m_lCurrentPos;

	bool m_fShowRS;
	bool m_fShow5Days;
	bool m_fShow10Days;
	bool m_fShow30Days;
	bool m_fShow60Days;
	bool m_fShow250Days;
	int m_iShowRSOption; // 显示相对相对强度的选项。1 = 线性； 2 = 对数；3 = 指数相对；
	vector<double> m_vRSShow;

	// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//	afx_msg void OnShowRs3();
	afx_msg void OnShowRs5();
	afx_msg void OnShowRs10();
	afx_msg void OnShowRs30();
	afx_msg void OnShowRs60();
	afx_msg void OnShowRs250();
	afx_msg void OnUpdateShowRs10(CCmdUI* pCmdUI);
	afx_msg void OnUpdateShowRs250(CCmdUI* pCmdUI);
	//	afx_msg void OnUpdateShowRs3(CCmdUI* pCmdUI);
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
	afx_msg void OnShowWeekLine();
	afx_msg void OnUpdateShowWeekLine(CCmdUI* pCmdUI);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnShowMonthLine();
	afx_msg void OnUpdateShowMonthLine(CCmdUI* pCmdUI);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnShowIndicatorRs();
	afx_msg void OnUpdateShowIndicatorRs(CCmdUI* pCmdUI);
	afx_msg void OnShowIndicatorKdj();
	afx_msg void OnUpdateShowIndicatorKdj(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // 调试版本在FireBirdView.cpp中
inline CFireBirdDoc* CFireBirdView::GetDocument() const {
	return reinterpret_cast<CFireBirdDoc*>(m_pDocument);
}
#endif
