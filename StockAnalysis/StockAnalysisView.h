// StockAnalysisView.h: CStockAnalysisView 类的接口
//

#pragma once

#include"ChinaStock.h"

class CStockAnalysisView : public CView
{
protected: // 仅从序列化创建
  CStockAnalysisView();
  DECLARE_DYNCREATE(CStockAnalysisView)

  // 特性
public:
  CStockAnalysisDoc* GetDocument() const;

  // 操作
public:
  bool ShowGuadan(CDC* pDC, CChinaStockPtr pStock, int iXStart, int iYStart, int iYEnd);
  bool ShowCurrentTransactionInfo(CDC* pDC, CChinaStockPtr pStock, int iXStart, int iYStart);

  void    ShowRealtimeStockData(CDC* pdc);
  void    ShowStockDayLine(CDC* pDC);

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
  void      Show(CDC* pdc);

protected:
  UINT      m_uIdTimer;
  int       m_iCurrentShowType;// 当前显示状态（日线或实时。。。）

  BOOL			m_fCreateMemoryDC;
  CDC				m_MemoryDC;
  CBitmap		m_Bitmap;

  CRect			m_rectClient;

  long      m_lCurrentPos;

  bool      m_fShowRS;
  bool      m_fShow3DayRS;
  bool      m_fShow5DayRS;
  bool      m_fShow10DayRS;
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
};

#ifndef _DEBUG  // StockAnalysisView.cpp 中的调试版本
inline CStockAnalysisDoc* CStockAnalysisView::GetDocument() const {
  return reinterpret_cast<CStockAnalysisDoc*>(m_pDocument);
}
#endif
