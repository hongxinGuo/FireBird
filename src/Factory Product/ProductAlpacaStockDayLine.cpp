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
void CProductAlpacaStockDayLine::InquireData(const string& strHeaders, const string& strParams, const string& strSuffix, const string& strInquiryToken) {
	shared_ptr<vector<CTiingoCandleLine>> pvDayLineWithSplit = make_shared<vector<CTiingoCandleLine>>();
	shared_ptr<vector<CTiingoCandleLine>> pvDayLine = make_shared<vector<CTiingoCandleLine>>();
	shared_ptr<vector<string>> pInquiry = CreateMessage();
	const auto pTiingoStock = gl_dataContainerTiingoStock.GetStock(m_index);
	long status = 200;
	cpr::Header headers = cpr::Header{
		{ "APCA-API-KEY-ID", "PK3J5QOOORALNDMELW2XS5RDZX" },
		{ "APCA-API-SECRET-KEY", "DybHyD53p5KCGLaSPd6oa6dKwA1cvtgSM5UGvC73oAfk" },
		{ "accept", "application/json" }
	};
	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		cpr::Response r = cpr::Get(cpr::Url{ inquiry }, headers);

		status = r.status_code;
		if (status != 200) {
			WebStatusCheck(r);
			pTiingoStock->SetUpdateDayLine(false);
			return;
		}
		Parse(pvDayLine, r, pTiingoStock->GetSymbol());
	}
	std::ranges::sort(*pvDayLine, [](const CTiingoCandleLine& a, const CTiingoCandleLine& b) {
		return a.GetDate() < b.GetDate();
	});

	auto inquireStrings2 = CreateMessageWithSplit();
	for (const auto& inquiry : *inquireStrings2) {
		cpr::Response r = cpr::Get(cpr::Url{ inquiry }, headers);

		status = r.status_code;
		if (status != 200) {
			WebStatusCheck(r);
			pTiingoStock->SetUpdateDayLine(false);
			return;
		}
		Parse(pvDayLineWithSplit, r, pTiingoStock->GetSymbol());
	}
	std::ranges::sort(*pvDayLineWithSplit, [](const CTiingoCandleLine& a, const CTiingoCandleLine& b) {
		return a.GetDate() < b.GetDate();
	});

	ABSL_DCHECK(pvDayLineWithSplit->size() == pvDayLine->size());
	if (!pvDayLine->empty()) {
		ABSL_DCHECK(pvDayLine->at(0).GetDate() == pvDayLineWithSplit->at(0).GetDate());
		ABSL_DCHECK(pvDayLine->at(pvDayLine->size() - 1).GetDate() == pvDayLineWithSplit->at(pvDayLineWithSplit->size() - 1).GetDate());
		CalculateSplitFactor(*pvDayLine, *pvDayLineWithSplit);
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
		string number = std::format("Error: Alpaca stock dayLine {:d}", r.status_code);
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
	shared_ptr<vector<string>> pInquiry = make_shared<vector<string>>();
	const auto pStock = gl_dataContainerTiingoStock.GetStock(GetIndex());
	ABSL_DCHECK(pStock->IsActive()); // 活跃股票
	chrono::local_days lStartDate{ 1980y / 01 / 01 };
	if (pStock->GetDayLineEndDate() > toLocalDays(19800101)) lStartDate = pStock->GetDayLineEndDate() - chrono::days(needMoreDayLineData_);
	auto endDate = gl_pWorldMarket->GetMarketDate();
	auto countNumber = std::chrono::duration_cast<std::chrono::days>(gl_pWorldMarket->GetMarketDate() - lStartDate).count() / 1000;

	string symbol = std::format("symbols={}&timeframe=1D&limit=1000&feed=iex{}&sort=asc", pStock->GetSymbol(), paramAdjust);
	if (countNumber > 0) {
		for (int i = 0; i < countNumber; ++i) {
			string sParam = std::format("symbols={}&timeframe=1D&limit=1000&feed=iex{}&sort=asc&start={:%F}&end={:%F}",
			                            pStock->GetSymbol(), paramAdjust, lStartDate + chrono::days(i * 1000), lStartDate + chrono::days((i + 1) * 1000 - 1)); // Note: 总是多申请一天的日线数据

			m_inquiryString = m_strInquiryFunction + sParam;
			pInquiry->push_back(m_inquiryString);
		}
		string sParam = std::format("symbols={}&timeframe=1D&limit=1000&feed=iex{}&sort=asc&start={:%F}&end={:%F}",
		                            pStock->GetSymbol(), paramAdjust, lStartDate + chrono::days(countNumber * 1000), gl_pWorldMarket->GetMarketDate()); // Note: 总是多申请一天的日线数据
		pInquiry->push_back(m_strInquiryFunction + sParam);
	}
	else {
		string sParam = std::format("symbols={}&timeframe=1D&limit=1000&feed=iex{}&sort=asc&start={:%F}&end={:%F}",
		                            pStock->GetSymbol(), paramAdjust, lStartDate, gl_pWorldMarket->GetMarketDate()); // Note: 总是多申请一天的日线数据
		pInquiry->push_back(m_strInquiryFunction + sParam);
	}
	return pInquiry;
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
///
void CProductAlpacaStockDayLine::Parse(shared_ptr<vector<CTiingoCandleLine>> pvDayLine, const cpr::Response& r, const string& stockSymbol) {
	CTiingoStock stock;
	nlohmannJson j = nlohmann::json::parse(r.text, nullptr, false);

	if (!j.at("next_page_token").is_null()) m_bDataEnded = false;
	//string nextPageToken = j.value("next_page_token", "");

	auto bars_data = j.at("bars").get<std::map<std::string, std::vector<Bar>>>();
	for (const auto& [symbol, bars] : bars_data) {
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
			pvDayLine->push_back(dayLine);
		}
	}
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
