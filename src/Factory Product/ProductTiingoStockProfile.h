#pragma once

#include "ContainerTiingoSymbol.h"
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

	shared_ptr<vector<string>> CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	shared_ptr<vector<shared_ptr<CTiingoStock>>> ParseTiingoStockSymbol(const CWebDataPtr& pWebData);

	void UpdateSystemStatus() override;
	void DeleteDuplicatedSymbol(const shared_ptr<vector<shared_ptr<CTiingoStock>>>& pvTiingoStock);
	static void SaveNewSymbol();
	void SaveDelistedSymbol();

protected:
	CContainerTiingoSymbol m_containerCurrentTiingoSymbols;
};

using CProductTiingoStockProfilePtr = shared_ptr<CProductTiingoStockProfile>;
