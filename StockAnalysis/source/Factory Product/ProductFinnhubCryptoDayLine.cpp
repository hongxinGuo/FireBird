#include "pch.h"

#include"globedef.h"
#include"WorldMarket.h"
#include"FinnhubCryptoSymbol.h"
#include"CallableFunction.h"

#include "ProductFinnhubCryptoDayLine.h"

IMPLEMENT_DYNCREATE(CProductFinnhubCryptoDayLine, CProductWebSourceData)

CProductFinnhubCryptoDayLine::CProductFinnhubCryptoDayLine() {
	m_strClassName = _T("Finnhub crypto dayline");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/crypto/candle?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubCryptoDayLine::CreatMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));
	CFinnhubCryptoSymbolPtr pCryptoSymbol = ((CWorldMarket*)m_pMarket)->GetCryptoSymbol(m_lIndex);
	CString strMessage = m_strInquiringStr + pCryptoSymbol->GetFinnhubDayLineInquiryString(((CWorldMarket*)m_pMarket)->GetUTCTime());

	return strMessage;
}

bool CProductFinnhubCryptoDayLine::ProcessWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));
	long lTemp = 0;

	CDayLineVectorPtr pvDayLine = nullptr;
	CFinnhubCryptoSymbolPtr pCryptoSymbol = ((CWorldMarket*)m_pMarket)->GetCryptoSymbol(m_lIndex);
	pvDayLine = ParseFinnhubCryptoCandle(pWebData);
	if (pvDayLine->size() > 0) {
		for (auto& pDayLine : *pvDayLine) {
			pDayLine->SetExchange(pCryptoSymbol->GetExchangeCode());
			pDayLine->SetStockSymbol(pCryptoSymbol->GetSymbol());
			lTemp = TransferToDate(pDayLine->m_time, ((CWorldMarket*)m_pMarket)->GetMarketTimeZone());
			pDayLine->SetDate(lTemp);
		}
		pCryptoSymbol->UpdateDayLine(*pvDayLine);
		pCryptoSymbol->SetIPOStatus(__STOCK_IPOED__);
		pCryptoSymbol->SetDayLineNeedUpdate(false);
		pCryptoSymbol->SetDayLineNeedSaving(true);
		pCryptoSymbol->SetUpdateProfileDB(true);
	}
	return true;
}

CDayLineVectorPtr CProductFinnhubCryptoDayLine::ParseFinnhubCryptoCandle(CWebDataPtr pWebData) {
	CDayLineVectorPtr pvDayLine = make_shared<vector<CDayLinePtr>>();
	ptree pt, pt2, pt3;
	string s;
	double dTemp = 0;
	long lTemp = 0;
	INT64 llTemp = 0;
	time_t tTemp = 0;
	CDayLinePtr pDayLine = nullptr;
	int i = 0;
	string sError;

	if (!ConvertToJSON(pt, pWebData)) { // �����̹߳���
		gl_systemMessage.PushErrorMessage(_T("����Ϊ��ЧJSon����"));
		return pvDayLine;
	}

	try {
		s = pt.get<string>(_T("s"));
		if (s.compare(_T("no_data")) == 0) { // û���������ݣ�������˹�Ʊ�����ߺ�ʵʱ����
			return pvDayLine;
		}
		if (s.compare(_T("ok")) != 0) {
			gl_systemMessage.PushErrorMessage(_T("���߷���ֵ��Ϊok"));
			return pvDayLine;
		}
	}
	catch (ptree_error& e) { // ��������Ǵ˴���������⡣�����������"error":"you don't have access this resource."
		ReportJSonErrorToSystemMessage(_T("Finnhub Crypto Candle "), e);
		return pvDayLine;
	}
	try {
		pt2 = pt.get_child(_T("t"));
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			tTemp = pt3.get_value<time_t>();
			pDayLine = make_shared<CDayLine>();
			pDayLine->SetTime(tTemp);
			pvDayLine->push_back(pDayLine);
		}
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Crypto Candle "), e);
		return pvDayLine;
	}
	try {
		pt2 = pt.get_child(_T("c"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			dTemp = pt3.get_value<double>();
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetClose(dTemp * 1000);
		}
		pt2 = pt.get_child(_T("h"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			dTemp = pt3.get_value<double>();
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetHigh(dTemp * 1000);
		}
		pt2 = pt.get_child(_T("l"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			dTemp = pt3.get_value<double>();
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetLow(dTemp * 1000);
		}
		pt2 = pt.get_child(_T("o"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			dTemp = pt3.get_value<double>();
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetOpen(dTemp * 1000);
		}
		pt2 = pt.get_child(_T("v"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			llTemp = pt3.get_value<double>();
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetVolume(llTemp);
		}
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Finnhub Crypto Candle "), e);
		// ��Щ��㽻�ײ��ṩ�ɽ��������ԾͿ�����
	}
	sort(pvDayLine->begin(), pvDayLine->end(), CompareDayLineDate);
	return pvDayLine;
}