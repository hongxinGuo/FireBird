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
		//TRACE("����%s��������\n", pForexSymbol->GetSymbol().GetBuffer());
		return;
	}
	else {
		pForexSymbol->SetUpdateDayLineDB(false);
		pForexSymbol->SetUpdateProfileDB(false);
		//TRACE("����%s��������\n", pForexSymbol->GetSymbol().GetBuffer());
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
			// û���������ݣ�������˹�Ʊ�����ߺ�ʵʱ����
			return pvDayLine;
		}
		if (s != _T("ok")) {
			gl_systemMessage.PushErrorMessage(_T("���߷���ֵ��Ϊok"));
			return pvDayLine;
		}
	} catch (nlohmann::ordered_json::exception& e) {
		// ��������Ǵ˴���������⡣�����������"error":"you don't have access this resource."
		ReportJSonErrorToSystemMessage(_T("Finnhub Forex Candleû��s��"), e.what());
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
		// ��Щ��㽻�ײ��ṩ�ɽ��������ԾͿ�����
	}
	std::ranges::sort(pvDayLine->begin(), pvDayLine->end(), CompareDayLineDate);

	return pvDayLine;
}
