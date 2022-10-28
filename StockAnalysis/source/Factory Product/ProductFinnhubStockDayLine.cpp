#include "pch.h"

#include"TimeConvert.h"
#include"jsonParse.h"

#include"WorldMarket.h"
#include"WorldStock.h"

#include "ProductFinnhubStockDayLine.h"

IMPLEMENT_DYNCREATE(CProductFinnhubStockDayLine, CProductFinnhub)

bool CompareDayLineDate(CDayLinePtr& p1, CDayLinePtr& p2);

CProductFinnhubStockDayLine::CProductFinnhubStockDayLine() {
	m_strClassName = _T("Finnhub company profile concise");
	m_strInquiringStr = _T("https://finnhub.io/api/v1/stock/candle?symbol=");
	m_lIndex = -1;
}

CString CProductFinnhubStockDayLine::CreatMessage(void) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	CString strMiddle = pStock->GetFinnhubDayLineInquiryString(((CWorldMarket*)m_pMarket)->GetUTCTime());

	m_strTotalInquiryMessage = m_strInquiringStr + strMiddle;
	return m_strTotalInquiryMessage;
}

bool CProductFinnhubStockDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CDayLineVectorPtr pvDayLine = nullptr;
	long lTemp = 0;

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	pvDayLine = ParseFinnhubStockCandle(pWebData);
	pStock->SetDayLineNeedUpdate(false);
	for (auto& pDayLine : *pvDayLine) {
		pDayLine->SetExchange(pStock->GetExchangeCode());
		pDayLine->SetStockSymbol(pStock->GetSymbol());
		pDayLine->SetDisplaySymbol(pStock->GetTicker());
		lTemp = TransferToDate(pDayLine->m_time, ((CWorldMarket*)m_pMarket)->GetMarketTimeZone());
		pDayLine->SetDate(lTemp);
	}
	if (pvDayLine->size() > 0) {
		pStock->UpdateDayLine(*pvDayLine);
		if (pStock->GetDayLineSize() > 0) { // 添加了新数据
			pStock->SetDayLineNeedSaving(true);
			pStock->SetUpdateProfileDB(true);
			long lSize = pStock->GetDayLineSize() - 1;
			CDayLinePtr pDayLine = pStock->GetDayLine(lSize);
			if (!IsEarlyThen(pDayLine->GetMarketDate(), ((CWorldMarket*)m_pMarket)->GetMarketDate(), 100)) {
				pStock->SetIPOStatus(__STOCK_IPOED__);
			}
			return true;
		}
	}
	//TRACE("处理%s日线数据\n", pStock->GetSymbol().GetBuffer());

	return false;
}

CDayLineVectorPtr CProductFinnhubStockDayLine::ParseFinnhubStockCandle(CWebDataPtr pWebData) {
	CDayLineVectorPtr pvDayLine = make_shared<vector<CDayLinePtr>>();
	ptree pt2, pt3;
	string s;
	double dTemp = 0;
	INT64 llTemp = 0;
	time_t tTemp = 0;
	CDayLinePtr pDayLine = nullptr;
	int i = 0;
	string sError;
	shared_ptr<ptree> ppt;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvDayLine;
	if (pWebData->IsVoidJSon()) { m_iReceivedDataStatus = __VOID_DATA__; return pvDayLine; }
	if (pWebData->NoRightToAccess()) { m_iReceivedDataStatus = __NO_ACCESS_RIGHT__; return pvDayLine; }

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
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Candle missing 's' ") + GetInquiringStr(), e);
		return pvDayLine;
	}

	try {
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
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Candle missing 't' ") + GetInquiringStr(), e);
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
		pt2 = ppt->get_child(_T("o"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			dTemp = pt3.get_value<double>();
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetOpen(dTemp * 1000);
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
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Candle Error#3 ") + GetInquiringStr(), e);
	}
	sort(pvDayLine->begin(), pvDayLine->end(), CompareDayLineDate); // 以日期早晚顺序排列。

	return pvDayLine;
}