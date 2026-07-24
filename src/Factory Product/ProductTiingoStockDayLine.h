#pragma once

#include"ProductTiingo.h"
#include"TiingoCandleLine.h"

#define needMoreDayLineData_ 10 // 申请日线数据时，总是多申请一天的数据，以便使用前日收盘价作为昨收。

class CProductTiingoStockDayLine final : public CProductTiingo {
public:
	CProductTiingoStockDayLine();
	// 不允许赋值。
	CProductTiingoStockDayLine(const CProductTiingoStockDayLine&) = delete;
	CProductTiingoStockDayLine& operator=(const CProductTiingoStockDayLine&) = delete;
	CProductTiingoStockDayLine(const CProductTiingoStockDayLine&&) noexcept = delete;
	CProductTiingoStockDayLine& operator=(const CProductTiingoStockDayLine&&) noexcept = delete;
	~CProductTiingoStockDayLine() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CTiingoCandleLinesPtr ParseTiingoStockDayLine(const CWebDataPtr& pWebData);

	string GetDayLineInquiryParam(const string& strSymbol, local_days lStartDate, local_days lCurrentDate);

private:
};

using CTiingoStockDayLinePtr = shared_ptr<CProductTiingoStockDayLine>;
