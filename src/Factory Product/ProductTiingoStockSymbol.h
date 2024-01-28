#pragma once

#include"ProductTiingo.h"
#include"TiingoStock.h"

class CProductTiingoStockSymbol final : public CProductTiingo {
public:
	CProductTiingoStockSymbol();
	~CProductTiingoStockSymbol() override = default;

	CString CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CTiingoStockVectorPtr ParseTiingoStockSymbol(const CWebDataPtr& pWebData);
	CTiingoStockVectorPtr ParseTiingoStockSymbol2(const CWebDataPtr& pWebData);

	void UpdateDataSourceStatus(CVirtualDataSourcePtr pDataSource) override;
};

using CTiingoStockSymbolsPtr = shared_ptr<CProductTiingoStockSymbol>;
