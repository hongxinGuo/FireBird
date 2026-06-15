//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Tiingo IEX last top of book。
///
/// 目前每天能够更新的总数大致是2万，其中在股票代码中的大致为六千多。
/// 闭市后可以使用此数据来更新当天的日线数据。
/// Note: tiingo IEX top of book缺乏分拆信息，且会更新已退市的股票（无意义的数据），不使用这种数据来更新日线资料，，只用于更新实时数据。
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
	m_strInquiryFunction = "https://api.tiingo.com/iex?";
}

string CProductTiingoIEXTopOfBook::CreateMessage() {
	m_strInquiringSymbol = "All";
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
	int i = 0;
	const auto pvTiingoIEXTopOFBook = ParseTiingoIEXTopOfBook(pWebData);
	auto lNewestTradeDay = gl_pWorldMarket->GetCurrentTradeDate();
	auto st = gl_pWorldMarket->ToSysTime(toLocalDateTime(lNewestTradeDay, chrono::local_seconds(chrono::seconds(0)))); // 使用当日数据，无论是否是闭市后的数据。
	if (pvTiingoIEXTopOFBook->empty()) return;
	TRACE(_T("Tiingo IEX TopOfBook number: %d\n"), pvTiingoIEXTopOFBook->size());
	for (auto& IEXTopOFBook : *pvTiingoIEXTopOFBook) {
		if (IEXTopOFBook.m_timeStamp < st) continue; // 只使用不早于一天的实时数据
		if (!gl_dataContainerTiingoStock.IsSymbol(IEXTopOFBook.m_strTicker)) continue; // 只更新已有代码
		auto pTiingoStock = gl_dataContainerTiingoStock.GetStock(IEXTopOFBook.m_strTicker);
		pTiingoStock->UpdateRTData(IEXTopOFBook);
		i++;
	}
	TRACE("Tiingo IEX active number: %d\n", i);
	if (gl_pWorldMarket->IsMarketClosed()) {
		gl_pWorldMarket->SetEndMarketIEXTopOfBookUpdate(true);
		gl_pWorldMarket->AddTask(WORLD_MARKET_TIINGO_BUILD_TODAY_STOCK_DAYLINE__, GetNextTime(gl_pWorldMarket->GetMarketTime(), 0h, 2min, 0s)); // 两分钟后处理
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
	auto pvTiingoIEXLastTopOFBook = make_shared<vector<CTiingoIEXTopOfBook>>();
	if (!IsValidData(pWebData)) return pvTiingoIEXLastTopOFBook;

	try {
		string s1;
		stringstream ss;
		string_view svJson = pWebData->GetStringView();
		ondemand::parser parser;
		const simdjson::padded_string jsonPadded(svJson);
		ondemand::document doc = parser.iterate(jsonPadded).value();

		int iCount = 0;
		for (auto item : doc) {
			CTiingoIEXTopOfBook IEXLastTopOFBook;
			CTiingoStock stock;
			auto itemValue = item.value();
			try {
				s1 = simdjsonGetStringView(itemValue, "ticker");
				IEXLastTopOFBook.m_strTicker = s1;
				s1 = simdjsonGetStringView(itemValue, "timestamp");
				ss.clear();
				ss.str(s1);
				chrono::from_stream(ss, "%FT%T%Ez", IEXLastTopOFBook.m_timeStamp);
				s1 = simdjsonGetStringView(itemValue, "lastSaleTimestamp");
				ss.clear();
				ss.str(s1);
				chrono::from_stream(ss, "%FT%T%0z", IEXLastTopOFBook.m_lastSale);
				s1 = simdjsonGetStringView(itemValue, "quoteTimestamp");
				ss.clear();
				ss.str(s1);
				chrono::from_stream(ss, "%FT%T%0z", IEXLastTopOFBook.m_quote);
			} catch (simdjson_error& error) {
				ReportJSonErrorToSystemMessage("Tiingo IEX Top of Book ", error.what());
			}

			IEXLastTopOFBook.m_lHigh = simdjsonGetDouble(itemValue, "high") * stock.GetRatio();
			IEXLastTopOFBook.m_lLow = simdjsonGetDouble(itemValue, "low") * stock.GetRatio();
			IEXLastTopOFBook.m_lLastClose = simdjsonGetDouble(itemValue, "prevClose") * stock.GetRatio();
			IEXLastTopOFBook.m_lOpen = simdjsonGetDouble(itemValue, "open") * stock.GetRatio();
			IEXLastTopOFBook.m_lNew = simdjsonGetDouble(itemValue, "last") * stock.GetRatio();
			try {
				IEXLastTopOFBook.m_llVolume = simdjsonGetDouble(itemValue, "volume");
			} catch (simdjson_error& error) {
				ReportJSonErrorToSystemMessage("Tiingo IEX Top of Book ", error.what());
			}

			pvTiingoIEXLastTopOFBook->push_back(IEXLastTopOFBook);
			IEXLastTopOFBook.Reset();
			iCount++;
		}
	} catch (simdjson_error& error) {
		ReportJSonErrorToSystemMessage("Tiingo IEX Top of Book ", error.what());
	}

	return pvTiingoIEXLastTopOFBook;
}

void CProductTiingoIEXTopOfBook::UpdateSystemStatus() {
	gl_pTiingoDataSource->SetUpdateIEXTopOfBook(false);
	gl_systemMessage.PushInformationMessage("Tiingo IEX top of book Updated");
	if (gl_pWorldMarket->IsMarketClosed()) { // 已闭市？
		gl_pTiingoDataSource->SetEndMarketIEXTopOfBookUpdate(true); // 本交易日数据是完整的。
	}
}
