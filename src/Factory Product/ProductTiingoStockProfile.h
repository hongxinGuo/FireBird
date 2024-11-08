#pragma once

#include"ProductTiingo.h"
#include"TiingoStock.h"

class CProductTiingoStockProfile final : public CProductTiingo {
public:
	CProductTiingoStockProfile();
	~CProductTiingoStockProfile() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CTiingoStocksPtr ParseTiingoStockSymbol(const CWebDataPtr& pWebData);

	void UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) override;
	static CTiingoStocksPtr DeleteDuplicatedSymbol(const CTiingoStocksPtr& pvTiingoStock);
};

using CProductTiingoStockProfilePtr = shared_ptr<CProductTiingoStockProfile>;
