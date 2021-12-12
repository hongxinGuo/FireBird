#include "pch.h"

#include"globedef.h"
#include"WorldMarket.h"
#include"WorldStock.h"

#include "FinnhubCompanyPeer.h"

CFinnhubCompanyPeer::CFinnhubCompanyPeer() {
	m_strClassName = _T("Finnhub company peer");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/stock/peers?symbol=");
	m_lIndex = -1;
}

CString CFinnhubCompanyPeer::CreatMessage(void) {
	CWorldStockPtr pStock = gl_pWorldMarket->GetStock(m_lIndex);
	CString strMessage = m_strInquiringStr + pStock->GetSymbol();
	pStock->SetPeerUpdated(true);

	return strMessage;
}

bool CFinnhubCompanyPeer::ProcessWebData(CWebDataPtr pWebData) {
	CWorldStockPtr pStock = gl_pWorldMarket->GetStock(m_lIndex);
	if (gl_pWorldMarket->ParseFinnhubStockPeer(pWebData, pStock)) {
		pStock->SetPeerUpdateDate(gl_pWorldMarket->GetMarketDate());
		pStock->SetUpdateProfileDB(true);
	}

	return true;
}