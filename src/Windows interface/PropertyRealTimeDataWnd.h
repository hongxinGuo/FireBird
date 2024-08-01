#pragma once

class CPropertiesToolBar2 : public CMFCToolBar {
public:
	void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler) override { CMFCToolBar::OnUpdateCmdUI(static_cast<CFrameWnd*>(GetOwner()), bDisableIfNoHndler); }
	BOOL AllowShowOnList() const override { return FALSE; }
};

class CFireBirdPropertyRealtimeGridCtrl : public CMFCPropertyGridCtrl {
	void OnPropertyChanged(CMFCPropertyGridProperty* pProp) const override;
};

class CPropertyRealtimeWnd : public CDockablePane {
	// Construction
public:
	CPropertyRealtimeWnd() noexcept;
	void AdjustLayout() override;

	// Attributes
public:
	void SetVSDotNetLook(BOOL bSet) {
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

protected:
	CFont m_fntPropList;
	CPropertiesToolBar2 m_wndToolBar;
	CFireBirdPropertyRealtimeGridCtrl m_wndPropList;

	// system status group
	CMFCPropertyGridProperty* m_pPropSystemOption;

	// chinaMarket realtime group

	UINT m_uIdTimer;

	// Implementation
public:
	~CPropertyRealtimeWnd() override;

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

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
