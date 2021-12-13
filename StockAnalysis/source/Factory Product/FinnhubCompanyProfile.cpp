#include "pch.h"

#include"globedef.h"
#include"worldMarket.h"

#include "FinnhubCompanyProfile.h"

IMPLEMENT_DYNCREATE(CFinnhubCompanyProfile, CWebSourceDataProduct)

CFinnhubCompanyProfile::CFinnhubCompanyProfile() {
	m_strClassName = _T("Finnhub company profile");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/stock/profile?symbol=");
	m_lIndex = -1;
}

CString CFinnhubCompanyProfile::CreatMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CString strMessage;
	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	strMessage = m_strInquiringStr + pStock->GetSymbol();
	pStock->SetProfileUpdated(true);

	return strMessage;
}

bool CFinnhubCompanyProfile::ProcessWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	if (((CWorldMarket*)m_pMarket)->ParseFinnhubStockProfile(pWebData, pStock)) {
		pStock->SetProfileUpdateDate(((CWorldMarket*)m_pMarket)->GetMarketDate());
		pStock->SetProfileUpdated(true);
		pStock->SetUpdateProfileDB(true);
	}
	return true;
}