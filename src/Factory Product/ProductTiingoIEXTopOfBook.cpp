//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Tiingo IEX last top of book��
///
/// Ŀǰÿ���ܹ����µ�����������2�������ڹ�Ʊ�����еĴ���Ϊ��ǧ�ࡣ
/// ���к����ʹ�ô����������µ�����������ݡ�
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

string CProductTiingoIEXTopOfBook::CreateMessage() {
	m_strInquiringSymbol = _T("All");
	m_strInquiry = m_strInquiryFunction;
	return m_strInquiry;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
//
// tiingo IEX top of book����������еĹ�Ʊ������������ݣ�����ʹ���������ݣ�ֻ����Ŀǰ��Ծ�Ĺ�Ʊ��
//
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CProductTiingoIEXTopOfBook::ParseAndStoreWebData(CWebDataPtr pWebData) {
	int i = 0;
	const auto pvTiingoIEXTopOFBook = ParseTiingoIEXTopOfBook(pWebData);
	long lNewestTradeDay = gl_pWorldMarket->GetCurrentTradeDate();
	time_t ttNewestTradeDay = gl_pWorldMarket->TransferToUTCTime(lNewestTradeDay, 0); //ʹ�õ�������
	if (pvTiingoIEXTopOFBook->empty()) return;
	TRACE("Tiingo IEX TopOfBook number: %d\n", pvTiingoIEXTopOFBook->size());
	for (const auto& pIEXTopOFBook : *pvTiingoIEXTopOFBook) {
		if (pIEXTopOFBook->m_timeStamp.time_since_epoch().count() < ttNewestTradeDay) continue; // ֻʹ�ò�����һ���ʵʱ����
		if (!gl_dataContainerTiingoStock.IsSymbol(pIEXTopOFBook->m_strTicker)) continue; // ֻ�������д���
		auto pTiingoStock = gl_dataContainerTiingoStock.GetStock(pIEXTopOFBook->m_strTicker);
		pTiingoStock->UpdateRTData(pIEXTopOFBook);
		i++;
	}
	TRACE("Tiingo IEX active number: %d\n", i);
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
	CTiingoIEXTopOfBookPtr pIEXLastTopOFBook = nullptr;
	string s1;
	if (!IsValidData(pWebData)) return pvTiingoIEXLastTopOFBook;

	try {
		stringstream ss;
		string_view svJson = pWebData->GetStringView();
		ondemand::parser parser;
		const simdjson::padded_string jsonPadded(svJson);
		ondemand::document doc = parser.iterate(jsonPadded).value();

		int iCount = 0;
		for (auto item : doc) {
			CTiingoStock stock;
			auto itemValue = item.value();
			pIEXLastTopOFBook = nullptr;
			pIEXLastTopOFBook = make_shared<CTiingoIEXTopOfBook>();
			s1 = simdjsonGetStringView(itemValue, _T("ticker"));
			pIEXLastTopOFBook->m_strTicker = s1;
			s1 = simdjsonGetStringView(itemValue, _T("timestamp"));
			ss.clear();
			ss.str(s1);
			chrono::from_stream(ss, _T("%FT%T%Ez"), pIEXLastTopOFBook->m_timeStamp);
			s1 = simdjsonGetStringView(itemValue, _T("lastSaleTimestamp"));
			ss.clear();
			ss.str(s1);
			chrono::from_stream(ss, "%FT%T%0z", pIEXLastTopOFBook->m_lastSale);
			s1 = simdjsonGetStringView(itemValue, _T("quoteTimestamp"));
			ss.clear();
			ss.str(s1);
			chrono::from_stream(ss, "%FT%T%0z", pIEXLastTopOFBook->m_quote);

			pIEXLastTopOFBook->m_lHigh = simdjsonGetDouble(itemValue, _T("high")) * stock.GetRatio();
			pIEXLastTopOFBook->m_lLow = simdjsonGetDouble(itemValue, _T("low")) * stock.GetRatio();
			pIEXLastTopOFBook->m_lLastClose = simdjsonGetDouble(itemValue, _T("prevClose")) * stock.GetRatio();
			pIEXLastTopOFBook->m_lOpen = simdjsonGetDouble(itemValue, _T("open")) * stock.GetRatio();
			pIEXLastTopOFBook->m_lNew = simdjsonGetDouble(itemValue, _T("last")) * stock.GetRatio();
			pIEXLastTopOFBook->m_llVolume = simdjsonGetInt64(itemValue, _T("volume"));

			pvTiingoIEXLastTopOFBook->push_back(pIEXLastTopOFBook);
			iCount++;
		}
	} catch (simdjson_error& error) {
		ReportJSonErrorToSystemMessage(_T("Tiingo market news "), error.what());
	}

	return pvTiingoIEXLastTopOFBook;
}

void CProductTiingoIEXTopOfBook::UpdateSystemStatus(CVirtualDataSourcePtr pDataSource) {
	ASSERT(strcmp(typeid(*pDataSource).name(), _T("class CTiingoDataSource")) == 0);
	dynamic_pointer_cast<CTiingoDataSource>(pDataSource)->SetUpdateIEXTopOfBook(false);
	gl_systemMessage.PushInformationMessage(_T("Tiingo IEX top of book Updated"));
	if (gl_pWorldMarket->IsMarketClosed()) { // �ѱ��У�
		dynamic_pointer_cast<CTiingoDataSource>(pDataSource)->SetEndMarketIEXTopOfBookUpdate(true); // �������������������ġ�
	}
}
