#pragma once

#include"ProductAlphaVantage.h"
#include"TiingoCandleLine.h"

class CProductAlphaVantageStockDayLine final : public CProductAlphaVantage {
public:
	CProductAlphaVantageStockDayLine();
	// 不允许赋值。
	CProductAlphaVantageStockDayLine(const CProductAlphaVantageStockDayLine&) = delete;
	CProductAlphaVantageStockDayLine& operator=(const CProductAlphaVantageStockDayLine&) = delete;
	CProductAlphaVantageStockDayLine(const CProductAlphaVantageStockDayLine&&) noexcept = delete;
	CProductAlphaVantageStockDayLine& operator=(const CProductAlphaVantageStockDayLine&&) noexcept = delete;
	~CProductAlphaVantageStockDayLine() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CTiingoCandleLinesPtr ParseAlphaVantageStockDayLine(const CWebDataPtr& pWebData);
};

using CAlphaVantageStockDayLinePtr = shared_ptr<CProductAlphaVantageStockDayLine>;
