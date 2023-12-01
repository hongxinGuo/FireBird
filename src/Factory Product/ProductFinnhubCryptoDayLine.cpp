#include "pch.h"

#include"TimeConvert.h"
#include"jsonParse.h"
#include"JsonGetValue.h"

#include"WorldMarket.h"
#include"FinnhubCryptoSymbol.h"

#include "ProductFinnhubCryptoDayLine.h"

using namespace std;

CProductFinnhubCryptoDayLine::CProductFinnhubCryptoDayLine() {
	m_strClassName = _T("Finnhub crypto dayline");
	m_strInquiryFunction = _T("https://finnhub.io/api/v1/crypto/candle?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubCryptoDayLine::CreateMessage() {
	ASSERT(std::strcmp(typeid(*m_pMarket).name(), _T("class CWorldMarket")) == 0);
	const auto pCryptoSymbol = dynamic_cast<CWorldMarket*>(m_pMarket)->GetFinnhubCryptoSymbol(m_lIndex);

	m_strInquiringExchange = pCryptoSymbol->GetExchangeCode();
	m_strInquiry = m_strInquiryFunction + pCryptoSymbol->GetFinnhubDayLineInquiryParam(GetUTCTime());
	return m_strInquiry;
}

bool CProductFinnhubCryptoDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(std::strcmp(typeid(*m_pMarket).name(), _T("class CWorldMarket")) == 0);
	bool fStatus = true;

	const auto pCryptoSymbol = dynamic_cast<CWorldMarket*>(m_pMarket)->GetFinnhubCryptoSymbol(m_lIndex);
	const auto pvDayLine = ParseFinnhubCryptoCandle(pWebData);
	pCryptoSymbol->SetDayLineNeedUpdate(false);
	if (!pvDayLine->empty()) {
		for (const auto& pDayLine : *pvDayLine) {
			pDayLine->SetExchange(pCryptoSymbol->GetExchangeCode());
			pDayLine->SetStockSymbol(pCryptoSymbol->GetSymbol());
			const long lTemp = ConvertToDate(pDayLine->m_time, dynamic_cast<CWorldMarket*>(m_pMarket)->GetMarketTimeZone());
			pDayLine->SetDate(lTemp);
		}
		pCryptoSymbol->UpdateDayLine(*pvDayLine);
		pCryptoSymbol->UpdateDayLineStartEndDate();
		if (gl_pWorldMarket->GetLastTradeDate() <= pCryptoSymbol->GetDayLineEndDate()) { pCryptoSymbol->SetIPOStatus(_STOCK_IPOED_); }
		else { pCryptoSymbol->SetIPOStatus(_STOCK_DELISTED_); }
		pCryptoSymbol->SetDayLineNeedSaving(true);
		pCryptoSymbol->SetUpdateProfileDB(true);
	}
	else {
		pCryptoSymbol->SetDayLineNeedSaving(false);
		if (!pCryptoSymbol->IsDelisted()) {
			pCryptoSymbol->SetIPOStatus(_STOCK_DELISTED_);
			pCryptoSymbol->SetUpdateProfileDB(true);
			fStatus = true;
		}
		else { fStatus = false; }
	}
	return fStatus;
}

CDayLineVectorPtr CProductFinnhubCryptoDayLine::ParseFinnhubCryptoCandle(CWebDataPtr pWebData) {
	auto pvDayLine = make_shared<vector<CDayLinePtr>>();
	auto pvDayLineReturn = make_shared<vector<CDayLinePtr>>();
	json js2;
	CDayLinePtr pDayLine = nullptr;
	string sError;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvDayLine;
	if (pWebData->IsVoidJson()) {
		m_iReceivedDataStatus = VOID_DATA_;
		return pvDayLine;
	}
	if (pWebData->CheckNoRightToAccess()) {
		m_iReceivedDataStatus = NO_ACCESS_RIGHT_;
		return pvDayLine;
	}

	const auto pjs = pWebData->GetJSon();
	try {
		string s;
		s = jsonGetString(pjs, _T("s"));
		if (s == _T("no_data")) {
			// 没有日线数据，无需检查此股票的日线和实时数据
			return pvDayLine;
		}
		if (s != _T("ok")) {
			gl_systemMessage.PushErrorMessage(_T("日线返回值不为ok"));
			return pvDayLine;
		}
	}
	catch (json::exception&) {
		// 这种请况是此代码出现问题。如服务器返回"error":"you don't have access this resource."
		ReportJSonErrorToSystemMessage(_T("Finnhub Crypto Candle missing 's': "), pWebData->GetDataBuffer().c_str());
		return pvDayLine;
	}
	try {
		time_t tTemp = 0;
		js2 = jsonGetChild(pjs, _T("t"));
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			tTemp = it->get<INT64>();
			pDayLine = make_shared<CDayLine>();
			pDayLine->SetTime(tTemp);
			pvDayLine->push_back(pDayLine);
		}
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Crypto Candle missing 't' "), e.what());
		return pvDayLine;
	}
	try {
		int i = 0;
		INT64 llTemp;
		double dTemp;
		js2 = jsonGetChild(pjs, _T("c"));
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetClose(static_cast<long>(dTemp * 1000));
		}
		js2 = jsonGetChild(pjs, _T("h"));
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetHigh(static_cast<long>(1000 * dTemp));
		}
		js2 = jsonGetChild(pjs, _T("l"));
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetLow(static_cast<long>(1000 * dTemp));
		}
		js2 = jsonGetChild(pjs, _T("o"));
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetOpen(static_cast<long>(1000 * dTemp));
		}
		js2 = jsonGetChild(pjs, _T("v"));
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			llTemp = jsonGetLongLong(it);
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetVolume(llTemp);
		}
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Crypto Candle "), e.what());
		// 有些外汇交易不提供成交量，忽略就可以了
	}
	ranges::sort(pvDayLine->begin(), pvDayLine->end(), CompareDayLineDate);
	// 清除掉交易日期为零的无效数据
	for (auto& pDayLine2 : *pvDayLine) { if (pDayLine2->m_time > 0) { pvDayLineReturn->push_back(pDayLine2); } }
	return pvDayLineReturn;
}
