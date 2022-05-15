#include "pch.h"

#include"globedef.h"
#include"accessory.h"
#include"WorldMarket.h"
#include"Country.h"
#include"CallableFunction.h"

#include "ProductFinnhubEconomicCountryList.h"

IMPLEMENT_DYNCREATE(CProductFinnhubEconomicCountryList, CProductWebSourceData)

CProductFinnhubEconomicCountryList::CProductFinnhubEconomicCountryList() {
	m_strClassName = _T("Finnhub economic country list");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/country?");
	m_lIndex = -1;
}

CString CProductFinnhubEconomicCountryList::CreatMessage(void) {
	return m_strInquiringStr;
}

bool CProductFinnhubEconomicCountryList::ProcessWebData(CWebDataPtr pWebData) {
	CCountryVectorPtr pvCountry;

	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	pvCountry = ParseFinnhubCountryList(pWebData);
	for (auto& pCountry : *pvCountry) {
		if (!((CWorldMarket*)m_pMarket)->IsCountry(pCountry)) {
			((CWorldMarket*)m_pMarket)->AddCountry(pCountry);
		}
	}
	((CWorldMarket*)m_pMarket)->SetCountryListUpdated(true);

	return true;
}

CCountryVectorPtr CProductFinnhubEconomicCountryList::ParseFinnhubCountryList(CWebDataPtr pWebData) {
	CCountryVectorPtr pvCountry = make_shared<vector<CCountryPtr>>();
	CCountryPtr pCountry = nullptr;
	ptree pt2;
	string s;
	shared_ptr<ptree> ppt;

	ASSERT(pWebData->IsJSonContentType());
	if (pWebData->IsSucceedCreatePTree()) {
		if (pWebData->IsVoidJSon()) return pvCountry;
		ppt = pWebData->GetPTree();
		try {
			for (ptree::iterator it = ppt->begin(); it != ppt->end(); ++it) {
				pCountry = make_shared<CCountry>();
				pt2 = it->second;
				s = pt2.get<string>(_T("code2"));
				if (s.size() > 0) pCountry->m_strCode2 = s.c_str();
				s = pt2.get<string>(_T("code3"));
				pCountry->m_strCode3 = s.c_str();
				s = pt2.get<string>(_T("codeNo"));
				pCountry->m_strCodeNo = s.c_str();
				s = pt2.get<string>(_T("country"));
				pCountry->m_strCountry = s.c_str();
				s = pt2.get<string>(_T("currency"));
				pCountry->m_strCurrency = s.c_str();
				s = pt2.get<string>(_T("currencyCode"));
				pCountry->m_strCurrencyCode = s.c_str();
				pvCountry->push_back(pCountry);
			}
		}
		catch (ptree_error& e) {
			ReportJSonErrorToSystemMessage(_T("Finnhub Country List "), e);
			return pvCountry;
		}
		sort(pvCountry->begin(), pvCountry->end(), CompareCountryList);
	}
	return pvCountry;
}