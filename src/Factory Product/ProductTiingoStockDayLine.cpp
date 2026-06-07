#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include"WorldMarket.h"

#include "ProductTiingoStockDayLine.h"

#include "WebData.h"

string CProductTiingoStockDayLine::GetDayLineInquiryParam(const string& strSymbol, long lStartDate, long lCurrentDate) {
	const long year = lCurrentDate / 10000;
	const long month = lCurrentDate / 100 - year * 100;
	const long date = lCurrentDate - year * 10000 - month * 100;

	const long yearStart = lStartDate / 10000;
	const long monthStart = lStartDate / 100 - yearStart * 100;
	const long dateStart = lStartDate - yearStart * 10000 - monthStart * 100;

	string sParam = std::format("{}/prices?&startDate={:4Ld}-{:Ld}-{:Ld}&endDate={:4Ld}-{:Ld}-{:Ld}", strSymbol, yearStart, monthStart, dateStart, year, month, date);
	return sParam;
}

CProductTiingoStockDayLine::CProductTiingoStockDayLine() {
	m_strInquiryFunction = "https://api.tiingo.com/tiingo/daily/";
}

///////////////////////////////////////////////////////////////////////////////////////////
///
/// 即使是免费账户，tiingo日线也能够提供30年以上的数据，故而申请全部数据。
///
///////////////////////////////////////////////////////////////////////////////////////////
string CProductTiingoStockDayLine::CreateMessage() {
	const auto pStock = gl_dataContainerTiingoStock.GetStock(GetIndex());
	ASSERT(pStock->IsActive()); // 活跃股票
	long lStartDate = 19800101;
	if (pStock->GetDayLineEndDate() > 19800101) lStartDate = pStock->GetDayLineEndDate();
	string strParam = GetDayLineInquiryParam(pStock->GetSymbol(), lStartDate, gl_pWorldMarket->GetMarketDate()); // 如果日线从未申请过时，申请完整日线。
	m_strInquiringSymbol = pStock->GetSymbol();

	m_strInquiry = m_strInquiryFunction + strParam;
	return m_strInquiry;
}

void CProductTiingoStockDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pTiingoStock = gl_dataContainerTiingoStock.GetStock(m_lIndex);

	auto pvDayLine = ParseTiingoStockDayLine(pWebData);
	if (!pvDayLine->empty()) {
		long lastClose = 0;
		for (auto& dayLine2 : *pvDayLine) {
			dayLine2.SetExchange(pTiingoStock->GetExchange());
			dayLine2.SetStockSymbol(pTiingoStock->GetSymbol());
			if (lastClose != 0 && dayLine2.GetLastClose() == 0) dayLine2.SetLastClose(lastClose);
			lastClose = dayLine2.GetClose();
		}
		pTiingoStock->UpdateDayLine(pvDayLine);
		pTiingoStock->SetUpdateDayLineDB(true);
		pvDayLine = nullptr;
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
CTiingoCandleLinesPtr CProductTiingoStockDayLine::ParseTiingoStockDayLine(const CWebDataPtr& pWebData) {
	auto pvDayLine = make_shared<vector<CTiingoCandleLine>>();
	pvDayLine->reserve(3000); // 预先分配7500个元素的空间，避免频繁扩容。Tiingo日线数据最多有几十年，每年250个交易日，1000个元素足够了。

	string s;
	nlohmannJson js;

	if (!pWebData->CreateJson(js)) return pvDayLine;
	if (!IsValidData(pWebData)) return pvDayLine;

	try {
		s = js.at("detail"); // 是否有报错信息
		string strMessage = "Tiingo stock dayLine ";
		strMessage += s;
		gl_systemMessage.PushErrorMessage(strMessage); // 报告错误信息
		return pvDayLine;
	} catch (nlohmannJson::exception&) {
		// 正确， do nothing，继续执行
	}
	try {
		for (auto it = js.begin(); it != js.end(); ++it) {
			CTiingoStock stock;
			CTiingoCandleLine dayLine;
			//pDayLine->SetExchange("US"); // 所有的Tiingo证券皆为美国市场。
			s = jsonGetString(it, "date");
			long lTemp = XferToYYYYMMDD(s);
			dayLine.SetDate(lTemp);
			double dTemp = jsonGetDouble(it, "close");
			dayLine.SetClose(dTemp * stock.GetRatio());
			dTemp = jsonGetDouble(it, "high");
			dayLine.SetHigh(dTemp * stock.GetRatio());
			dTemp = jsonGetDouble(it, "low");
			dayLine.SetLow(dTemp * stock.GetRatio());
			dTemp = jsonGetDouble(it, "open");
			dayLine.SetOpen(dTemp * stock.GetRatio());
			lTemp = jsonGetLong(it, "volume");
			dTemp = jsonGetDouble(it, "divCash");
			dayLine.SetDividend(dTemp);
			dTemp = jsonGetDouble(it, "splitFactor");
			dayLine.SetSplitFactor(dTemp);
			dayLine.SetVolume(lTemp);
			pvDayLine->push_back(dayLine);
			dayLine.Reset(); //Note 需要重置对象，以免下次循环时，之前的数据还在，导致数据错误。
		}
	} catch (nlohmannJson::exception& e) {
		string str3 = pWebData->GetDataBuffer();
		str3 = str3.substr(0, 120);
		ReportJSonErrorToSystemMessage("Tiingo Stock DayLine " + str3, e.what());
		return pvDayLine; // 数据解析出错的话，则放弃。
	}
	std::ranges::sort(*pvDayLine, [](const CTiingoCandleLine& pData1, const CTiingoCandleLine& pData2) { return pData1.GetDate() < pData2.GetDate(); }); // 以日期早晚顺序排列。

	return pvDayLine;
}
