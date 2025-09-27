#pragma once

#include"ProductTiingo.h"
#include"TiingoStock.h"

class CProductTiingoStockProfile final : public CProductTiingo {
public:
	CProductTiingoStockProfile();
	~CProductTiingoStockProfile() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CTiingoStocksPtr ParseTiingoStockSymbol(const CWebDataPtr& pWebData);

	void UpdateSystemStatus(CVirtualDataSourcePtr pDataSource) override;
	static CTiingoStocksPtr DeleteDuplicatedSymbol(const CTiingoStocksPtr& pvTiingoStock);
	void SaveNewSymbol();
	void SaveDelistedSymbol();
};

using CProductTiingoStockProfilePtr = shared_ptr<CProductTiingoStockProfile>;
