#pragma once

#include"GridProperty.h"

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
	CGridProperty* m_pPropertyStock{ nullptr };
	CGridProperty* m_pPropNew{ nullptr };
	CGridProperty* m_pPropVolume{ nullptr };

	CGridProperty* m_pPropStockOrdinaryBuy{ nullptr };
	CGridProperty* m_pPropStockAttackBuy{ nullptr };
	CGridProperty* m_pPropStockStrongBuy{ nullptr };
	CGridProperty* m_pPropStockCancelBuy{ nullptr };
	CGridProperty* m_pPropStockOrdinarySell{ nullptr };
	CGridProperty* m_pPropStockAttackSell{ nullptr };
	CGridProperty* m_pPropStockStrongSell{ nullptr };
	CGridProperty* m_pPropStockCancelSell{ nullptr };

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
