#pragma once

import FireBird.Product.Tiingo.Base;
import FireBird.Stock.TiingoStock;

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
