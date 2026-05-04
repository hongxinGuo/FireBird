#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include"Country.h"

#include "ProductFinnhubEconomicCountryList.h"

#include "FinnhubDataSource.h"
#include "WebData.h"

CProductFinnhubEconomicCountryList::CProductFinnhubEconomicCountryList() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/country?";
}

string CProductFinnhubEconomicCountryList::CreateMessage() {
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
	auto pvCountry = make_shared<vector<CCountry>>();
	pvCountry->reserve(300);

	nlohmannJson js;

	if (!pWebData->CreateJson(js)) return pvCountry;
	if (!IsValidData(pWebData)) return pvCountry;

	try {
		string s;
		for (auto it = js.begin(); it != js.end(); ++it) {
			CCountry country;
			s = jsonGetString(it, "code2");
			if (!s.empty()) country.m_strCode2 = s;
			s = jsonGetString(it, "code3");
			country.m_strCode3 = s;
			s = jsonGetString(it, "codeNo");
			country.m_strCodeNo = s;
			s = jsonGetString(it, "country");
			country.m_strCountry = s;
			s = jsonGetString(it, "currency");
			country.m_strCurrency = s;
			s = jsonGetString(it, "currencyCode");
			country.m_strCurrencyCode = s;
			pvCountry->push_back(country);
			country.Reset();
		}
	} catch (nlohmannJson::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub Country List ", e.what());
		return pvCountry;
	}
	std::ranges::sort(pvCountry->begin(), pvCountry->end(),
	                  [](const CCountry& p1, const CCountry& p2) { return p1.m_strCountry < p2.m_strCountry; });
	return pvCountry;
}

void CProductFinnhubEconomicCountryList::UpdateSystemStatus() {
	gl_pFinnhubDataSource->SetUpdateCountryList(false);
	gl_systemMessage.PushInformationMessage("Finnhub economic country List updated");
}
