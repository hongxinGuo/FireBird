#include "pch.h"

#include"TimeConvert.h"
#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include"WorldMarket.h"
#include"FinnhubStock.h"

#include "ProductFinnhubStockDayLine.h"

#include "WebData.h"

CProductFinnhubStockDayLine::CProductFinnhubStockDayLine() {
	m_strInquiryFunction = "https://finnhub.io/api/v1/stock/candle?symbol=";
}

string CProductFinnhubStockDayLine::CreateMessage() {
	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_lIndex);
	const auto strParam = pStock->GetFinnhubDayLineInquiryParam(GetUTCTime());

	m_strInquiringExchange = pStock->GetExchangeCode();
	m_strInquiry = m_strInquiryFunction + strParam;
	return m_strInquiry;
}

void CProductFinnhubStockDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pStock = gl_dataContainerFinnhubStock.GetItem(m_lIndex);
	const auto pvDayLine = ParseFinnhubStockCandle(pWebData);
	pStock->SetUpdateDayLine(false);
	long lastClose = 0;
	for (auto& dayLine : *pvDayLine) {
		dayLine.SetExchange(pStock->GetExchangeCode());
		dayLine.SetStockSymbol(pStock->GetSymbol());
		const auto lTemp = gl_pWorldMarket->ConvertToDate(dayLine.GetMarketTime());
		dayLine.SetDate(lTemp);
		if ((lastClose != 0) && (dayLine.GetLastClose() == 0)) dayLine.SetLastClose(lastClose);
		lastClose = dayLine.GetClose();
	}
	if (!pvDayLine->empty()) {
		pStock->UpdateDayLine(pvDayLine);
		if (pStock->GetDayLineSize() > 0) {// 添加了新数据
			pStock->SetUpdateDayLineDB(true);
			pStock->SetUpdateProfileDB(true);
			const auto lSize = pStock->GetDayLineSize() - 1;
			const auto pDayLine = pStock->GetDayLine(lSize);
			if (!IsEarlyThen(pDayLine->GetDate(), gl_pWorldMarket->GetMarketDate(), 100)) {
				pStock->SetIPOStatus(_STOCK_IPOED_);
			}
			return;
		}
	}
}

CDayLinesPtr CProductFinnhubStockDayLine::ParseFinnhubStockCandle(const CWebDataPtr& pWebData) {
	auto pvDayLine = make_shared<vector<CDayLine>>();
	pvDayLine->reserve(7500); // 预先分配空间，避免频繁扩容。一般来说，日线数据不会超过1000条。

	nlohmannJson js2;
	CDayLine pDayLine;
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
		ReportJSonErrorToSystemMessage("Finnhub Stock Candle missing 's' " + GetInquiryFunction(), e.what());
		return pvDayLine;
	}

	try {
		time_t tTemp;
		js2 = jsonGetChild(js, "t");
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			tTemp = jsonGetLongLong(it);
			pDayLine.SetTime(tTemp);
			pvDayLine->push_back(pDayLine);
		}
	} catch (nlohmannJson::exception& e) {
		ReportJSonErrorToSystemMessage("Finnhub Stock Candle missing 't' " + GetInquiryFunction(), e.what());
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
		js2 = jsonGetChild(js, "o");
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pvDayLine->at(i++).SetOpen(dTemp * 1000);
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
		js2 = jsonGetChild(js, "v");
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			llTemp = jsonGetLongLong(it);
			pvDayLine->at(i++).SetVolume(llTemp);
		}
	} catch (nlohmannJson::exception& e) { ReportJSonErrorToSystemMessage("Finnhub Stock Candle Error#3 " + GetInquiryFunction(), e.what()); }
	std::ranges::sort(pvDayLine->begin(), pvDayLine->end(), CompareDayLineDate); // 以日期早晚顺序排列。

	return pvDayLine;
}
