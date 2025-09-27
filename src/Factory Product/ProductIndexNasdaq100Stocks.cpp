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

	if (gl_vNasdaq100Stocks.size() >= 90) {
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
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  https://www.slickcharts.com/nasdaq100网页中，有效内容格式为：
//
// {
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

	string_view svData = pWebData->GetStringView();
	string s(svData);
	size_t positionStart = svData.find(_T("<script> window.__sc_init_state__ = ")); // 有效数据前面的字符串
	positionStart += 36; // 跨过此字符串
	string_view svData2 = svData.substr(positionStart);
	size_t positionEnd = svData2.find(_T("; </script>")); // 有效数据后的字符串
	string_view sv = svData2.substr(0, positionEnd);

	ondemand::parser parser;
	const simdjson::padded_string jsonPadded(sv);
	ondemand::document doc = parser.iterate(jsonPadded).value();
	auto item1 = doc["companyListComponent"];
	auto item2 = item1["companyList"];

	for (auto item : item2) {
		auto itemValue = item.value();
		auto svSymbol = simdjsonGetStringView(itemValue, _T("symbol"));
		string s(svSymbol.data(), svSymbol.length());
		vSymbol.push_back(s);
	}

	return vSymbol;
}

void CProductIndexNasdaq100Stocks::UpdateSystemStatus(CVirtualDataSourcePtr pDataSource) {
	ASSERT(strcmp(typeid(*pDataSource).name(), _T("class CAccessoryDataSource")) == 0);
	dynamic_pointer_cast<CAccessoryDataSource>(pDataSource)->SetUpdateIndexNasdaq100Stocks(false);
	gl_systemMessage.PushInnerSystemInformationMessage(_T("Nasdaq 100 stock list updated"));
}
