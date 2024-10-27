#pragma once

enum configuration_option {
	SYSTEM_LOG_LEVEL_ = 1,
	SYSTEM_FAST_CPU,
	SYSTEM_DEBUG_MODE_,
	SYSTEM_RELOAD_SYSTEM_,
};

class CPropertiesToolBar : public CMFCToolBar {
public:
	void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler) override { CMFCToolBar::OnUpdateCmdUI(static_cast<CFrameWnd*>(GetOwner()), bDisableIfNoHndler); }
	BOOL AllowShowOnList() const override { return FALSE; }
};

class CFireBirdPropertyGridCtrl : public CMFCPropertyGridCtrl {
	void OnPropertyChanged(CMFCPropertyGridProperty* pProp) const override;
};

class CFireBirdComboBox : public CComboBox {
public:
	CFireBirdComboBox() {
	}
	~CFireBirdComboBox() override {
	}

	afx_msg void OnCbnSelChange();
	DECLARE_MESSAGE_MAP()
};

class CPropertiesWnd : public CDockablePane {
	// Construction
public:
	CPropertiesWnd() noexcept;
	void AdjustLayout() override;

	// Attributes
public:
	void SetVSDotNetLook(BOOL bSet) {
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

	UINT m_uIdTimer{ 0 };

	// Implementation
public:
	~CPropertiesWnd() override;

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

protected:
	CFont m_fntPropList;
	CFireBirdComboBox m_wndObjectCombo;
	CPropertiesToolBar m_wndToolBar;
	CFireBirdPropertyGridCtrl m_wndPropList;

	// system status group
	CMFCPropertyGridProperty* m_pPropSystemOption{ nullptr };
	CMFCPropertyGridColorProperty* m_pPropChinaMarketWebStatus{ nullptr };
	CMFCPropertyGridProperty* m_pPropWorldMarketWebStatus{ nullptr };

	// finnhub.io
	CMFCPropertyGridProperty* m_pPropFinnhubCurrentFunction{ nullptr };

	// Tiingo.com
	CMFCPropertyGridProperty* m_pPropTiingoCurrentFunction{ nullptr };

	// web socket group
	CMFCPropertyGridProperty* m_pPropFinnhubWebSocket{ nullptr };
	CMFCPropertyGridProperty* m_pPropTiingoIEXWebSocket{ nullptr };
	CMFCPropertyGridProperty* m_pPropTiingoForexWebSocket{ nullptr };
	CMFCPropertyGridProperty* m_pPropTiingoCryptoWebSocket{ nullptr };

	// chinaMarket realtime group

	int m_nComboHeight{ 0 };

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
