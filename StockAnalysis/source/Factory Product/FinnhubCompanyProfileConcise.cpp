#include "pch.h"

#include"globedef.h"
#include"worldMarket.h"

#include "FinnhubCompanyProfileConcise.h"

IMPLEMENT_DYNCREATE(CFinnhubCompanyProfileConcise, CWebSourceDataProduct)

CFinnhubCompanyProfileConcise::CFinnhubCompanyProfileConcise() {
	m_strClassName = _T("Finnhub company profile concise");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/stock/profile2?symbol=");
	m_lIndex = -1;
}

CString CFinnhubCompanyProfileConcise::CreatMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CString strMessage;
	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	strMessage = m_strInquiringStr + pStock->GetSymbol();
	pStock->SetProfileUpdated(true);

	return strMessage;
}

bool CFinnhubCompanyProfileConcise::ProcessWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	if (((CWorldMarket*)m_pMarket)->ParseFinnhubStockProfileConcise(pWebData, pStock)) {
		pStock->SetProfileUpdateDate(((CWorldMarket*)m_pMarket)->GetMarketDate());
		pStock->SetProfileUpdated(true);
		pStock->SetUpdateProfileDB(true);
	}
	return true;
}