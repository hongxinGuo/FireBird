//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Tiingo IEX last top of book。
///
///
///
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "pch.h"

#include"jsonParse.h"

#include "ProductTiingoIEXTopOFBook.h"

#include "TiingoDataSource.h"

#include"simdjsonGetValue.h"
#include "TimeConvert.h"
#include "WorldMarket.h"

CProductTiingoIEXTopOfBook::CProductTiingoIEXTopOfBook() {
	m_strInquiryFunction = _T("https://api.tiingo.com/iex?");
}

CString CProductTiingoIEXTopOfBook::CreateMessage() {
	m_strInquiringSymbol = _T("All");
	m_strInquiry = m_strInquiryFunction;
	return m_strInquiry;
}

void CProductTiingoIEXTopOfBook::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pvTiingoIEXTopOFBook = ParseTiingoIEXTopOfBook(pWebData);
	long lNewestTradeDay = gl_pWorldMarket->GetNewestTradeDate();
	time_t ttNewestTradeDay = ConvertToTTime(lNewestTradeDay, 0, 160000); // 美股下午4点收市
	if (!pvTiingoIEXTopOFBook->empty()) {
		for (const auto& pIEXTopOFBook : *pvTiingoIEXTopOFBook) {
			if (pIEXTopOFBook->m_llTimestamp > ttNewestTradeDay) { // 只使用不早于一天的实时数据
				if (gl_dataContainerTiingoStock.IsSymbol(pIEXTopOFBook->m_strTicker)) { // 只更新已有代码
					auto pTiingoStock = gl_dataContainerTiingoStock.GetStock(pIEXTopOFBook->m_strTicker);
					pTiingoStock->UpdateRTData(pIEXTopOFBook);
				}
			}
		}
	}
	gl_systemConfiguration.DecreaseTiingoBandWidth(pWebData->GetBufferLength());
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
	int year, month, day, hour, minute, second;
	int hourOffset, minuteOffset;
	if (!IsValidData(pWebData)) return pvTiingoIEXLastTopOFBook;

	try {
		string_view svJson = pWebData->GetStringView(0, pWebData->GetBufferLength());
		ondemand::parser parser;
		const simdjson::padded_string jsonPadded(svJson);
		ondemand::document doc = parser.iterate(jsonPadded).value();

		CString str;
		int iCount = 0;
		for (auto item : doc) {
			auto itemValue = item.value();
			pIEXLastTopOFBook = make_shared<CTiingoIEXTopOfBook>();
			s1 = jsonGetStringView(itemValue, _T("ticker"));
			pIEXLastTopOFBook->m_strTicker = s1.c_str();
			s1 = jsonGetStringView(itemValue, _T("timestamp"));
			sscanf_s(s1.c_str(), _T("%04d-%02d-%02dT%02d:%02d:%02d+%02d:%02d"), &year, &month, &day, &hour, &minute, &second, &hourOffset, &minuteOffset);
			pIEXLastTopOFBook->m_llTimestamp = ConvertToTTime(year, month, day, hour, minute, second, hourOffset * 100 + minuteOffset);

			pIEXLastTopOFBook->m_lHigh = jsonGetDouble(itemValue, _T("high")) * 1000;
			pIEXLastTopOFBook->m_lLow = jsonGetDouble(itemValue, _T("low")) * 1000;
			pIEXLastTopOFBook->m_lLastClose = jsonGetDouble(itemValue, _T("prevClose")) * 1000;
			pIEXLastTopOFBook->m_lOpen = jsonGetDouble(itemValue, _T("open")) * 1000;
			pIEXLastTopOFBook->m_lNew = jsonGetDouble(itemValue, _T("last")) * 1000;
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
}
