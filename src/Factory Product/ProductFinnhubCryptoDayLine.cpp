#include "pch.h"

#include"TimeConvert.h"
#include"jsonParse.h"
#include"JsonGetValue.h"

#include"WorldMarket.h"
#include"FinnhubCrypto.h"

#include "ProductFinnhubCryptoDayLine.h"

CProductFinnhubCryptoDayLine::CProductFinnhubCryptoDayLine() {
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/crypto/candle?symbol=");
}

CString CProductFinnhubCryptoDayLine::CreateMessage() {
	const auto pCryptoSymbol = gl_dataFinnhubCryptoSymbol.GetSymbol(m_lIndex);

	m_strInquiringExchange = pCryptoSymbol->GetExchangeCode();
	m_strInquiry = m_strInquiryFunction + pCryptoSymbol->GetFinnhubDayLineInquiryParam(GetUTCTime());
	return m_strInquiry;
}

void CProductFinnhubCryptoDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(std::strcmp(typeid(*GetMarket()).name(), _T("class CWorldMarket")) == 0);

	const auto pCryptoSymbol = gl_dataFinnhubCryptoSymbol.GetSymbol(m_lIndex);
	const auto pvDayLine = ParseFinnhubCryptoCandle(pWebData);
	pCryptoSymbol->SetUpdateDayLine(false);
	if (!pvDayLine->empty()) {
		for (const auto& pDayLine : *pvDayLine) {
			pDayLine->SetExchange(pCryptoSymbol->GetExchangeCode());
			pDayLine->SetStockSymbol(pCryptoSymbol->GetSymbol());
			const long lTemp = ConvertToDate(pDayLine->m_time, GetMarket()->GetMarketTimeZone());
			pDayLine->SetDate(lTemp);
		}
		pCryptoSymbol->UpdateDayLine(*pvDayLine);
		pCryptoSymbol->UpdateDayLineStartEndDate();
		if (gl_pWorldMarket->GetLastTradeDate() <= pCryptoSymbol->GetDayLineEndDate()) { pCryptoSymbol->SetIPOStatus(_STOCK_IPOED_); }
		else { pCryptoSymbol->SetIPOStatus(_STOCK_DELISTED_); }
		pCryptoSymbol->SetUpdateDayLineDB(true);
		pCryptoSymbol->SetUpdateProfileDB(true);
	}
	else {
		pCryptoSymbol->SetUpdateDayLineDB(false);
		if (!pCryptoSymbol->IsDelisted()) {
			pCryptoSymbol->SetIPOStatus(_STOCK_DELISTED_);
			pCryptoSymbol->SetUpdateProfileDB(true);
		}
	}
}

CDayLinesPtr CProductFinnhubCryptoDayLine::ParseFinnhubCryptoCandle(CWebDataPtr pWebData) {
	auto pvDayLine = make_shared<vector<CDayLinePtr>>();
	auto pvDayLineReturn = make_shared<vector<CDayLinePtr>>();
	json js2;
	CDayLinePtr pDayLine = nullptr;
	string sError;
	json js;

	if (!pWebData->CreateJson(js)) return pvDayLine;
	if (!IsValidData(pWebData)) return pvDayLine;

	try {
		string s;
		s = jsonGetString(js, _T("s"));
		if (s == _T("no_data")) {
			// û���������ݣ�������˹�Ʊ�����ߺ�ʵʱ����
			return pvDayLine;
		}
		if (s != _T("ok")) {
			gl_systemMessage.PushErrorMessage(_T("���߷���ֵ��Ϊok"));
			return pvDayLine;
		}
	} catch (json::exception&) {
		// ��������Ǵ˴���������⡣�����������"error":"you don't have access this resource."
		ReportJSonErrorToSystemMessage(_T("Finnhub Crypto Candle missing 's': "), pWebData->GetDataBuffer().c_str());
		return pvDayLine;
	}
	try {
		time_t tTemp = 0;
		js2 = jsonGetChild(js, _T("t"));
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			tTemp = it->get<INT64>();
			pDayLine = make_shared<CDayLine>();
			pDayLine->SetTime(tTemp);
			pvDayLine->push_back(pDayLine);
		}
	} catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Crypto Candle missing 't' "), e.what());
		return pvDayLine;
	}
	try {
		int i = 0;
		INT64 llTemp;
		double dTemp;
		js2 = jsonGetChild(js, _T("c"));
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetClose(static_cast<long>(dTemp * 1000));
		}
		js2 = jsonGetChild(js, _T("h"));
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetHigh(static_cast<long>(1000 * dTemp));
		}
		js2 = jsonGetChild(js, _T("l"));
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetLow(static_cast<long>(1000 * dTemp));
		}
		js2 = jsonGetChild(js, _T("o"));
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetOpen(static_cast<long>(1000 * dTemp));
		}
		js2 = jsonGetChild(js, _T("v"));
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			llTemp = jsonGetLongLong(it);
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetVolume(llTemp);
		}
	} catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Crypto Candle "), e.what());
		// ��Щ��㽻�ײ��ṩ�ɽ��������ԾͿ�����
	}
	std::ranges::sort(pvDayLine->begin(), pvDayLine->end(), CompareDayLineDate);
	// �������������Ϊ�����Ч����
	for (auto& pDayLine2 : *pvDayLine) { if (pDayLine2->m_time > 0) { pvDayLineReturn->push_back(pDayLine2); } }
	return pvDayLineReturn;
}
