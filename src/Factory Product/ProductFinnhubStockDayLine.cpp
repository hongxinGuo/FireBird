#include "pch.h"

#include"TimeConvert.h"
#include"jsonParse.h"
#include"JsonGetValue.h"

#include"WorldMarket.h"
#include"WorldStock.h"

#include "ProductFinnhubStockDayLine.h"

using namespace std;

bool CompareDayLineDate(CDayLinePtr& p1, CDayLinePtr& p2);

CProductFinnhubStockDayLine::CProductFinnhubStockDayLine() {
	m_strClassName = _T("Finnhub stock dayline");
	m_strInquiry = _T("https://finnhub.io/api/v1/stock/candle?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubStockDayLine::CreateMessage(void) {
		ASSERT(std::strcmp(typeid(*m_pMarket).name(), _T("class CWorldMarket")) == 0);

	const auto pStock = dynamic_cast<CWorldMarket*>(m_pMarket)->GetStock(m_lIndex);
	const auto strMiddle = pStock->GetFinnhubDayLineInquiryString(CVirtualMarket::GetUTCTime());

	m_strInquiringExchange = pStock->GetExchangeCode();
	m_strTotalInquiryMessage = m_strInquiry + strMiddle;
	return m_strTotalInquiryMessage;
}

bool CProductFinnhubStockDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
		ASSERT(std::strcmp(typeid(*m_pMarket).name(), _T("class CWorldMarket")) == 0);

	const auto pStock = dynamic_cast<CWorldMarket*>(m_pMarket)->GetStock(m_lIndex);
	const auto pvDayLine = ParseFinnhubStockCandle(pWebData);
	pStock->SetDayLineNeedUpdate(false);
	for (const auto& pDayLine : *pvDayLine) {
		pDayLine->SetExchange(pStock->GetExchangeCode());
		pDayLine->SetStockSymbol(pStock->GetSymbol());
		pDayLine->SetDisplaySymbol(pStock->GetTicker());
		const auto lTemp = ConvertToDate(pDayLine->m_time, m_pMarket->GetMarketTimeZone());
		pDayLine->SetDate(lTemp);
	}
	if (!pvDayLine->empty()) {
		pStock->UpdateDayLine(*pvDayLine);
		if (pStock->GetDayLineSize() > 0) {
			// ������������
			pStock->SetDayLineNeedSaving(true);
			pStock->SetUpdateProfileDB(true);
			const long lSize = pStock->GetDayLineSize() - 1;
			const auto pDayLine = pStock->GetDayLine(lSize);
			if (!IsEarlyThen(pDayLine->GetMarketDate(), m_pMarket->GetMarketDate(), 100)) { pStock->SetIPOStatus(_STOCK_IPOED_); }
			return true;
		}
	}
	//TRACE("����%s��������\n", pStock->GetSymbol().GetBuffer());

	return false;
}

CDayLineVectorPtr CProductFinnhubStockDayLine::ParseFinnhubStockCandle(CWebDataPtr pWebData) {
	auto pvDayLine = make_shared<vector<CDayLinePtr>>();
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
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Candle missing 's' ") + GetInquiry(), e.what());
		return pvDayLine;
	}

	try {
		time_t tTemp;
		js2 = jsonGetChild(pjs, _T("t"));
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			tTemp = jsonGetLongLong(it);
			pDayLine = make_shared<CDayLine>();
			pDayLine->SetTime(tTemp);
			pvDayLine->push_back(pDayLine);
		}
	}
	catch (json::exception& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Candle missing 't' ") + GetInquiry(), e.what());
		return pvDayLine;
	}
	try {
		int i;
		INT64 llTemp;
		double dTemp;
		js2 = jsonGetChild(pjs, _T("c"));
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetClose(dTemp * 1000);
		}
		js2 = jsonGetChild(pjs, _T("o"));
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetOpen(dTemp * 1000);
		}
		js2 = jsonGetChild(pjs, _T("h"));
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetHigh(dTemp * 1000);
		}
		js2 = jsonGetChild(pjs, _T("l"));
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			dTemp = jsonGetDouble(it);
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetLow(dTemp * 1000);
		}
		js2 = jsonGetChild(pjs, _T("v"));
		i = 0;
		for (auto it = js2.begin(); it != js2.end(); ++it) {
			llTemp = jsonGetLongLong(it);
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetVolume(llTemp);
		}
	}
	catch (json::exception& e) { ReportJSonErrorToSystemMessage(_T("Finnhub Stock Candle Error#3 ") + GetInquiry(), e.what()); }
	ranges::sort(pvDayLine->begin(), pvDayLine->end(), CompareDayLineDate); // ����������˳�����С�

	return pvDayLine;
}