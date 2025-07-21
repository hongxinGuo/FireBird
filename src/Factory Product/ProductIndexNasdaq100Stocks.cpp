#include "pch.h"

#include "ProductIndexNasdaq100Stocks.h"

#include "AccessoryDataSource.h"
#include "SetIndexNasdaq100.h"
#include "simdjsonGetValue.h"

#include"WebData.h"

CProductIndexNasdaq100Stocks::CProductIndexNasdaq100Stocks() {
	m_strInquiryFunction = _T("https://www.slickcharts.com/nasdaq100");
}

string CProductIndexNasdaq100Stocks::CreateMessage() {
	m_strInquiry = m_strInquiryFunction;
	return m_strInquiry;
}

void CProductIndexNasdaq100Stocks::ParseAndStoreWebData(CWebDataPtr pWebData) {
	gl_vNasdaq100Stocks = ParseIndexNasdaq100Stocks(pWebData);
	std::ranges::sort(gl_vNasdaq100Stocks, [](const string& s1, const string& s2) { return s1 < s2; });

	CSetIndexNasdaq100 setNasdaq100;

	setNasdaq100.Open();
	setNasdaq100.m_pDatabase->BeginTrans();
	while (!setNasdaq100.IsEOF()) {
		setNasdaq100.Delete();
		setNasdaq100.MoveNext();
	}
	for (auto& s : gl_vNasdaq100Stocks) {
		setNasdaq100.AddNew();
		setNasdaq100.m_Symbol = s.c_str();
		setNasdaq100.Update();
	}
	setNasdaq100.m_pDatabase->CommitTrans();
	setNasdaq100.Close();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//
//{
//	"companyListComponent": {
//		"companyList": [
//		{
//			"name": "Nvidia",
//				"symbol" : "NVDA",
//				"lastPrice" : "172.93",
//				"netChange" : "0.52",
//				"changePercent" : "0.30",
//				"changeColor" : "green",
//				"changeImage" : "/img/up.gif",
//				"marketCap" : 4217315855693.3853,
//				"rank" : 1,
//				"weight" : "14.15%"
//		},
//		{
//				"name": "Microsoft",
//				"symbol" : "MSFT",
//				"lastPrice" : "509.45",
//				"netChange" : "-0.60",
//				"changePercent" : "-0.12",
//				"changeColor" : "red",
//				"changeImage" : "/img/down.gif",
//				"marketCap" : 3786509472024.25,
//				"rank" : 2,
//				"weight" : "12.70%"
//		},
//   ]
//  }
// }
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
vector<string> CProductIndexNasdaq100Stocks::ParseIndexNasdaq100Stocks(const CWebDataPtr& pWebData) {
	vector<string> vSymbol;

	string_view svData = pWebData->GetStringView(0, pWebData->GetBufferLength());
	size_t positionStart = svData.find(_T("<script> window.__sc_init_state__ = "));
	positionStart += 36; // ¿ç¹ý´Ë×Ö·û´®
	string_view svDataStart = svData.substr(positionStart);
	size_t positionEnd = svDataStart.find(_T("; </script>"));
	string_view sv = svDataStart.substr(0, positionEnd);

	ondemand::parser parser;
	const simdjson::padded_string jsonPadded(sv);
	ondemand::document doc = parser.iterate(jsonPadded).value();
	auto item1 = doc["companyListComponent"];
	auto item2 = item1["companyList"];

	for (auto item : item2) {
		auto itemValue = item.value();
		auto svSymbol = jsonGetStringView(itemValue, _T("symbol"));
		string s(svSymbol.data(), svSymbol.length());
		vSymbol.push_back(s);
	}

	return vSymbol;
}

void CProductIndexNasdaq100Stocks::UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) {
	ASSERT(strcmp(typeid(*pDataSource).name(), _T("class CAccessoryDataSource")) == 0);
	dynamic_pointer_cast<CAccessoryDataSource>(pDataSource)->SetUpdateIndexNasdaq100Stocks(false);
}
