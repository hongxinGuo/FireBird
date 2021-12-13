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

	gl_pWorldMarket->ParseFinnhubCountryList(pWebData, vCountry);
	for (auto& pCountry : vCountry) {
		if (!gl_pWorldMarket->IsCountry(pCountry)) {
			gl_pWorldMarket->AddCountry(pCountry);
		}
	}
	gl_pWorldMarket->SetCountryListUpdated(true);

	return true;
}