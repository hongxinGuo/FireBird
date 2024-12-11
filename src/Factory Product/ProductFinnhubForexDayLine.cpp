#include "pch.h"

#include"globedef.h"
#include"WorldMarket.h"
#include "ProductFinnhubForexDayLine.h"

import FireBird.System.Message;

import FireBird.Accessory.JsonParse;
import FireBird.Accessory.JsonGetValue;

import FireBird.Stock.FinnhubForex;


CProductFinnhubForexDayLine::CProductFinnhubForexDayLine() {
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/forex/candle?symbol=");
}

CString CProductFinnhubForexDayLine::CreateMessage() {
	const auto pForexSymbol = gl_dataFinnhubForexSymbol.GetSymbol(m_lIndex);

	m_strInquiringExchange = pForexSymbol->GetExchangeCode();
	m_strInquiry = m_strInquiryFunction + pForexSymbol->GetFinnhubDayLineInquiryParam(GetUTCTime());
	return m_strInquiry;
}

void CProductFinnhubForexDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(std::strcmp(typeid(*GetMarket()).name(), _T("class CWorldMarket")) == 0);

	const auto pForexSymbol = gl_dataFinnhubForexSymbol.GetSymbol(m_lIndex);
	const CDayLinesPtr pvDayLine = ParseFinnhubForexCandle(pWebData);
	pForexSymbol->SetUpdateDayLine(false);
	if (!pvDayLine->empty()) {
		for (const auto& pDayLine : *pvDayLine) {
			pDayLine->SetExchange(pForexSymbol->GetExchangeCode());
			pDayLine->SetStockSymbol(pForexSymbol->GetSymbol());
			const long lTemp = GetMarket()->ConvertToDate(pDayLine->GetMarketTime());
			pDayLine->SetDate(lTemp);
		}
		pForexSymbol->SetIPOStatus(_STOCK_IPOED_);
		pForexSymbol->UpdateDayLine(*pvDayLine);
		pForexSymbol->SetUpdateDayLineDB(true);
		pForexSymbol->SetUpdateProfileDB(true);
		//TRACE("处理%s日线数据\n", pForexSymbol->GetSymbol().GetBuffer());
		return;
	}
	else {
		pForexSymbol->SetUpdateDayLineDB(false);
		pForexSymbol->SetUpdateProfileDB(false);
		//TRACE("处理%s日线数据\n", pForexSymbol->GetSymbol().GetBuffer());
	}
}

CDayLinesPtr CProductFinnhubForexDayLine::ParseFinnhubForexCandle(CWebDataPtr pWebData) {
	auto pvDayLine = make_shared<vector<CDayLinePtr>>();
	nlohmann::ordered_json js2;
	CDayLinePtr pDayLine = nullptr;
	string sError;
	nlohmann::ordered_json js;

	if (!pWebData->CreateJson(js)) return pvDayLine;
	if (!IsValidData(pWebData)) return pvDayLine;

	try {
		auto s = jsonGetString(js, _T("s"));
		if (s == _T("no_data")) {
			// 没有日线数据，无需检查此股票的日线和实时数据
			return pvDayLine;
		}
		if (s != _T("ok")) {
			gl_systemMessage.PushErrorMessage(_T("日线返回值不为ok"));
			return pvDayLine;
		}
	} catch (nlohmann::ordered_json::exception& e) {
		// 这种请况是此代码出现问题。如服务器返回"error":"you don't have access this resource."
		ReportJSonErrorToSystemMessage(_T("Finnhub Forex Candle没有s项"), e.what());
		return pvDayLine;
	}
	try {
		time_t tTemp;
		long lTemp;
		js2 = jsonGetChild(js, _T("t"));
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			tTemp = jsonGetLongLong(it);
			pDayLine = make_shared<CDayLine>();
			pDayLine->SetTime(tTemp);
			lTemp = gl_pWorldMarket->ConvertToDate(tTemp);
			pDayLine->SetDate(lTemp);
			pvDayLine->push_back(pDayLine);
		}
	} catch (nlohmann::ordered_json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Forex Candle missing 't' "), e.what());
		return pvDayLine;
	}
	try {
		int i;
		INT64 llTemp;
		double dTemp;
		js2 = jsonGetChild(js, _T("c"));
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetClose(dTemp * 1000);
		}
		js2 = jsonGetChild(js, _T("h"));
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetHigh(dTemp * 1000);
		}
		js2 = jsonGetChild(js, _T("l"));
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetLow(dTemp * 1000);
		}
		js2 = jsonGetChild(js, _T("o"));
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetOpen(dTemp * 1000);
		}
		js2 = jsonGetChild(js, _T("v"));
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			llTemp = jsonGetLongLong(it);
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetVolume(llTemp);
		}
	} catch (nlohmann::ordered_json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Forex Candle missing 'v' "), e.what());
		// 有些外汇交易不提供成交量，忽略就可以了
	}
	std::ranges::sort(pvDayLine->begin(), pvDayLine->end(), CompareDayLineDate);

	return pvDayLine;
}
