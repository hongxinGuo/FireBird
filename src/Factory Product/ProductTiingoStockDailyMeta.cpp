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

#include"simdjsonGetValue.h"
#include "WebData.h"
#include "WorldMarket.h"

CProductTiingoStockDailyMeta::CProductTiingoStockDailyMeta() {
	m_strInquiryFunction = "https://api.tiingo.com/tiingo/daily/";
}

string CProductTiingoStockDailyMeta::CreateMessage() {
	const auto pStock = gl_dataContainerTiingoStock.GetStock(GetIndex());
	m_strInquiringSymbol = pStock->GetSymbol();

	m_strInquiry = m_strInquiryFunction + m_strInquiringSymbol + "?";
	return m_strInquiry;
}

void CProductTiingoStockDailyMeta::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pTiingoStockDailyMeta = ParseTiingoStockDailyMeta(pWebData);
	if (pTiingoStockDailyMeta == nullptr) {
		auto pStock = gl_dataContainerTiingoStock.GetStock(m_lIndex);
		pStock->SetUpdateStockDailyMeta(false);
		return;
	}
	auto pStock = gl_dataContainerTiingoStock.GetStock(m_lIndex);
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
CTiingoStockDailyMetaPtr CProductTiingoStockDailyMeta::ParseTiingoStockDailyMeta(const CWebDataPtr& pWebData) {
	string s1, s2;
	chrono::local_days ld, ld2;
	if (!IsValidData(pWebData)) return nullptr;

	auto pTiingoStockDailyMeta = make_shared<CTiingoStockDailyMeta>();

	try {
		string_view svJson = pWebData->GetStringView();
		ondemand::parser parser;
		const simdjson::padded_string jsonPadded(svJson);
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
