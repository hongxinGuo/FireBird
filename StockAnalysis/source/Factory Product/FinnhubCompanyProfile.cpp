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

CString CFinnhubCompanyProfile::CreatMessage(void)
{
	CString strMessage;
	CWorldStockPtr pStock = gl_pWorldMarket->GetStock(m_lIndex);
	strMessage = m_strInquiringStr + pStock->GetSymbol();
	pStock->SetProfileUpdated(true);

	return strMessage;
}

bool CFinnhubCompanyProfile::ProcessWebData(CWebDataPtr pWebData)
{
	CWorldStockPtr pStock = gl_pWorldMarket->GetStock(m_lIndex);
	if (gl_pWorldMarket->ParseFinnhubStockProfile(pWebData, pStock)) {
		pStock->SetProfileUpdateDate(gl_pWorldMarket->GetMarketDate());
		pStock->SetProfileUpdated(true);
		pStock->SetUpdateProfileDB(true);
	}
	return true;
}