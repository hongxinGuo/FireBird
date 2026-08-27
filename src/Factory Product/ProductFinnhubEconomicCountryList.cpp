#include "pch.h"

#include"cpr/cpr.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include"Country.h"

#include "ProductFinnhubEconomicCountryList.h"

#include "ContainerFinnhubCountry.h"
#include "FinnhubDataSource.h"
#include "SystemMessage.h"

using std::make_shared;

CProductFinnhubEconomicCountryList::CProductFinnhubEconomicCountryList() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/country?";
}

void CProductFinnhubEconomicCountryList::InquireData(const std::stop_token& st) {
	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		if (st.stop_requested()) break;
		string inquireString = inquiry + "&token=" + gl_pFinnhubDataSource->GetToken();
		cpr::Response r = cpr::Get(cpr::Url{ inquireString });
		m_statusCode = r.status_code;
		m_elapsed = r.elapsed;

		if (m_statusCode != 200) {
			WebStatusCheck(r);
			return;
		}

		auto pvCountry = Parse(r.text);
		for (const auto& pCountry : *pvCountry) {
			if (!gl_dataContainerFinnhubCountry.IsCountry(pCountry)) {
				gl_dataContainerFinnhubCountry.Add(pCountry);
			}
		}
	}
}

void CProductFinnhubEconomicCountryList::WebStatusCheck(cpr::Response& r) {
}

shared_ptr<vector<string>> CProductFinnhubEconomicCountryList::CreateMessage() {
	m_inquiryString = m_strInquiryFunction;
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	pInquiry->push_back(m_inquiryString);
	return pInquiry;
}

CCountriesPtr CProductFinnhubEconomicCountryList::Parse(const std::string& text) {
	auto pvCountry = make_shared<vector<CCountry>>();
	nlohmannJson js;

	if (text.empty()) return pvCountry;
	if (!::CreateJsonWithNlohmann(js, text)) return pvCountry;
	if (::IsVoidJson(text)) return pvCountry;

	pvCountry->reserve(300);
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
	if (m_statusCode != 200) return;

	gl_pFinnhubDataSource->SetUpdateCountryList(false);
	gl_systemMessage.PushInformationMessage("Finnhub economic country List updated");
}
