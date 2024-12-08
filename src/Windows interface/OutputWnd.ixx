#include"pch.h"
module;
export module FireBird.WindowInterface.OutputWnd;

export {
	class COutputList : public CListBox {
		// 构造
	public:
		COutputList();

		// 实现
		void TruncateList(long lNumberOfTruncation = 1000);
		void SetCurAtLastLine();

		int GetLineNumber() const noexcept { return m_iLineNumber; }

	public:
		~COutputList() override;

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
	public:
		~COutputWnd() override;

	protected:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnSize(UINT nType, int cx, int cy);

		DECLARE_MESSAGE_MAP()

		time_t timeLast{ 0 };

	public:
		afx_msg void OnTimer(UINT_PTR nIDEvent);
	};
}
