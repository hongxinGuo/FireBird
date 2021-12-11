#include "pch.h"

#include"globedef.h"
#include"worldMarket.h"

#include "FinnhubCompanyProfileConcise.h"

CFinnhubCompanyProfileConcise::CFinnhubCompanyProfileConcise() {
	m_strClassName = _T("Finnhub company profile concise");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/stock/profile2?symbol=");
	m_lIndex = -1;
}

CString CFinnhubCompanyProfileConcise::CreatMessage(void) {
	CString strMessage;
	CWorldStockPtr pStock = gl_pWorldMarket->GetStock(m_lIndex);
	strMessage = m_strInquiringStr + pStock->GetSymbol();
	pStock->SetProfileUpdated(true);

	return strMessage;
}

bool CFinnhubCompanyProfileConcise::ProcessWebData(CWebDataPtr pWebData) {
	CWorldStockPtr pStock = gl_pWorldMarket->GetStock(m_lIndex);
	if (gl_pWorldMarket->ParseFinnhubStockProfileConcise(pWebData, pStock)) {
		pStock->SetProfileUpdateDate(gl_pWorldMarket->GetMarketDate());
		pStock->SetProfileUpdated(true);
		pStock->SetUpdateProfileDB(true);
	}
	return true;
}