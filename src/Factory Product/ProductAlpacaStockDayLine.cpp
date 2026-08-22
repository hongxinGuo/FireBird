///////////////////////////////////////////////////////////////////////////
///
/// Alpaca股票日线申请接受处理
///
/// 当网络
///
///
///
///
//////////////////////////////////////////////////////////////////////////
#include "pch.h"

#include "ProductAlpacaStockDayLine.h"
#include "AlpacaDataSource.h"
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

#include<cpr/cpr.h>

#include "ContainerAlpacaStockSymbol.h"
#include "ContainerTiingoSymbol.h"

using namespace std;

namespace {
	struct Bar {
		std::string timeStamp; // t
		double open; // o
		double high; // h
		double low; // l
		double close; // c
		long volume; // v
		double vwap; // vw
	};

	void from_json(const nlohmann::json& j, Bar& b) {
		j.at("t").get_to(b.timeStamp);
		j.at("o").get_to(b.open);
		j.at("h").get_to(b.high);
		j.at("l").get_to(b.low);
		j.at("c").get_to(b.close);
		j.at("v").get_to(b.volume);
		j.at("vw").get_to(b.vwap);
	}
}

CProductAlpacaStockDayLine::CProductAlpacaStockDayLine() {
	m_strInquiryFunction = "https://data.alpaca.markets/v2/stocks/bars?";
	m_currentMarketDate = gl_pWorldMarket->GetMarketDate();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// <summary>
/// 
/// </summary>
/// <param name="strHeaders"></param>
/// <param name="strParams"></param>
/// <param name="strSuffix"></param>
/// <param name="strInquiryToken"></param>
/// 
/// 使用cpr库，由本函数申请网络数据。注意：此函数只在使用cpr库时才会被调用。
/// 
/// Note: Alpaca的日线数据不提供split信息，故而需要两次申请来确定股票的split信息。第一次申请是获取股票的原始日线数据，第二次申请是获取股票的前向除权数据。
/// 
////////////////////////////////////////////////////////////////////////////////////////////////////////
void CProductAlpacaStockDayLine::InquireData(const std::stop_token& st, const string& strHeaders, const string& strParams, const string& strSuffix, const string& strInquiryToken) {
	shared_ptr<vector<TiingoDayLine>> pvDayLineWithSplit = make_shared<vector<TiingoDayLine>>();
	shared_ptr<vector<TiingoDayLine>> pvDayLine = make_shared<vector<TiingoDayLine>>();
	const auto pTiingoStock = gl_dataContainerTiingoStock.GetStock(m_index);
	long status;

	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		if (st.stop_requested()) break;
		cpr::Response r = cpr::Get(cpr::Url{ inquiry }, gl_pAlpacaDataSource->GetHeader());

		status = r.status_code;
		if (status != 200) {
			WebStatusCheck(r);
			ClearUpdateDayLineFlag();
			return;
		}
		Parse(pvDayLine, r, pTiingoStock->GetSymbol());
	}

	auto inquireStrings2 = CreateMessageWithSplit();
	for (const auto& inquiry : *inquireStrings2) {
		if (st.stop_requested()) break;
		cpr::Response r = cpr::Get(cpr::Url{ inquiry }, gl_pAlpacaDataSource->GetHeader());

		status = r.status_code;
		if (status != 200) {
			WebStatusCheck(r);
			ClearUpdateDayLineFlag();
			return;
		}
		Parse(pvDayLineWithSplit, r, pTiingoStock->GetSymbol());
	}

	if (st.stop_requested()) return;
	map<string, size_t> mapSymbolIndex;
	for (size_t index = 0; index < pvDayLine->size(); index++) {
		mapSymbolIndex[pvDayLine->at(index).m_symbol] = index;
	}

	for (size_t index = 0; index < pvDayLineWithSplit->size(); index++) {
		auto pos = mapSymbolIndex.at(pvDayLineWithSplit->at(index).m_symbol);
		UpdateDayLine(pvDayLine->at(pos).m_symbol, pvDayLine->at(pos).m_dayLine, pvDayLineWithSplit->at(pos).m_dayLine);
	}
	ClearUpdateDayLineFlag();
}

void CProductAlpacaStockDayLine::ClearUpdateDayLineFlag() {
	for (const auto& symbol : m_vStockSymbols) {
		if (gl_dataContainerTiingoStock.IsSymbol(symbol)) {
			auto pStock = gl_dataContainerTiingoStock.GetStock(symbol);
			pStock->SetUpdateDayLine(false);
			pStock->SetUpdateProfileDB(true);
		}
	}
	m_vStockSymbols.clear();
}

void CProductAlpacaStockDayLine::UpdateDayLine(const string& stockSymbol, vector<CTiingoCandleLine>& vDayLine, vector<CTiingoCandleLine>& vDayLineWithSplit) {
	ABSL_DCHECK(vDayLineWithSplit.size() == vDayLine.size());
	ABSL_DCHECK(gl_dataContainerTiingoStock.IsSymbol(stockSymbol));
	auto pTiingoStock = gl_dataContainerTiingoStock.GetStock(stockSymbol);
	if (!vDayLine.empty()) {
		ABSL_DCHECK(vDayLine.at(0).GetDate() == vDayLineWithSplit.at(0).GetDate());
		ABSL_DCHECK(vDayLine.at(vDayLine.size() - 1).GetDate() == vDayLineWithSplit.at(vDayLineWithSplit.size() - 1).GetDate());

		std::ranges::sort(vDayLine, [](const CTiingoCandleLine& a, const CTiingoCandleLine& b) {
			return a.GetDate() < b.GetDate();
		});
		std::ranges::sort(vDayLineWithSplit, [](const CTiingoCandleLine& a, const CTiingoCandleLine& b) {
			return a.GetDate() < b.GetDate();
		});

		CalculateSplitFactor(vDayLine, vDayLineWithSplit);
		long lastClose = 0;
		for (auto& dayLine : vDayLine) {// 使用前日收盘数据作为昨收
			dayLine.SetExchange(pTiingoStock->GetExchange());
			dayLine.SetStockSymbol(pTiingoStock->GetSymbol());
			dayLine.SetLastClose(lastClose);
			lastClose = dayLine.GetClose();
		}
		if ((vDayLine.size() > 1) && pTiingoStock->GetDayLineEndDate() != chrono::local_days{ 1980y / 01 / 01 }) {
			vDayLine.erase(vDayLine.begin()); // 删除重复日线数据
		}
		pTiingoStock->UpdateDayLine(vDayLine);
		pTiingoStock->SetUpdateDayLineDB(true);
	}
	// 清除tiingo stock的日线更新标识
	pTiingoStock->SetUpdateDayLine(false);
	pTiingoStock->SetUpdateProfileDB(true);
	if (gl_dataContainerTiingoNewSymbol.IsSymbol(pTiingoStock->GetSymbol())) { // 新股票？
		gl_dataContainerTiingoNewSymbol.Delete(pTiingoStock); // 下载完日线数据后，就从Tiingo新代码容器中删除之。
	}
}

void CProductAlpacaStockDayLine::WebStatusCheck(cpr::Response& r) {
	nlohmannJson j;
	string message;
	switch (r.status_code) {
	case 200: // good;
		break;
	case 400: // one of request parameters is invalid.See the return message for detail.
		j = nlohmann::json::parse(r.text, nullptr, false);
		message = "Alpaca stock dayLine: ";
		message += j.at("message");
		gl_systemMessage.PushErrorMessage(message);
		break;
	case 401: // Authentication headers are missing or invalid.
		break;
	case 403: // The requested resource is forbidden.
		break;
	case 429: // Too many requests.You hit the rate limit.
		break;
	case 500: // Internal server error.
		break;
	default: // unknown problem
		string number = std::format("stock {} Error: Alpaca stock dayLine, status code: {:d}", m_strInquiringSymbol, r.status_code);
		gl_systemMessage.PushErrorMessage(number);
		break;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////
///
/// Alpaca免费账户的日线提供2016年以来的数据。
/// 对于日线数据，Alpaca的免费账户提供的数据已经足够了，因此不区分免费账户和付费账户的申请参数。
/// 
///
///////////////////////////////////////////////////////////////////////////////////////////
shared_ptr<std::vector<std::string>> CProductAlpacaStockDayLine::CreateMessage() {
	return CreateMessageInternal("&adjustment=raw");
}

shared_ptr<std::vector<std::string>> CProductAlpacaStockDayLine::CreateMessageWithSplit() {
	return CreateMessageInternal("&adjustment=split");
}

shared_ptr<std::vector<std::string>> CProductAlpacaStockDayLine::CreateMessageInternal(string paramAdjust) {
	chrono::local_days startDate = GetStartInquireDay(GetIndex());
	int totalDays = std::chrono::duration_cast<std::chrono::days>(m_currentMarketDate - startDate).count();
	if (totalDays >= 1000) {
		return InquireOneStock(paramAdjust);
	}
	else {
		return InquireMultipleStocks(paramAdjust);
	}
}

shared_ptr<std::vector<std::string>> CProductAlpacaStockDayLine::InquireOneStock(string paramAdjust) {
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	const auto pStock = gl_dataContainerTiingoStock.GetStock(GetIndex());
	chrono::local_days startDate = GetStartInquireDay(GetIndex());
	int totalDays = std::chrono::duration_cast<std::chrono::days>(m_currentMarketDate - startDate).count();
	string symbol = pStock->GetSymbol();
	int countNumber = totalDays / 1000;
	for (int i = 0; i < countNumber; ++i) {
		string sParam = std::format("symbols={}&timeframe=1D&limit=1000&feed=iex{}&sort=asc&start={:%F}&end={:%F}",
		                            symbol, paramAdjust, startDate + chrono::days(i * 1000), startDate + chrono::days((i + 1) * 1000 - 1)); // Note: 总是多申请一天的日线数据

		m_inquiryString = m_strInquiryFunction + sParam;
		pInquiry->push_back(m_inquiryString);
	}
	string sParam = std::format("symbols={}&timeframe=1D&limit=1000&feed=iex{}&sort=asc&start={:%F}&end={:%F}",
	                            symbol, paramAdjust, startDate + chrono::days(countNumber * 1000), m_currentMarketDate); // Note: 总是多申请一天的日线数据
	pInquiry->push_back(m_strInquiryFunction + sParam);
	m_vStockSymbols.push_back(symbol);

	return pInquiry;
}

shared_ptr<std::vector<std::string>> CProductAlpacaStockDayLine::InquireMultipleStocks(string paramAdjust) {
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	auto pStock = gl_dataContainerTiingoStock.GetStock(GetIndex());
	chrono::local_days startDate = GetStartInquireDay(GetIndex());
	int stepLength = std::chrono::duration_cast<std::chrono::days>(m_currentMarketDate - startDate).count();
	ABSL_DCHECK(stepLength < 1000);
	string symbols = pStock->GetSymbol();
	m_vStockSymbols.push_back(pStock->GetSymbol());
	int totalInquiryStocks = 1;
	size_t pos = 1;
	while (true) {
		if (totalInquiryStocks > 60) break;
		if (GetIndex() + pos >= gl_dataContainerTiingoStock.Size()) break; // 到达终点了
		pStock = gl_dataContainerTiingoStock.GetStock(GetIndex() + pos);
		if (!gl_dataContainerAlpacaStockSymbol.IsSymbol(pStock->GetSymbol())) { // Alpaca代码集中没有的股票不查询
			pos++;
			continue;
		}
		auto currentStartDay = GetStartInquireDay(GetIndex() + pos);
		int currentDays = std::chrono::duration_cast<std::chrono::days>(m_currentMarketDate - currentStartDay).count();
		ABSL_DCHECK(currentDays > 0);
		if (currentStartDay < startDate) {
			if (currentDays * (totalInquiryStocks + 1) >= 1000) break;
			stepLength = currentDays;
			startDate = currentStartDay;
		}
		else {
			if (stepLength * (totalInquiryStocks + 1) >= 1000) break;
		}

		symbols += ",";
		symbols += pStock->GetSymbol();
		m_vStockSymbols.push_back(pStock->GetSymbol());
		pos++;
		totalInquiryStocks++;
	}
	string sParam = std::format("symbols={}&timeframe=1D&limit=1000&feed=iex{}&sort=asc&start={:%F}&end={:%F}",
	                            symbols, paramAdjust, startDate, m_currentMarketDate); // Note: 总是多申请一天的日线数据
	pInquiry->push_back(m_strInquiryFunction + sParam);

	return pInquiry;
}

local_days CProductAlpacaStockDayLine::GetStartInquireDay(size_t stockIndex) const {
	const auto pStock = gl_dataContainerTiingoStock.GetStock(stockIndex);
	chrono::local_days lStartDate{ 1980y / 01 / 01 };
	if (pStock->GetDayLineEndDate() > toLocalDays(19800101)) {
		lStartDate = pStock->GetDayLineEndDate() - chrono::days(needMoreDayLineData_);
	}
	return lStartDate;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// 
/// {
///	"bars": {
///		"RIG": [
///		{
///			"c": 5.14,
///				"h" : 5.24,
///				"l" : 5.095,
///				"n" : 2736,
///				"o" : 5.195,
///				"t" : "2026-08-03T04:00:00Z",
///				"v" : 1398585,
///				"vw" : 5.172631
///		},
///		{
///				"c": 5.22,
///				"h" : 5.28,
///				"l" : 5.045,
///				"n" : 3228,
///				"o" : 5.055,
///				"t" : "2026-08-04T04:00:00Z",
///				"v" : 2681580,
///				"vw" : 5.225845
///		}
///		]
///	},
///		"next_page_token": "UklHfER8MTc4NTkwMjQwMDAwMDAwMDAwMA=="
/// }
/// 
/// Alpaca股票日线申请时，默认只申请一个股票且数量不超过1000个。这种情况下，返回的json数据中只有一个股票的日线数据，且next_page_token字段为null。
///
///
///
///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CProductAlpacaStockDayLine::Parse(shared_ptr<vector<TiingoDayLine>> pvDayLine, const cpr::Response& r, const string& stockSymbol) {
	CTiingoStock stock;
	TiingoDayLine tiingoDayLine;
	nlohmannJson j = nlohmann::json::parse(r.text, nullptr, false);

	if (!j.at("next_page_token").is_null()) {
		ABSL_DCHECK(false) << stockSymbol;
		m_bDataEnded = false;
		return false;
	}

	auto bars_data = j.at("bars").get<std::map<std::string, std::vector<Bar>>>();
	for (const auto& [symbol, bars] : bars_data) {
		tiingoDayLine.m_symbol = symbol;
		for (const auto& bar : bars) {
			CTiingoCandleLine dayLine;
			chrono::sys_seconds utc_tp;
			istringstream ss(bar.timeStamp);
			ss >> chrono::parse("%FT%T%Z", utc_tp);
			chrono::local_time<chrono::milliseconds> local_tp = gl_pWorldMarket->ToLocalTime(utc_tp);
			chrono::local_days localDay = chrono::local_days(chrono::floor<chrono::days>(local_tp));
			dayLine.SetDate(localDay);
			dayLine.SetOpen(bar.open * stock.GetRatio());
			dayLine.SetHigh(bar.high * stock.GetRatio());
			dayLine.SetLow(bar.low * stock.GetRatio());
			dayLine.SetClose(bar.close * stock.GetRatio());
			dayLine.SetVolume(bar.volume);
			tiingoDayLine.m_dayLine.push_back(dayLine);
		}
		pvDayLine->push_back(tiingoDayLine);
		tiingoDayLine.m_symbol.clear();
		tiingoDayLine.m_dayLine.clear();
	}
	return true;
}

void CProductAlpacaStockDayLine::CalculateSplitFactor(vector<CTiingoCandleLine>& vDayLine, vector<CTiingoCandleLine>& vDayLineWithSplit) {
	ABSL_DCHECK(vDayLine.size() == vDayLineWithSplit.size());
	for (size_t i = vDayLine.size() - 1; i > 0; --i) {
		double splitFactor = static_cast<double>(vDayLine[i].GetClose()) / vDayLineWithSplit[i].GetClose();
		if (splitFactor < 0.99 || splitFactor > 1.01) {
			vDayLine[i].SetSplitFactor(splitFactor); // 记录分割因子
			for (size_t j = 0; j <= i; ++j) {
				vDayLineWithSplit[j].SetClose(vDayLineWithSplit[j].GetClose() * splitFactor);
			}
		}
	}
}
