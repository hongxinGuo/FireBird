#include "pch.h"

#include"WorldMarket.h"
#include"FinnhubStock.h"

#include "ProductFinnhubSECFilings.h"

#include"jsonParse.h"
#include"simdjsonGetValue.h"
#include "WebData.h"

CProductFinnhubSECFilings::CProductFinnhubSECFilings() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/stock/filings?symbol=";
}

string CProductFinnhubSECFilings::CreateMessage() {
	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_lIndex);

	m_strInquiringExchange = pStock->GetExchange();
	m_strInquiry = m_strInquiryFunction + pStock->GetSymbol();
	return m_strInquiry;
}

void CProductFinnhubSECFilings::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_lIndex);
	auto pvSECFilings = ParseFinnhubStockSECFilings(pWebData);
	auto size = pvSECFilings->size();
	pStock->SetUpdateSECFilings(false);
	pStock->SetSECFilingsUpdateDate(gl_pWorldMarket->GetMarketDate());
	pStock->SetUpdateProfileDB(true);
	if (size > 0) {
		pStock->SetSECFilings(pvSECFilings);
		pStock->SetUpdateSECFilingsDB(true);
	}
	pvSECFilings = nullptr;
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
	CSECFilingsPtr pvSECFilings = make_shared<vector<CSECFiling>>();
	pvSECFilings->reserve(100);

	string s1;
	if (!IsValidData(pWebData)) return pvSECFilings;
	try {
		std::istringstream ss;
		string_view svJson = pWebData->GetStringView();
		ondemand::parser parser;
		const simdjson::padded_string jsonPadded(svJson);
		ondemand::document doc = parser.iterate(jsonPadded).value();

		for (auto item : doc) {
			auto itemValue = item.value();
			CSECFiling SECFiling;
			SECFiling.m_strSymbol = simdjsonGetStringView(itemValue, "symbol");
			SECFiling.m_strAccessNumber = simdjsonGetStringView(itemValue, "accessNumber");
			s1 = simdjsonGetStringView(itemValue, "cik");
			SECFiling.m_iCIK = atoi(s1.c_str());
			SECFiling.m_strForm = simdjsonGetStringView(itemValue, "form");
			s1 = simdjsonGetStringView(itemValue, "filedDate");
			ss.clear();
			ss.str(s1);
			chrono::local_seconds tpTime;
			chrono::from_stream(ss, "%F %T", tpTime);
			SECFiling.m_iFiledDate = gl_pWorldMarket->ToSysTime(tpTime).time_since_epoch().count();
			s1 = simdjsonGetStringView(itemValue, "acceptedDate");
			ss.clear();
			ss.str(s1);
			chrono::from_stream(ss, "%F %T", tpTime);
			SECFiling.m_iAcceptedDate = gl_pWorldMarket->ToSysTime(tpTime).time_since_epoch().count();
			s1 = simdjsonGetStringView(itemValue, "reportUrl");
			SECFiling.m_strReportURL = s1;
			s1 = simdjsonGetStringView(itemValue, "filingUrl");
			SECFiling.m_strFilingURL = s1;
			pvSECFilings->push_back(SECFiling);
			SECFiling.Reset();
		}
	} catch (simdjson_error& error) {
		ReportJSonErrorToSystemMessage("finnhub SEC Filings ", error.what());
	}
	// 按accessNumber排序
	std::ranges::sort(*pvSECFilings, [](const CSECFiling& p1, const CSECFiling& p2) { return p1.m_strAccessNumber.compare(p2.m_strAccessNumber); });

	return pvSECFilings;
}
