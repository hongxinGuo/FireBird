#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

#include"WorldStock.h"
#include"WorldMarket.h"

#include "ProductTiingoStockDayLine.h"

#include "TimeConvert.h"

CProductTiingoStockDayLine::CProductTiingoStockDayLine() {
	m_strInquiryFunction = _T("https://api.tiingo.com/tiingo/daily/");
}

///////////////////////////////////////////////////////////////////////////////////////////
///
/// 即使是免费账户，tiingo日线也能够提供30年以上的数据，故而至少申请一次全部数据（自19800101开始）。
///	此后为了减少数据流量，可以只申请未下载的数据。
///	Finnhub的免费日线只提供一年的。本系统最初的执行时间为2019年，即finnhub没有2018年以前的日线。
///
///////////////////////////////////////////////////////////////////////////////////////////
CString CProductTiingoStockDayLine::CreateMessage() {
	ASSERT(std::strcmp(typeid(*GetMarket()).name(), _T("class CWorldMarket")) == 0);

	const auto pStock = gl_dataContainerFinnhubStock.GetStock(GetIndex());
	CString strParam;
	if (pStock->GetDayLineStartDate() > 20180101) {
		strParam = pStock->GetTiingoDayLineInquiryParam(19800101, GetMarket()->GetMarketDate()); // 如果日线未完全申请过时，申请完整日线。
	}
	else {
		strParam = pStock->GetTiingoDayLineInquiryParam(pStock->GetDayLineEndDate(), GetMarket()->GetMarketDate());
	}
	pStock->SetDayLineNeedUpdate(false);

	m_strInquiry = m_strInquiryFunction + strParam;
	return m_strInquiry;
}

void CProductTiingoStockDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(m_lIndex >= 0);

	const auto pStock = gl_dataContainerFinnhubStock.GetStock(m_lIndex);
	const CDayLinesPtr pvDayLine = ParseTiingoStockDayLine(pWebData);
	pStock->SetDayLineNeedUpdate(false);
	if (!pvDayLine->empty()) {
		for (const auto& pDayLine2 : *pvDayLine) {
			pDayLine2->SetExchange(pStock->GetExchangeCode());
			pDayLine2->SetStockSymbol(pStock->GetSymbol());
			pDayLine2->SetDisplaySymbol(pStock->GetTicker());
		}
		pStock->UpdateDayLine(*pvDayLine);
		pStock->SetDayLineNeedSaving(true);
		pStock->SetUpdateProfileDB(true);
		//TRACE("处理Tiingo %s日线数据\n", pStock->GetSymbol().GetBuffer());
		return;
	}
	else {
		pStock->SetDayLineNeedSaving(false);
		pStock->SetUpdateProfileDB(false);
		//TRACE("处理Tiingo %s日线数据\n", pStock->GetSymbol().GetBuffer());
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
CDayLinesPtr CProductTiingoStockDayLine::ParseTiingoStockDayLine(const CWebDataPtr& pWebData) {
	auto pvDayLine = make_shared<vector<CDayLinePtr>>();
	string s;
	long year, month, day;
	json js;

	if (!pWebData->CreateJson(js)) return pvDayLine;
	if (!IsValidData(pWebData)) return pvDayLine;

	try {
		s = js.at(_T("detail")); // 是否有报错信息
		CString strMessage = _T("Tiingo stock dayLine ");
		strMessage += s.c_str();
		gl_systemMessage.PushErrorMessage(strMessage); // 报告错误信息
		return pvDayLine;
	} catch (json::exception&) {
		// 正确， do nothing，继续执行
	}
	try {
		for (auto it = js.begin(); it != js.end(); ++it) {
			auto pDayLine = make_shared<CDayLine>();
			s = jsonGetString(it, _T("date"));
			CString str = s.c_str();
			sscanf_s(str.GetBuffer(), _T("%04d-%02d-%02d"), &year, &month, &day);
			long lTemp = XferYearMonthDayToYYYYMMDD(year, month, day);
			pDayLine->SetDate(lTemp);
			double dTemp = jsonGetDouble(it, _T("close"));
			pDayLine->SetClose(dTemp * 1000);
			dTemp = jsonGetDouble(it, _T("high"));
			pDayLine->SetHigh(dTemp * 1000);
			dTemp = jsonGetDouble(it, _T("low"));
			pDayLine->SetLow(dTemp * 1000);
			dTemp = jsonGetDouble(it, _T("open"));
			pDayLine->SetOpen(dTemp * 1000);
			lTemp = jsonGetLong(it, _T("volume"));
			pDayLine->SetVolume(lTemp);
			pvDayLine->push_back(pDayLine);
		}
	} catch (json::exception& e) {
		CString str3 = pWebData->GetDataBuffer().c_str();
		str3 = str3.Left(120);
		ReportJSonErrorToSystemMessage(_T("Tiingo Stock DayLine ") + str3, e.what());
		return pvDayLine; // 数据解析出错的话，则放弃。
	}
	std::ranges::sort(pvDayLine->begin(), pvDayLine->end(), CompareDayLineDate); // 以日期早晚顺序排列。

	return pvDayLine;
}
