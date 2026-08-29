//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Tiingo市场新闻。
///	Note 此信息为premium，使用此信息来判断Tiingo账户类型（免费还是付费）
///
///
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"

#include"jsonParse.h"

#include "ProductTiingoStockDailyMeta.h"
#include "TiingoStockDailyMeta.h"

#include "ContainerTiingoStock.h"
#include "log.h"
#include"simdjsonGetValue.h"
#include "SystemMessage.h"
#include "TiingoDataSource.h"
#include "WorldMarket.h"
#include "TiingoStock.h"
#include"cpr/cpr.h"

using namespace std;

CProductTiingoStockDailyMeta::CProductTiingoStockDailyMeta() {
	m_strInquiryFunction = "https://api.tiingo.com/tiingo/daily/";
}

void CProductTiingoStockDailyMeta::InquireData(const std::stop_token& st) {
	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		if (st.stop_requested()) break;
		string s = inquiry + "&token=" + gl_pTiingoDataSource->GetToken();
		cpr::Response r = cpr::Get(cpr::Url{ s });
		m_statusCode = r.status_code;
		m_elapsed = r.elapsed;

		if (m_statusCode != 200) {
			WebStatusCheck(r);
		}

		const auto pTiingoStockDailyMeta = Parse(r.text);
		if (pTiingoStockDailyMeta == nullptr) {
			auto pStock = gl_dataContainerTiingoStock.GetStock(m_index);
			pStock->SetUpdateStockDailyMeta(false);
			return;
		}
		auto pStock = gl_dataContainerTiingoStock.GetStock(m_index);
		if (gl_dataContainerTiingoStock.IsSymbol(pTiingoStockDailyMeta->m_strCode)) {
			auto pStock2 = gl_dataContainerTiingoStock.GetStock(pTiingoStockDailyMeta->m_strCode);
			auto str = pStock->GetSymbol();
			if (str == pStock2->GetSymbol()) {
				pStock->UpdateDailyMeta(pTiingoStockDailyMeta); // 目前只更新HistoryDayLineBeginDate和HistoryDayLineEndDate。
				pStock->SetUpdateStockDailyMetaDate(gl_pWorldMarket->GetCurrentTradeDate());
				pStock->SetUpdateProfileDB(true);
			}
			else {
				string str2 = "Tiingo stock daily meta not match: ";
				str2 += pStock->GetSymbol() + "  " + pStock2->GetSymbol();
				gl_systemMessage.PushInnerSystemInformationMessage(str2);
				gl_errorLogger->warn("{}", str2);
			}
		}
		pStock->SetUpdateStockDailyMeta(false);
	}
}

void CProductTiingoStockDailyMeta::WebStatusCheck(cpr::Response& r) {
	switch (r.status_code) {
	case 0:
		break;
	case 403: // forbidden
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		break;
	default:
		break;
	}
}

void CProductTiingoStockDailyMeta::UpdateSystemStatus() {
}

shared_ptr<vector<string>> CProductTiingoStockDailyMeta::CreateMessage() {
	const auto pStock = gl_dataContainerTiingoStock.GetStock(GetIndex());
	m_strInquiringSymbol = pStock->GetSymbol();

	m_inquiryString = m_strInquiryFunction + m_strInquiringSymbol + "?";
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	pInquiry->push_back(m_inquiryString);
	return pInquiry;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//	{
//		"ticker": "AAPL",
//		"name" : "Apple Inc.",
//		"exchangeCode" : "NASDAQ",
//		"startDate" : "1980-12-12",
//		"endDate" : "2019-01-25",
//		"description" : "Apple Inc. (Apple) designs, manufactures and markets mobile communication and media devices"
//	}
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTiingoStockDailyMetaPtr CProductTiingoStockDailyMeta::Parse(const string& text) {
	if (::IsVoidJson(text)) return nullptr;
	if (IsNoRightToAccess()) return nullptr;

	auto pTiingoStockDailyMeta = make_shared<CTiingoStockDailyMeta>();

	try {
		chrono::local_days ld2;
		chrono::local_days ld;
		string s2;
		string s1;
		ondemand::parser parser;
		const simdjson::padded_string jsonPadded(text);
		ondemand::document doc = parser.iterate(jsonPadded).value();

		s1 = simdjsonGetStringView(doc, "ticker");
		pTiingoStockDailyMeta->m_strCode = s1;
		s1 = simdjsonGetStringView(doc, "name");
		pTiingoStockDailyMeta->m_strName = s1;
		s1 = simdjsonGetStringView(doc, "exchangeCode");
		pTiingoStockDailyMeta->m_strExchange = s1;
		s1 = simdjsonGetStringView(doc, "description");
		pTiingoStockDailyMeta->m_strDescription = s1;
		s1 = simdjsonGetStringView(doc, "startDate", "1900-01-01"); // 如果没有日线开始日期（即没有日线数据），则设置为19000101
		istringstream ss(s1);
		ss >> chrono::parse("%F", ld);
		pTiingoStockDailyMeta->m_lHistoryDayLineStartDate = ld;
		s2 = simdjsonGetStringView(doc, "endDate", "1900-01-01"); // 如果没有日线结束日期（即没有日线数据），则设置为19000101
		istringstream ss2(s2);
		ss2 >> chrono::parse("%F", ld2);
		pTiingoStockDailyMeta->m_lHistoryDayLineEndDate = ld2;
	} catch (simdjson_error& error) {
		ReportJSonErrorToSystemMessage("Tiingo ticker daily: ", error.what());
		pTiingoStockDailyMeta = nullptr;
	}

	return pTiingoStockDailyMeta;
}
