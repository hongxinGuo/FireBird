#pragma once

#include"ProductAlphaVantage.h"

class CWebData;
class CStockSplit;

using std::shared_ptr;
using std::vector;

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
	void ParseAndStoreWebData(shared_ptr<CWebData> pWebData) override;
	shared_ptr<vector<shared_ptr<CStockSplit>>> ParseAlphaVantageStockSplits(const shared_ptr<CWebData>& pWebData);
};

using CAlphaVantageStockSplitsPtr = shared_ptr<CProductAlphaVantageStockSplits>;
