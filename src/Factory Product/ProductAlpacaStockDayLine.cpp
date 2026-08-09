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

#include<cpr/cpr.h>

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

	void calculateSplitFactor(vector<CTiingoCandleLine>& vDayLine, vector<CTiingoCandleLine>& vDayLineWithSplit) {
		ABSL_DCHECK(vDayLine.size() == vDayLineWithSplit.size());
		for (size_t i = vDayLine.size() - 1; i > 0; --i) {
			double splitFactor = static_cast<double>(vDayLineWithSplit[i].GetClose()) / vDayLine[i].GetClose();
			if (splitFactor < 0.99 || splitFactor > 1.01) {
				vDayLine[i].SetSplitFactor(splitFactor); // 记录分割因子
				for (size_t j = 0; j <= i; ++j) {
					vDayLineWithSplit[j].SetClose(vDayLineWithSplit[j].GetClose() * splitFactor);
				}
			}
		}
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

	auto inquireStrings = CreateMessage();
	for (const auto& inquiry : *inquireStrings) {
		cpr::Response r = cpr::Get(cpr::Url{ inquiry },
		                           cpr::Header{
			                           { "APCA-API-KEY-ID", "PK3J5QOOORALNDMELW2XS5RDZX" },
			                           { "APCA-API-SECRET-KEY", "DybHyD53p5KCGLaSPd6oa6dKwA1cvtgSM5UGvC73oAfk" },
			                           { "accept", "application/json" }
		                           });

		auto status = r.status_code;
		Parse(pvDayLine, r, pTiingoStock->GetSymbol());
	}
	std::ranges::sort(*pvDayLine, [](const CTiingoCandleLine& a, const CTiingoCandleLine& b) {
		return a.GetDate() < b.GetDate();
	});

	auto inquireStrings2 = CreateMessageWithSplit();
	for (const auto& inquiry : *inquireStrings2) {
		cpr::Response r = cpr::Get(cpr::Url{ inquiry },
		                           cpr::Header{
			                           { "APCA-API-KEY-ID", "PK3J5QOOORALNDMELW2XS5RDZX" },
			                           { "APCA-API-SECRET-KEY", "DybHyD53p5KCGLaSPd6oa6dKwA1cvtgSM5UGvC73oAfk" },
			                           { "accept", "application/json" }
		                           });

		auto status = r.status_code;
		Parse(pvDayLineWithSplit, r, pTiingoStock->GetSymbol());
	}
	std::ranges::sort(*pvDayLineWithSplit, [](const CTiingoCandleLine& a, const CTiingoCandleLine& b) {
		return a.GetDate() < b.GetDate();
	});

	ABSL_DCHECK(pvDayLineWithSplit->size() == pvDayLine->size());
	ABSL_DCHECK(pvDayLine->at(0).GetDate() == pvDayLineWithSplit->at(0).GetDate());
	ABSL_DCHECK(pvDayLine->at(pvDayLine->size() - 1).GetDate() == pvDayLineWithSplit->at(pvDayLineWithSplit->size() - 1).GetDate());
	calculateSplitFactor(*pvDayLine, *pvDayLineWithSplit);

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
	lStartDate = chrono::local_days(2016y / 01 / 01); // Alpaca免费账户的日线提供2016年以来的数据。
	auto endDate = gl_pWorldMarket->GetMarketDate();
	auto countNumber = std::chrono::duration_cast<std::chrono::days>(gl_pWorldMarket->GetMarketDate() - lStartDate).count() / 1000;

	string symbol = std::format("symbols={}&timeframe=1D&limit=1000&feed=iex{}&sort=asc", pStock->GetSymbol(), paramAdjust);
	if (countNumber > 0) {
		for (int i = 0; i < countNumber; ++i) {
			string sParam = std::format("symbols={}&timeframe=1D&limit=1000&feed=iex{}&sort=asc&start={:%F}&end={:%F}",
			                            pStock->GetSymbol(), paramAdjust, lStartDate + chrono::days(i * 1000 + 1), lStartDate + chrono::days((i + 1) * 1000)); // Note: 总是多申请一天的日线数据

			m_inquiryString = m_strInquiryFunction + sParam;
			pInquiry->push_back(m_inquiryString);
		}
		string sParam = std::format("symbols={}&timeframe=1D&limit=1000&feed=iex{}&sort=asc&start={:%F}&end={:%F}",
		                            pStock->GetSymbol(), paramAdjust, lStartDate + chrono::days(countNumber * 1000 + 1), gl_pWorldMarket->GetMarketDate()); // Note: 总是多申请一天的日线数据
		pInquiry->push_back(m_strInquiryFunction + sParam);
	}
	else {
		string sParam = std::format("symbols={}&timeframe=1D&limit=1000&feed=iex{}&sort=asc&start={:%F}&end={:%F}",
		                            pStock->GetSymbol(), paramAdjust, lStartDate, gl_pWorldMarket->GetMarketDate()); // Note: 总是多申请一天的日线数据
		pInquiry->push_back(m_strInquiryFunction + sParam);
	}
	return pInquiry;
}

void CProductAlpacaStockDayLine::Parse(shared_ptr<vector<CTiingoCandleLine>> pvDayLine, const cpr::Response& r, const string& stockSymbol) {
	CTiingoStock stock;
	nlohmannJson j = nlohmann::json::parse(r.text, nullptr, false);
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
