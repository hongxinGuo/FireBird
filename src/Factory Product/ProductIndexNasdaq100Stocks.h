#pragma once

#include"VirtualWorldMarketWebProduct.h"

using std::dynamic_pointer_cast;

class CProductIndexNasdaq100Stocks : public CVirtualWorldMarketWebProduct {
public:
	CProductIndexNasdaq100Stocks();
	~CProductIndexNasdaq100Stocks() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	vector<string> ParseIndexNasdaq100Stocks(const CWebDataPtr& pWebData);

	void UpdateSystemStatus(CVirtualDataSourcePtr pDataSource) override;
};

using CProductIndexNasdaq100StocksPtr = shared_ptr<CProductIndexNasdaq100Stocks>;
