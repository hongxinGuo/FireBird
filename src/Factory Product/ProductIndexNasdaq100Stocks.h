#pragma once

#include"VirtualWorldMarketWebProduct.h"

using std::dynamic_pointer_cast;

class CProductIndexNasdaq100Stocks : public CVirtualWorldMarketWebProduct {
public:
	CProductIndexNasdaq100Stocks();
	// 不允许赋值。
	CProductIndexNasdaq100Stocks(const CProductIndexNasdaq100Stocks&) = delete;
	CProductIndexNasdaq100Stocks& operator=(const CProductIndexNasdaq100Stocks&) = delete;
	CProductIndexNasdaq100Stocks(const CProductIndexNasdaq100Stocks&&) noexcept = delete;
	CProductIndexNasdaq100Stocks& operator=(const CProductIndexNasdaq100Stocks&&) noexcept = delete;
	~CProductIndexNasdaq100Stocks() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	vector<string> ParseIndexNasdaq100Stocks(const CWebDataPtr& pWebData);

	void UpdateSystemStatus() override;
};

using CProductIndexNasdaq100StocksPtr = shared_ptr<CProductIndexNasdaq100Stocks>;
