#include "pch.h"

#include "ProductIndexNasdaq100Stocks.h"

#include "AccessoryDataSource.h"
#include "simdjsonGetValue.h"

#include<sqlpp23/sqlpp23.h>
#include"StockMarketSQLTable.h"

#include"dataBaseConnector.h"
#include "SystemMessage.h"

#include"cpr/cpr.h"

using std::make_shared;

CProductIndexNasdaq100Stocks::CProductIndexNasdaq100Stocks() {
	m_strInquiryFunction = "https://www.slickcharts.com/nasdaq100";
}

void CProductIndexNasdaq100Stocks::InquireData(const std::stop_token& st, const string& strHeaders, const string& strParams, const string& strSuffix, const string& strInquiryToken) {
	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		if (st.stop_requested()) break;
		cpr::Response r = cpr::Get(cpr::Url{ inquiry });
		m_statusCode = r.status_code;
		m_elapsed = r.elapsed;

		if (m_statusCode != 200) {
			WebStatusCheck(r);
			return;
		}

		gl_vNasdaq100Stocks = Parse(r.text);
		std::ranges::sort(gl_vNasdaq100Stocks, [](const string& s1, const string& s2) { return s1 < s2; });

		if (gl_vNasdaq100Stocks.size() >= 90) {
			using namespace StockMarket;
			const auto& t = IndexNasdaq100{};
			auto db = gl_dbStockMarket.get();
			auto tx = sqlpp::start_transaction(db);
			auto multi_insert = insert_into(t).columns(t.Symbol);

			db(delete_from(t));

			for (auto& s : gl_vNasdaq100Stocks) {
				multi_insert.add_values(t.Symbol = s);
			}
			if (!gl_vNasdaq100Stocks.empty()) {
				db(multi_insert);
			}
			tx.commit();
		}
	}
}
void CProductIndexNasdaq100Stocks::WebStatusCheck(cpr::Response& r) {
}

void CProductIndexNasdaq100Stocks::UpdateSystemStatus() {
	gl_pAccessoryDataSource->SetUpdateIndexNasdaq100Stocks(false);
	gl_systemMessage.PushInnerSystemInformationMessage("Nasdaq 100 stock list updated");
}

shared_ptr<vector<string>> CProductIndexNasdaq100Stocks::CreateMessage() {
	m_inquiryString = m_strInquiryFunction;
	shared_ptr<vector<string>> pInquiryStrings = make_shared<vector<string>>();
	pInquiryStrings->push_back(m_inquiryString);
	return pInquiryStrings;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  https://www.slickcharts.com/nasdaq100网页中，有效内容格式为：
//
// {
//		nasdaq100List: [
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
// }
//
////////////////////////////////////////////////////////////////////////////////////////////////////////
vector<string> CProductIndexNasdaq100Stocks::Parse(const string& text) {
	vector<string> vSymbol;

	size_t positionStart = text.find("[null,{type:\"data\",data:{nasdaq100List:"); // 有效数据前面的字符串
	if (positionStart == string_view::npos) {
		return vSymbol;
	}

	positionStart += 39; // 跨过此字符串
	string_view svData2 = text.substr(positionStart);
	size_t foundEnd = svData2.find("]");
	if (foundEnd == string_view::npos) {
		return vSymbol;
	}
	// include the closing bracket
	std::string jsonText(svData2.substr(0, foundEnd + 1));

	try {
		using json = nlohmann::json;
		json j = json::parse(jsonText);

		if (!j.is_array()) {
			gl_systemMessage.PushErrorMessage("Nasdaq 100 List format changed");
			return vSymbol;
		}

		for (const auto& item : j) {
			if (item.contains("symbol") && item["symbol"].is_string()) {
				vSymbol.push_back(item["symbol"].get<std::string>());
			}
		}
	} catch (const nlohmann::json::parse_error& e) {
		gl_systemMessage.PushErrorMessage("Nasdaq 100 List format changed");
		return vSymbol;
	} catch (const std::exception& ex) {
		gl_systemMessage.PushErrorMessage("Nasdaq 100 List parse error");
		return vSymbol;
	}

	return vSymbol;
}
