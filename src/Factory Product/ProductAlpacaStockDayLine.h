/////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Note Alpaca日线目前一次能够提供1000个数据。当日线总量超过1000个时，服务器分次提供。
/// 对于每个股票查询数据量较少（<100）的情况，可以一次申请多个股票，以减少总查询时间。
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <cpr/response.h>

#include "TiingoCandleLine.h"
#include"VirtualChinaMarketWebProduct.h"

class CDayLine;

struct TiingoDayLine {
	string m_symbol;
	vector<CTiingoCandleLine> m_dayLine;
};

using std::vector;
using std::unordered_map;

class CProductAlpacaStockDayLine final : public CVirtualChinaMarketWebProduct {
	struct StockDayLine {
		string symbol;
		shared_ptr<vector<CTiingoCandleLine>> pvDayLine;
	};

public:
	CProductAlpacaStockDayLine();
	// 不允许复制和赋值。
	CProductAlpacaStockDayLine(const CProductAlpacaStockDayLine&) = delete;
	CProductAlpacaStockDayLine& operator=(const CProductAlpacaStockDayLine&) = delete;
	CProductAlpacaStockDayLine(const CProductAlpacaStockDayLine&&) noexcept = delete;
	CProductAlpacaStockDayLine& operator=(const CProductAlpacaStockDayLine&&) noexcept = delete;
	~CProductAlpacaStockDayLine() override = default;

	void InquireData(const std::stop_token& st, const string& strHeaders, const string& strParams, const string& strSuffix, const string& strInquiryToken) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void ClearUpdateDayLineFlag();
	void UpdateDayLine(const string& stockSymbol, vector<CTiingoCandleLine>& vDayLine,
	                   vector<CTiingoCandleLine>& vDayLineWithSplit);

	shared_ptr<vector<string>> CreateMessage() override;
	shared_ptr<std::vector<std::string>> CreateMessageWithSplit();
	shared_ptr<std::vector<std::string>> CreateMessageInternal(string paramAdjust);
	shared_ptr<std::vector<std::string>> InquireOneStock(string paramAdjust);
	shared_ptr<std::vector<std::string>> InquireMultipleStocks(string paramAdjust);
	local_days GetStartInquireDay(size_t stockIndex) const;

	bool Parse(shared_ptr<vector<TiingoDayLine>> pvDayLine, const string& text, const string& stockSymbol);
	void CalculateSplitFactor(vector<CTiingoCandleLine>& vDayLine, vector<CTiingoCandleLine>& vDayLineWithSplit);

	bool IsDataEnded() const noexcept { return m_bDataEnded; }

protected:
	std::chrono::local_days m_currentMarketDate;
	bool m_bDataEnded{ true };

	vector<string> m_vStockSymbols;
	vector<StockDayLine> m_vStockDayLine; // 每个股票的日线数据
	unordered_map<string, size_t> m_mapStockDayLine;
	vector<StockDayLine> m_vStockDayLineSplit; // 每个股票的日线数据
};
