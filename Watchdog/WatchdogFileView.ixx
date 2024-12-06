#include"pch.h"
module;
export module Watchdog.FileView;
import Watchdog.ViewTree;

export {
	class CFileViewToolBar : public CMFCToolBar {
		void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler) override {
			CMFCToolBar::OnUpdateCmdUI(static_cast<CFrameWnd*>(GetOwner()), bDisableIfNoHndler);
		}

		BOOL AllowShowOnList() const override { return FALSE; }
	};

	class CWatchdogFileView : public CDockablePane {
		// Construction
	public:
		CWatchdogFileView() noexcept;

		void AdjustLayout() override;
		void OnChangeVisualStyle();

		// Attributes
	protected:
		CViewTree m_wndFileView;
		CImageList m_FileViewImages;
		CFileViewToolBar m_wndToolBar;

	protected:
		void FillFileView();

		// Implementation
	public:
		~CWatchdogFileView() override;

	protected:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
		afx_msg void OnProperties();
		afx_msg void OnFileOpen();
		afx_msg void OnFileOpenWith();
		afx_msg void OnDummyCompile();
		afx_msg void OnEditCut();
		afx_msg void OnEditCopy();
		afx_msg void OnEditClear();
		afx_msg void OnPaint();
		afx_msg void OnSetFocus(CWnd* pOldWnd);

		DECLARE_MESSAGE_MAP()
	};
}
