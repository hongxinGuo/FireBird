#include "pch.h"

#include"globedef.h"
#include"WorldMarket.h"
#include"WorldStock.h"

#include "FinnhubCompanyPeer.h"

IMPLEMENT_DYNCREATE(CFinnhubCompanyPeer, CWebSourceDataProduct)

CFinnhubCompanyPeer::CFinnhubCompanyPeer() {
	m_strClassName = _T("Finnhub company peer");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/stock/peers?symbol=");
	m_lIndex = -1;
}

CString CFinnhubCompanyPeer::CreatMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	CString strMessage = m_strInquiringStr + pStock->GetSymbol();
	pStock->SetPeerUpdated(true);

	return strMessage;
}

bool CFinnhubCompanyPeer::ProcessWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	if (((CWorldMarket*)m_pMarket)->ParseFinnhubStockPeer(pWebData, pStock)) {
		pStock->SetPeerUpdateDate(((CWorldMarket*)m_pMarket)->GetMarketDate());
		pStock->SetUpdateProfileDB(true);
	}

	return true;
}