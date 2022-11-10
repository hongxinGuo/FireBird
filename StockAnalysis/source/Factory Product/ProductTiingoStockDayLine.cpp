#include "pch.h"

#include"jsonParse.h"

#include"WorldStock.h"
#include"WorldMarket.h"
#include"CallableFunction.h"

#include "ProductTiingoStockDayLine.h"

IMPLEMENT_DYNCREATE(CProductTiingoStockDayLine, CVirtualProductWebData)

CProductTiingoStockDayLine::CProductTiingoStockDayLine() : CVirtualProductWebData() {
	m_strClassName = _T("Tiingo stock price candle");
	m_strInquiry = _T("https://api.tiingo.com/tiingo/daily/");
	m_lIndex = -1;
}

CString CProductTiingoStockDayLine::CreatMessage(void) {
	CWorldStockPtr pStock = nullptr;
	CString strMiddle;

	ASSERT(m_pMarket->IsKindOf(RUNTIME_CLASS(CWorldMarket)));

	pStock = ((CWorldMarket*)m_pMarket)->GetStock(GetIndex());
	strMiddle = pStock->GetTiingoDayLineInquiryString(((CWorldMarket*)m_pMarket)->GetMarketDate());
	pStock->SetDayLineNeedUpdate(false);

	m_strTotalInquiryMessage = m_strInquiry + strMiddle;
	return m_strTotalInquiryMessage;
}

bool CProductTiingoStockDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
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
// 如果没有股票600600.SS日线数据，则返回：{"detail":"Error:Ticker '600600.SS' not found"}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDayLineVectorPtr CProductTiingoStockDayLine::ParseTiingoStockDayLine(CWebDataPtr pWebData) {
	CDayLineVectorPtr pvDayLine = make_shared<vector<CDayLinePtr>>();
	ptree pt2;
	string s;
	double dTemp = 0;
	long lTemp = 0;
	CDayLinePtr pDayLine = nullptr;
	CString str;
	long year, month, day;
	shared_ptr<ptree> ppt;

	ASSERT(pWebData->IsJSonContentType());
	if (!pWebData->IsParsed()) return pvDayLine;
	if (pWebData->IsVoidJson()) return pvDayLine;

	ppt = pWebData->GetPTree();
	try {
		s = ppt->get<string>(_T("detail")); // 是否有报错信息
		CString strMessage = _T("Tiingo stock dayLine ");
		strMessage += s.c_str();
		gl_systemMessage.PushErrorMessage(strMessage); // 报告错误信息
		return pvDayLine;
	}
	catch (ptree_error&) {
		// 正确， do nothing，继续执行
	}
	try {
		for (ptree::iterator it = ppt->begin(); it != ppt->end(); ++it) {
			pDayLine = make_shared<CDayLine>();
			pt2 = it->second;
			s = pt2.get<string>(_T("date"));
			str = s.c_str();
			sscanf_s(str.GetBuffer(), _T("%04d-%02d-%02d"), &year, &month, &day);
			lTemp = year * 10000 + month * 100 + day;
			pDayLine->SetDate(lTemp);
			dTemp = ptreeGetDouble(pt2, _T("close"));
			pDayLine->SetClose(dTemp * 1000);
			dTemp = ptreeGetDouble(pt2, _T("high"));
			pDayLine->SetHigh(dTemp * 1000);
			dTemp = ptreeGetDouble(pt2, _T("low"));
			pDayLine->SetLow(dTemp * 1000);
			dTemp = ptreeGetDouble(pt2, _T("open"));
			pDayLine->SetOpen(dTemp * 1000);
			lTemp = pt2.get<long>(_T("volume"));
			pDayLine->SetVolume(lTemp);
			pvDayLine->push_back(pDayLine);
		}
	}
	catch (ptree_error& e) {
		CString str3 = pWebData->GetDataBuffer().c_str();
		str3 = str3.Left(120);
		ReportJSonErrorToSystemMessage(_T("Tiingo Stock DayLine ") + str3, e);
		return pvDayLine; // 数据解析出错的话，则放弃。
	}
	sort(pvDayLine->begin(), pvDayLine->end(), CompareDayLineDate); // 以日期早晚顺序排列。

	return pvDayLine;
}