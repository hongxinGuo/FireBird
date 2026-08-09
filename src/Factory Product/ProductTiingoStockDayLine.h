#pragma once

#include"ProductTiingo.h"

class CTiingoCandleLine;

using std::string;
using std::chrono::local_days;
using std::vector;
using std::shared_ptr;

#define needMoreDayLineData_ 10 // 申请日线数据时，总是多申请至少一天的数据，以便使用前日收盘价作为昨收。

class CProductTiingoStockDayLine final : public CProductTiingo {
public:
	CProductTiingoStockDayLine();
	// 不允许赋值。
	CProductTiingoStockDayLine(const CProductTiingoStockDayLine&) = delete;
	CProductTiingoStockDayLine& operator=(const CProductTiingoStockDayLine&) = delete;
	CProductTiingoStockDayLine(const CProductTiingoStockDayLine&&) noexcept = delete;
	CProductTiingoStockDayLine& operator=(const CProductTiingoStockDayLine&&) noexcept = delete;
	~CProductTiingoStockDayLine() override = default;

	shared_ptr<vector<string>> CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	shared_ptr<vector<CTiingoCandleLine>> ParseTiingoStockDayLine(const CWebDataPtr& pWebData);

	string GetDayLineInquiryParam(const string& strSymbol, local_days lStartDate, local_days lCurrentDate);

private:
};

using CTiingoStockDayLinePtr = shared_ptr<CProductTiingoStockDayLine>;
