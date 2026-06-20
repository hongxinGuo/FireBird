#pragma once

#include"GridProperty.h"

enum configuration_option {
	SYSTEM_LOG_LEVEL_ = 1,
	SYSTEM_FAST_CPU,
	SYSTEM_DEBUG_MODE_,
	SYSTEM_RELOAD_SYSTEM_,
	CHINA_MARKET_REALTIME_DATA_SOURCE_ENABLE_,
	FINNHUB_DATA_SOURCE_ENABLE_,
	TIINGO_DATA_SOURCE_ENABLE_
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
	CFireBirdComboBox() = default;
	CFireBirdComboBox(const CFireBirdComboBox&) = delete;
	CFireBirdComboBox& operator=(const CFireBirdComboBox&) = delete;
	CFireBirdComboBox(const CFireBirdComboBox&&) = delete;
	CFireBirdComboBox& operator=(const CFireBirdComboBox&&) = delete;
	~CFireBirdComboBox() override = default;

protected:
	afx_msg void OnCbnSelChange();
	DECLARE_MESSAGE_MAP()
};

class CPropertiesWnd : public CDockablePane {
	// Construction
public:
	CPropertiesWnd() noexcept;
	CPropertiesWnd(const CPropertiesWnd&) = delete;
	CPropertiesWnd& operator=(const CPropertiesWnd&) = delete;
	CPropertiesWnd(const CPropertiesWnd&&) = delete;
	CPropertiesWnd& operator=(const CPropertiesWnd&&) = delete;
	~CPropertiesWnd() override;

	void AdjustLayout() override;

	// Attributes
public:
	void SetVSDotNetLook(BOOL bSet) {
		m_wndPropList.SetVSDotNetLook(bSet);
		m_wndPropList.SetGroupNameFullWidth(bSet);
	}

	UINT m_uIdTimer{ 0 };

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
	CGridProperty* m_pPropRealTimeDataReceived{ nullptr };
	CGridProperty* m_pPropChinaMarketRealTimeDataNumber{ nullptr };
	CGridProperty* m_pPropCurrentWorkingThread{ nullptr };

	CGridProperty* m_pPropSystemOption{ nullptr };

	// china market
	CGridProperty* m_pPropChinaMarketWebRealTimeStatus{ nullptr };
	CGridProperty* m_pPropChinaMarketWebDayLineStatus{ nullptr };
	CGridProperty* m_pPropChinaMarketThreadStatus{ nullptr };

	// world market
	CGridProperty* m_pPropFinnhubDataSourceWebStatus{ nullptr };

	// finnhub.io
	CGridProperty* m_pPropFinnhubCurrentFunction{ nullptr };

	// Tiingo.com
	CGridProperty* m_pPropTiingoDataSourceWebStatus{ nullptr };
	CGridProperty* m_pPropTiingoCurrentFunction{ nullptr };

	// web socket group
	CGridProperty* m_pPropFinnhubWebSocket{ nullptr };
	CGridProperty* m_pPropTiingoIEXWebSocket{ nullptr };
	CGridProperty* m_pPropTiingoForexWebSocket{ nullptr };
	CGridProperty* m_pPropTiingoCryptoWebSocket{ nullptr };

	// chinaMarket realtime group

	int m_nComboHeight{ 0 };

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
