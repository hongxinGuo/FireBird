#pragma once

#include"ProductTiingo.h"

class CTiingoStock;

using std::shared_ptr;
using std::vector;
using std::string;

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
	shared_ptr<vector<shared_ptr<CTiingoStock>>> ParseTiingoStockSymbol(const CWebDataPtr& pWebData);

	void UpdateSystemStatus() override;
	static shared_ptr<vector<shared_ptr<CTiingoStock>>> DeleteDuplicatedSymbol(const shared_ptr<vector<shared_ptr<CTiingoStock>>>& pvTiingoStock);
	static void SaveNewSymbol();
	void SaveDelistedSymbol();
};

using CProductTiingoStockProfilePtr = shared_ptr<CProductTiingoStockProfile>;
