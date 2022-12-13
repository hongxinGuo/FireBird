#include "pch.h"

//
#include"TimeConvert.h"
#include"jsonParse.h"

#include"WorldMarket.h"
#include"FinnhubCryptoSymbol.h"
#include"CallableFunction.h"

#include "ProductFinnhubCryptoDayLine.h"

IMPLEMENT_DYNCREATE(CProductFinnhubCryptoDayLine, CProductFinnhub)

CProductFinnhubCryptoDayLine::CProductFinnhubCryptoDayLine() {
	m_strClassName = _T("Finnhub crypto dayline");
	m_strInquiry = _T("https://finnhub.io/api/v1/crypto/candle?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubCryptoDayLine::CreateMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));
	const auto pCryptoSymbol = dynamic_cast<CWorldMarket*>(m_pMarket)->GetFinnhubCryptoSymbol(m_lIndex);

	m_strInquiringExchange = pCryptoSymbol->GetExchangeCode();
	m_strTotalInquiryMessage = m_strInquiry + pCryptoSymbol->GetFinnhubDayLineInquiryString(CWorldMarket::GetUTCTime());
	return m_strTotalInquiryMessage;
}

bool CProductFinnhubCryptoDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));
	bool fStatus = true;

	const auto pCryptoSymbol = dynamic_cast<CWorldMarket*>(m_pMarket)->GetFinnhubCryptoSymbol(m_lIndex);
	const auto pvDayLine = ParseFinnhubCryptoCandle(pWebData);
	pCryptoSymbol->SetDayLineNeedUpdate(false);
	if (!pvDayLine->empty()) {
		for (const auto& pDayLine : *pvDayLine) {
			pDayLine->SetExchange(pCryptoSymbol->GetExchangeCode());
			pDayLine->SetStockSymbol(pCryptoSymbol->GetSymbol());
			const long lTemp = TransferToDate(pDayLine->m_time, ((CWorldMarket*)m_pMarket)->GetMarketTimeZone());
			pDayLine->SetDate(lTemp);
		}
		pCryptoSymbol->UpdateDayLine(*pvDayLine);
		pCryptoSymbol->UpdateDayLineStartEndDate();
		if (gl_pWorldMarket->GetLastTradeDate() <= pCryptoSymbol->GetDayLineEndDate()) {
			pCryptoSymbol->SetIPOStatus(_STOCK_IPOED_);
		}
		else {
			pCryptoSymbol->SetIPOStatus(_STOCK_DELISTED_);
		}
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
		else {
			fStatus = false;
		}
	}
	return fStatus;
}

CDayLineVectorPtr CProductFinnhubCryptoDayLine::ParseFinnhubCryptoCandle(CWebDataPtr pWebData) {
	auto pvDayLine = make_shared<vector<CDayLinePtr>>();
	auto pvDayLineReturn = make_shared<vector<CDayLinePtr>>();
	ptree pt2, pt3;
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

	const auto ppt = pWebData->GetPTree();
	try {
		string s;
		s = ppt->get<string>(_T("s"));
		if (s == _T("no_data")) {
			// û���������ݣ�������˹�Ʊ�����ߺ�ʵʱ����
			return pvDayLine;
		}
		if (s != _T("ok")) {
			gl_systemMessage.PushErrorMessage(_T("���߷���ֵ��Ϊok"));
			return pvDayLine;
		}
	}
	catch (ptree_error& e) {
		// ��������Ǵ˴���������⡣�����������"error":"you don't have access this resource."
		ReportJSonErrorToSystemMessage(_T("Finnhub Crypto Candle missing 's': "), pWebData->GetDataBuffer(), e);
		return pvDayLine;
	}
	try {
		time_t tTemp = 0;
		pt2 = ppt->get_child(_T("t"));
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			tTemp = pt3.get_value<time_t>();
			pDayLine = make_shared<CDayLine>();
			pDayLine->SetTime(tTemp);
			pvDayLine->push_back(pDayLine);
		}
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Crypto Candle missing 't' "), e);
		return pvDayLine;
	}
	try {
		int i = 0;
		INT64 llTemp;
		double dTemp;
		pt2 = ppt->get_child(_T("c"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			dTemp = pt3.get_value<double>();
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetClose(static_cast<long>(dTemp * 1000));
		}
		pt2 = ppt->get_child(_T("h"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			dTemp = pt3.get_value<double>();
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetHigh(static_cast<long>(1000 * dTemp));
		}
		pt2 = ppt->get_child(_T("l"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			dTemp = pt3.get_value<double>();
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetLow(static_cast<long>(1000 * dTemp));
		}
		pt2 = ppt->get_child(_T("o"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			dTemp = pt3.get_value<double>();
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetOpen(static_cast<long>(1000 * dTemp));
		}
		pt2 = ppt->get_child(_T("v"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			llTemp = static_cast<INT64>(pt3.get_value<double>());
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetVolume(llTemp);
		}
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Crypto Candle "), e);
		// ��Щ��㽻�ײ��ṩ�ɽ��������ԾͿ�����
	}
	ranges::sort(pvDayLine->begin(), pvDayLine->end(), CompareDayLineDate);
	// �������������Ϊ�����Ч����
	for (auto& pDayLine2 : *pvDayLine) {
		if (pDayLine2->m_time > 0) {
			pvDayLineReturn->push_back(pDayLine2);
		}
	}
	return pvDayLineReturn;
}
