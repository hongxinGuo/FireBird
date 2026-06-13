#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include"WorldMarket.h"
#include"FinnhubForex.h"

#include "ProductFinnhubForexDayLine.h"

#include "WebData.h"

CProductFinnhubForexDayLine::CProductFinnhubForexDayLine() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/forex/candle?symbol=";
}

string CProductFinnhubForexDayLine::CreateMessage() {
	const auto pForexSymbol = gl_dataFinnhubForexSymbol.GetItem(m_lIndex);

	m_strInquiringExchange = pForexSymbol->GetExchange();
	m_strInquiry = m_strInquiryFunction + pForexSymbol->GetFinnhubDayLineInquiryParam(GetUTCTime());
	return m_strInquiry;
}

void CProductFinnhubForexDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pForexSymbol = gl_dataFinnhubForexSymbol.GetItem(m_lIndex);
	const CDayLinesPtr pvDayLine = ParseFinnhubForexCandle(pWebData);
	pForexSymbol->SetUpdateDayLine(false);
	if (!pvDayLine->empty()) {
		for (auto& dayLine : *pvDayLine) {
			dayLine.SetExchange(pForexSymbol->GetExchange());
			dayLine.SetStockSymbol(pForexSymbol->GetSymbol());
		}
		pForexSymbol->UpdateDayLine(pvDayLine);
		pForexSymbol->SetUpdateDayLineDB(true);
		pForexSymbol->SetUpdateProfileDB(true);
		//TRACE(_T("处理%s日线数据\n"), pForexSymbol->GetSymbol().GetBuffer());
		return;
	}
	else {
		pForexSymbol->SetUpdateDayLineDB(false);
		pForexSymbol->SetUpdateProfileDB(false);
		//TRACE(_T("处理%s日线数据\n"), pForexSymbol->GetSymbol().GetBuffer());
	}
}

CDayLinesPtr CProductFinnhubForexDayLine::ParseFinnhubForexCandle(const CWebDataPtr& pWebData) {
	auto pvDayLine = make_shared<vector<CDayLine>>();
	pvDayLine->reserve(1000); // 预先分配空间，避免频繁扩容。一般来说，外汇的日线数据不会超过1000条。

	nlohmannJson js2;
	string sError;
	nlohmannJson js;

	if (!pWebData->CreateJson(js)) return pvDayLine;
	if (!IsValidData(pWebData)) return pvDayLine;

	try {
		auto s = jsonGetString(js, "s");
		if (s == "no_data") {
			// 没有日线数据，无需检查此股票的日线和实时数据
			return pvDayLine;
		}
		if (s != "ok") {
			gl_systemMessage.PushErrorMessage("日线返回值不为ok");
			return pvDayLine;
		}
	} catch (nlohmannJson::exception& e) {
		// 这种请况是此代码出现问题。如服务器返回"error":"you don't have access this resource."
		ReportJSonErrorToSystemMessage("Finnhub Forex Candle没有s项", e.what());
		return pvDayLine;
	}
	try {
		CDayLine dayLine;
		time_t tTemp;
		long lTemp;
		js2 = jsonGetChild(js, "t");
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			tTemp = jsonGetLongLong(it);
			chrono::local_seconds tp{ chrono::seconds{ tTemp } };
			dayLine.SetDate(tp);
			pvDayLine->push_back(dayLine);
			dayLine.Reset();
		}
	} catch (nlohmannJson::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub Forex Candle missing 't' ", e.what());
		return pvDayLine;
	}
	try {
		int i;
		INT64 llTemp;
		double dTemp;
		js2 = jsonGetChild(js, "c");
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pvDayLine->at(i++).SetClose(dTemp * 1000);
		}
		js2 = jsonGetChild(js, "h");
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pvDayLine->at(i++).SetHigh(dTemp * 1000);
		}
		js2 = jsonGetChild(js, "l");
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pvDayLine->at(i++).SetLow(dTemp * 1000);
		}
		js2 = jsonGetChild(js, "o");
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pvDayLine->at(i++).SetOpen(dTemp * 1000);
		}
		js2 = jsonGetChild(js, "v");
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			llTemp = jsonGetLongLong(it);
			pvDayLine->at(i++).SetVolume(llTemp);
		}
	} catch (nlohmannJson::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub Forex Candle missing 'v' ", e.what());
		// 有些外汇交易不提供成交量，忽略就可以了
	}
	std::ranges::sort(pvDayLine->begin(), pvDayLine->end(), CompareDayLineDate);

	return pvDayLine;
}
