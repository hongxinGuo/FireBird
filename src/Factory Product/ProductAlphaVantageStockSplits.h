#pragma once

#include"WebData.h"
#include"ProductAlphaVantage.h"
#include"StockSplit.h"

class CProductAlphaVantageStockSplits final : public CProductAlphaVantage {
public:
	CProductAlphaVantageStockSplits();
	// 不允许赋值。
	CProductAlphaVantageStockSplits(const CProductAlphaVantageStockSplits&) = delete;
	CProductAlphaVantageStockSplits& operator=(const CProductAlphaVantageStockSplits&) = delete;
	CProductAlphaVantageStockSplits(const CProductAlphaVantageStockSplits&&) noexcept = delete;
	CProductAlphaVantageStockSplits& operator=(const CProductAlphaVantageStockSplits&&) noexcept = delete;
	~CProductAlphaVantageStockSplits() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CStockSplitsPtr ParseAlphaVantageStockSplits(const CWebDataPtr& pWebData);
};

using CAlphaVantageStockSplitsPtr = shared_ptr<CProductAlphaVantageStockSplits>;
