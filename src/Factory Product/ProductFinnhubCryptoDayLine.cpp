#include "pch.h"

#include"TimeConvert.h"
#include"jsonParse.h"
#include"JsonGetValue.h"

#include"WorldMarket.h"
#include"FinnhubCryptoSymbol.h"
#include"CallableFunction.h"

#include "ProductFinnhubCryptoDayLine.h"

using namespace std;

CProductFinnhubCryptoDayLine::CProductFinnhubCryptoDayLine() {
	m_strClassName = _T("Finnhub crypto dayline");
	m_strInquiry = _T("https://finnhub.io/api/v1/crypto/candle?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubCryptoDayLine::CreateMessage(void) {
		ASSERT(std::strcmp(typeid(*m_pMarket).name(), _T("class CWorldMarket")) == 0);
	const auto pCryptoSymbol = dynamic_cast<CWorldMarket*>(m_pMarket)->GetFinnhubCryptoSymbol(m_lIndex);

	m_strInquiringExchange = pCryptoSymbol->GetExchangeCode();
	m_strTotalInquiryMessage = m_strInquiry + pCryptoSymbol->GetFinnhubDayLineInquiryString(CWorldMarket::GetUTCTime());
	return m_strTotalInquiryMessage;
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
			const long lTemp = ConvertToDate(pDayLine->m_time, ((CWorldMarket*)m_pMarket)->GetMarketTimeZone());
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
			// û���������ݣ�������˹�Ʊ�����ߺ�ʵʱ����
			return pvDayLine;
		}
		if (s != _T("ok")) {
			gl_systemMessage.PushErrorMessage(_T("���߷���ֵ��Ϊok"));
			return pvDayLine;
		}
	}
	catch (json::exception&) {
		// ��������Ǵ˴���������⡣�����������"error":"you don't have access this resource."
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
		// ��Щ��㽻�ײ��ṩ�ɽ��������ԾͿ�����
	}
	ranges::sort(pvDayLine->begin(), pvDayLine->end(), CompareDayLineDate);
	// �������������Ϊ�����Ч����
	for (auto& pDayLine2 : *pvDayLine) { if (pDayLine2->m_time > 0) { pvDayLineReturn->push_back(pDayLine2); } }
	return pvDayLineReturn;
}