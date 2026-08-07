#include "pch.h"

#include"jsonParse.h"
#include"nlohmannJsonGetValue.h"

#include "ProductAlpacaStockDayLine.h"

#include "AlpacaDataSource.h"
#include "ChinaMarket.h"
#include"ContainerTiingoStockDayLine.h"

#include "ContainerTiingoStock.h"
#include"TiingoStock.h"
#include "SystemData.h"
#include "SystemMessage.h"
#include "WebData.h"
#include"DayLineWebData.h"
#include "ProductTiingoStockDayLine.h"
#include"TiingoCandleLine.h"
#include "TimeConvert.h"
#include "worldMarket.h"

using namespace std;

CProductAlpacaStockDayLine::CProductAlpacaStockDayLine() {
	m_strInquiryFunction = "https://data.alpaca.markets/v2/stocks/bars?symbols=";
https://data.alpaca.markets/v2/stocks/bars?symbols=rig&timeframe=1D&limit=2&feed=iex&adjustment=raw&sort=asc&start=2026-01-01&end=2026-08-08

}

///////////////////////////////////////////////////////////////////////////////////////////
///
/// Alpha Vantage免费账户的日线提供100个的数据；付费账户提供20年的数据。
/// 对于日线数据，Alpha Vantage的免费账户提供的数据已经足够了，因此不区分免费账户和付费账户的申请参数。
/// 
///
///////////////////////////////////////////////////////////////////////////////////////////
string CProductAlpacaStockDayLine::CreateMessage() {
	const auto pStock = gl_dataContainerTiingoStock.GetStock(GetIndex());
	ABSL_DCHECK(pStock->IsActive()); // 活跃股票
	chrono::local_days lStartDate{ 1980y / 01 / 01 };
	if (pStock->GetDayLineEndDate() > toLocalDays(19800101)) lStartDate = pStock->GetDayLineEndDate() - chrono::days(needMoreDayLineData_);
	string sParam = std::format("{}&timeframe=1D&limit=2&feed=iex&adjustment=raw&sort=asc&start={:%F}&end={:%F}", pStock->GetSymbol(), lStartDate, gl_pWorldMarket->GetMarketDate()); // Note: 总是多申请一天的日线数据

	m_strInquiry = m_strInquiryFunction + sParam;
	return m_strInquiry;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Alpaca日线目前一次能够提供1000个数据。当日线总量超过1000个时，需要多次查询不同日期的数据方可。查询到的网络数据存储于pvWebData中。
// 
///////////////////////////////////////////////////////////////////////////////////////////////////////
void CProductAlpacaStockDayLine::ParseAndStoreWebData(shared_ptr<vector<CWebDataPtr>> pvWebData) {
	if (gl_pAlpacaDataSource->GetHTTPStatusCode() != 200) return; // 网络数据不正常时不处理。
	ABSL_DCHECK(pvWebData->size() <= m_iInquiryNumber);

	const auto pTiingoStock = gl_dataContainerTiingoStock.GetStock(m_index);

	shared_ptr<vector<CTiingoCandleLine>> pvDayLine = make_shared<vector<CTiingoCandleLine>>();
	string strStockSymbol;
	for (auto& pWebData : *pvWebData) { // 小于2000个数据时，只需一次查询即可，这时此vector中只有一个网络数据。
		const auto pDayLines = ParseWebData(pWebData);
		strStockSymbol = pWebData->GetStockCode();
		for (auto& pData : *pDayLines) {
			pvDayLine->push_back(pData);
		}
	}
	if (!pvDayLine->empty()) {
		long lastClose = 0;
		for (auto& dayLine : *pvDayLine) {// 使用前日收盘数据作为昨收
			dayLine.SetExchange(pTiingoStock->GetExchange());
			dayLine.SetStockSymbol(pTiingoStock->GetSymbol());
			dayLine.SetLastClose(lastClose);
			lastClose = dayLine.GetClose();
		}
		if ((pvDayLine->size() > 1) && pTiingoStock->GetDayLineEndDate() != chrono::local_days{ 1980y / 01 / 01 }) {
			pvDayLine->erase(pvDayLine->begin()); // 删除重复日线数据
		}
		pTiingoStock->UpdateDayLine(pvDayLine);
		pTiingoStock->SetUpdateDayLineDB(true);
		pvDayLine = nullptr;
	}
	// 清除tiingo stock的日线更新标识
	pTiingoStock->SetUpdateDayLine(false);
	pTiingoStock->SetUpdateProfileDB(true);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// {
//	"bars": {
//		"RIG": [
//		{
//			"c": 4.24,
//				"h" : 4.285,
//				"l" : 4.085,
//				"n" : 1426,
//				"o" : 4.155,
//				"t" : "2026-01-02T05:00:00Z",
//				"v" : 1219479,
//				"vw" : 4.226218
//		},
//		{
//				"c": 4.315,
//				"h" : 4.415,
//				"l" : 4.105,
//				"n" : 2731,
//				"o" : 4.415,
//				"t" : "2026-01-05T05:00:00Z",
//				"v" : 1584278,
//				"vw" : 4.261628
//		}
//		]
//	},
//		"next_page_token": "UklHfER8MTc2NzY3NTYwMDAwMDAwMDAwMA=="
// }
//
// 如果没有股票600600.SS日线数据，则返回：{"detail":"Error:Ticker '600600.SS' not found"}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CTiingoCandleLinesPtr CProductAlpacaStockDayLine::ParseWebData(CWebDataPtr pWebData) {
	auto pvDayLine = make_shared<vector<CTiingoCandleLine>>();
	pvDayLine->reserve(300); // Alpaca账户的日线提供1000个的数据。预先分配300个空间，避免多次分配内存。
	string s;
	nlohmannJson js;

	if (!pWebData->CreateJson(js)) return pvDayLine;

	try {
		s = js.at("detail"); // 是否有报错信息
		string strMessage = "Alpaca stock dayLine ";
		strMessage += s;
		gl_systemMessage.PushErrorMessage(strMessage); // 报告错误信息
		return pvDayLine;
	} catch (nlohmannJson::exception&) {
		// 正确， do nothing，继续执行
	}
	try {
		// New format: { "bars": { "RIG": [ { "t":"...", "c":..., "h":..., "l":..., "o":..., "v":... }, ... ] }, ... }
		if (js.contains("bars")) {
			auto& bars = js["bars"];
			for (auto itBar = bars.begin(); itBar != bars.end(); ++itBar) {
				// itBar.key() is the symbol (e.g. "RIG"), itBar.value() is the array of bar objects
				auto& arr = itBar.value();
				for (auto& item : arr) {
					CTiingoStock stock;
					CTiingoCandleLine dayLine;

					// time string like "2026-01-02T05:00:00Z"
					s = item.value("t", string());
					chrono::sys_time<chrono::milliseconds> utc_tp;
					istringstream ss(s);
					ss >> chrono::parse("%FT%T%Z", utc_tp);
					chrono::year_month_day ymd = chrono::year_month_day{ chrono::sys_days(chrono::floor<chrono::days>(utc_tp)) };
					long lTemp = static_cast<int>(ymd.year()) * 10000 + static_cast<unsigned>(ymd.month()) * 100 + static_cast<unsigned>(ymd.day());
					dayLine.SetDate(lTemp);

					double dTemp = item.value("c", 0.0);
					dayLine.SetClose(dTemp * stock.GetRatio());
					dTemp = item.value("h", 0.0);
					dayLine.SetHigh(dTemp * stock.GetRatio());
					dTemp = item.value("l", 0.0);
					dayLine.SetLow(dTemp * stock.GetRatio());
					dTemp = item.value("o", 0.0);
					dayLine.SetOpen(dTemp * stock.GetRatio());

					long vol = static_cast<long>(item.value("v", 0));
					dayLine.SetVolume(vol);

					dayLine.SetDividend(item.value("divCash", 0.0));
					dayLine.SetSplitFactor(item.value("splitFactor", 1.0));

					pvDayLine->push_back(dayLine);
				}
			}
		}
		else {
			// Fallback to previous format (keys like "date", "close", etc.)
			for (auto it = js.begin(); it != js.end(); ++it) {
				CTiingoStock stock;
				CTiingoCandleLine dayLine;
				s = jsonGetString(it, "date");
				chrono::sys_time<chrono::milliseconds> utc_tp;
				istringstream ss(s);
				ss >> chrono::parse("%FT%T%Z", utc_tp);
				chrono::year_month_day ymd = chrono::year_month_day{ chrono::sys_days(chrono::floor<chrono::days>(utc_tp)) };
				long lTemp = static_cast<int>(ymd.year()) * 10000 + static_cast<unsigned>(ymd.month()) * 100 + static_cast<unsigned>(ymd.day());
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

void CProductAlpacaStockDayLine::CheckAndPrepareDayLine(vector<CTiingoCandleLine>& vDayLine) {
	if (vDayLine.size() > 1) {
		std::ranges::sort(vDayLine, [](const CTiingoCandleLine& p1, const CTiingoCandleLine& p2) { return p1.GetDate() < p2.GetDate(); });

		for (size_t i = 0; i < vDayLine.size() - 1; i++) {
			const auto& p1 = vDayLine.at(i);
			auto& p2 = vDayLine.at(i + 1);
			ABSL_DCHECK(p1.GetDate() < p2.GetDate()); // 没有重复数据
			p2.SetLastClose(p1.GetClose());
		}
	}
}
