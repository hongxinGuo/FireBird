#include "pch.h"

#include"globedef.h"
#include"Country.h"

#include "FinnhubEconomicCountryList.h"

IMPLEMENT_DYNCREATE(CFinnhubEconomicCountryList, CWebSourceDataProduct)

CFinnhubEconomicCountryList::CFinnhubEconomicCountryList() {
	m_strClassName = _T("Finnhub economic country list");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/country?");
	m_lIndex = -1;
}

CString CFinnhubEconomicCountryList::CreatMessage(void) {
	return m_strInquiringStr;
}

bool CFinnhubEconomicCountryList::ProcessWebData(CWebDataPtr pWebData) {
	vector<CCountryPtr> vCountry;

	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	((CWorldMarket*)m_pMarket)->ParseFinnhubCountryList(pWebData, vCountry);
	for (auto& pCountry : vCountry) {
		if (!((CWorldMarket*)m_pMarket)->IsCountry(pCountry)) {
			((CWorldMarket*)m_pMarket)->AddCountry(pCountry);
		}
	}
	((CWorldMarket*)m_pMarket)->SetCountryListUpdated(true);

	return true;
}