// ChildFrm.h : interface of the CWatchdogChildFrame class
//

#include"pch.h"
module;
export module Watchdog.ChildFrm;
export import std;

export {
	class CWatchdogChildFrame : public CMDIChildWndEx {
		DECLARE_DYNCREATE(CWatchdogChildFrame)

	public:
		CWatchdogChildFrame() noexcept;

		// Attributes
	protected:
		CSplitterWndEx m_wndSplitter;

		// Operations
	public:
		// Overrides
		BOOL PreCreateWindow(CREATESTRUCT& cs) override;

		// Implementation
	public:
		~CWatchdogChildFrame() override;
#ifdef _DEBUG
		void AssertValid() const override;
		void Dump(CDumpContext& dc) const override;
#endif

		// Generated message map functions
	protected:
		DECLARE_MESSAGE_MAP()
	};
}
