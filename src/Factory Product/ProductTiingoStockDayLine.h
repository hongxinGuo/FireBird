#pragma once

#include"ProductTiingo.h"
#include"TiingoCandleLine.h"

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

	string GetDayLineInquiryParam(const string& strSymbol, chrono::local_days lStartDate, chrono::local_days lCurrentDate);

private:
};

using CTiingoStockDayLinePtr = shared_ptr<CProductTiingoStockDayLine>;
