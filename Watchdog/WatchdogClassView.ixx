#include"pch.h"
module;
export module Watchdog.ClassView;
import Watchdog.ViewTree;

export {
	class CClassToolBar : public CMFCToolBar {
		void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler) override {
			CMFCToolBar::OnUpdateCmdUI(static_cast<CFrameWnd*>(GetOwner()), bDisableIfNoHndler);
		}

		BOOL AllowShowOnList() const override { return FALSE; }
	};

	class CWatchdogClassView : public CDockablePane {
	public:
		CWatchdogClassView() noexcept;
		~CWatchdogClassView() override;

		void AdjustLayout() override;
		void OnChangeVisualStyle();

	protected:
		CClassToolBar m_wndToolBar;
		CViewTree m_wndClassView;
		CImageList m_ClassViewImages;
		UINT m_nCurrSort;

		void FillClassView();

		// Overrides
	public:
		BOOL PreTranslateMessage(MSG* pMsg) override;

	protected:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
		afx_msg void OnClassAddMemberFunction();
		afx_msg void OnClassAddMemberVariable();
		afx_msg void OnClassDefinition();
		afx_msg void OnClassProperties();
		afx_msg void OnNewFolder();
		afx_msg void OnPaint();
		afx_msg void OnSetFocus(CWnd* pOldWnd);
		afx_msg LRESULT OnChangeActiveTab(WPARAM, LPARAM);
		afx_msg void OnSort(UINT id);
		afx_msg void OnUpdateSort(CCmdUI* pCmdUI);

		DECLARE_MESSAGE_MAP()
	};
}
