#include"pch.h"
module;
export module Watchdog.PropertiesWnd;
export import std;

export {
	class CPropertiesToolBar : public CMFCToolBar {
	public:
		void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler) override {
			CMFCToolBar::OnUpdateCmdUI(static_cast<CFrameWnd*>(GetOwner()), bDisableIfNoHndler);
		}

		BOOL AllowShowOnList() const override { return FALSE; }
	};

	class CWatchdogPropertiesWnd : public CDockablePane {
		// Construction
	public:
		CWatchdogPropertiesWnd() noexcept;

		void AdjustLayout() override;

		// Attributes
	public:
		void SetVSDotNetLook(BOOL bSet) {
			m_wndPropList.SetVSDotNetLook(bSet);
			m_wndPropList.SetGroupNameFullWidth(bSet);
		}

	protected:
		CFont m_fntPropList;
		CComboBox m_wndObjectCombo;
		CPropertiesToolBar m_wndToolBar;
		CMFCPropertyGridCtrl m_wndPropList;

		// Implementation
	public:
		~CWatchdogPropertiesWnd() override;

	protected:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg void OnExpandAllProperties();
		afx_msg void OnUpdateExpandAllProperties(CCmdUI* pCmdUI);
		afx_msg void OnSortProperties();
		afx_msg void OnUpdateSortProperties(CCmdUI* pCmdUI);
		afx_msg void OnProperties1();
		afx_msg void OnUpdateProperties1(CCmdUI* pCmdUI);
		afx_msg void OnProperties2();
		afx_msg void OnUpdateProperties2(CCmdUI* pCmdUI);
		afx_msg void OnSetFocus(CWnd* pOldWnd);
		afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);

		DECLARE_MESSAGE_MAP()

		void InitPropList();
		void SetPropListFont();

		int m_nComboHeight;
	};
}
