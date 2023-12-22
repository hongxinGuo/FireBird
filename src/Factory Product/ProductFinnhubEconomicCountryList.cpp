#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

#include"WorldMarket.h"
#include"Country.h"

#include "ProductFinnhubEconomicCountryList.h"

#include "FinnhubDataSource.h"

using namespace std;

CProductFinnhubEconomicCountryList::CProductFinnhubEconomicCountryList() {
	m_strClassName = _T("Finnhub economic country list");
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/country?");
	m_lIndex = -1;
}

CString CProductFinnhubEconomicCountryList::CreateMessage() {
	m_strInquiry = m_strInquiryFunction;
	return m_strInquiry;
}

bool CProductFinnhubEconomicCountryList::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(std::strcmp(typeid(*GetMarket()).name(), _T("class CWorldMarket")) == 0);

	const auto pvCountry = ParseFinnhubCountryList(pWebData);
	for (const auto& pCountry : *pvCountry) {
		if (!GetMarket()->IsCountry(pCountry)) {
			GetMarket()->AddCountry(pCountry);
		}
	}

	return true;
}

CCountryVectorPtr CProductFinnhubEconomicCountryList::ParseFinnhubCountryList(const CWebDataPtr& pWebData) {
	auto pvCountry = make_shared<vector<CCountryPtr>>();
	CCountryPtr pCountry = nullptr;
	string s;

	if (!IsValidData(pWebData)) return pvCountry;

	const auto pjs = pWebData->GetJSon();
	try {
		for (auto it = pjs->begin(); it != pjs->end(); ++it) {
			pCountry = make_shared<CCountry>();
			s = jsonGetString(it, _T("code2"));
			if (!s.empty()) pCountry->m_strCode2 = s.c_str();
			s = jsonGetString(it,_T("code3"));
			pCountry->m_strCode3 = s.c_str();
			s = jsonGetString(it,_T("codeNo"));
			pCountry->m_strCodeNo = s.c_str();
			s = jsonGetString(it,_T("country"));
			pCountry->m_strCountry = s.c_str();
			s = jsonGetString(it,_T("currency"));
			pCountry->m_strCurrency = s.c_str();
			s = jsonGetString(it,_T("currencyCode"));
			pCountry->m_strCurrencyCode = s.c_str();
			pvCountry->push_back(pCountry);
		}
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Country List "), e.what());
		return pvCountry;
	}
	ranges::sort(pvCountry->begin(), pvCountry->end(),
	             [](const CCountryPtr& p1, const CCountryPtr& p2) { return p1->m_strCountry < p2->m_strCountry; });
	return pvCountry;
}

void CProductFinnhubEconomicCountryList::UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) {
	ASSERT(strcmp(typeid(*pDataSource).name(), _T("class CFinnhubDataSource")) == 0);
	dynamic_pointer_cast<CFinnhubDataSource>(pDataSource)->m_fUpdateCountryList = false;
}
