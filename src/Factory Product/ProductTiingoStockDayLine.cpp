#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

#include"WorldStock.h"
#include"WorldMarket.h"

#include "ProductTiingoStockDayLine.h"

#include "accessory.h"
#include "TimeConvert.h"

CProductTiingoStockDayLine::CProductTiingoStockDayLine() {
	m_strInquiryFunction = _T("https://api.tiingo.com/tiingo/daily/");
}

///////////////////////////////////////////////////////////////////////////////////////////
///
/// 即使是免费账户，tiingo日线也能够提供30年以上的数据，故而申请全部数据。
///
///////////////////////////////////////////////////////////////////////////////////////////
CString CProductTiingoStockDayLine::CreateMessage() {
	ASSERT(std::strcmp(typeid(*GetMarket()).name(), _T("class CWorldMarket")) == 0);

	const auto pStock = gl_dataContainerTiingoStock.GetStock(GetIndex());
	CString strParam = GetTiingoDayLineInquiryParam(pStock->GetSymbol(), 19800101, GetMarket()->GetMarketDate()); // 如果日线未完全申请过时，申请完整日线。

	m_strInquiry = m_strInquiryFunction + strParam;
	return m_strInquiry;
}

void CProductTiingoStockDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(std::strcmp(typeid(*GetMarket()).name(), _T("class CWorldMarket")) == 0);

	const auto pTiingoStock = gl_dataContainerTiingoStock.GetStock(m_lIndex);
	const CDayLinesPtr pvDayLine = ParseTiingoStockDayLine(pWebData);
	if (!pvDayLine->empty()) {
		long lastClose = 0;
		for (const auto& pDayLine2 : *pvDayLine) {
			pDayLine2->SetExchange(pTiingoStock->GetExchangeCode());
			pDayLine2->SetStockSymbol(pTiingoStock->GetSymbol());
			pDayLine2->SetDisplaySymbol(pTiingoStock->GetDisplaySymbol());
			if ((lastClose != 0) && (pDayLine2->GetLastClose() == 0)) pDayLine2->SetLastClose(lastClose);
			lastClose = pDayLine2->GetClose();
		}
		if (!IsEarlyThen(pvDayLine->at(pvDayLine->size() - 1)->GetMarketDate(), GetMarket()->GetMarketDate(), 100)) {
			pTiingoStock->SetIPOStatus(_STOCK_IPOED_);
		}
		pTiingoStock->UpdateDayLine(*pvDayLine);
		pTiingoStock->SetDayLineUpdateDate(pvDayLine->at(pvDayLine->size() - 1)->GetMarketDate());
		pTiingoStock->SetUpdateDayLineDB(true);
		pTiingoStock->SetUpdateProfileDB(true);
	}
	else {
		pTiingoStock->SetUpdateDayLineDB(false);
		pTiingoStock->SetUpdateProfileDB(false);
	}
	// 清除tiingo stock的日线更新标识
	pTiingoStock->SetUpdateDayLine(false);
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
			//pDayLine->SetExchange(_T("US")); // 所有的Tiingo证券皆为美国市场。
			s = jsonGetString(it, _T("date"));
			CString str = s.c_str();
			sscanf_s(str.GetBuffer(), _T("%04d-%02d-%02d"), &year, &month, &day);
			long lTemp = XferYearMonthDayToYYYYMMDD(year, month, day);
			pDayLine->SetDate(lTemp);
			double dTemp = jsonGetDouble(it,_T("close"));
			pDayLine->SetClose(dTemp * 1000);
			dTemp = jsonGetDouble(it,_T("high"));
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
