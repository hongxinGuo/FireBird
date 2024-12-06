#include"pch.h"
module;
export module Watchdog.OutputWnd;

export {
	class COutputList : public CListBox {
		// Construction
	public:
		COutputList() noexcept;

		// Implementation
	public:
		~COutputList() override;

		void TruncateList(long lNumberOfTruncation);
		void SetCurAtLastLine();

	protected:
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
		afx_msg void OnEditCopy();
		afx_msg void OnEditClear();
		afx_msg void OnViewOutput();

		DECLARE_MESSAGE_MAP()
	};

	class CWatchdogOutputWnd : public CDockablePane {
		// Construction
	public:
		CWatchdogOutputWnd() noexcept;

		void UpdateFonts();

		// Attributes
	protected:
		CMFCTabCtrl m_wndTabs;

		COutputList m_wndOutputBuild;
		COutputList m_wndOutputDebug;
		COutputList m_wndOutputFind;

		UINT_PTR m_uIdTimer;

	protected:
		void FillDebugWindow();
		void FillFindWindow();

		void AdjustHorzScroll(CListBox& wndListBox);

		// Implementation
	public:
		~CWatchdogOutputWnd() override;

		void ReportInfo(const CString& strInfo);

	protected:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnSize(UINT nType, int cx, int cy);

		DECLARE_MESSAGE_MAP()

	public:
		afx_msg void OnTimer(UINT_PTR nIDEvent);
	};
}
