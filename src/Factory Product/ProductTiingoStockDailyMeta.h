#pragma once

#include"ProductTiingo.h"
#include "TiingoStockDailyMeta.h"

class CProductTiingoStockDailyMeta final : public CProductTiingo {
public:
	CProductTiingoStockDailyMeta();
	// 不允许赋值。
	CProductTiingoStockDailyMeta(const CProductTiingoStockDailyMeta&) = delete;
	CProductTiingoStockDailyMeta& operator=(const CProductTiingoStockDailyMeta&) = delete;
	CProductTiingoStockDailyMeta(const CProductTiingoStockDailyMeta&&) noexcept = delete;
	CProductTiingoStockDailyMeta& operator=(const CProductTiingoStockDailyMeta&&) noexcept = delete;
	~CProductTiingoStockDailyMeta() override = default;

	string CreateMessage() override;
	void ParseAndStoreWebData(CWebDataPtr pWebData) override;
	CTiingoStockDailyMetaPtr ParseTiingoStockDailyMeta(const CWebDataPtr& pWebData);
};

using CProductTiingoStockDailyMetaPtr = shared_ptr<CProductTiingoStockDailyMeta>;
