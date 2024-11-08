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
#include "WorldMarket.h"

CProductTiingoStockDailyMeta::CProductTiingoStockDailyMeta() {
	m_strInquiryFunction = _T("https://api.tiingo.com/tiingo/daily/");
}

CString CProductTiingoStockDailyMeta::CreateMessage() {
	ASSERT(std::strcmp(typeid(*GetMarket()).name(), _T("class CWorldMarket")) == 0);

	const auto pStock = gl_dataContainerTiingoStock.GetStock(GetIndex());
	m_strInquiringSymbol = pStock->GetSymbol();

	m_strInquiry = m_strInquiryFunction + m_strInquiringSymbol + _T("?");
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
		if (pStock->GetSymbol().Compare(pStock2->GetSymbol()) == 0) {
			pStock->UpdateDailyMeta(pTiingoStockDailyMeta);
			pStock->SetUpdateStockDailyMetaDate(gl_pWorldMarket->GetMarketDate());
		}
		else {
			CString str = _T("Tiingo stock daily meta not match: ");
			str += pStock->GetSymbol() + _T("  ") + pStock2->GetSymbol();
			gl_systemMessage.PushInnerSystemInformationMessage(str);
		}
	}
	pStock->SetUpdateStockDailyMeta(false);
	gl_systemConfiguration.DecreaseTiingoBandWidth(pWebData->GetBufferLength());
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
	string s1;
	int year, month, day;
	if (!IsValidData(pWebData)) return nullptr;

	auto pTiingoStockDailyMeta = make_shared<CTiingoStockDailyMeta>();

	try {
		string_view svJson = pWebData->GetStringView(0, pWebData->GetBufferLength());
		ondemand::parser parser;
		const simdjson::padded_string jsonPadded(svJson);
		ondemand::document doc = parser.iterate(jsonPadded).value();

		s1 = jsonGetStringView(doc, "ticker");
		pTiingoStockDailyMeta->m_strCode = s1.c_str();
		s1 = jsonGetStringView(doc, "name");
		pTiingoStockDailyMeta->m_strName = s1.c_str();
		s1 = jsonGetStringView(doc, "exchangeCode");
		pTiingoStockDailyMeta->m_strExchange = s1.c_str();
		s1 = jsonGetStringView(doc, "description");
		pTiingoStockDailyMeta->m_strDescription = s1.c_str();
		s1 = jsonGetStringView(doc, "startDate", "1900-01-01"); // 如果没有日线开始日期（即没有日线数据），则设置为19000101
		sscanf_s(s1.c_str(), _T("%04d-%02d-%02d"), &year, &month, &day);
		pTiingoStockDailyMeta->m_lHistoryDayLineStartDate = year * 10000 + month * 100 + day;
		s1 = jsonGetStringView(doc, "endDate", "1900-01-01"); // 如果没有日线结束日期（即没有日线数据），则设置为19000101
		sscanf_s(s1.c_str(), _T("%04d-%02d-%02d"), &year, &month, &day);
		pTiingoStockDailyMeta->m_lHistoryDayLineEndDate = year * 10000 + month * 100 + day;
	} catch (simdjson_error& error) {
		ReportJSonErrorToSystemMessage(_T("Tiingo ticker daily: "), error.what());
		pTiingoStockDailyMeta = nullptr;
	}

	return pTiingoStockDailyMeta;
}
