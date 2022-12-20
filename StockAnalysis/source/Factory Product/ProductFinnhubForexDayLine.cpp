#include "pch.h"

#include"TimeConvert.h"
#include"jsonParse.h"
#include"JsonGetValue.h"

#include"WorldMarket.h"
#include"FinnhubForexSymbol.h"
#include"CallableFunction.h"

#include "ProductFinnhubForexDayLine.h"

IMPLEMENT_DYNCREATE(CProductFinnhubForexDayLine, CProductFinnhub)

CProductFinnhubForexDayLine::CProductFinnhubForexDayLine() {
	m_strClassName = _T("Finnhub forex dayline");
	m_strInquiry = _T("https://finnhub.io/api/v1/forex/candle?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubForexDayLine::CreateMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	const auto pForexSymbol = static_cast<CWorldMarket*>(m_pMarket)->GetForexSymbol(m_lIndex);

	m_strInquiringExchange = pForexSymbol->GetExchangeCode();
	m_strTotalInquiryMessage = m_strInquiry + pForexSymbol->GetFinnhubDayLineInquiryString(((CWorldMarket*)m_pMarket)->GetUTCTime());
	return m_strTotalInquiryMessage;
}

bool CProductFinnhubForexDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CDayLineVectorPtr pvDayLine = nullptr;

	const auto pForexSymbol = dynamic_cast<CWorldMarket*>(m_pMarket)->GetForexSymbol(m_lIndex);
	pvDayLine = ParseFinnhubForexCandle(pWebData);
	pForexSymbol->SetDayLineNeedUpdate(false);
	if (!pvDayLine->empty()) {
		for (const auto& pDayLine : *pvDayLine) {
			pDayLine->SetExchange(pForexSymbol->GetExchangeCode());
			pDayLine->SetStockSymbol(pForexSymbol->GetSymbol());
			const long lTemp = TransferToDate(pDayLine->m_time, ((CWorldMarket*)m_pMarket)->GetMarketTimeZone());
			pDayLine->SetDate(lTemp);
		}
		pForexSymbol->SetIPOStatus(_STOCK_IPOED_);
		pForexSymbol->UpdateDayLine(*pvDayLine);
		pForexSymbol->SetDayLineNeedSaving(true);
		pForexSymbol->SetUpdateProfileDB(true);
		TRACE("����%s��������\n", pForexSymbol->GetSymbol().GetBuffer());
		return true;
	}
	else {
		pForexSymbol->SetDayLineNeedSaving(false);
		pForexSymbol->SetUpdateProfileDB(false);
		TRACE("����%s��������\n", pForexSymbol->GetSymbol().GetBuffer());
		return false;
	}
}

CDayLineVectorPtr CProductFinnhubForexDayLine::ParseFinnhubForexCandle(CWebDataPtr pWebData) {
	auto pvDayLine = make_shared<vector<CDayLinePtr>>();
	json pt2, pt3;
	CDayLinePtr pDayLine = nullptr;
	string sError;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvDayLine;
	if (pWebData->IsVoidJson()) {
		m_iReceivedDataStatus = _VOID_DATA_;
		return pvDayLine;
	}
	if (pWebData->CheckNoRightToAccess()) {
		m_iReceivedDataStatus = _NO_ACCESS_RIGHT_;
		return pvDayLine;
	}
	const auto pjs = pWebData->GetJSon();
	try {
		auto s = jsonGetString(pjs, _T("s"));
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
		if (!jsonGetChild(pjs, _T("t"), &pt2)) return pvDayLine;
		for (auto it = pt2.begin(); it != pt2.end(); ++it) {
			tTemp = jsonGetLongLong(it);
			pDayLine = make_shared<CDayLine>();
			pDayLine->SetTime(tTemp);
			lTemp = TransferToDate(tTemp, gl_pWorldMarket->GetMarketTimeZone());
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
		if (jsonGetChild(pjs, _T("c"), &pt2)) {
			i = 0;
			for (auto it = pt2.begin(); it != pt2.end(); ++it) {
				dTemp = jsonGetDouble(it);
				pDayLine = pvDayLine->at(i++);
				pDayLine->SetClose(dTemp * 1000);
			}
		}
		if (jsonGetChild(pjs, _T("h"), &pt2)) {
			i = 0;
			for (auto it = pt2.begin(); it != pt2.end(); ++it) {
				dTemp = jsonGetDouble(it);
				pDayLine = pvDayLine->at(i++);
				pDayLine->SetHigh(dTemp * 1000);
			}
		}
		if (jsonGetChild(pjs, _T("l"), &pt2)) {
			i = 0;
			for (auto it = pt2.begin(); it != pt2.end(); ++it) {
				dTemp = jsonGetDouble(it);
				pDayLine = pvDayLine->at(i++);
				pDayLine->SetLow(dTemp * 1000);
			}
		}
		if (jsonGetChild(pjs, _T("o"), &pt2)) {
			i = 0;
			for (auto it = pt2.begin(); it != pt2.end(); ++it) {
				dTemp = jsonGetDouble(it);
				pDayLine = pvDayLine->at(i++);
				pDayLine->SetOpen(dTemp * 1000);
			}
		}
		if (jsonGetChild(pjs, _T("v"), &pt2)) {
			i = 0;
			for (auto it = pt2.begin(); it != pt2.end(); ++it) {
				llTemp = jsonGetLongLong(it);
				pDayLine = pvDayLine->at(i++);
				pDayLine->SetVolume(llTemp);
			}
		}
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Forex Candle missing 'v' "), e.what());
		// ��Щ��㽻�ײ��ṩ�ɽ��������ԾͿ�����
	}
	ranges::sort(pvDayLine->begin(), pvDayLine->end(), CompareDayLineDate);

	return pvDayLine;
}
