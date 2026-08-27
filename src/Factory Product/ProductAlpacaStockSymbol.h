/////////////////////////////////////////////////////////////////////////////////////////////////////////
///
/// Note Alpaca日线目前一次能够提供1000个数据。当日线总量超过1000个时，服务器分次提供。
/// 对于每个股票查询数据量较少（<100）的情况，可以一次申请多个股票，以减少总查询时间。
///
/////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <cpr/response.h>

#include "AlpacaStock.h"
#include "TiingoCandleLine.h"
#include"VirtualChinaMarketWebProduct.h"

class CDayLine;

using std::vector;
using std::unordered_map;

class CProductAlpacaStockSymbol final : public CVirtualChinaMarketWebProduct {
	struct StockSymbol {
		string symbol;
		shared_ptr<vector<CTiingoCandleLine>> pvDayLine;
	};

public:
	CProductAlpacaStockSymbol();
	// 不允许复制和赋值。
	CProductAlpacaStockSymbol(const CProductAlpacaStockSymbol&) = delete;
	CProductAlpacaStockSymbol& operator=(const CProductAlpacaStockSymbol&) = delete;
	CProductAlpacaStockSymbol(const CProductAlpacaStockSymbol&&) noexcept = delete;
	CProductAlpacaStockSymbol& operator=(const CProductAlpacaStockSymbol&&) noexcept = delete;
	~CProductAlpacaStockSymbol() override = default;

	void InquireData(const std::stop_token& st) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;

	shared_ptr<vector<string>> CreateMessage() override;

	shared_ptr<vector<CAlpacaStockPtr>> Parse(const cpr::Response& r);
};
