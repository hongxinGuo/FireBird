module;
#include"pch.h"
export module Watchdog.View;

import Watchdog.Document;
export import std;

export {
	class CWatchdogView : public CView {
	protected: // create from serialization only
		CWatchdogView() noexcept;
		DECLARE_DYNCREATE(CWatchdogView)

		// Attributes
	public:
		CWatchdogDoc* GetDocument() const;

		// Operations
	public:
		// Overrides
	public:
		void OnDraw(CDC* pDC) override;  // overridden to draw this view
		BOOL PreCreateWindow(CREATESTRUCT& cs) override;

	protected:
		BOOL OnPreparePrinting(CPrintInfo* pInfo) override;
		void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) override;
		void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) override;

		// Implementation
	public:
		~CWatchdogView() override;
#ifdef _DEBUG
		void AssertValid() const override;
		void Dump(CDumpContext& dc) const override;
#endif

		// Generated message map functions
	protected:
		afx_msg void OnFilePrintPreview();
		afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
		DECLARE_MESSAGE_MAP()
	};

#ifndef _DEBUG  // debug version in WatchdogView.cpp
	inline CWatchdogDoc* CWatchdogView::GetDocument() const
	{
		return reinterpret_cast<CWatchdogDoc*>(m_pDocument);
	}
#endif
}