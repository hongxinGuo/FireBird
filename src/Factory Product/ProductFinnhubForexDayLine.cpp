#include "pch.h"

#include"TimeConvert.h"
#include"jsonParse.h"
#include"JsonGetValue.h"

#include"WorldMarket.h"
#include"FinnhubForexSymbol.h"

#include "ProductFinnhubForexDayLine.h"

#include <execution>

using namespace std;

CProductFinnhubForexDayLine::CProductFinnhubForexDayLine() {
	m_strClassName = _T("Finnhub forex dayline");
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/forex/candle?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubForexDayLine::CreateMessage() {
	const auto pForexSymbol = gl_dataFinnhubForexSymbol.GetSymbol(m_lIndex);

	m_strInquiringExchange = pForexSymbol->GetExchangeCode();
	m_strInquiry = m_strInquiryFunction + pForexSymbol->GetFinnhubDayLineInquiryParam(GetUTCTime());
	return m_strInquiry;
}

bool CProductFinnhubForexDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(std::strcmp(typeid(*GetMarket()).name(), _T("class CWorldMarket")) == 0);

	const auto pForexSymbol = gl_dataFinnhubForexSymbol.GetSymbol(m_lIndex);
	const CDayLineVectorPtr pvDayLine = ParseFinnhubForexCandle(pWebData);
	pForexSymbol->SetDayLineNeedUpdate(false);
	if (!pvDayLine->empty()) {
		for (const auto& pDayLine : *pvDayLine) {
			pDayLine->SetExchange(pForexSymbol->GetExchangeCode());
			pDayLine->SetStockSymbol(pForexSymbol->GetSymbol());
			const long lTemp = ConvertToDate(pDayLine->m_time, GetMarket()->GetMarketTimeZone());
			pDayLine->SetDate(lTemp);
		}
		pForexSymbol->SetIPOStatus(_STOCK_IPOED_);
		pForexSymbol->UpdateDayLine(*pvDayLine);
		pForexSymbol->SetDayLineNeedSaving(true);
		pForexSymbol->SetUpdateProfileDB(true);
		TRACE("����%s��������\n", pForexSymbol->GetSymbol().GetBuffer());
		return true;
	}
	pForexSymbol->SetDayLineNeedSaving(false);
	pForexSymbol->SetUpdateProfileDB(false);
	TRACE("����%s��������\n", pForexSymbol->GetSymbol().GetBuffer());
	return false;
}

CDayLineVectorPtr CProductFinnhubForexDayLine::ParseFinnhubForexCandle(CWebDataPtr pWebData) {
	auto pvDayLine = make_shared<vector<CDayLinePtr>>();
	json js2;
	CDayLinePtr pDayLine = nullptr;
	string sError;
	json js;

	if (!pWebData->CreateJson(js)) return pvDayLine;
	if (!IsValidData(pWebData)) return pvDayLine;

	try {
		auto s = jsonGetString(&js, _T("s"));
		if (s == _T("no_data")) {
			// û���������ݣ�������˹�Ʊ�����ߺ�ʵʱ����
			return pvDayLine;
		}
		if (s != _T("ok")) {
			gl_systemMessage.PushErrorMessage(_T("���߷���ֵ��Ϊok"));
			return pvDayLine;
		}
	}
	catch (json::exception& e) {
		// ��������Ǵ˴���������⡣�����������"error":"you don't have access this resource."
		ReportJSonErrorToSystemMessage(_T("Finnhub Forex Candleû��s��"), e.what());
		return pvDayLine;
	}
	try {
		time_t tTemp;
		long lTemp;
		js2 = jsonGetChild(&js, _T("t"));
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			tTemp = jsonGetLongLong(it);
			pDayLine = make_shared<CDayLine>();
			pDayLine->SetTime(tTemp);
			lTemp = ConvertToDate(tTemp, gl_pWorldMarket->GetMarketTimeZone());
			pDayLine->SetDate(lTemp);
			pvDayLine->push_back(pDayLine);
		}
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Forex Candle missing 't' "), e.what());
		return pvDayLine;
	}
	try {
		int i;
		INT64 llTemp;
		double dTemp;
		js2 = jsonGetChild(&js, _T("c"));
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetClose(dTemp * 1000);
		}
		js2 = jsonGetChild(&js, _T("h"));
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetHigh(dTemp * 1000);
		}
		js2 = jsonGetChild(&js, _T("l"));
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetLow(dTemp * 1000);
		}
		js2 = jsonGetChild(&js, _T("o"));
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetOpen(dTemp * 1000);
		}
		js2 = jsonGetChild(&js, _T("v"));
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			llTemp = jsonGetLongLong(it);
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetVolume(llTemp);
		}
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Forex Candle missing 'v' "), e.what());
		// ��Щ��㽻�ײ��ṩ�ɽ��������ԾͿ�����
	}
	ranges::sort(pvDayLine->begin(), pvDayLine->end(), CompareDayLineDate);

	return pvDayLine;
}
