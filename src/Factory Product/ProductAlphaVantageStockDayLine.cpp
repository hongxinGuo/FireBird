#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include"FinnhubStock.h"
#include"WorldMarket.h"

#include "ProductAlphaVantageStockDayLine.h"

#include "WebData.h"

CProductAlphaVantageStockDayLine::CProductAlphaVantageStockDayLine() {
	m_strInquiryFunction = "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY&symbol=";
}

///////////////////////////////////////////////////////////////////////////////////////////
///
/// Alpha Vantage免费账户的日线提供100个的数据；付费账户提供20年的数据。
/// 对于日线数据，Alpha Vantage的免费账户提供的数据已经足够了，因此不区分免费账户和付费账户的申请参数。
/// 
///
///////////////////////////////////////////////////////////////////////////////////////////
string CProductAlphaVantageStockDayLine::CreateMessage() {
	const auto pStock = gl_dataContainerTiingoStock.GetStock(GetIndex());
	ASSERT(pStock->IsActive()); // 活跃股票
	m_strInquiringSymbol = pStock->GetSymbol();

	m_strInquiry = m_strInquiryFunction + m_strInquiringSymbol;
	return m_strInquiry;
}

void CProductAlphaVantageStockDayLine::ParseAndStoreWebData(CWebDataPtr pWebData) {
	const auto pTiingoStock = gl_dataContainerTiingoStock.GetStock(m_lIndex);

	auto pvDayLine = ParseAlphaVantageStockDayLine(pWebData);
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
	}
	// 清除tiingo stock的日线更新标识
	pTiingoStock->SetUpdateDayLine(false);
	pTiingoStock->SetUpdateProfileDB(true);
	if (gl_dataContainerTiingoNewSymbol.IsSymbol(pTiingoStock->GetSymbol())) { // 新股票？
		gl_dataContainerTiingoNewSymbol.Delete(pTiingoStock); // 下载完日线数据后，就从AlphaVantage新代码容器中删除之。
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// AlphaVantage日线格式：
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
CTiingoCandleLinesPtr CProductAlphaVantageStockDayLine::ParseAlphaVantageStockDayLine(const CWebDataPtr& pWebData) {
	auto pvDayLine = make_shared<vector<CTiingoCandleLine>>();
	pvDayLine->reserve(3000); // AlphaVantage免费账户的日线提供100个的数据；付费账户提供20年的数据。预先分配100个空间，避免多次分配内存。
	string s;
	nlohmannJson js;

	if (!pWebData->CreateJson(js)) return pvDayLine;
	if (!IsValidData(pWebData)) return pvDayLine;

	try {
		s = js.at("detail"); // 是否有报错信息
		string strMessage = "AlphaVantage stock dayLine ";
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
			//pDayLine->SetExchange("US"); // 所有的AlphaVantage证券皆为美国市场。
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
		}
	} catch (nlohmannJson::exception& e) {
		string str3 = pWebData->GetDataBuffer();
		str3 = str3.substr(0, 120);
		ReportJSonErrorToSystemMessage("AlphaVantage Stock DayLine " + str3, e.what());
		return pvDayLine; // 数据解析出错的话，则放弃。
	}
	std::ranges::sort(*pvDayLine, [](const CTiingoCandleLine& pData1, const CTiingoCandleLine& pData2) { return pData1.GetDate() < pData2.GetDate(); }); // 以日期早晚顺序排列。

	return pvDayLine;
}
