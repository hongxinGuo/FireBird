#pragma once

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
	CMFCPropertyGridCtrl m_wndPropList;

	// chinaMarket realtime group
	CMFCPropertyGridProperty* m_pChinaMarketStock{ nullptr };
	CMFCPropertyGridProperty* m_pPropNew{ nullptr };
	CMFCPropertyGridProperty* m_pPropVolume{ nullptr };

	CMFCPropertyGridProperty* m_pPropStockOrdinaryBuy{ nullptr };
	CMFCPropertyGridProperty* m_pPropStockAttackBuy{ nullptr };
	CMFCPropertyGridProperty* m_pPropStockStrongBuy{ nullptr };
	CMFCPropertyGridProperty* m_pPropStockCancelBuy{ nullptr };
	CMFCPropertyGridProperty* m_pPropStockOrdinarySell{ nullptr };
	CMFCPropertyGridProperty* m_pPropStockAttackSell{ nullptr };
	CMFCPropertyGridProperty* m_pPropStockStrongSell{ nullptr };
	CMFCPropertyGridProperty* m_pPropStockCancelSell{ nullptr };

	UINT m_uIdTimer;

	// Implementation
public:
	~CPropertyRealtimeWnd() override;

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSettingChange(UINT uFlags, LPCTSTR lpszSection);
	DECLARE_MESSAGE_MAP()

	void InitPropList();
	void SetPropListFont();

	int m_nComboHeight;

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
