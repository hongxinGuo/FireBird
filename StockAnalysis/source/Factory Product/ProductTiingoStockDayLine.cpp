#include "pch.h"

#include"globedef.h"
#include"WorldStock.h"
#include"WorldMarket.h"
#include"CallableFunction.h"

#include "ProductTiingoStockDayLine.h"

IMPLEMENT_DYNCREATE(CProductTiingoStockDayLine, CProductWebSourceData)

CProductTiingoStockDayLine::CProductTiingoStockDayLine() : CProductWebSourceData() {
	m_strClassName = _T("Tiingo stock price candle");
	m_strInquiringStr = _T("https://api.tiingo.com/tiingo/daily/");
	m_lIndex = -1;
}

CString CProductTiingoStockDayLine::CreatMessage(void) {
	CWorldStockPtr pStock = nullptr;
	CString strMiddle;

	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	pStock = ((CWorldMarket*)m_pMarket)->GetStock(GetIndex());
	strMiddle = pStock->GetTiingoDayLineInquiryString(((CWorldMarket*)m_pMarket)->GetMarketDate());
	pStock->SetDayLineNeedUpdate(false);

	return m_strInquiringStr + strMiddle;
}

bool CProductTiingoStockDayLine::ProcessWebData(CWebDataPtr pWebData) {
	ASSERT(m_lIndex >= 0);
	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	CDayLineVectorPtr pvDayLine = nullptr;

	CWorldStockPtr pStock = ((CWorldMarket*)m_pMarket)->GetStock(m_lIndex);
	pvDayLine = ParseTiingoStockDayLine(pWebData);
	if (pvDayLine->size() > 0) {
		for (auto& pDayLine2 : *pvDayLine) {
			pDayLine2->SetExchange(pStock->GetExchangeCode());
			pDayLine2->SetStockSymbol(pStock->GetSymbol());
			pDayLine2->SetDisplaySymbol(pStock->GetTicker());
		}
		pStock->UpdateDayLine(*pvDayLine);
		pStock->SetDayLineNeedUpdate(false);
		pStock->SetDayLineNeedSaving(true);
		pStock->SetUpdateProfileDB(true);
		TRACE("处理Tiingo %s日线数据\n", pStock->GetSymbol().GetBuffer());
		return true;
	}
	else {
		pStock->SetDayLineNeedUpdate(false);
		pStock->SetDayLineNeedSaving(false);
		pStock->SetUpdateProfileDB(false);
		TRACE("处理Tiingo %s日线数据\n", pStock->GetSymbol().GetBuffer());
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Tiingo日线格式：
// [{
//	"date":"2019-01-02T00:00:00.000Z",
//		"close" : 157.92,
//		"high" : 158.85,
//		"low" : 154.23,
//		"open" : 154.89,
//		"volume" : 37039737,
//		"adjClose" : 157.92,
//		"adjHigh" : 158.85,
//		"adjLow" : 154.23,
//		"adjOpen" : 154.89,
//		"adjVolume" : 37039737,
//		"divCash" : 0.0,
//		"splitFactor" : 1.0
// },
//	{
//		"date":"2019-01-03T00:00:00.000Z",
//		"close" : 142.19,
//		"high" : 145.72,
//		"low" : 142.0,
//		"open" : 143.98,
//		"volume" : 91312195,
//		"adjClose" : 142.19,
//		"adjHigh" : 145.72,
//		"adjLow" : 142.0,
//		"adjOpen" : 143.98,
//		"adjVolume" : 91312195,
//		"divCash" : 0.0,
//		"splitFactor" : 1.0
//	}
// ]
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDayLineVectorPtr CProductTiingoStockDayLine::ParseTiingoStockDayLine(CWebDataPtr pWebData) {
	CDayLineVectorPtr pvDayLine = make_shared<vector<CDayLinePtr>>();
	ptree pt, pt2;
	string s;
	double dTemp = 0;
	long lTemp = 0;
	CDayLinePtr pDayLine = nullptr;
	CString str;
	long year, month, day;

	if (!ConvertToJSON(pt, pWebData)) { // 工作线程故障
		str = _T("日线为无效JSon数据\n");
		gl_systemMessage.PushErrorMessage(str);
		return pvDayLine;
	}

	try {
		for (ptree::iterator it = pt.begin(); it != pt.end(); ++it) {
			pDayLine = make_shared<CDayLine>();
			pt2 = it->second;
			s = pt2.get<string>(_T("date"));
			str = s.c_str();
			sscanf_s(str.GetBuffer(), _T("%04d-%02d-%02d"), &year, &month, &day);
			lTemp = year * 10000 + month * 100 + day;
			pDayLine->SetDate(lTemp);
			dTemp = pt2.get<double>(_T("close"));
			pDayLine->SetClose(dTemp * 1000);
			dTemp = pt2.get<double>(_T("high"));
			pDayLine->SetHigh(dTemp * 1000);
			dTemp = pt2.get<double>(_T("low"));
			pDayLine->SetLow(dTemp * 1000);
			dTemp = pt2.get<double>(_T("open"));
			pDayLine->SetOpen(dTemp * 1000);
			lTemp = pt2.get<long>(_T("volume"));
			pDayLine->SetVolume(lTemp);
			pvDayLine->push_back(pDayLine);
		}
	}
	catch (ptree_error& e) {
		ReportJSonErrorToSystemMessage(_T("Tiingo Stock DayLine "), e);
		return pvDayLine; // 数据解析出错的话，则放弃。
	}
	sort(pvDayLine->begin(), pvDayLine->end(), CompareDayLineDate); // 以日期早晚顺序排列。
	return pvDayLine;
}