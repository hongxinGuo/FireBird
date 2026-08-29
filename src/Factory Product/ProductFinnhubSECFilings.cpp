#include "pch.h"

#include"WorldMarket.h"
#include"FinnhubStock.h"
#include "ContainerFinnhubStock.h"
#include"SECFiling.h"

#include "ProductFinnhubSECFilings.h"

#include "FinnhubDataSource.h"
#include"jsonParse.h"
#include"simdjsonGetValue.h"
#include "SystemMessage.h"
#include"cpr/cpr.h"

using namespace std;

CProductFinnhubSECFilings::CProductFinnhubSECFilings() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/stock/filings?symbol=";
}

void CProductFinnhubSECFilings::InquireData(const std::stop_token& st) {
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
		const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_index);
		auto pvSECFilings = Parse(r.text);
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
}

void CProductFinnhubSECFilings::WebStatusCheck(cpr::Response& r) {
	switch (r.status_code) {
	case 0: //
		// do nothing
		break;
	case 401: // no right to access
	case 403:
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		CheckInaccessible();
		break;
	default:
		string sType = typeid(this).name();
		string s = std::format("{} error. http code: {}, error code:{}, message:{}", sType, r.status_code, static_cast<int>(r.error.code), r.error.message);
		gl_systemMessage.PushErrorMessage(s);
		break;
	}
}

void CProductFinnhubSECFilings::UpdateSystemStatus() {
}

shared_ptr<vector<string>> CProductFinnhubSECFilings::CreateMessage() {
	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_index);

	m_strInquiringExchange = pStock->GetExchange();
	m_inquiryString = m_strInquiryFunction + pStock->GetSymbol();
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	pInquiry->push_back(m_inquiryString);
	return pInquiry;
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
CSECFilingsPtr CProductFinnhubSECFilings::Parse(const string& text) {
	CSECFilingsPtr pvSECFilings = make_shared<vector<CSECFiling>>();
	pvSECFilings->reserve(100);

	if (::IsVoidJson(text)) return pvSECFilings; // 即使为空，也完成了查询。
	if (IsNoRightToAccess()) return pvSECFilings;

	string s1;
	try {
		std::istringstream ss;
		ondemand::parser parser;
		const simdjson::padded_string jsonPadded(text);
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
			SECFiling.m_iFiledDate = gl_pWorldMarket->ToUTCTime(tpTime).time_since_epoch().count();
			s1 = simdjsonGetStringView(itemValue, "acceptedDate");
			ss.clear();
			ss.str(s1);
			chrono::from_stream(ss, "%F %T", tpTime);
			SECFiling.m_iAcceptedDate = gl_pWorldMarket->ToUTCTime(tpTime).time_since_epoch().count();
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
