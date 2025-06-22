#include "pch.h"

#include"jsonParse.h"
#include"JsonGetValue.h"

#include"FinnhubStock.h"
#include"WorldMarket.h"

#include "ProductTiingoStockDayLine.h"

#include "WebData.h"

CString CProductTiingoStockDayLine::GetDayLineInquiryParam(const CString& strSymbol, long lStartDate, long lCurrentDate) {
	const long year = lCurrentDate / 10000;
	const long month = lCurrentDate / 100 - year * 100;
	const long date = lCurrentDate - year * 10000 - month * 100;

	const long yearStart = lStartDate / 10000;
	const long monthStart = lStartDate / 100 - yearStart * 100;
	const long dateStart = lStartDate - yearStart * 10000 - monthStart * 100;

	string sParam = fmt::format("{}/prices?&startDate={:4Ld}-{:Ld}-{:Ld}&endDate={:4Ld}-{:Ld}-{:Ld}", strSymbol.GetString(), yearStart, monthStart, dateStart, year, month, date);
	return sParam.c_str();
}

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
	ASSERT(pStock->IsActive()); // 活跃股票
	long lStartDate = 19800101;
	if (pStock->GetDayLineEndDate() > 19800101) lStartDate = pStock->GetDayLineEndDate();
	CString strParam = GetDayLineInquiryParam(pStock->GetSymbol(), lStartDate, GetMarket()->GetMarketDate()); // 如果日线从未申请过时，申请完整日线。
	m_strInquiringSymbol = pStock->GetSymbol();

	m_strInquiry = m_strInquiryFunction + strParam;
	return m_strInquiry;
}

void CProductTiingoStockDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
	ASSERT(std::strcmp(typeid(*GetMarket()).name(), _T("class CWorldMarket")) == 0);

	const auto pTiingoStock = gl_dataContainerTiingoStock.GetStock(m_lIndex);

	const CTiingoDayLinesPtr pvDayLine = ParseTiingoStockDayLine(pWebData);
	if (!pvDayLine->empty()) {
		long lastClose = 0;
		for (const auto& pDayLine2 : *pvDayLine) {
			pDayLine2->SetExchange(pTiingoStock->GetExchangeCode());
			pDayLine2->SetStockSymbol(pTiingoStock->GetSymbol());
			pDayLine2->SetDisplaySymbol(pTiingoStock->GetDisplaySymbol());
			if (lastClose != 0 && pDayLine2->GetLastClose() == 0) pDayLine2->SetLastClose(lastClose);
			lastClose = pDayLine2->GetClose();
		}
		pTiingoStock->UpdateDayLine(pvDayLine);
		pTiingoStock->SetUpdateDayLineDB(true);
	}
	// 清除tiingo stock的日线更新标识
	pTiingoStock->SetUpdateDayLine(false);
	pTiingoStock->SetUpdateProfileDB(true);
	if (gl_dataContainerTiingoNewSymbol.IsSymbol(pTiingoStock->GetSymbol())) { // 新股票？
		gl_dataContainerTiingoNewSymbol.Delete(pTiingoStock); // 下载完日线数据后，就从Tiingo新代码容器中删除之。
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
CTiingoDayLinesPtr CProductTiingoStockDayLine::ParseTiingoStockDayLine(const CWebDataPtr& pWebData) {
	auto pvDayLine = make_shared<vector<CTiingoDayLinePtr>>();
	string s;
	json js;

	if (!pWebData->CreateJson(js)) return pvDayLine;
	if (!IsValidData(pWebData)) return pvDayLine;

	try {
		s = js.at(_T("detail")); // 是否有报错信息
		string strMessage = _T("Tiingo stock dayLine ");
		strMessage += s;
		gl_systemMessage.PushErrorMessage(strMessage); // 报告错误信息
		return pvDayLine;
	} catch (json::exception&) {
		// 正确， do nothing，继续执行
	}
	try {
		for (auto it = js.begin(); it != js.end(); ++it) {
			CTiingoStock stock;
			auto pDayLine = make_shared<CTiingoDayLine>();
			//pDayLine->SetExchange(_T("US")); // 所有的Tiingo证券皆为美国市场。
			s = jsonGetString(it, _T("date"));
			long lTemp = XferToYYYYMMDD(s);
			pDayLine->SetDate(lTemp);
			double dTemp = jsonGetDouble(it,_T("close"));
			pDayLine->SetClose(dTemp * stock.GetRatio());
			dTemp = jsonGetDouble(it,_T("high"));
			pDayLine->SetHigh(dTemp * stock.GetRatio());
			dTemp = jsonGetDouble(it, _T("low"));
			pDayLine->SetLow(dTemp * stock.GetRatio());
			dTemp = jsonGetDouble(it, _T("open"));
			pDayLine->SetOpen(dTemp * stock.GetRatio());
			lTemp = jsonGetLong(it, _T("volume"));
			dTemp = jsonGetDouble(it, _T("divCash"));
			pDayLine->SetDividend(dTemp);
			dTemp = jsonGetDouble(it, _T("splitFactor"));
			pDayLine->SetSplitFactor(dTemp);
			pDayLine->SetVolume(lTemp);
			pvDayLine->push_back(pDayLine);
		}
	} catch (json::exception& e) {
		CString str3 = pWebData->GetDataBuffer().c_str();
		str3 = str3.Left(120);
		ReportJSonErrorToSystemMessage(_T("Tiingo Stock DayLine ") + str3, e.what());
		return pvDayLine; // 数据解析出错的话，则放弃。
	}
	std::ranges::sort(*pvDayLine, [](const CTiingoDayLinePtr& pData1, const CTiingoDayLinePtr& pData2) { return pData1->GetMarketDate() < pData2->GetMarketDate(); }); // 以日期早晚顺序排列。

	return pvDayLine;
}
