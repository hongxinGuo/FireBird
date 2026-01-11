#pragma once

#include"ProductTiingo.h"
#include"TiingoStock.h"

class CProductTiingoStockProfile final : public CProductTiingo {
public:
	CProductTiingoStockProfile();
	// 不允许赋值
	CProductTiingoStockProfile(const CProductTiingoStockProfile&) = delete;
	CProductTiingoStockProfile& operator=(const CProductTiingoStockProfile&) = delete;
	CProductTiingoStockProfile(const CProductTiingoStockProfile&&) noexcept = delete;
	CProductTiingoStockProfile& operator=(const CProductTiingoStockProfile&&) noexcept = delete;
	~CProductTiingoStockProfile() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CTiingoStocksPtr ParseTiingoStockSymbol(const CWebDataPtr& pWebData);

	void UpdateSystemStatus() override;
	static CTiingoStocksPtr DeleteDuplicatedSymbol(const CTiingoStocksPtr& pvTiingoStock);
	void SaveNewSymbol();
	void SaveDelistedSymbol();
};

using CProductTiingoStockProfilePtr = shared_ptr<CProductTiingoStockProfile>;
