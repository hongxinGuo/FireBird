#pragma once

#include"ProductTiingo.h"
#include "TiingoCandleLine.h"

class CTiingoCandleLine;

using std::string;
using std::chrono::local_days;
using std::vector;
using std::shared_ptr;

constexpr int needMoreDayLineData_{ 5 }; // 申请日线数据时，总是多申请至少一天的数据，以便使用前日收盘价作为昨收。

class CProductTiingoStockDayLine final : public CProductTiingo {
public:
	CProductTiingoStockDayLine();
	// 不允许赋值。
	CProductTiingoStockDayLine(const CProductTiingoStockDayLine&) = delete;
	CProductTiingoStockDayLine& operator=(const CProductTiingoStockDayLine&) = delete;
	CProductTiingoStockDayLine(const CProductTiingoStockDayLine&&) noexcept = delete;
	CProductTiingoStockDayLine& operator=(const CProductTiingoStockDayLine&&) noexcept = delete;
	~CProductTiingoStockDayLine() override = default;

	void InquireData(const std::stop_token& st) override; // default do nothing
	void WebStatusCheck(cpr::Response& r) override;
	void UpdateSystemStatus() override;

	shared_ptr<vector<string>> CreateMessage() override;
	CTiingoCandleLinesPtr Parse(const string& text);

	string GetDayLineInquiryParam(const string& strSymbol, local_days lStartDate, local_days lCurrentDate);

private:
};

using CTiingoStockDayLinePtr = shared_ptr<CProductTiingoStockDayLine>;
