
// StockAnalysisView.h: CStockAnalysisView 类的接口
//

#pragma once

#include"Stock.h"

class CStockAnalysisView : public CView
{
protected: // 仅从序列化创建
	CStockAnalysisView() noexcept;
	DECLARE_DYNCREATE(CStockAnalysisView)

// 特性
public:
	CStockAnalysisDoc* GetDocument() const;

// 操作
public:
  bool ShowGuaDan(CDC * pDC, CStockPtr pStock, int iXStart, int iYStart, int iYEnd);
  bool ShowCurrentTransactionInfo(CDC * pDC, CStockPtr pStock, int iXStart, int iYStart);

  void    ShowRealtimeStockData(CDC * pdc);
  void    ShowStockDayLine(CDC * pDC);

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CStockAnalysisView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
protected:
  UINT      m_uIdTimer;
  int       m_iCurrentShowType;// 当前显示状态（日线或实时。。。）

  CRect			m_rectClient;

  long      m_lCurrentPos;

  bool      m_fShowRS;
  bool      m_fShow3DayRS;
  bool      m_fShow5DayRS;
  bool      m_fShow10RS;
  bool      m_fShow30DayRS;
  bool      m_fShow60DayRS;
  bool      m_fShow120DayRS;

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
};

#ifndef _DEBUG  // StockAnalysisView.cpp 中的调试版本
inline CStockAnalysisDoc* CStockAnalysisView::GetDocument() const
   { return reinterpret_cast<CStockAnalysisDoc*>(m_pDocument); }
#endif

