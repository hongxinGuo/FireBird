#include "pch.h"

#include"WorldMarket.h"
#include"FinnhubStock.h"

#include "ProductFinnhubSECFilings.h"

#include"jsonParse.h"
#include"simdjsonGetValue.h"
#include "WebData.h"

CProductFinnhubSECFilings::CProductFinnhubSECFilings() {
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/stock/filings?symbol=");
}

string CProductFinnhubSECFilings::CreateMessage() {
	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_lIndex);

	m_strInquiringExchange = pStock->GetExchangeCode();
	m_strInquiry = m_strInquiryFunction + pStock->GetSymbol();
	return m_strInquiry;
}

void CProductFinnhubSECFilings::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(std::strcmp(typeid(*GetMarket()).name(), _T("class CWorldMarket")) == 0);

	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_lIndex);
	auto pvSECFilings = ParseFinnhubStockSECFilings(pWebData);
	auto size = pvSECFilings->size();
	pStock->SetSECFilings(pvSECFilings);
	pStock->SetUpdateSECFilings(false);
	pStock->SetSECFilingsUpdateDate(GetMarket()->GetMarketDate());
	pStock->SetUpdateProfileDB(true);
	if (size > 0) {
		pStock->SetUpdateSECFilingsDB(true);
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// [
// {
//	"accessNumber": "0001193125-20-050884",
// 		"symbol" : "AAPL",
// 		"cik" : "320193",
//		"form" : "8-K",
//		"filedDate" : "2020-02-27 00:00:00",
//		"acceptedDate" : "2020-02-27 06:14:21",
//		"reportUrl" : "https://www.sec.gov/ix?doc=/Archives/edgar/data/320193/000119312520050884/d865740d8k.htm",
//		"filingUrl" : "https://www.sec.gov/Archives/edgar/data/320193/000119312520050884/0001193125-20-050884-index.html"
// },
// 	{
//		"accessNumber": "0001193125-20-039203",
//		"symbol" : "AAPL",
//		"cik" : "320193",
//		"form" : "8-K",
//		"filedDate" : "2020-02-18 00:00:00",
//		"acceptedDate" : "2020-02-18 06:24:57",
//		"reportUrl" : "https://www.sec.gov/ix?doc=/Archives/edgar/data/320193/000119312520039203/d845033d8k.htm",
//		"filingUrl" : "https://www.sec.gov/Archives/edgar/data/320193/000119312520039203/0001193125-20-039203-index.html"
//	},
//	...
// ]
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CSECFilingsPtr CProductFinnhubSECFilings::ParseFinnhubStockSECFilings(const CWebDataPtr& pWebData) {
	CSECFilingsPtr pvSECFilings = make_shared<vector<CSECFilingPtr>>();
	string s1;
	if (!IsValidData(pWebData)) return pvSECFilings;
	try {
		std::stringstream ss;
		string_view svJson = pWebData->GetStringView(0, pWebData->GetBufferLength());
		ondemand::parser parser;
		const simdjson::padded_string jsonPadded(svJson);
		ondemand::document doc = parser.iterate(jsonPadded).value();

		for (auto item : doc) {
			auto itemValue = item.value();
			auto pSECFiling = make_shared<CSECFiling>();
			s1 = simdjsonGetStringView(itemValue, _T("symbol"));
			pSECFiling->m_strSymbol = s1;
			s1 = simdjsonGetStringView(itemValue, _T("accessNumber"));
			pSECFiling->m_strAccessNumber = s1;
			s1 = simdjsonGetStringView(itemValue, _T("cik"));
			pSECFiling->m_iCIK = atoi(s1.c_str());
			s1 = simdjsonGetStringView(itemValue, _T("form"));
			pSECFiling->m_strForm = s1;
			s1 = simdjsonGetStringView(itemValue, _T("filedDate"));
			ss.clear();
			ss.str(s1);
			chrono::sys_seconds tpTime;
			chrono::from_stream(ss, "%F %T", tpTime);
			tpTime -= gl_pWorldMarket->GetTimeZoneOffset();
			pSECFiling->m_iFiledDate = tpTime.time_since_epoch().count();
			s1 = simdjsonGetStringView(itemValue, _T("acceptedDate"));
			ss.clear();
			ss.str(s1);
			chrono::from_stream(ss, "%F %T", tpTime);
			tpTime -= gl_pWorldMarket->GetTimeZoneOffset();
			pSECFiling->m_iAcceptedDate = tpTime.time_since_epoch().count();
			s1 = simdjsonGetStringView(itemValue, _T("reportUrl"));
			pSECFiling->m_strReportURL = s1;
			s1 = simdjsonGetStringView(itemValue, _T("filingUrl"));
			pSECFiling->m_strFilingURL = s1;
			pvSECFilings->push_back(pSECFiling);
		}
	} catch (simdjson_error& error) {
		ReportJSonErrorToSystemMessage(_T("finnhub SEC Filings "), error.what());
	}
	// °´accessNumberÅÅÐò
	std::ranges::sort(*pvSECFilings, [](const CSECFilingPtr& p1, const CSECFilingPtr& p2) { return p1->m_strAccessNumber.compare(p2->m_strAccessNumber); });

	return pvSECFilings;
}
