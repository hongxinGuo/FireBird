//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Tiingo IEX last top of book。
///
/// 目前每天能够更新的总数大致是2万，其中在股票代码中的大致为六千多。
/// 闭市后可以使用此数据来更新当天的日线数据。
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"

#include"jsonParse.h"

#include "ProductTiingoIEXTopOFBook.h"

#include "TiingoDataSource.h"

#include"simdjsonGetValue.h"
#include "WebData.h"
#include "WorldMarket.h"

CProductTiingoIEXTopOfBook::CProductTiingoIEXTopOfBook() {
	m_strInquiryFunction = _T("https://api.tiingo.com/iex?");
}

CString CProductTiingoIEXTopOfBook::CreateMessage() {
	m_strInquiringSymbol = _T("All");
	m_strInquiry = m_strInquiryFunction;
	return m_strInquiry;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
//
// tiingo IEX top of book会更新已退市的股票（无意义的数据），不使用这种数据，只更新目前活跃的股票。
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CProductTiingoIEXTopOfBook::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvTiingoIEXTopOFBook = ParseTiingoIEXTopOfBook(pWebData);
	long lNewestTradeDay = gl_pWorldMarket->GetCurrentTradeDate();
	time_t ttNewestTradeDay = gl_pWorldMarket->TransferToUTCTime(lNewestTradeDay, gl_pWorldMarket->GetMarketCloseTime()); // 美股下午4点收市
	if (pvTiingoIEXTopOFBook->empty()) return;
	for (const auto& pIEXTopOFBook : *pvTiingoIEXTopOFBook) {
		if (pIEXTopOFBook->m_timeStamp.time_since_epoch().count() < ttNewestTradeDay) continue; // 只使用不早于一天的实时数据
		if (!gl_dataContainerTiingoStock.IsSymbol(pIEXTopOFBook->m_strTicker)) continue; // 只更新已有代码
		auto pTiingoStock = gl_dataContainerTiingoStock.GetStock(pIEXTopOFBook->m_strTicker);
		pTiingoStock->UpdateRTData(pIEXTopOFBook);
	}
	if (gl_pWorldMarket->IsMarketClosed()) {
		gl_pWorldMarket->SetEndMarketIEXTopOfBookUpdate(true);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// [
//   {
//     "ticker":"000001",
//     "timestamp" : "2024-09-30T20:00:00+00:00",
//     "lastSaleTimestamp" : "2024-09-30T20:00:00+00:00",
//     "quoteTimestamp" : "2024-09-30T20:00:00+00:00",
//     "open" : 11.62,
//     "high" : 12.3,
//     "low" : 11.56,
//     "mid" : null,
//     "tngoLast" : 12.21,
//     "last" : 12.21,
//     "lastSize" : null,
//     "bidSize" : null,
//     "bidPrice" : null,
//     "askPrice" : null,
//     "askSize" : null,
//     "volume" : 543028360,
//     "prevClose" : 11.42
//   },
//   {
//		 "ticker":"000002",
//		 "timestamp" : "2024-09-30T20:00:00+00:00",
//		 "lastSaleTimestamp" : "2024-09-30T20:00:00+00:00",
//		 "quoteTimestamp" : "2024-09-30T20:00:00+00:00",
//		 "open" : 9.72,
//		 "high" : 9.72,
//		 "low" : 9.27,
//		 "mid" : null,
//		 "tngoLast" : 9.72,
//		 "last" : 9.72,
//		 "lastSize" : null,
//		 "bidSize" : null,
//		 "bidPrice" : null,
//		 "askPrice" : null,
//		 "askSize" : null,
//		 "volume" : 579726570,
//		 "prevClose" : 8.84
//	  }
// ]
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTiingoIEXTopOfBooksPtr CProductTiingoIEXTopOfBook::ParseTiingoIEXTopOfBook(const CWebDataPtr& pWebData) {
	auto pvTiingoIEXLastTopOFBook = make_shared<vector<CTiingoIEXTopOfBookPtr>>();
	CString strNULL = _T(" ");
	CTiingoIEXTopOfBookPtr pIEXLastTopOFBook = nullptr;
	string s1;
	CString strNumber;
	if (!IsValidData(pWebData)) return pvTiingoIEXLastTopOFBook;

	try {
		stringstream ss;
		string_view svJson = pWebData->GetStringView(0, pWebData->GetBufferLength());
		ondemand::parser parser;
		const simdjson::padded_string jsonPadded(svJson);
		ondemand::document doc = parser.iterate(jsonPadded).value();

		CString str;
		int iCount = 0;
		for (auto item : doc) {
			CTiingoStock stock;
			auto itemValue = item.value();
			pIEXLastTopOFBook = nullptr;
			pIEXLastTopOFBook = make_shared<CTiingoIEXTopOfBook>();
			s1 = jsonGetStringView(itemValue, _T("ticker"));
			pIEXLastTopOFBook->m_strTicker = s1.c_str();
			s1 = jsonGetStringView(itemValue, _T("timestamp"));
			ss.clear();
			ss.str(s1);
			chrono::from_stream(ss, _T("%FT%T%Ez"), pIEXLastTopOFBook->m_timeStamp);
			s1 = jsonGetStringView(itemValue, _T("lastSaleTimestamp"));
			ss.clear();
			ss.str(s1);
			chrono::from_stream(ss, "%FT%T%0z", pIEXLastTopOFBook->m_lastSale);
			s1 = jsonGetStringView(itemValue, _T("quoteTimestamp"));
			ss.clear();
			ss.str(s1);
			chrono::from_stream(ss, "%FT%T%0z", pIEXLastTopOFBook->m_quote);

			pIEXLastTopOFBook->m_lHigh = jsonGetDouble(itemValue, _T("high")) * stock.GetRatio();
			pIEXLastTopOFBook->m_lLow = jsonGetDouble(itemValue, _T("low")) * stock.GetRatio();
			pIEXLastTopOFBook->m_lLastClose = jsonGetDouble(itemValue, _T("prevClose")) * stock.GetRatio();
			pIEXLastTopOFBook->m_lOpen = jsonGetDouble(itemValue, _T("open")) * stock.GetRatio();
			pIEXLastTopOFBook->m_lNew = jsonGetDouble(itemValue, _T("last")) * stock.GetRatio();
			pIEXLastTopOFBook->m_llVolume = jsonGetInt64(itemValue, _T("volume"));

			pvTiingoIEXLastTopOFBook->push_back(pIEXLastTopOFBook);
			iCount++;
		}
	} catch (simdjson_error& error) {
		ReportJSonErrorToSystemMessage(_T("Tiingo market news "), error.what());
	}

	return pvTiingoIEXLastTopOFBook;
}

void CProductTiingoIEXTopOfBook::UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) {
	ASSERT(strcmp(typeid(*pDataSource).name(), _T("class CTiingoDataSource")) == 0);
	dynamic_pointer_cast<CTiingoDataSource>(pDataSource)->SetUpdateIEXTopOfBook(false);
	if (gl_pWorldMarket->IsMarketClosed()) { // 已闭市？
		dynamic_pointer_cast<CTiingoDataSource>(pDataSource)->SetEndMarketIEXTopOfBookUpdate(true); // 本交易日数据是完整的。
	}
}
