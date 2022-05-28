#include "pch.h"

#include"globedef.h"
#include"accessory.h"
#include"SystemMessage.h"
#include"WorldMarket.h"
#include"FinnhubForexSymbol.h"
#include"CallableFunction.h"

#include "ProductFinnhubForexDayLine.h"

IMPLEMENT_DYNCREATE(CProductFinnhubForexDayLine, CProductWebSourceData)

CProductFinnhubForexDayLine::CProductFinnhubForexDayLine() {
	m_strClassName = _T("Finnhub forex dayline");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/forex/candle?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubForexDayLine::CreatMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CForexSymbolPtr pForexSymbol = ((CWorldMarket*)m_pMarket)->GetForexSymbol(m_lIndex);
	CString strMessage = m_strInquiringStr + pForexSymbol->GetFinnhubDayLineInquiryString(((CWorldMarket*)m_pMarket)->GetUTCTime());
	pForexSymbol->SetDayLineNeedUpdate(false);

	return strMessage;
}

bool CProductFinnhubForexDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CDayLineVectorPtr pvDayLine = nullptr;
	long lTemp = 0;

	CForexSymbolPtr pForexSymbol = ((CWorldMarket*)m_pMarket)->GetForexSymbol(m_lIndex);
	pvDayLine = ParseFinnhubForexCandle(pWebData);
	if (pvDayLine->size() > 0) {
		for (auto pDayLine : *pvDayLine) {
			pDayLine->SetExchange(pForexSymbol->GetExchangeCode());
			pDayLine->SetStockSymbol(pForexSymbol->GetSymbol());
			lTemp = TransferToDate(pDayLine->m_time, ((CWorldMarket*)m_pMarket)->GetMarketTimeZone());
			pDayLine->SetDate(lTemp);
		}
		pForexSymbol->SetIPOStatus(__STOCK_IPOED__);
		pForexSymbol->UpdateDayLine(*pvDayLine);
		pForexSymbol->SetDayLineNeedUpdate(false);
		pForexSymbol->SetDayLineNeedSaving(true);
		pForexSymbol->SetUpdateProfileDB(true);
		TRACE("处理%s日线数据\n", pForexSymbol->GetSymbol().GetBuffer());
		return true;
	}
	else {
		pForexSymbol->SetDayLineNeedUpdate(false);
		pForexSymbol->SetDayLineNeedSaving(false);
		pForexSymbol->SetUpdateProfileDB(false);
		TRACE("处理%s日线数据\n", pForexSymbol->GetSymbol().GetBuffer());
		return false;
	}
}

CDayLineVectorPtr CProductFinnhubForexDayLine::ParseFinnhubForexCandle(CWebDataPtr pWebData) {
	CDayLineVectorPtr pvDayLine = make_shared<vector<CDayLinePtr>>();
	ptree pt2, pt3;
	string s;
	double dTemp = 0;
	long lTemp = 0;
	INT64 llTemp = 0;
	time_t tTemp = 0;
	CDayLinePtr pDayLine = nullptr;
	int i = 0;
	string sError;
	shared_ptr<ptree> ppt;

	ASSERT(pWebData->IsJSonContentType());
	if (pWebData->IsParsed()) {
		if (pWebData->IsVoidJSon()) return pvDayLine;
		ppt = pWebData->GetPTree();
		try {
			s = ppt->get<string>(_T("s"));
			if (s.compare(_T("no_data")) == 0) { // 没有日线数据，无需检查此股票的日线和实时数据
				return pvDayLine;
			}
			if (s.compare(_T("ok")) != 0) {
				gl_systemMessage.PushErrorMessage(_T("日线返回值不为ok"));
				return pvDayLine;
			}
		}
		catch (ptree_error& e) { // 这种请况是此代码出现问题。如服务器返回"error":"you don't have access this resource."
			ReportJSonErrorToSystemMessage(_T("Finnhub Forex Candle没有s项"), e);
			return pvDayLine;
		}
		try {
			pt2 = ppt->get_child(_T("t"));
			for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
				pt3 = it->second;
				tTemp = pt3.get_value<time_t>();
				pDayLine = make_shared<CDayLine>();
				pDayLine->SetTime(tTemp);
				lTemp = TransferToDate(tTemp, gl_pWorldMarket->GetMarketTimeZone());
				pDayLine->SetDate(lTemp);
				pvDayLine->push_back(pDayLine);
			}
		}
		catch (ptree_error& e) {
			ReportJSonErrorToSystemMessage(_T("Finnhub Forex Candle "), e);
			return pvDayLine;
		}
		try {
			pt2 = ppt->get_child(_T("c"));
			i = 0;
			for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
				pt3 = it->second;
				dTemp = pt3.get_value<double>();
				pDayLine = pvDayLine->at(i++);
				pDayLine->SetClose(dTemp * 1000);
			}
			pt2 = ppt->get_child(_T("h"));
			i = 0;
			for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
				pt3 = it->second;
				dTemp = pt3.get_value<double>();
				pDayLine = pvDayLine->at(i++);
				pDayLine->SetHigh(dTemp * 1000);
			}
			pt2 = ppt->get_child(_T("l"));
			i = 0;
			for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
				pt3 = it->second;
				dTemp = pt3.get_value<double>();
				pDayLine = pvDayLine->at(i++);
				pDayLine->SetLow(dTemp * 1000);
			}
			pt2 = ppt->get_child(_T("o"));
			i = 0;
			for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
				pt3 = it->second;
				dTemp = pt3.get_value<double>();
				pDayLine = pvDayLine->at(i++);
				pDayLine->SetOpen(dTemp * 1000);
			}
			pt2 = ppt->get_child(_T("v"));
			i = 0;
			for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
				pt3 = it->second;
				llTemp = pt3.get_value<double>();
				pDayLine = pvDayLine->at(i++);
				pDayLine->SetVolume(llTemp);
			}
		}
		catch (ptree_error& e) {
			ReportJSonErrorToSystemMessage(_T("Finnhub Forex Candle "), e);
			// 有些外汇交易不提供成交量，忽略就可以了
		}
		sort(pvDayLine->begin(), pvDayLine->end(), CompareDayLineDate);
	}

	return pvDayLine;
}