#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

#include"Country.h"

#include "ProductFinnhubEconomicCountryList.h"

#include "FinnhubDataSource.h"
#include "WebData.h"

CProductFinnhubEconomicCountryList::CProductFinnhubEconomicCountryList() {
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/country?");
}

CString CProductFinnhubEconomicCountryList::CreateMessage() {
	m_strInquiry = m_strInquiryFunction;
	return m_strInquiry;
}

void CProductFinnhubEconomicCountryList::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvCountry = ParseFinnhubCountryList(pWebData);
	for (const auto& pCountry : *pvCountry) {
		if (!gl_dataContainerFinnhubCountry.IsCountry(pCountry)) {
			gl_dataContainerFinnhubCountry.Add(pCountry);
		}
	}
}

CCountriesPtr CProductFinnhubEconomicCountryList::ParseFinnhubCountryList(const CWebDataPtr& pWebData) {
	auto pvCountry = make_shared<vector<CCountryPtr>>();
	CCountryPtr pCountry = nullptr;
	string s;
	json js;

	if (!pWebData->CreateJson(js)) return pvCountry;
	if (!IsValidData(pWebData)) return pvCountry;

	try {
		for (auto it = js.begin(); it != js.end(); ++it) {
			pCountry = make_shared<CCountry>();
			s = jsonGetString(it, _T("code2"));
			if (!s.empty()) pCountry->m_strCode2 = s;
			s = jsonGetString(it,_T("code3"));
			pCountry->m_strCode3 = s;
			s = jsonGetString(it,_T("codeNo"));
			pCountry->m_strCodeNo = s;
			s = jsonGetString(it,_T("country"));
			pCountry->m_strCountry = s;
			s = jsonGetString(it,_T("currency"));
			pCountry->m_strCurrency = s;
			s = jsonGetString(it,_T("currencyCode"));
			pCountry->m_strCurrencyCode = s;
			pvCountry->push_back(pCountry);
		}
	} catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Country List "), e.what());
		return pvCountry;
	}
	std::ranges::sort(pvCountry->begin(), pvCountry->end(),
	                  [](const CCountryPtr& p1, const CCountryPtr& p2) { return p1->m_strCountry < p2->m_strCountry; });
	return pvCountry;
}

void CProductFinnhubEconomicCountryList::UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) {
	ASSERT(strcmp(typeid(*pDataSource).name(), _T("class CFinnhubDataSource")) == 0);
	dynamic_pointer_cast<CFinnhubDataSource>(pDataSource)->SetUpdateCountryList(false);
	gl_systemMessage.PushInformationMessage(_T("Finnhub economic country List updated"));
}
