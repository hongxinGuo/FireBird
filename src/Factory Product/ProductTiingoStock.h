#pragma once

#include"ProductTiingo.h"
#include"TiingoStock.h"

class CProductTiingoStock final : public CProductTiingo {
public:
	CProductTiingoStock();
	~CProductTiingoStock() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CTiingoStocksPtr ParseTiingoStockSymbol(const CWebDataPtr& pWebData);

	void UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) override;
};

using CProductTiingoStockPtr = shared_ptr<CProductTiingoStock>;
