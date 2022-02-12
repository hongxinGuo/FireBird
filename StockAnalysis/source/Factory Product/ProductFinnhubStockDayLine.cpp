#include "pch.h"

#include"globedef.h"
#include"accessory.h"
#include"WorldMarket.h"
#include"WorldStock.h"

#include "ProductFinnhubStockDayLine.h"

#include<boost/property_tree/ptree.hpp>
#include<boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;

IMPLEMENT_DYNCREATE(CProductFinnhubStockDayLine, CProductWebSourceData)

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
	pStock->SetDayLineNeedUpdate(false);

	return m_strInquiringStr + strMiddle;
}

bool CProductFinnhubStockDayLine::ProcessWebData(CWebDataPtr pWebData) {
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CDayLineVectorPtr pvDayLine = nullptr;
	long lTemp = 0;

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	pvDayLine = ParseFinnhubStockCandle(pWebData);

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
			pStock->SetDayLineNeedUpdate(false);
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
	ptree pt, pt2, pt3;
	string s;
	double dTemp = 0;
	long lTemp = 0;
	INT64 llTemp = 0;
	time_t tTemp = 0;
	CDayLinePtr pDayLine = nullptr;
	int i = 0;
	string sError;

	if (!ConvertToJSON(pt, pWebData)) { // 工作线程故障
		gl_systemMessage.PushErrorMessage(_T("日线为无效JSon数据"));
		return pvDayLine;
	}

	try {
		s = pt.get<string>(_T("s"));
		if (s.compare(_T("no_data")) == 0) { // 没有日线数据，无需检查此股票的日线和实时数据
			return pvDayLine;
		}
		if (s.compare(_T("ok")) != 0) {
			gl_systemMessage.PushErrorMessage(_T("日线返回值不为ok"));
			return pvDayLine;
		}
	}
	catch (ptree_error& e) { // 这种请况是此代码出现问题。如服务器返回"error":"you don't have access this resource."
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Candle "), e);
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
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Candle "), e);
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
		pt2 = pt.get_child(_T("o"));
		i = 0;
		for (ptree::iterator it = pt2.begin(); it != pt2.end(); ++it) {
			pt3 = it->second;
			dTemp = pt3.get_value<double>();
			pDayLine = pvDayLine->at(i++);
			pDayLine->SetOpen(dTemp * 1000);
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
		ReportJSonErrorToSystemMessage(_T("Finnhub Stock Candle "), e);
	}
	sort(pvDayLine->begin(), pvDayLine->end(), CompareDayLineDate); // 以日期早晚顺序排列。
	return pvDayLine;
}